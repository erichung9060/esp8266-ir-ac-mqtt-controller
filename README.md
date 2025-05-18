# NodeMCU ESP8266 IR Air Conditioner Controller via MQTT (HiveMQ Cloud)

This project uses an NodeMCU ESP8266 microcontroller and an IR LED to simulate a SANYO SL-AS0 air conditioner remote control. By integrating HiveMQ Cloud MQTT service, the AC can be turned on or off remotely — even from outside your home. The project also supports integration with Siri Shortcuts for voice control via iPhone.

![IMG_6123](https://github.com/user-attachments/assets/92dbd7a8-1cb5-4cb6-ade0-fed6c71e1f56)

## 📦 Materials Required
- NodeMCU ESP8266 development board
- IR receiver
- IR LED
- Breadboard and jumper wires
- Micro USB cable (for power and flashing code)


## 🔧 Setup Instructions

1. **Capture IR Codes:**
   - Connect an IR receiver to the NodeMCU ESP8266.
   - Use `ir_receiver.ino` to decode the IR signals from the original AC remote and test if it can actually control the AC (you may need to be very close for it to work).
   - Save these codes for later transmission.

2. **MQTT Integration:**
   - Sign up for a free account at [HiveMQ Cloud](https://www.hivemq.com/mqtt-cloud-broker/) (The Serverless Plan is sufficient).
   - In `main.ino`, Configure your NodeMCU ESP8266 to connect to HiveMQ using your credentials.
   - Subscribe to a topic like `ac` to receive remote commands.

3. **Send IR Commands:**
   - When a message is received via MQTT (e.g., `open`, `up`, `down`), the NodeMCU ESP8266 sends the corresponding IR code via the IR LED to control the AC.

4. **Optional: Add Siri Shortcut:**
   - Use a third-party MQTT client (like EasyMQTT) on your Iphone.
   - Create Shortcuts that send specific MQTT messages to trigger your NodeMCU ESP8266.
