#include <WiFi.h> 
#include <WiFiClient.h> 
#include <PubSubClient.h>
#include "Arduino.h"
#define RXD2 16
#define TXD2 17

const char* ssid = "arifin";  
const char* password = "12345678";
const char* mqtt_server = "202.154.58.25"; 
const char* mqtt_user = "logika"; 
const char* mqtt_password = "pratama";
WiFiClient espClient;
PubSubClient client(espClient);

unsigned char incomingByte;
int a;
String karakter;
int Panjang;
String Data;
String Str1;
String hasil;
String idReader = "414753303031";
String Kirim;
String stringOne;


long lastMsg = 0;
char msg[20];

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);
  String payloadtemp;

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void mqttconnect() {

  while (!client.connected()) {
    Serial.print("MQTT connecting ...");

    String clientId = "ESP32Client";

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else{
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }

}

void setup ()
{
  Serial.begin (115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial2.println("serial2test");
  Serial.println ("begin initial Serial!\n");
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
  client.setCallback(receivedCallback);
  }
  
}

void loop ()
{
  if (!client.connected()) {
    mqttconnect();
  }
  client.loop();
  
  while(Serial2.available () > 0)
  {
      incomingByte=Serial2.read ();
      karakter = String(incomingByte, HEX);
      Panjang = karakter.length();
      if(Panjang == 1){
        Str1 = "0" + karakter;
      }else{
      Str1 = karakter;
      }
      hasil = hasil + Str1;
      
     stringOne = hasil;
      stringOne.replace("20", "");
      stringOne.replace("0d0a", "");
      stringOne.replace("30303030312c30312c30342c", "");
       Data = stringOne;
       Kirim = Data + idReader;
  } 

  if(hasil != ""){
    Serial.println(Kirim);
    client.publish("reader/reader", String(Kirim).c_str(), true);
    hasil="";      
  }
  delay (1000);
      long now = millis();
  if (now - lastMsg > 3000) {
    lastMsg = now;
  }
}
