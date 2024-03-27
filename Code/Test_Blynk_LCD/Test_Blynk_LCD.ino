#define BLYNK_FIRMWARE_VERSION        "1.0.0"
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6XAznY3Lo"
#define BLYNK_TEMPLATE_NAME "SMART HOME"
#define BLYNK_AUTH_TOKEN "DTN0jSU9VzT6GvEJr8ULeTh4nWK8ChY1"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MINH KHA";
char pass[] = "0855508877";

WidgetLCD lcd(V0);

void setup() 
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  lcd.clear();
  lcd.print(0, 0, String("Khang đẹp zai!")); 
}

void loop() 
{
  Blynk.run();
  delay(2000);
}