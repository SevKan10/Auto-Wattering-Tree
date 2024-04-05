#define BLYNK_FIRMWARE_VERSION "1.0.0"
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6XAznY3Lo"
#define BLYNK_TEMPLATE_NAME "SMART HOME"
#define BLYNK_AUTH_TOKEN "DTN0jSU9VzT6GvEJr8ULeTh4nWK8ChY1"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include <RTClib.h>
RTC_DS3231 rtc;

#include <EEPROM.h>

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
bool buttonState = false;
bool buttonBlynk = false;
unsigned long setTimeInSecs;
unsigned long wait;

char daysOfTheWeek[8][12] = { "None", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

void ringBell(int delayTimes, int repeatTimes);

void setup() {
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  Wire.begin(SDA_PIN, SCL_PIN);
  EEPROM.begin(1024);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(relayPump, OUTPUT);
  pinMode(buttonPump, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  wait = millis();

  ringBell(100, 2);
  Serial.println("");
  Serial.println("PROGRAMING IS STARTED");
}

void loop() {
  buttonRelayPump();
  Blynk.run();
  DateTime now = rtc.now();

  if (buttonBlynk) {
    if (setTimeInSecs == 0) {
      digitalWrite(relayPump, HIGH);
      Blynk.virtualWrite(V4, 1);
      lcd.print(0, 0, "MÁY BƠM ĐANG MỞ ");
      Serial.println("VALUE TIME=0");
    } else {
      if (setTimeInSecs > 0 && millis() - wait > setTimeInSecs * 1000) {
        digitalWrite(relayPump, LOW);
        Blynk.virtualWrite(V4, 0);
        lcd.print(0, 0, "MÁY BƠM ĐANG TẮT");
        Serial.println("TIME OVER");
      } else {
        digitalWrite(relayPump, HIGH);
        Blynk.virtualWrite(V4, 1);
        lcd.print(0, 0, "MÁY BƠM ĐANG MỞ ");
        Serial.println("VALUE TIME>0");
      }
    }
  } else {
    digitalWrite(relayPump, LOW);
    Blynk.virtualWrite(V4, 0);
    lcd.print(0, 0, "MÁY BƠM ĐANG TẮT");
    Serial.println("BUTTON BLYNK=0");
  }
}

BLYNK_WRITE(V4) {
  buttonBlynk = param.asInt();
}

BLYNK_WRITE(V3) {
  setTimeInSecs = param[0].asLong();
}

void ringBell(int delayTimes, int repeatTimes) {
  for (int i = 1; i <= repeatTimes; i++) {
    digitalWrite(buzzer, HIGH);
    delay(delayTimes);
    digitalWrite(buzzer, LOW);
    delay(delayTimes);
  }
}

void buttonRelayPump() {
  static bool lastButtonState = false;
  bool currentButtonState = digitalRead(buttonPump);

  if (currentButtonState != lastButtonState) {
    delay(50);
    flagPump++;
    ringBell(700, 1);
    if (flagPump > 3) {
      flagPump = 1;
    }
  }

  switch (flagPump) {
    case 1:
      digitalWrite(relayPump, HIGH);
      Blynk.virtualWrite(V4, 1);
      lcd.print(0, 0, "MÁY BƠM ĐANG MỞ ");
      Serial.println("RELAY ON");
      buttonBlynk = true;
      break;

    case 2:
      digitalWrite(relayPump, LOW);
      Blynk.virtualWrite(V4, 0);
      lcd.print(0, 0, "MÁY BƠM ĐANG TẮT");
      Serial.println("RELAY OFF");
      buttonBlynk = false;
      flagPump = 3;
      break;

    case 3:
      // Additional actions can be added here for the third state
      break;
  }
  lastButtonState = currentButtonState;
}
