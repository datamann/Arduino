// Example 55.1
 
#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); // configure software serial port
 
void setup()
{
  SIM900.begin(19200);               
  SIM900power();  
  delay(20000);  // give time to log on to network. 
}
 
void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(5000);
}
 
void callSomeone()
{
  SIM900.println("ATD + +4794885446;"); // dial US (212) 8675309
  delay(100);
  SIM900.println();
  delay(30000);            // wait for 30 seconds...
  SIM900.println("ATH");   // hang up
}
 
void loop()
{
  callSomeone(); // call someone
  SIM900power();   // power off GSM shield
  do {} while (1); // do nothing
}
