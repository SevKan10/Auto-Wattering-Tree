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
char pass[] = "085550887";

char daysOfTheWeek[8][12] = {"None","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
WidgetLCD lcd(V0);

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("Failed to connect");
    delay(5000); 
    ESP.restart(); 
  }
  lcd.clear();
  lcd.print(0, 0, String("Khang đẹp zai!")); 
}

void loop() {
  Blynk.run();
  delay(2000);
}

BLYNK_WRITE(V1)
{
  TimeInputParam t(param);

  if (t.hasStartTime())
  {
    Serial.println(String("Start: ") +
                   t.getStartHour() + ":" +
                   t.getStartMinute() + ":" +
                   t.getStartSecond());
  }

  if (t.hasStopTime())
  {
    Serial.println(String("Stop: ") +
                   t.getStopHour() + ":" +
                   t.getStopMinute() + ":" +
                   t.getStopSecond());
  }

  Serial.println(String("Time zone: ") + t.getTZ());
  Serial.println(String("Time zone offset: ") + t.getTZ_Offset());

  for (int i = 1; i <= 7; i++) {
    if (t.isWeekdaySelected(i)) {
      Serial.print(i);
      Serial.print("  ");
      Serial.println(String("Day ") + daysOfTheWeek[i]  + " is selected");
    }
  }
  Serial.println();
}

BLYNK_WRITE(V3) 
{
  long startTimeInSecs = param[0].asLong();
  int hour = startTimeInSecs / 3600; 
  int minute = (startTimeInSecs % 3600) / 60; 

  Serial.print("Hour: ");
  Serial.println(hour);
  Serial.print("Minute: ");
  Serial.println(minute);
}


