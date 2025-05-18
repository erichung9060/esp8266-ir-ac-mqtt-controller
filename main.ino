#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <TZ.h>
#include <FS.h>
#include <LittleFS.h>
#include <CertStoreBearSSL.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>

const uint16_t SEND_PIN = 14;   // D5 on NodeMCU
IRsend irsend(SEND_PIN);

// Update these with values suitable for your network.
const char* ssid = "your wifi ssid";
const char* password = "your wifi password";
const char* mqtt_server = "your HivemMQ cloud server adddress";
const char* mqtt_username = "your HiveMQ Cloud server username";
const char* mqtt_password = "your HiveMQ Cloud server password";


// A single, global CertStore which can be used by all connections.
// Needs to stay live the entire time any of the WiFiClientBearSSLs
// are present.
BearSSL::CertStore certStore;

WiFiClientSecure espClient;
PubSubClient * client;

void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}


void setDateTime() {
    // You can use your own timezone, but the exact time is not used at all.
    // Only the date is needed for validating the certificates.
    configTime(TZ_Europe_Berlin, "pool.ntp.org", "time.nist.gov");

    Serial.print("Waiting for NTP time sync: ");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(100);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println();

    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.printf("%s %s", tzname[0], asctime(&timeinfo));
}

void IR_Sent(uint64_t code){
    digitalWrite(LED_BUILTIN, LOW);
    irsend.send(decode_type_t::NEC, code, 32);
    delay(0.5);
    digitalWrite(LED_BUILTIN, HIGH);
}

void callback(char* topic, byte* payload, unsigned int length) {
    if(length == 0) return;

    if(strcmp(topic, "ac") == 0){
        if ((char)payload[0] == 'o') {
            IR_Sent(217256175); // power
            client->publish("ac", "The power is turned on.", true);
        }
        if ((char)payload[0] == 'u') {
            IR_Sent(217305135); // tempup
            delay(500);
            IR_Sent(217305135); // tempup
            client->publish("ac", "Temperature has risen.", true);
        }
        if ((char)payload[0] == 'd') {
            IR_Sent(217307175); // tempdown
            delay(500);
            IR_Sent(217307175); // tempdown
            client->publish("ac", "Temperature has dropped.", true);
        }
        if ((char)payload[0] == 'w') {
            IR_Sent(217311255); // wind
            client->publish("ac", "Air volume has been enhanced.", true);
        }
    }
}


void reconnect() {
  // Loop until we’re reconnected
    while (!client->connected()) {
        Serial.println("Attempting MQTT connection ...");
        String clientId = "ESP8266Client - MyClient";
        // Attempt to connect
        // Insert your password
        if (client->connect(clientId.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("connected");
            // Once connected, publish an announcement…
            client->publish("ac", "ESP8266 is ready");
            client->subscribe("ac");

        } else {
            Serial.print("failed, rc = ");
            Serial.print(client->state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);

    LittleFS.begin();
    setup_wifi();
    setDateTime();

    pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output

    // you can use the insecure mode, when you want to avoid the certificates
    // espclient->setInsecure();

    int numCerts = certStore.initCertStore(LittleFS, PSTR("/certs.idx"), PSTR("/certs.ar"));
    Serial.printf("Number of CA certs read: %d\n", numCerts);
    if (numCerts == 0) {
        Serial.printf("No certs found.\n");
        Serial.printf("Did you run certs-from-mozilla.py and upload the LittleFS directory before running?\n");
        return; // Can't connect to anything w/o certs!
    }

    BearSSL::WiFiClientSecure *bear = new BearSSL::WiFiClientSecure();
    // Integrate the cert store with this connection
    bear->setCertStore(&certStore);

    client = new PubSubClient(*bear);

    client->setServer(mqtt_server, 8883);
    client->setCallback(callback);

    irsend.begin();
    Serial.println("IR Receiver and Transmitter initialized");
}

void loop() {
    if (!client->connected()) {
        reconnect();
    }
    client->loop();
}
