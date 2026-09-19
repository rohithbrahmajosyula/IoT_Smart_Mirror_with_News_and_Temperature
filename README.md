# IoT Smart Mirror with News and Temperature
# Overview
IoT Smart Mirror is an IoT-based project that combines a mirror with a display system to provide useful information such as temperature, humidity, date, time, and news updates.
The system uses a Raspberry Pi Pico, DHT11 temperature and humidity sensor, and a 20x4 LCD display. A local web dashboard is also provided for monitoring sensor values, updating date and time, and controlling switches.
# Features
- Displays date and time
- Displays temperature and humidity
- Displays news updates
- Provides a local web dashboard
- Allows date and time configuration through the web interface
- Provides four switch controls
- Uses a mirror and display for an integrated smart-mirror setup
# Hardware
- Raspberry Pi Pico
- DHT11 Temperature and Humidity Sensor
- 20x4 LCD Display
- Jumper Wires
- Mirror
- Connector/Cable
- Cardboard or frame for assembly
# Software
- Arduino IDE
- C/C++
- Wi-Fi
- Web Server
- I2C LCD
- DHT11 sensor library
# Working
The DHT11 sensor measures temperature and humidity and sends the readings to the Raspberry Pi Pico.
The Raspberry Pi Pico processes the sensor data and displays the information on the 20x4 LCD.
The system also creates a local Wi-Fi access point and provides a web dashboard. Through the dashboard, users can:
- View temperature and humidity
- View the current date and time
- Update the date and time
- Control four switches
The project uses a mirror with the display system to create the smart-mirror interface.
# System Flow
```text
DHT11 Sensor
     |
     | Temperature & Humidity
     v
Raspberry Pi Pico
     |
     +-------------> 20x4 LCD
     |                    |
     |                    v
     |              Smart Mirror
     |
     +-------------> Local Web Dashboard
                           |
                           +-- Date & Time
                           +-- Temperature
                           +-- Humidity
                           +-- Switch Controls
```
# Web Dashboard
The Raspberry Pi Pico creates a local Wi-Fi access point named:

```text
SmartMirrorAP
```
The dashboard can be accessed through the device's local IP address.
The dashboard provides monitoring and control functions within the same Wi-Fi network.
# Project Structure
```text
IoT-Smart-Mirror/
│
├── code/
│   └── smart_mirror.ino
│
├── README.md
│
└── hardware/
    └── circuit/
```
# How to Run
1. Install Arduino IDE.
2. Connect the Raspberry Pi Pico to the computer.
3. Open the project code in Arduino IDE.
4. Install the required libraries.
5. Select the appropriate board and port.
6. Upload the code to the Raspberry Pi Pico.
7. Connect the DHT11 sensor and 20x4 LCD according to the circuit design.
8. Power the system.
9. Connect to the `SmartMirrorAP` Wi-Fi network.
10. Open the device IP address in a web browser to access the dashboard.
# Project Result

The completed prototype displays:
- Date and time
- Temperature
- Humidity
- Smart-mirror information
- Web-based control options
The project demonstrates the integration of IoT, sensors, a microcontroller, LCD display, Wi-Fi networking, and a web interface into a smart-mirror system.
# Future Scope
The system can be extended with:
- Home automation
- Light and fan control
- Voice interaction
- Additional sensors
- Weather information
- Calendar integration
- Mobile device integration
- Google Maps and location-based features
- Virtual trial-room functionality


