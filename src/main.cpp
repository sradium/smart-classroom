#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <UIPEthernet.h>
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

void load_conf() {
  Serial.println(); 
  File file = SD.open("FIRE.txt");
  if (!file) {
    Serial.println(F("Failed to read file"));
    while(true);
  }
  StaticJsonDocument<384> doc;
  DeserializationError error = deserializeJson(doc, file);
  if (error){
    Serial.println(F("Failed to read file, using default configuration"));
    Serial.println(error.f_str());
    while(true);
  }
  JsonArray FireEquipment = doc["FireEquipment"];
  fireControler = FireControler(FireEquipment);
  delay(15);
  file.close();

  Serial.println();  
  File file2 = SD.open("ILLUM.txt");
  if (!file2) {
    Serial.println(F("Failed to read file"));
    while(true);
  }
  StaticJsonDocument<768> doc2;
  error = deserializeJson(doc2, file2);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    while(true);
  }
  JsonObject illuminationEquipment = doc2["illuminationEquipment"];
  ligthControler = LigthControler(illuminationEquipment);
  delay(15);
}

void setup()
{
  Serial.begin(9600);

  Serial.println("Initialize Ethernet");
  Ethernet.init(53);
  Ethernet.begin(mac, ip, myDns);
  Serial.println("My IP address: ");
  Serial.println(Ethernet.localIP());
  
  Serial.println();
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset or reopen this serial monitor after fixing your issue!");
    while (true);
  }
  Serial.println("initialization done.");
  load_conf();
  Serial.println();

  mqttClient.setServer(server, port);
  mqttClient.setCallback(on_message);
  last_publish = millis();
  interval_publish = 300000L * 1L;
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
    Serial.println(mqttClient.state());
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
    // reconnect();
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