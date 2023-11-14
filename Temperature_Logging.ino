/*
# File Name      : Smart Temp Room Detection
# Customer       : - 
# Judul Research : Deteksi Suhu cerdas pada ruangan
                   Built-in ESP 8266 Wifi berbasis IOT Blynk 2.0

# Deskripsi      : Perangkat Elektronika yang digunakan
                   untuk memonitoring suhu ruangan 
                   secara realtime terhubung ke internet dengan media 
                   yang dapat di akses melalui andriod smartpgone melalui 
                   aplikasi Blynk Iot (verse. 2.0).
                
# Maker by       : Kelompok - 3 Komunikasi & jaringan IOT
# Start Created  : 11 November 2023
# Last Update    : 14 November 2023
# Revisi         : 0
# Version        : Prototype
# IoT Platform   : Blynk Iot v2.0
 
# Software Requirement
      - Arduino IDE v1.8.19
# Library
      - Blynk
      - Arduino Json
      - LCD i2C 
      - DHT Library 
# Hardware Requirement
      - Main Core : Esp-8266  Built-in WiFi
      - Input     : Sensor DHT 11 
      - Output    : Relay (High trigger), Buzzer, Led
      - Aksesoris : -
                    
# Catatan : 
 */

#define BLYNK_FIRMWARE_VERSION        "1.0"
#define BLYNK_TEMPLATE_ID "TMPL6zCfWEKjS"
#define BLYNK_DEVICE_NAME "Monitoring Suhu"
#define BLYNK_AUTH_TOKEN "yS5_MCTYrBmInLDn2N3GniOG8CdvG4IQ"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>

#define APP_DEBUG
//#define USE_WEMOS_D1_MINI
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "tselhome";  // type your wifi name
char pass[] = "merdeka1";  // type your wifi password

const int pot = A0; 
int buzzer = D5;
int LED_1 = D0;
int LED_2 = D6;
int LED_3 = D7;
int LED_4 = D8;

#define DHTPIN D3          // Mention the digital pin where you connected 
#define DHTTYPE DHT11     // DHT 11  
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Inisialisasi Port Virtual Blynk 
#define BLYNK_SENSOR_TEMP    V0
#define BLYNK_SENSOR_HUM     V1
#define BLYNK_LED1           V2
#define BLYNK_LED2           V3


void deteksi_sensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Failed to read sensor!");
    return;
  }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp:" + String(t)+" " + "℃");
    lcd.setCursor(0, 1);
    lcd.print("Hum :" + String(h)+" " + "%");
    
  Serial.println(t);
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print("Humidity : ");
  Serial.println(h);
}


void send_sensor_blynk(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  Blynk.virtualWrite(BLYNK_SENSOR_HUM, h);  
  Blynk.virtualWrite(BLYNK_SENSOR_TEMP, t);

   if(t > 33){
   // Blynk.email("shameer50@gmail.com", "Alert", "Temperature over 28C!");
    Blynk.logEvent("temp_alert","Suhu Telah Melebihi 30 C ! ");
    buzzer_on();
    }
   }

void buzzer_on(){
 digitalWrite(buzzer, HIGH);
 digitalWrite(LED_3, HIGH);
 delay(2000);
 digitalWrite(buzzer, LOW);
 digitalWrite(LED_3, LOW);
 delay(2000);
}


BLYNK_WRITE(BLYNK_LED1){
 int val = param.asInt();
 if (val == 1) 
  {
    digitalWrite(LED_1, HIGH);
  } else { 
    digitalWrite(LED_1, LOW);
}
}

BLYNK_WRITE(BLYNK_LED2){
 int val = param.asInt();
 if (val == 1) 
  {
    digitalWrite(LED_2, HIGH);
  } else { 
    digitalWrite(LED_2, LOW);
}
}


void setup(){
  lcd.begin();
  lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("KELOMPOK 3");
    lcd.setCursor(0, 1);
    lcd.print("SMART ROOM");
    delay(3000);
    lcd.clear();
 // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);
  lcd.setCursor(0, 0);
  lcd.print("Connecting to...");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  delay(2000);
  lcd.clear();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  lcd.setCursor(0, 0);
  lcd.print("Initializing . .");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  delay(2000);
  lcd.clear();
    
  }
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected...");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Start Device...");
  delay(2000);
  lcd.clear();
  pinMode(buzzer, OUTPUT);  
  pinMode(LED_1, OUTPUT);  
  pinMode(LED_2, OUTPUT);  
  pinMode(LED_3, OUTPUT);  
  pinMode(LED_4, OUTPUT);  
  pinMode(pot, INPUT);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, deteksi_sensor);
  timer.setInterval(1000L, send_sensor_blynk);
}

void loop(){
  Blynk.run();
  timer.run();
}
