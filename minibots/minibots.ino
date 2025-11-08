#include <WiFi.h>
#include <WiFiUdp.h>
#include "transform.h"

// WiFi network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// UDP settings
WiFiUDP broadcastUdp;
WiFiUDP dataUdp;
const int broadcastPort = 2367;
const int dataPort = 2380;
char incomingPacket[255];
const char* robotName = "minibot";


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

  // Start UDP listeners
  broadcastUdp.begin(broadcastPort);
  dataUdp.begin(dataPort);
  Serial.printf("Broadcast UDP server started on port %d\n", broadcastPort);
  Serial.printf("Data UDP server started on port %d\n", dataPort);
}

void loop() {
  // Handle broadcast packets
  int broadcastPacketSize = broadcastUdp.parsePacket();
  if (broadcastPacketSize) {
    int len = broadcastUdp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    // The broadcast message from the host is its IP address.
    // We just need to respond to the sender with our robot name.
    String response = "ROB:" + String(robotName);
    broadcastUdp.beginPacket(broadcastUdp.remoteIP(), broadcastUdp.remotePort());
    broadcastUdp.write((const uint8_t*)response.c_str(), response.length());
    broadcastUdp.endPacket();
  }

  // Handle data packets
  int dataPacketSize = dataUdp.parsePacket();
  if (dataPacketSize) {
    int len = dataUdp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    
    if (strncmp(incomingPacket, "!ROB#", 5) == 0) {
      parseRobData(incomingPacket);
    }
  }
}

void parseRobData(char* data) {
  char* p = data;
  p += 5; // Skip "!ROB#"
  char* token;
  int values[8];
  int i = 0;
  
  token = strtok(p, ":?");
  while (token != NULL && i < 8) {
    values[i++] = atoi(token);
    token = strtok(NULL, ":?");
  }

  if (i == 8) {
    int leftY = values[0];
    int rightX = values[1];
    
    int leftMotorSpeed, rightMotorSpeed;
    transformer.transform(leftY, rightX, leftMotorSpeed, rightMotorSpeed);

    ledcWrite(leftMotorChannel, leftMotorSpeed);
    ledcWrite(rightMotorChannel, rightMotorSpeed);
  }
}
