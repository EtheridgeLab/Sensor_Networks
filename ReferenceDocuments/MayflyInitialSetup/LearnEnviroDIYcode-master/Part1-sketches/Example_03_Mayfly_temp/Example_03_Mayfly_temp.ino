/**************************************************************************

* Written By:  Shannon Hicks <shicks@stroudcenter.org>
* Documentation By: Anthony Aufdenkampe <aaufdenkampe@limno.com>
* Creation Date: 2016
* Development Environment: Arduino IDE 1.6.5+
* Hardware Platform: Stroud Center, EnviroDIY Mayfly Arduino Datalogger

All of the Mayfly boards have been pre-programmed with this initial
demonstration sketch. This sketch uses a library called *Sodaq_DS3231.h*,
which is necessary to interact with the Real Time Clock's DS3231 RTC chip.
In order to load this sketch, you first need to install the SODAQ_DS3231
library, either by: using the Arudino IDE from this menu utility:
Skectch > Include Library > Manage Libraries...; or by
directly obtaining the library from:
https://github.com/SodaqMoja/Sodaq_DS3231

**************************************************************************/

//Zgeneral

/*

Hi there! The LearnEnviroDIYcode-master sketches are developed by
EnviroDIY and Stroud Water Research Center. These are used to set up the
Mayfly the first time.

This third sketch makes sure the temperature sensor built into the Mayfly
is working correctly. The temperature sensor is on the RTC (real-time clock)
on the Mayfly, which will be used to set the time in the next sketch.
It displays the temperature in degrees Celsius to the serial monitor.

The serial monitor can be accessed using the Shift+Alt+M keys, or by clicking
the Serial Monitor tab on the left of the screen (it looks like a wall plug).

The correct Port should auto load into the dialogue box.

Choose the same Baud Rate as shown in the code where Serial.begin( is located.
(typically 115200, but occasionally 57600).


*/

#include <Arduino.h>
#include <Wire.h>
#include "Sodaq_DS3231.h"   // Install this library to interact with the Real Time Clock

int State8 = LOW;
int State9 = LOW;

int LEDtime = 1000;   //milliseconds

void setup ()
{
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);

    Serial.begin(115200);
    Wire.begin();
    rtc.begin();

    Serial.println("EnviroDIY Mayfly: Blink demo with serial temperature");

}

void loop ()
{
    if (State8 == LOW) {
      State8 = HIGH;
    } else {
      State8 = LOW;
    }
    digitalWrite(8, State8);

    State9 = !State8;
    digitalWrite(9, State9);

    rtc.convertTemperature();             //convert current temperature into registers
    Serial.print(rtc.getTemperature(),2); //read registers and display the temperature
    Serial.println("deg C");

    delay(LEDtime);
}
