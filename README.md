# 🌐 Smart Street Lighting System (IoT) with Solar Integration

An IoT-based smart street lighting system using Arduino and ESP8266 that 
automatically adjusts street light brightness based on ambient light and motion detection, 
while sending real-time data to Firebase for monitoring.

---

## 🚀 Features

- 🌙 Automatic Day/Night Detection using LDR sensor  
- 🚶 Motion Detection using Ultrasonic Sensor  
- 💡 Adaptive Brightness Control (Full / Dim / OFF)  
- ☁️ Real-time data update to Firebase  
- ⚡ Energy-efficient system (~40% power savings)  

---

## 🛠️ Tech Stack

- **Hardware:** Arduino, ESP8266 (NodeMCU)  
- **Sensors:** LDR, Ultrasonic Sensor  
- **Programming:** Arduino (C/C++)  
- **Cloud:** Firebase Realtime Database  
- **Communication:** Serial Communication (SoftwareSerial)  

---

## 📂 Project Structure


smart-street-lighting-iot/
│
├── arduino/
│ └── arduino.ino # Handles sensors & brightness logic
│
├── esp8266/
│ └── esp8266.ino # Handles WiFi & Firebase communication
│
├── config.example.h # Sample config file (no secrets)
├── README.md


---

## ⚙️ How It Works

1. **LDR Sensor** detects ambient light conditions  
2. **Ultrasonic Sensor** detects nearby motion  
3. Arduino processes the data:
   - Night + Motion → Full Brightness  
   - Night + No Motion → Dim Mode  
   - Day → Lights OFF  
4. Data is sent to ESP8266 via Serial Communication  
5. ESP8266 uploads data to Firebase in real-time  

---

## 🔌 Circuit Overview

- LDR → Analog Pin (A0)  
- Ultrasonic Sensor → Digital Pins  
- MOSFET → Controls LED brightness  
- ESP8266 ↔ Arduino → Serial Communication  

> ⚠️ Use a voltage divider when connecting Arduino TX to ESP8266 RX.

---

## 🔐 Configuration (Important)

Create a file named:

config.h

Add your credentials:

```cpp
#define WIFI_SSID "YOUR_WIFI"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define API_KEY "YOUR_API_KEY"
#define DATABASE_URL "YOUR_DATABASE_URL"
#define DATABASE_SECRET "YOUR_SECRET"

⚠️ This file is ignored using .gitignore for security reasons.

📊 Firebase Data Structure
/streetLight/
   ├── ldr: <value>
   ├── distance: <value>
   ├── brightness: <value>


📈 Results

Achieved ~40% energy savings

Improved efficiency using adaptive lighting

Enabled real-time monitoring through cloud integration


🔮 Future Improvements

Mobile app for monitoring and control

AI-based traffic prediction for smarter lighting

Integration with smart city infrastructure

Cloud dashboards for analytics


🤝 Contributions

Feel free to fork this repository and improve the project!


📌 Note


👨‍💻 Author

Beniel J

GitHub: https://github.com/beniel-j

LinkedIn: https://www.linkedin.com/in/beniel-j-049b26298