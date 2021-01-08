#include <Arduino.h>
#include <MCP4725.h>
#include <Servo.h>
#include <Stepper.h>
#include <DHT.h>

// #define DHTPIN 2
// #define DHTTYPE DHT11

#define LDRPIN 6
#define STEPPERPWR 2
#define STEPPERDIR 3
#define STOPD 4
#define STOPU 5

// Servo servo;
// DHT dht(DHTPIN, DHTTYPE);
MCP4725 LIGHT_1(0x60);
STEPPER stepper_1(STEPPERPWR, STEPPERDIR);
// int angulo = 0;
// int start = 0;

void setup()
{
  // put your setup code here, to run once:
  // Init components
  Serial.begin(9600);
  LIGHT_1.begin();
  // dht.begin();
  stepper_1.begin();
  // servo.attach(9);
 
  pinMode(LDRPIN, INPUT);
  pinMode(STOPD, INPUT);
  pinMode(STOPU, INPUT);
  // start = millis();
  // servo.write(0);
}

void projector_control(){
  if (stepper_1.start_move(digitalRead(LDRPIN)))
  {
    Serial.println("Turn on stepper motor");
  }
  if (stepper_1.stop_move(digitalRead(STOPD)))
  {
    stepper_1.change_direction();
    Serial.println("Stop motor and change his direction");
  }
  if (stepper_1.stop_move(digitalRead(STOPU)))
  {
    stepper_1.change_direction();
    Serial.println("Stop motor and change his direction");
  }
}

void lighting_control(){

}

void loop()
{
  // put your main code here, to run repeatedly:
  projector_control();          
  Serial.print("LIGHT SENSOR: ");
  Serial.println(analogRead(A0));
  delay(100);
  /*
  int adc_sound = analogRead(A0);
  Serial.print("adc: ");
  Serial.println(adc_sound);
  LIGHT_1.writeDAC(i); // 0<i<4096
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int proyector = digitalRead(LDRPIN);
  */
}