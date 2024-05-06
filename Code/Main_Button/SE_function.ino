void ringBell(int delayTimes, int repeatTimes)
{
  for(int i=1; i<=repeatTimes; i++)
  {
    digitalWrite(buzzer, 1); delay(delayTimes);
    digitalWrite(buzzer, 0); delay(delayTimes);
  }
}

void buttonRelayPump() {
  static bool lastButtonState = 1; 
  static bool relayState = 0; // Trạng thái ban đầu của relay
  bool currentButtonState = digitalRead(buttonPump);

  if (currentButtonState != lastButtonState) {
    // Chờ một khoảng thời gian để chống nhiễu
    delay(100);  
    
    // Nếu nút nhấn được nhấn
    if (currentButtonState == 1) {
      // Đảo trạng thái của relay
      relayState = !relayState;
      delay(100);
      // Đặt trạng thái mới cho relay
      digitalWrite(relayPump, relayState);
      
      // In thông báo tương ứng
      if (relayState == 1) {
        Serial.println("RELAY ON");
      } else {
        Serial.println("RELAY OFF");
      }
    }
  }
  
  // Cập nhật trạng thái cuối cùng của nút nhấn
  lastButtonState = currentButtonState;
}


void resetFuction()
{
  DateTime now = rtc.now();
  if(now.hour()== 0 && now.minute() == 0)
  {
    ESP.restart();
  }
  if( now.hour()== 12 && now.minute() == 0 )
  {
    ESP.restart();
  }
  if(now.hour()== 5 && now.minute() == 0)
  {
    ESP.restart();
  }
}


