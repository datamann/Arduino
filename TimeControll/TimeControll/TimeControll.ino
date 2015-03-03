unsigned long previousMillis = 0; // last time update
long interval = 30000; // interval at which to do something (milliseconds)

void setup(){
  Serial.begin(19200);
}

void loop(){
  unsigned long currentMillis = millis();
  Serial.println("Loop");
  if(currentMillis - previousMillis > interval) {
     previousMillis = currentMillis;  

     Serial.println("Send SMS!"); // do something
  }
  delay(1000);
}
