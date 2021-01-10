#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


bool occupied = false;
bool fire_alarm = false;
bool projection_mode = false;

int setpoint = 300;
int highest = 100;
int high = 70;
int medium = 45;
int low = 0;

unsigned long start_time;

// TODO: Convertir todas estas variables para ser configuradas de forma dinámica
byte mac[] = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 135);
IPAddress myDns(200, 44, 32, 12);
const char *server = "broker.hivemq.com";
const char *client_id = "A1314";
int port = 1883;

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);


void on_message(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup()
{
  Serial.begin(9600);

  occupied = false;
  fire_alarm = false;
  projection_mode = false;

  Serial.println("Initialize Ethernet with DHCP:");
  Ethernet.init(53);
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware");
      while (true)
        ;
    }
    else if (Ethernet.linkStatus() == LinkOFF)
    {
      Serial.println("Ethernet cable is not connected");
    }
    else
    {
      Ethernet.begin(mac, ip, myDns);
      Serial.println("My IP address: ");
      Serial.println(Ethernet.localIP());
    }
  }
  else
  {
    Serial.println("DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  delay(1500);

  mqttClient.setServer(server, port);
  mqttClient.setCallback(on_message);

  start_time = millis();
}

void reconnect(){
  Serial.print("Attempting MQTT connection...");
  if (mqttClient.connect(client_id)) {
    Serial.println("connected");
    char topic [20];
    sprintf(topic, "%s/CFG/ILLUM", client_id);
    mqttClient.subscribe(topic);
  } else {
    Serial.print("failed, rc=");
    Serial.print(mqttClient.state());
  }
}

void report_status(){
  char topic [20];
  char payload [300];
  DynamicJsonDocument doc(24);

  doc["occupied"] = occupied;
  doc["fire_alarm"] = fire_alarm;
  doc["projection_mode"] = projection_mode;

  serializeJson(doc, payload);
  sprintf(topic, "%s/OUT/STATUS", client_id);
  mqttClient.publish(topic, payload);
}  

void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  } else {
    if(millis() - start_time > 60000){
      report_status();
      start_time = millis();
    }
    mqttClient.loop();
  }
}