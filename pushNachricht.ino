#include <WiFi.h> 
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#define BOT_TOKEN "..."
//#define CHATID "6939401605"
#define CHATID "-4125811031"  //group ID

#define TELEGRAMTEXT "Hallo"

WiFiClientSecure secured_client; 
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

const char* ssid     = "LV-PK";
const char* password = "wlMDsvlVKr6aAc0cQS";

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);

  Serial.println("Telegram Bot");
  Serial.println();
  Serial.print("[WiFi] Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay (500);
  }

  bot.sendMessage(CHATID, TELEGRAMTEXT);


}

void loop() {
  // put your main code here, to run repeatedly:

}
