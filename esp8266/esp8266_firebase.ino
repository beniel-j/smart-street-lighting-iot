#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <SoftwareSerial.h>
#include "config.h"

// WiFi & Firebase Credentials
#define WIFI_SSID "your_wifi"
#define WIFI_PASSWORD "your_password"
#define API_KEY "your_api_key"
#define DATABASE_URL "your_database_url"
#define DATABASE_SECRET "your_secret"

// SoftwareSerial: RX = D5 (GPIO 14), TX = D6 (GPIO 12 - not used)
SoftwareSerial arduinoSerial(14, 12); 

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long lastHeartbeat = 0;

void setup() {
  Serial.begin(9600);
  arduinoSerial.begin(9600); 
  delay(1000);
  
  Serial.println("\n\n--- ESP8266 D5 RECEIVER READY ---");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("Listening for Arduino data on Pin D5...");
}

void loop() {
  // Check if data is arriving from Arduino
  if (arduinoSerial.available() > 0) {
    String data = arduinoSerial.readStringUntil('\n');
    data.trim();

    if (data.startsWith("LDR:")) {
      Serial.println("📥 Data Captured: " + data);

      // Parsing the data
      int ldr = data.substring(data.indexOf("LDR:") + 4, data.indexOf(",DIS:")).toInt();
      int distance = data.substring(data.indexOf("DIS:") + 4, data.indexOf(",BRI:")).toInt();
      int brightness = data.substring(data.indexOf("BRI:") + 4).toInt();

      // Push to Firebase
      if (Firebase.ready()) {
        Firebase.RTDB.setInt(&fbdo, "/streetLight/ldr", ldr);
        Firebase.RTDB.setInt(&fbdo, "/streetLight/distance", distance);
        Firebase.RTDB.setInt(&fbdo, "/streetLight/brightness", brightness);
        Serial.println("🔥 Firebase Updated!");
      }
    }
  }

  // Heartbeat: Print every 10 seconds just to show the ESP isn't frozen
  if (millis() - lastHeartbeat > 10000) {
    Serial.println("...Still listening on D5...");
    lastHeartbeat = millis();
  }
}