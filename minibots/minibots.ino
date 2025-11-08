#include <WiFi.h>
#include <WiFiUdp.h>
#include "transform.h"

// WiFi network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// UDP settings
WiFiUDP udp;
const int udpPort = 2380;
char incomingPacket[255];

// Motor pins
int leftMotorPin = 16;
int rightMotorPin = 17;

// PWM settings
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int leftMotorChannel = 0;
const int rightMotorChannel = 1;

Transform transformer(true, false);

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup PWM
  ledcSetup(leftMotorChannel, pwmFreq, pwmResolution);
  ledcSetup(rightMotorChannel, pwmFreq, pwmResolution);
  ledcAttachPin(leftMotorPin, leftMotorChannel);
  ledcAttachPin(rightMotorPin, rightMotorChannel);

  // Start UDP listener
  udp.begin(udpPort);
  Serial.printf("UDP server started on port %d\n", udpPort);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    
    // Expected format: "JOY:LX:LY:RX:RY"
    if (strncmp(incomingPacket, "JOY:", 4) == 0) {
      int lx, ly, rx, ry;
      sscanf(incomingPacket, "JOY:%d:%d:%d:%d", &lx, &ly, &rx, &ry);

      int leftMotorSpeed, rightMotorSpeed;
      transformer.transform(ly, rx, leftMotorSpeed, rightMotorSpeed);

      ledcWrite(leftMotorChannel, leftMotorSpeed);
      ledcWrite(rightMotorChannel, rightMotorSpeed);
    }
  }
}
