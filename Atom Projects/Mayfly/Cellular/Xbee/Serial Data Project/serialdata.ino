char incomingchar; // for any incoming data
String time;
String field1;
String field2;
String field3;
String field4;
String field5;
String part1;
String part2;
String part3;
String part4;
String part5;

#include <Arduino.h>
// EnableInterrupt is used by ModularSensors for external and pin change
// interrupts and must be explicitly included in the main program.
#include <EnableInterrupt.h>

// To get all of the base classes for ModularSensors, include LoggerBase.
// NOTE:  Individual sensor definitions must be included separately.
#include <LoggerBase.h>

//Edited, for Grove I2C

#include <Wire.h> //Edited, for Grove I2C

#include <SPI.h> //Edited, for Grove I2C

#include <AltSoftSerial.h>

AltSoftSerial altSerial;



//Defines the XBEE Sleep pin
//#define XBEE_SleepPin 23

/*
void setup() {
  //Initialise the serial connection
  Serial.begin(115200);
  Serial1.begin(9600);  //this is the Xbee UART, default Xbee speed is 9600 baud
}

void loop() {
if (Serial1.available()) {
  Serial1.println("#Pfield1=20&field2=50&field3=20&field4=5&field5=20\r\n");
  delay(15000);
  }
}
*/

/*
void setup() {
  //Initialise the serial connection
  Serial.begin(115200);
  Serial1.begin(9600);  //this is the Xbee UART, default Xbee speed is 9600 baud
}

void loop() {
if (Serial1.available()) {
incomingstring = Serial1.readString();
Serial.print(incomingstring);
  //Serial.print("#P" + incomingstring);
  }
}
*/


void setup() {
  //Initialise the serial connection
  Serial.begin(115200);
  //Serial1.begin(115200);  //this is the Xbee UART, default Xbee speed is 9600 baud
  altSerial.begin(9600);
  altSerial.println("Hello");
}

void loop() {
  if (Serial.available()) {
    incomingchar = Serial.read();
    Serial.print(incomingchar);
  }

//if (altSerial.available()) {
//  incomingchar = altSerial.read();
  //Serial.print(incomingchar);
}
//}
  /*String time = getValue(incomingstring,',', 0);
  String field1 = getValue(incomingstring,',',1);
  String field2 = getValue(incomingstring,',',2);
  String field3 = getValue(incomingstring,',',3);
  String field4 = getValue(incomingstring,',',4);
  String field5 = getValue(incomingstring,',',5);
  String part1 = "&field1=" + field1;
  String part2 = "&field2=" + field2;
  String part3 = "&field3=" + field3;
  String part4 = "&field4=" + field4;
  String part5 = "&field5=" + field5;


  Serial.print("#P" + part1 + part2 + part3 + part4 + part5 + "\r\n");
  Serial1.print("#P" + part1 + part2 + part3 + part4 + part5 + "\r\n");
  //Serial.println("field5 = " + field5);
  //Serial.print("#P" + incomingstring);
  }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
    */
