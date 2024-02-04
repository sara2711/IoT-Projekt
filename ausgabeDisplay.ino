#include <Wire.h>
#include <SeeedOLED.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <WiFi.h> 
#include <WiFiClient.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

int btnGPIO = 0;
int btnState = false;

Adafruit_BME680 bme;
WiFiClient client;

const char* ssid     = "UPC0605169";
const char* password = "Pcs6ka7Fuyvm";
char servername[] = "worldtimeapi.org";

void setup() {
  Serial.begin(115200);
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  bme.begin(0x76);
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.performReading();

  pinMode(btnGPIO, INPUT);

  Serial.println();
  Serial.print("[WiFi] Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

}

void loop() {

  btnState = digitalRead(btnGPIO);
  if (btnState == LOW) {
    // Disconnect from WiFi
    WiFi.disconnect(true);
    Serial.println("Disconnected from WiFi!");
    delay(1000);
  }

  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :");
    return;
  }

  String Uhrzeit = gettime();
  Serial.println(Uhrzeit);
  delay(1000);
  String Datum = getdate();
  Serial.println(Datum);
  delay(1000);

  werteDisplay();
  delay(5000);

}

void werteDisplay(){

  String temp;
  String hum; 

  temp = String(bme.temperature) + "C"; 
  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(4,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Temp:");      //Print the String
  SeeedOled.putString(temp.c_str());
   

  hum = String(bme.humidity) + "%"; 
  SeeedOled.setTextXY(5,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Humi:");      //Print the String
  SeeedOled.putString(hum.c_str()); 

  }

String gettime() {

  String uhrzeit;
  if (client.connect(servername, 80)) {
    Serial.println("connected");
    client.println("GET /api/timezone/Europe/Berlin.txt HTTP/1.1");
    client.println();

    while (!client.available());  //wait for data
    String time;

    while (client.available()) {
      char c = client.read();
      if (c != '\n') {
        time += c;

      } else {

        if (time.startsWith("datetime")) {
          //Serial.println("time " +time);
          String uhrzeit = time.substring(21, 29);
          Serial.println(uhrzeit);
          SeeedOled.init();
          SeeedOled.clearDisplay();
          SeeedOled.setNormalDisplay();
          SeeedOled.setPageMode();
          SeeedOled.setTextXY(0,0);
          SeeedOled.putString(uhrzeit.c_str());
        }
        time = "";
      }
    }
  }
  return uhrzeit;
}

String getdate() {

  String datum;

  if (client.connect(servername, 80)) {
    Serial.println("connected");
    client.println("GET /api/timezone/Europe/Berlin.txt HTTP/1.1");
    client.println();

    while (!client.available());  //wait for data

    String date;

    while (client.available()) {
      char c = client.read();
      if (c != '\n') {
        date += c;
      } else {

        if (date.startsWith("datetime")) {
          //Serial.println("date: "  +date);
          String datum = date.substring(10, 20);
          Serial.println(datum);
          SeeedOled.init();
          SeeedOled.clearDisplay();
          SeeedOled.setNormalDisplay();
          SeeedOled.setPageMode();
          SeeedOled.setTextXY(2,0);
          SeeedOled.putString(datum.c_str());
        }
        date = "";
      }
    }
  }
  return datum;
}

