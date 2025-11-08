#include <WiFi.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include <cstring>
#include <cstdlib>
#include "transform.h"

// ESP32 specific LEDC includes for maximum compatibility
#if defined(ARDUINO_ARCH_ESP32) || defined(ESP32)
#include "driver/ledc.h"
#include "esp_err.h"
#endif

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

// PWM settings
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int leftMotorChannel = 0;
const int rightMotorChannel = 1;

Transform transformer(true, false);

// ESP32 LEDC implementation
#if defined(ARDUINO_ARCH_ESP32) || defined(ESP32)

void setupLEDCPWM(int pin, int channel, int freq = 5000, int resolution = 8) {
    Serial.print("Setting up LEDC PWM on pin ");
    Serial.print(pin);
    Serial.print(" channel ");
    Serial.println(channel);
    
    // Configure LEDC timer
    ledc_timer_config_t timer_conf = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = (ledc_timer_bit_t)resolution,
        .freq_hz          = freq,
        .clk_cfg          = LEDC_AUTO_CLK,
        .timer_num        = (ledc_timer_t)(channel / 4) // Each timer can drive 4 channels
    };
    
    esp_err_t timer_result = ledc_timer_config(&timer_conf);
    if (timer_result == ESP_OK) {
        Serial.println("LEDC timer configured successfully");
    } else {
        Serial.print("LEDC timer config failed: ");
        Serial.println(esp_err_to_name(timer_result));
    }
    
    // Configure LEDC channel
    ledc_channel_config_t channel_conf = {
        .gpio_num       = (gpio_num_t)pin,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = (ledc_channel_t)channel,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = (ledc_timer_t)(channel / 4),
        .duty           = 0,
        .hpoint         = 0
    };
    
    esp_err_t channel_result = ledc_channel_config(&channel_conf);
    if (channel_result == ESP_OK) {
        Serial.println("LEDC channel configured successfully");
    } else {
        Serial.print("LEDC channel config failed: ");
        Serial.println(esp_err_to_name(channel_result));
    }
}

void writeLEDCPWM(int channel, int duty) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel);
}

#endif

// Fallback for non-ESP32 or LEDC not available
void setupFallbackPWM(int pin, int channel) {
    Serial.print("Setting up fallback PWM on pin ");
    Serial.println(pin);
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void writeFallbackPWM(int pin, int value) {
    // Simple digital simulation for testing
    digitalWrite(pin, (value > 127) ? HIGH : LOW);
    
    // Try analogWrite if available
    if (value <= 255) {
        analogWrite(pin, value);
    }
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Give serial time to initialize
  
  Serial.println("ESP32 Minibots - LEDC PWM Version");
  Serial.println("===================================");

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup PWM
  Serial.println("Setting up PWM for motor control...");
  
  #if defined(ARDUINO_ARCH_ESP32) || defined(ESP32)
    // Try LEDC first
    setupLEDCPWM(leftMotorPin, leftMotorChannel, pwmFreq, pwmResolution);
    setupLEDCPWM(rightMotorPin, rightMotorChannel, pwmFreq, pwmResolution);
    
    // Test LEDC outputs
    Serial.println("Testing LEDC outputs...");
    writeLEDCPWM(leftMotorChannel, 128); // 50% duty
    delay(1000);
    writeLEDCPWM(leftMotorChannel, 0);
    writeLEDCPWM(rightMotorChannel, 128); // 50% duty  
    delay(1000);
    writeLEDCPWM(rightMotorChannel, 0);
    Serial.println("LEDC test completed");
    
  #else
    // Fallback for other platforms
    Serial.println("Using fallback PWM method");
    setupFallbackPWM(leftMotorPin, leftMotorChannel);
    setupFallbackPWM(rightMotorPin, rightMotorChannel);
  #endif

  Serial.println("PWM setup completed");

  // Start UDP listeners
  broadcastUdp.begin(broadcastPort);
  dataUdp.begin(dataPort);
  Serial.printf("Broadcast UDP server started on port %d\n", broadcastPort);
  Serial.printf("Data UDP server started on port %d\n", dataPort);
  
  Serial.println("Setup complete. Ready for DriverStation connection.");
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
    
    Serial.println("Responded to broadcast packet");
  }

  // Handle data packets
  int dataPacketSize = dataUdp.parsePacket();
  if (dataPacketSize) {
    int len = dataUdp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    
    Serial.print("Received data packet: ");
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

    Serial.print("Left Motor PWM: ");
    Serial.print(leftMotorSpeed);
    Serial.print(", Right Motor PWM: ");
    Serial.println(rightMotorSpeed);

    // Apply motor control
    #if defined(ARDUINO_ARCH_ESP32) || defined(ESP32)
      writeLEDCPWM(leftMotorChannel, leftMotorSpeed);
      writeLEDCPWM(rightMotorChannel, rightMotorSpeed);
    #else
      writeFallbackPWM(leftMotorPin, leftMotorSpeed);
      writeFallbackPWM(rightMotorPin, rightMotorSpeed);
    #endif
    
  } else {
    Serial.println("Insufficient data received");
  }
}
