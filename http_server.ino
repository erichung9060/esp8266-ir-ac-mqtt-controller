#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>

const char* ssid = "your wifi ssid";
const char* pass = "your wifi password";
ESP8266WebServer server(80);

void rootRouter(){
    server.send(200, "text/html", "<form action=\"/door\" method=\"post\"><input type=\"password\" name=\"password\" placeholder=\"Password\"/><input type=\"submit\" name=\"action\" value=\"Open\" /><input type=\"submit\" name=\"action\" value=\"Close\" /></form>");
}

void doorRouter(){
    String password = server.arg("password");
    if (password != "password"){
        server.send(200, "text/html", "Wrong Password! <a href=\"/\">Back</>");
        return;
    }

    String action = server.arg("action");
    if (action == "Open"){
        digitalWrite(LED_BUILTIN, LOW);
        server.send(200, "text/html", "Opened! <a href=\"/\">Back</>");

    }else if (action == "Close"){
        digitalWrite(LED_BUILTIN, HIGH);
        server.send(200, "text/html", "Closed! <a href=\"/\">Back</>");

    }
}

void setup(){
    Serial.begin(115200);
    
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    
    server.on("/index.html", rootRouter);
    server.on("/", rootRouter);
    server.on("/door", doorRouter);

    server.onNotFound([]() {
        server.send(404, "text/plain", "Page NOT found!");
    });

    server.begin();
    Serial.println("Server is now running on IP: ");
    Serial.println(WiFi.localIP());
    
}

void loop(){
    server.handleClient();
}