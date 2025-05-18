# ESP8266 IR Air Conditioner Controller via MQTT (HiveMQ Cloud)

This project uses an ESP8266 NodeMCU microcontroller and an IR LED to simulate a SANYO SL-AS0 air conditioner remote control. By integrating HiveMQ Cloud MQTT service, the AC can be turned on or off remotely — even from outside your home. The project also supports integration with Siri Shortcuts for voice control via iPhone.


## 📦 Materials Required
- ESP8266 NodeMCU development board
- IR receiver
- IR LED
- Breadboard and jumper wires
- Micro USB cable (for power and flashing code)


## 🔧 Setup Instructions

1. **Capture IR Codes:**
   - Connect an IR receiver to the ESP8266.
   - Use `ir_receiver.ino` to decode the IR signals from the original AC remote and test if it can turly control the ac(the distance may need to be very close).
   - Save these codes for later transmission.

2. **MQTT Integration:**
   - Sign up for a free account at [HiveMQ Cloud](https://www.hivemq.com/mqtt-cloud-broker/).
   - Configure your ESP8266 to connect to HiveMQ using your credentials in `main.ino`.
   - Subscribe to a topic like `ac` to receive remote commands.

3. **Send IR Commands:**
   - When a message is received via MQTT (e.g., `open`, `up`, `down`), the ESP8266 sends the corresponding IR code via the IR LED to control the AC.

4. **Optional: Add Siri Shortcut:**
   - Use a third-party MQTT client (like EasyMQTT).
   - Create Shortcuts that send specific MQTT messages to trigger your ESP8266.
