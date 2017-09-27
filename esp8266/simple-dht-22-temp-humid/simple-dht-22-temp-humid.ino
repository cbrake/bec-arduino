
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <DHT.h>
//DHT dht(2, DHT22, 20);
const char *ssid =   "bec3";
const char *pass =   "beclinux";
long previousMillis = 0;
long interval = 60000;
IPAddress server(192, 168, 1, 139);

WiFiClient wclient
PubSubClient client(wclient, server);

void setup(){
  //dht.begin();
  WiFi.begin(ssid, pass);
client.connect("clientName");
}

void loop(){
  client.loop();
  unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) {
      /*
      float h = dht.readHumidity();
      float f = dht.readTemperature(true);
        if (isnan(h) || isnan(f)) {
          return;
        }
        */
      previousMillis = currentMillis;
      client.publish("m_bed/temperature",String(25));
      client.publish("m_bed/humidity",String(70));
  }
}
