#include "MicroGear.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

const char* ssid     = "Dragon6830";
const char* password = "krit1234";

#define APPID       "AKPlantSensor"
#define GEARKEY     "YlcPGKABMaTFkEl"
#define GEARSECRET  "KOP6Z5frttdNk1QvAuNesWpIT"
#define ALLAS       "Plant001"
#define SCOPE       ""

const int moistureAO = A0;
WiFiClient client;
AuthClient *authclient;

int moisture = 0;

int timer = 0;
MicroGear microgear(client);

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  microgear.setName("soil_moisture_sensor");
}

void setup() {
  Serial.begin(115200);

  microgear.on(CONNECTED, onConnected);

  Serial.println("Starting...");

  if (WiFi.begin(ssid, password)) {

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    //uncomment the line below if you want to reset token -->
    microgear.resetToken();
    microgear.init(GEARKEY, GEARSECRET, SCOPE);
    microgear.connect(APPID);
  }
}

void loop() {
  if (microgear.connected()) {
    microgear.loop();
    timer = 0;

    Serial.println("connected");
    microgear.loop();
    moisture = analogRead( moistureAO );
    //Serial.println(moisture);

    int tmp_moi = (int)((moisture - 424) / 6);
    if (tmp_moi > 100) {
      tmp_moi = 100;
    }

    if (tmp_moi < 0) {
      tmp_moi = 0;
    }

    tmp_moi = 100 - tmp_moi;

    char msg[128];
    sprintf(msg, "%d,,,soil_moisture_sensor,soil moisture sensor", (int)tmp_moi);

    microgear.chat("webapp", msg);

  } else {
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
    delay(timer);
    timer += 500;
  }
  delay(1000);
}
