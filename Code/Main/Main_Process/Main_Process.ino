/*==========LIBRARY==========*/

#define BLYNK_FIRMWARE_VERSION "1.0.1"
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6XAznY3Lo"
#define BLYNK_TEMPLATE_NAME "SMART HOME"
#define BLYNK_AUTH_TOKEN "DTN0jSU9VzT6GvEJr8ULeTh4nWK8ChY1"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
/*---------BLYNK----------*/

#include <Wire.h>
#include <RTClib.h>
RTC_DS3231 rtc;
/*---------DS3231----------*/

#include <EEPROM.h>
/*---------EEPROM----------*/

/*==========VARIABLE & PERIPHERAL=========*/
#define buzzer 2
#define relayPump 18
#define buttonPump 15

#define SDA_PIN 21
#define SCL_PIN 19

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MINH KHA";
char pass[] = "0855508877";

WidgetLCD lcd(V0);

int flagPump = 0;
bool buttonState = 1;
bool buttonBlynk;
unsigned long currentTime;
unsigned long setTimeInSecs;
unsigned long wait; 
unsigned long timeWait; 

char daysOfTheWeek[8][12] = { "None", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

/*==========FUNCTION PROTOTYPES==========*/
void ringBell(int delayTimes, int repeatTimes);


/*==========MAIN==========*/
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Blynk.begin(auth, ssid, pass);
  Wire.begin(SDA_PIN, SCL_PIN);
  EEPROM.begin(1024);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(relayPump, OUTPUT);
  pinMode(buttonPump, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  wait = millis(); 

  ringBell(100,2);
  Serial.println("");
  Serial.println("PROGRAMING IS STARTED");
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  if (millis() - timeWait >=700){Blynk.virtualWrite(V5,random(1,100)); timeWait = millis();}

  if (!Blynk.connected() || WiFi.status() != WL_CONNECTED){ESP.restart();}
  resetFuction();
  buttonRelayPump();
  Blynk.run();
  DateTime now = rtc.now();

  if (buttonBlynk == 1) 
  {
    if (setTimeInSecs == 0)
    {
      digitalWrite(relayPump, 1);
      Blynk.virtualWrite(V4, 1);
      lcd.print(0, 0, String("MÁY BƠM ĐANG MỞ "));
      Serial.println("VALUE TIME=0");
    }
    else if (setTimeInSecs > 0 && millis() - wait > setTimeInSecs*1000+60000) 
    {
      digitalWrite(relayPump, 0);
      Blynk.virtualWrite(V4, 0);
      lcd.print(0, 0, String("MÁY BƠM ĐANG TẮT"));
      Serial.println("TIME OVER");
      ESP.restart();
    }
    else 
    {
      digitalWrite(relayPump, 1);
      Blynk.virtualWrite(V4, 1);
      lcd.print(0, 0, String("MÁY BƠM ĐANG MỞ "));
      Serial.println("VALUE TIME>0");
    }
  } 
  else if (buttonBlynk == 0) 
  {
    digitalWrite(relayPump, 0);
    Blynk.virtualWrite(V4, 0);
    lcd.print(0, 0, String("MÁY BƠM ĐANG TẮT"));
    Serial.println("BUTTON BLYNK=0");
  }
}

BLYNK_WRITE(V4) {buttonBlynk = param.asInt();}
BLYNK_WRITE(V3) {setTimeInSecs = param[0].asLong();}
