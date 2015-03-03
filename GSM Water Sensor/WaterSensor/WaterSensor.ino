//Code collected from various sources
//Modified by Stig B. Sivertsen
//Date: 26.02.2015

#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial CELL(7, 8);
const int ledPin =  10;
long interval = 30000; // interval at which to do something (milliseconds)
unsigned long previousMillis = 0; // last time update
unsigned char buffer[64];
//int count = 0;
//boolean containMsg = false;

char inData[64]; // Allocate some space for the string
char inChar = -1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

// Commands to send to GPRS Shield

//Init...
String cellInit = "AT\r";
String cellSMSInit = "AT+CNMI=3,2,2,0\r";
String initStatus = "false";

//Send SMS
String sendSMSInTXTMode = "AT+CMGF=1\r";
String sendToNumber = "AT+CMGS=\"+4790813585\"";
String SMSToSend = "Vann oppdaget!";

//Cleanup
String deleteAllSMS = "AT+CMGD=1,4\r";
String showAllSMS = "AT+CMGL=\"ALL\"\r";

// Respons from GPRS Shield
String receiveSMS = "\r\n+CMTI: \"SM\",1\r\n";
String receiveOK = "\r\nOK\r\n";

void setup() {
  // initialize serial communication at 9600 bits per second:
  pinMode(ledPin, OUTPUT);
  CELL.begin(19200);               // the GPRS baud rate
  Serial.begin(19200);    // the GPRS baud rate
  delay(500);
}
void loop() {
  if (initStatus == "false") {
    initCell();
  }
  CheckWaterSensor();
  readCell();
  //readGPRSShield();
  //recieveTextMessage();
  delay(1000);
}

void initCell() {
  CELL.write("AT\r");
  delay(100);
  CELL.write("AT+CNMI=3,2,0,0,0\r");
  delay(100);
  initStatus = "true";
  CELL.flush();
}

void readTextMessage() {
  delay(1000);
  if (CELL.available()) {
    Serial.println("Trying to print content!");
    //Serial.println(CELL.print("AT+CMGR=1,0\r"));
    CELL.print("AT+CMGR=1,0\r");
    delay(100);
    CELL.flush();
  } else {
    Serial.println("Could not read SMS!");
    CELL.flush();
  }
  deleteMSG();

  //  if (CELL.available()) {
  //    while (CELL.available()) {        // reading data into char array
  //      if (index < 63) { // One less than the size of the array
  //        inChar = CELL.read(); // Read a character
  //        inData[index] = inChar; // Store it
  //        index++; // Increment where to write next
  //        inData[index] = '\0'; // Null terminate the string
  //      }
  //    }
  //  }
  //  Serial.print("Meldingen innholder: ");
  //  String String(inData);
  //  Serial.println(inData);
  //  clearDataArray();

}

///deleteMSG
/// Deletes incomming messages since they are stored on the GPRS Shield.
void deleteMSG() {
  delay(1000);
  //  if (CELL.available()) {
  Serial.println("Deleting...");
  //    //Serial.println(CELL.print("AT+CMGD=1,4\r"));       // Deleting SMS
  CELL.print("AT+CMGD=1,4\r");
  delay(100);
  CELL.flush();
  //  } else {
  //    Serial.println("Could not delete SMS!");
  //  }
}

///SendTextMessage()
///this function is to send a sms message
void SendTextMessage() {
  if (CELL.available()) {
    Serial.println(CELL.print(sendSMSInTXTMode));    //Because we want to send the SMS in text mode
    delay(100);
    Serial.println(CELL.println(sendToNumber));//send sms message, be careful need to add a country code before the cellphone number
    delay(100);
    Serial.println(CELL.println(SMSToSend));//the content of the message
    delay(100);
    Serial.println(CELL.println((char)26));//the ASCII code of the ctrl+z is 26
    delay(100);
    Serial.println("Sender SMS...");
    CELL.flush();
  } else {
    Serial.println("GPRS Shield not available when trying to send SMS!");
    CELL.flush();
  }
}

void clearDataArray() {             // function to clear buffer array
  for (int i = 0; i < index; i++)
  {
    inData[i] = NULL; // clear all index of array with command NULL
  }
}

///CheckWaterSensor()
/// Checking water sensor
void CheckWaterSensor() {
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);

  if (sensorValue < 20) {
    unsigned long currentMillis = millis();
    Serial.println("Vann oppdaget!");
    digitalWrite(ledPin, HIGH);
    if (currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      SendTextMessage();
    }
  } else {
    digitalWrite(ledPin, LOW);
  }
  delay(1000);
}

void readCell() {
  String content = "";
  String item = "";
  char character;

  if (CELL.available()) {             // if date is comming from softwareserial port ==> data is comming from gprs shield

    while (CELL.available()) {        // reading data into char array
      if (index < 64) { // One less than the size of the array
        inChar = char(CELL.read()); // Read a character
        inData[index] = inChar; // Store it
        index++; // Increment where to write next
        inData[index] = '\0'; // Null terminate the string
      }
    }
    int size = sizeof(inData);
    for (int k = 0; k < sizeof(inData); k++) {

      item = String(inData[k]);
      if (item == "S") {
        Serial.println("Fant:" + item);
      }
      item = item + item;
    }

    if (content != "") {
      Serial.println(content);
      int strLength = content.length();
      int index = content.lastIndexOf("4");
      //Serial.println(strLength);
      //Serial.println(index);
      content = "";
      index = NULL;
    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",1\r\n")  == 0) {
    //      Serial.println("Inn data er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",2\r\n")  == 0) {
    //      Serial.println("Inn data2 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",3\r\n")  == 0) {
    //      Serial.println("Inn data3 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",4\r\n")  == 0) {
    //      Serial.println("Inn data4 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",5\r\n")  == 0) {
    //      Serial.println("Inn data5 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",6\r\n")  == 0) {
    //      Serial.println("Inn data6 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",7\r\n")  == 0) {
    //      Serial.println("Inn data7 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",8\r\n")  == 0) {
    //      Serial.println("Inn data8 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",9\r\n")  == 0) {
    //      Serial.println("Inn data9 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",10\r\n")  == 0) {
    //      Serial.println("Inn data10 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",11\r\n")  == 0) {
    //      Serial.println("Inn data11 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",12\r\n")  == 0) {
    //      Serial.println("Inn data12 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",13\r\n")  == 0) {
    //      Serial.println("Inn data13 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",14\r\n")  == 0) {
    //      Serial.println("Inn data14 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",15\r\n")  == 0) {
    //      Serial.println("Inn data15 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",16\r\n")  == 0) {
    //      Serial.println("Inn data16 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",17\r\n")  == 0) {
    //      Serial.println("Inn data17 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",18\r\n")  == 0) {
    //      Serial.println("Inn data18 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",19\r\n")  == 0) {
    //      Serial.println("Inn data19 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    //    if (strcmp(inData, "\r\n+CMTI: \"SM\",20\r\n")  == 0) {
    //      Serial.println("Inn data20 er (likt):");
    //      Serial.write(inData);
    //      clearDataArray();
    //      CELL.flush();
    //      delay(100);
    //      readTextMessage();
    //      delay(100);
    //    }
    clearDataArray();
    CELL.flush();
  } else {
    //Serial.println("GPRS Shield read not available!");
    CELL.flush();
    //deleteMSG();
  }
}

