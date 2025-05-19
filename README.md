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

in `ir_receiver.ino`
1. **Capture IR Codes:**
   - Connect an IR receiver and an IR LED to the NodeMCU ESP8266.
   - Decode the IR signals from the original AC remote.
   - Test if the captured signals can control your AC (you may need to be very close to the AC).
   - Save these codes for later transmission.

in `main.ino`
1. **Update wifi configuration.**
2. **Update the IR codes with the ones you captured in `callback` function.**
3. **MQTT Integration:**
   - Sign up for a free account at [HiveMQ Cloud](https://www.hivemq.com/mqtt-cloud-broker/) (The Serverless Plan is sufficient).
   - Follow the instructions provided in the `Getting Start - Arduino` session on the HiveMQ website to install additional libraries and certificates.
   - Add your HiveMQ credentials in `main.ino`.
   - Subscribe to a topic like `ac` to receive remote commands in `reconnect` function.

4. **Send IR Commands via MQTT:**
	- Use an MQTT client (e.g., HiveMQ Web Client, EasyMQTT) to publish messages to the `ac` topic.
	- Example payloads: `open`, `up`, `down`, `wind`, etc.
	- The ESP8266 will send the corresponding IR signal via the IR LED to your air conditioner.

5. **Optional: Add Siri Shortcut:**
   - Use a third-party MQTT client (like EasyMQTT) on your iPhone.
   - Create Shortcuts that send specific MQTT messages to trigger your NodeMCU ESP8266.
