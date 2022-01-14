/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. On the Mayfly there are LEDs
  at pins 8 and 9. If you're unsure what pin the on-board LED is connected
  to on your Arduino model, check the documentation at http://www.arduino.cc

  This example code is in the public domain.

  Modified 12 August 2016
  by Jeff Horsburgh
  modified 9 February 2018 for Mayfly
  by Beth Fisher
 */

//Zgeneral
/*

Hi there! The LearnEnviroDIYcode-master sketches are developed by
EnviroDIY and Stroud Water Research Center. These are used to set up the
Mayfly the first time.

This second sketch just makes sure the serial monitor are working correctly.
It turns the green LED on for second and then it turns off for second. It then
loops over and over again. It also prints messages to the serial monitor saying
the state of the green LED.

The serial monitor can be accessed using the Shift+Alt+M keys, or by clicking
the Serial Monitor tab on the left of the screen (it looks like a wall plug).

The correct Port should auto load into the dialogue box.

Choose the same Baud Rate as shown in the code where Serial.begin( is located.
(typically 115200, but occasionally 57600).


*/

#include <Arduino.h>

// Original Comment
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 8 as an output.
  pinMode(8, OUTPUT);

  // initialize a serial port
  Serial.begin(115200);
  // print a header line to the serial port
  Serial.println("This is the output of the Blink Arduino sketch.");
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("The LED is on."); // print a message to the serial port
  delay(1000);              // wait for a second

  digitalWrite(8, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("The LED is off.");
  delay(1000);              // wait for a second
}
