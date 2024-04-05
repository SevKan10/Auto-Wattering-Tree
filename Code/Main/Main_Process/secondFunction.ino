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
  if (digitalRead(buttonPump) != lastButtonState) {
    delay(50);  
    flagPump++;
    ringBell(700,1);     
    if (flagPump>3) {flagPump=1;}
  }
  switch(flagPump)
  {
    case 1:
      digitalWrite(relayPump, 1);  
      Blynk.virtualWrite(V4, 1);  
      lcd.print(0, 0, String("MÁY BƠM ĐANG MỞ ")); 
      Serial.println("RELAY ON"); 
      buttonBlynk = 1;
    break;

    case 2:
      digitalWrite(relayPump, 0);  
      Blynk.virtualWrite(V4, 0);  
      lcd.print(0, 0, String("MÁY BƠM ĐANG TẮT")); 
      Serial.println("RELAY OFF"); 
      buttonBlynk = 0;
      flagPump = 3;
    break;

    case 3:
    
    break;
  }
  lastButtonState = digitalRead(buttonPump);
}

