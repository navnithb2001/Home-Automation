#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ThingSpeak.h>
#define DS18B20PIN 16
#define CHANNEL_ID 1490374
#define WRITE_API_KEY "E4NPOX101H66IZE5"
#define READ_API_KEY "AIOTW3N310SUU94Q"
WiFiClient client;
OneWire oneWire(DS18B20PIN);
DallasTemperature sensor(&oneWire);
const char* ssid = "TP-Link_5A94";
const char* password = "10977798";
unsigned int lightField = 3;
unsigned int fanField = 4;
int pir_data = 10;
int ldr = 36;
int lightInit; // initial value
int lightVal; // light reading
float tempinC;
int led1 = 2;
int led2 = 23;
int led_reading = 22;
int led_party = 19;
int led3 = 21;
int led4 = 4;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
  pinMode(pir_data, INPUT);
  pinMode(ldr , INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led_reading, OUTPUT);
  pinMode(led_party, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  lightInit = analogRead(A0);
  Serial.print("light sensor init = ");
  Serial.print(lightInit, DEC);
  Serial.println("");
  sensor.begin();
}

float readTSData( long TSChannel, unsigned int TSField ) {
 float data = ThingSpeak.readFloatField( TSChannel, TSField, READ_API_KEY );
 // Serial.println( " Data read from ThingSpeak: " + String( data, 9 ) );
 return data;
}

void loop() {
  delay(40000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
 float lightFlag = readTSData(CHANNEL_ID, lightField);
 delay(15000);
 float fanFlag = readTSData(CHANNEL_ID, fanField);
 delay(15000);
 //pir sensor *
 bool PIR_status = digitalRead(pir_data);
 if (lightFlag == 0){
 if (PIR_status) {
 Serial.println("PIR motion detected");
 digitalWrite(led1, HIGH);
 //ldr sensor***
 lightVal = ldrSensor();
 Serial.print("light sensor = ");
 Serial.print(lightVal - lightInit);
 Serial.println("");
 }
 else {
 Serial.println("NO PIR motion detected");
 digitalWrite(led1, LOW);
 Serial.println("");
 }
 ThingSpeak.writeField(CHANNEL_ID, 2, lightVal - lightInit, WRITE_API_KEY);
 delay(15000);
 }
 else if(lightFlag == 1){
 Serial.println("Manual: reading");
 digitalWrite(led2, LOW);
 digitalWrite(led_party, LOW);
 digitalWrite(led_reading, HIGH);
 }
 else if(lightFlag == 2){
 Serial.println("Manual: party");
 digitalWrite(led_reading, LOW);
 digitalWrite(led2, LOW);
 digitalWrite(led_party, HIGH);
 }
 else if(lightFlag == 3){
 Serial.println("Manual: other");
 digitalWrite(led_reading, LOW);
 digitalWrite(led_party, LOW);
 digitalWrite(led2, HIGH);
 }
 else{
 Serial.println("Manual: switch off");
 digitalWrite(led_reading, LOW);
 digitalWrite(led_party, LOW);
 digitalWrite(led2, LOW);
 }

 if(fanFlag == 0){
 if (PIR_status) {
 Serial.println("PIR motion detected");
 digitalWrite(led1, HIGH);
 //temperature sensor **
 tempinC = temperatureSensor();
 Serial.print("Temperature = ");
 Serial.print(tempinC);
 Serial.println("ºC");
 Serial.println("");
 }
 else {
 Serial.println("NO PIR motion detected");
 digitalWrite(led1, LOW);
 Serial.println("");
 }
 ThingSpeak.writeField(CHANNEL_ID, 1, tempinC, WRITE_API_KEY);
 delay(15000);
 }
 else if(fanFlag == 1){
 Serial.println("Manual: speed 1");
 digitalWrite(led4, LOW);
 digitalWrite(led3, HIGH);
 }
 else if(fanFlag == 2){
 Serial.println("Manual: speed 2");
 digitalWrite(led3, LOW);
 digitalWrite(led4, HIGH);
 }
 else{
 Serial.println("Manual: switch off");
 digitalWrite(led3, LOW);
 digitalWrite(led4, LOW);
 }
}

float temperatureSensor(){
 sensor.requestTemperatures();
 float tempinC1 = sensor.getTempCByIndex(0);
 if (tempinC1 > 28 && tempinC1 < 32) {
 digitalWrite(led4, LOW);
 digitalWrite(led3, HIGH);
 }
 else if (tempinC1 >= 32) {
  digitalWrite(led3, LOW);
 digitalWrite(led4, HIGH);
 }
 else {
 digitalWrite(led3, LOW);
 digitalWrite(led4, LOW);
 }
 return tempinC1;
 }
 int ldrSensor(){
 int lightVal1 = analogRead(A0);
 if (lightVal1 - lightInit < 100) {
 Serial.println("Bright");
 digitalWrite(led2, LOW);
 }
 else {
 Serial.println("Dark");
 digitalWrite(led2, HIGH);
 }
 return lightVal1;
}
 }
