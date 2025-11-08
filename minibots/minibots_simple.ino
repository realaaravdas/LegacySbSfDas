#include <WiFi.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include "transform.h"

// WiFi network credentials
const char* ssid = "ROBOTWIFINET";
const char* password = "robo8711$$W";

// UDP settings
WiFiUDP broadcastUdp;
WiFiUDP dataUdp;
const int broadcastPort = 2367;
const int dataPort = 2380;
char incomingPacket[255];
const char* robotName = "minibotTestingDas";

// Motor pins - original hardware configuration
int leftMotorPin = 16;  // GPIO16 - Original left motor pin
int rightMotorPin = 19; // GPIO19 - Original right motor pin

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

  // Setup PWM output for ESP32
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);
  
  // Test if analogWrite works on these pins
  Serial.println("Testing PWM capability...");
  analogWrite(leftMotorPin, 0);
  delay(100);
  analogWrite(rightMotorPin, 0);
  delay(100);
  
  // Quick test cycle to verify motor connection
  analogWrite(leftMotorPin, 64);   // ~25% duty cycle
  delay(1000);
  analogWrite(leftMotorPin, 0);
  analogWrite(rightMotorPin, 64);  // ~25% duty cycle  
  delay(1000);
  analogWrite(rightMotorPin, 0);
  
  Serial.println("PWM pins initialized and tested");

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
    
    Serial.println("Responded to broadcast");
  }

  // Handle data packets
  int dataPacketSize = dataUdp.parsePacket();
  if (dataPacketSize) {
    int len = dataUdp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    
    Serial.print("Received packet: ");
    Serial.println(incomingPacket);
    
    if (strncmp(incomingPacket, "!ROB#", 5) == 0) {
      parseRobData(incomingPacket);
    }
  }
  
  delay(10); // Small delay to prevent overwhelming
}

void parseRobData(char* data) {
  Serial.println("Parsing robot data...");
  char* p = data;
  p += 5; // Skip "!ROB#"
  char* token;
  int values[8];
  int i = 0;
  
  token = strtok(p, ":?");
  while (token != NULL && i < 8) {
    values[i++] = atoi(token);
    Serial.print("Value ");
    Serial.print(i-1);
    Serial.print(": ");
    Serial.println(values[i-1]);
    token = strtok(NULL, ":?");
  }

  if (i >= 2) {  // Only need first 2 values for basic control
    int leftY = values[0];
    int rightX = values[1];
    
    Serial.print("Left Y: ");
    Serial.print(leftY);
    Serial.print(", Right X: ");
    Serial.println(rightX);
    
    int leftMotorSpeed, rightMotorSpeed;
    transformer.transform(leftY, rightX, leftMotorSpeed, rightMotorSpeed);

    Serial.print("Left Motor: ");
    Serial.print(leftMotorSpeed);
    Serial.print(", Right Motor: ");
    Serial.println(rightMotorSpeed);

    // Simple PWM output - this should work with most ESP32 cores
    analogWrite(leftMotorPin, leftMotorSpeed);
    analogWrite(rightMotorPin, rightMotorSpeed);
    
    // Fallback if analogWrite doesn't work
    digitalWrite(leftMotorPin, (leftMotorSpeed > 127) ? HIGH : LOW);
    digitalWrite(rightMotorPin, (rightMotorSpeed > 127) ? HIGH : LOW);
    
  } else {
    Serial.println("Insufficient data received");
  }
}
