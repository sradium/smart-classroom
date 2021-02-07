#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <FireControl.h>
#include <LightControl.h>

unsigned long last_publish;
unsigned long interval_publish;

// TODO: Convertir todas estas variables para ser configuradas de forma dinámica
byte mac[] = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 135);
IPAddress myDns(8, 8, 8, 8);
const char *server = "broker.hivemq.com";
const char *client_id = "A1314";
int port = 1883;

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

FireControler fireControler;
LigthControler ligthControler;

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

  fireControler = FireControler();
  ligthControler = LigthControler();

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

  last_publish = millis();
  interval_publish = 60000L * 1L;
}


void reconnect(){
  Serial.print("Attempting MQTT connection...");
  if (mqttClient.connect(client_id)) {
    Serial.println("connected");
    char topic [30];
    sprintf(topic, "%s/CFG/ILLUM/SCENE", client_id);
    mqttClient.subscribe(topic);
    sprintf(topic, "%s/CFG/ILLUM/DELAY", client_id);
    mqttClient.subscribe(topic);
  } else {
    Serial.print("failed, rc=");
    Serial.print(mqttClient.state());
  }
}


void report_status(){
  char topic [30];
  char payload [300];
  DynamicJsonDocument doc(24);

  doc["fire_alarm"] = fireControler.fireAlarm;
  doc["occupied"] = ligthControler.occupied;
  doc["projection_mode"] = ligthControler.projectionOn;

  serializeJson(doc, payload);
  sprintf(topic, "%s/OUT/STATUS", client_id);
  mqttClient.publish(topic, payload);
}  

// TODO: Compantar todas las funciones de report_* a un report_sensor

void report_temp(){
  char topic [30];
  char payload [500];
  DynamicJsonDocument doc(192);

  doc["value"] = 23;
  doc["unit"] = "c";
  doc["s1"]["value"] = 23.2;
  doc["s2"]["value"] = 23;
  doc["s3"]["value"] = 23.1;
  doc["s4"]["value"] = 23;
  doc["s5"]["value"] = 23.1;
  doc["s6"]["value"] = 23;

  serializeJson(doc, payload);
  sprintf(topic, "%s/OUT/TEMP", client_id);
  mqttClient.publish(topic, payload);
}


void report_hum(){
  char topic [30];
  char payload [500];
  DynamicJsonDocument doc(192);

  doc["value"] = 65;
  doc["unit"] = "percent";
  doc["s1"]["value"] = 65.2;
  doc["s2"]["value"] = 65;
  doc["s3"]["value"] = 64.1;
  doc["s4"]["value"] = 65;
  doc["s5"]["value"] = 64.1;
  doc["s6"]["value"] = 65;

  serializeJson(doc, payload);
  sprintf(topic, "%s/OUT/HUM", client_id);
  mqttClient.publish(topic, payload);
}


void report_illum(){
  char topic [30];
  char payload [500];
  DynamicJsonDocument doc(192);

  doc["unit"] = "lux";
  doc["loop1"]["value"] = 302;
  doc["loop2"]["value"] = 301;
  doc["loop3"]["value"] = 300;
  doc["loop4"]["value"] = 300;
  doc["loop5"]["value"] = 302;
  doc["loop6"]["value"] = 301;
  doc["loop7"]["value"] = 299;
  doc["loop8"]["value"] = 300;

  serializeJson(doc, payload);
  sprintf(topic, "%s/OUT/ILLUM", client_id);
  mqttClient.publish(topic, payload);
}


void report_co2(){
  char topic [30];
  char payload [500];
  DynamicJsonDocument doc(192);

  doc["value"] = 200;
  doc["unit"] = "ppm";
  doc["s1"]["value"] = 200;
  doc["s2"]["value"] = 200;
  doc["s3"]["value"] = 200;
  doc["s4"]["value"] = 200;
  doc["s5"]["value"] = 200;
  doc["s6"]["value"] = 200;

  serializeJson(doc, payload);
  sprintf(topic, "%s/OUT/CO2", client_id);
  mqttClient.publish(topic, payload);
}


void loop()
{ 
  fireControler.superviseSensors();
  ligthControler.superviseLoops();

  if (!mqttClient.connected()) {
    reconnect();
  } else {
    if(millis() - last_publish > interval_publish){
      report_status();
      report_illum();
      report_temp();
      report_co2();
      last_publish = millis();
    }
    mqttClient.loop();
  }
}