#include <SoftwareSerial.h>

#define LDR_PIN A0
#define TRIG_PIN 6
#define ECHO_PIN 7
#define MOSFET_PIN 5

// Use Pin 3 for the ESP8266 (Voltage Divider required on this path!)
SoftwareSerial espSerial(2, 3); 

// --- SETTINGS ---
int nightThreshold = 600;   // Level to turn light ON (Adjust based on Serial Monitor)
int hysteresis = 150;       // Buffer to prevent blinking when LED turns on
int motionDistance = 25;    // Detection range in cm
bool isNight = false;       // Track state to prevent rapid switching

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(MOSFET_PIN, OUTPUT);
  
  Serial.begin(9600);
  espSerial.begin(9600); 
  
  Serial.println("--- System Started: Monitoring LDR & Motion ---");
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  float distance = getDistance();
  int brightness = 0;

  // --- LOGIC: NIGHT DETECTION WITH HYSTERESIS ---
  // If light is currently OFF, wait for it to get darker than nightThreshold
  if (!isNight && ldrValue < nightThreshold) {
    isNight = true;
  } 
  // If light is currently ON, it must get significantly brighter than nightThreshold + hysteresis to turn OFF
  else if (isNight && ldrValue > (nightThreshold + hysteresis)) {
    isNight = false;
  }

  // --- LOGIC: BRIGHTNESS CONTROL ---
  if (isNight) {
    if (distance <= motionDistance) {
      brightness = 255;  // Motion detected: Full Bright
    } else {
      brightness = 50;   // No motion: Dim Mode (Stable)
    }
  } else {
    brightness = 0;      // Day Mode: Light OFF
  }

  analogWrite(MOSFET_PIN, brightness);

  // --- DATA COMMUNICATION ---
  // Format for ESP8266/Firebase: LDR:123,DIS:45,BRI:255
  String dataString = "LDR:" + String(ldrValue) + 
                      ",DIS:" + String((int)distance) + 
                      ",BRI:" + String(brightness);
                      
  espSerial.println(dataString); // Send to ESP8266
  Serial.println("Sent: " + dataString); // Debug to PC

  delay(800); // Slower loop for better power stability
}