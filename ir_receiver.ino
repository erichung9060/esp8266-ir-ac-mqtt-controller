#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>

const uint16_t RECV_PIN = 2; // D4 on NodeMCU
const uint16_t SEND_PIN = 14;   // D5 on NodeMCU


IRrecv irrecv(RECV_PIN);
IRsend irsend(SEND_PIN);

decode_results results;

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();
  irsend.begin();
  Serial.println("IR Receiver and Transmitter initialized");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println("\n--- IR Code Received ---");
    Serial.println(resultToHumanReadableBasic(&results));
    Serial.println(resultToSourceCode(&results));
    Serial.println();

    if (results.decode_type == decode_type_t::UNKNOWN && results.rawlen > 0) {
      delay(1000);
      Serial.println("Sending RAW data (unknown format)...");
      irsend.sendRaw((const uint16_t*)(results.rawbuf + 1), results.rawlen - 1, 38);
    } else {
      // 否則就用已知格式發送
      delay(1000);
      Serial.println(results.decode_type);
      Serial.println(results.value);
      Serial.println(results.bits);
      irsend.send(results.decode_type, results.value, results.bits);
      // irsend.send(decode_type_t::NEC, 217256175, 32); // power
      // irsend.send(decode_type_t::NEC, 217307175, 32); // tempdown
      // irsend.send(decode_type_t::NEC, 217305135, 32); // tempup
      // irsend.send(decode_type_t::NEC, 217311255, 32); // wind
      // irsend.send(decode_type_t::NEC, 1641435311, 32); // Kolin fan
      Serial.println("IR signal resent using known protocol.");
    }

    irrecv.resume();
  }
}
