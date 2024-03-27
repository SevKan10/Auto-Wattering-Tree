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

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  lcd.clear();
  lcd.print(0, 0, String("Khang đẹp zai!")); 
}

void loop() {
  Blynk.run();
  delay(2000);
}

BLYNK_WRITE(V1) {
  TimeInputParam t(param);

  // Process start time

  if (t.hasStartTime())
  {
    Serial.println(String("Start: ") +
                   t.getStartHour() + ":" +
                   t.getStartMinute() + ":" +
                   t.getStartSecond());
  }
  else if (t.isStartSunrise())
  {
    Serial.println("Start at sunrise");
  }
  else if (t.isStartSunset())
  {
    Serial.println("Start at sunset");
  }
  else
  {
    // Do nothing
  }

  // Process stop time

  if (t.hasStopTime())
  {
    Serial.println(String("Stop: ") +
                   t.getStopHour() + ":" +
                   t.getStopMinute() + ":" +
                   t.getStopSecond());
  }
  else if (t.isStopSunrise())
  {
    Serial.println("Stop at sunrise");
  }
  else if (t.isStopSunset())
  {
    Serial.println("Stop at sunset");
  }
  else
  {
    // Do nothing: no stop time was set
  }

  // Process timezone
  // Timezone is already added to start/stop time

  Serial.println(String("Time zone: ") + t.getTZ());

  // Get timezone offset (in seconds)
  Serial.println(String("Time zone offset: ") + t.getTZ_Offset());

  // Process weekdays (1. Mon, 2. Tue, 3. Wed, ...)

  for (int i = 1; i <= 7; i++) {
    if (t.isWeekdaySelected(i)) {
      Serial.println(String("Day ") + i + " is selected");
    }
  }

  Serial.println();
}

BLYNK_WRITE(V3) {
  long startTimeInSecs = param[0].asLong();
  int hour = startTimeInSecs / 3600; // Chia tổng số giây cho 3600 để lấy số giờ
  int minute = (startTimeInSecs % 3600) / 60; // Lấy phần dư sau khi chia cho 3600, sau đó chia cho 60 để lấy số phút

  Serial.print("Hour: ");
  Serial.println(hour);
  Serial.print("Minute: ");
  Serial.println(minute);
}

