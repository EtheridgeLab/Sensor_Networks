/* ***********************************************************************************************
 *
 * Sara Damiano - January 12, 2017
 *
 * This script is intended to synchronize the RTC clock chip of an EnviroDIY Mayfly.
 * After uploading this script to your Mayfly, the time can be set by either manually sending
 * a "T" followed by a unix time stamp to the Mayfly (ie, T1451606400) or by running the
 * sync_clock_PC.py python script which will automatically synchronize the RTC to UTC based
 * on the computer's clock or NTP (if internet connection is available).
 *
 * This script is meant to be used on a naked EnviroDIY Mayfly board with no connection other than
 * directly to the computer via microUSB.  If a GPRSbee or other internet/radio access shields are
 * attached to they Mayfly, it is more efficient to utilize the internet directly to synchronize the
 * RTC chip rather than using this script.  An example of that type of script is available on Sodaq's
 * website.
 *
 * This script requires the wire library generally built into the Arduino IDE and the Sodaq DS3231
 * library, linked below.
 *
 ***********************************************************************************************

*/

/*

Hi there! The LearnEnviroDIYcode-master sketches are developed by
EnviroDIY and Stroud Water Research Center. These are used to set up the
Mayfly the first time.

This fourth sketch is used to set the time on the real-time clock (RTC).
This sketch is very important in the set up process. Z? It requires a different
baud rate than the other Example sketches (57600 as opposed to 115200) and
will set the default time for the Mayfly.

The Mayfly uses a coin battery and the RTC in conjunction as clock for the
Mayfly. Do not run this sketch until you have put a coin battery into the
Mayfly (with the flat smooth side of the battery facing up).

The serial monitor can be accessed using the Shift+Alt+M keys, or by clicking
the Serial Monitor tab on the left of the screen (it looks like a wall plug).

The correct Port should auto load into the dialogue box.

Choose the same Baud Rate as shown in the code where Serial.begin( is located.
(For this sketch, the baud rate is 57600).

Once you have uploaded the sketch to the Mayfly. Type a T and the current
Sodaq time into the dialogue box that pops up when uploading a sketch to the
Mayfly. This dialogue box is different from the Serial Monitor dialogue box
and is usually in the middle of the screen.

Access the Sodaq time from this website. http://time.sodaq.net/
Refreshing your browser will update the time to a new value. Quickly copy
and paste this (along with a T at the beginning, like T1029384710) into the
dialogue box. This will set the time on the Mayfly.

Once you do this sketch, you are done


*/

#include <Arduino.h>
#include <Wire.h>  //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)
#include <Sodaq_DS3231.h> //Sodaq's library for the DS3231: https://github.com/SodaqMoja/Sodaq_DS3231

String getDateTime()
{
  String dateTimeStr;

  //Create a DateTime object from the current time
  DateTime dt(rtc.makeDateTime(rtc.now().getEpoch()));

  //Convert it to a String
  dt.addToString(dateTimeStr);

  return dateTimeStr;
}


/*  code to process time sync messages from the serial port   */
#define TIME_HEADER  'T'   // Header tag for serial time sync message

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1451606400; // Jan 1 2016 00:00:00.000
  const unsigned long MAX_TIME = 2713910400; // Jan 1 2056 00:00:00.000

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    Serial.println("Received:" + String(pctime));
    if ( pctime < DEFAULT_TIME) // check the value is a valid time (greater than Jan 1 2016)
    {
      Serial.println("Time out of range");
      pctime = 0L; // return 0 to indicate that the time is not valid
    }
    if ( pctime > MAX_TIME) // check the value is a valid time (greater than Jan 1 2016)
    {
      Serial.println("Time out of range");
      pctime = 0L; // return 0 to indicate that the time is not valid
    }
  }
  return pctime;
}


void syncRTCwithBatch()
{
  // Read the timestamp from the PC's batch program
  uint32_t newTs = processSyncMessage();

  if (newTs > 0)
  {
    //Serial.println(newTs);

    // Add the timezone difference plus a few seconds
    // to compensate for transmission and processing delay
    //newTs += SYNC_DELAY + TIME_ZONE_SEC;

    //Get the old time stamp and print out difference in times
    uint32_t oldTs = rtc.now().getEpoch();
    int32_t diffTs = newTs - oldTs;
    int32_t diffTs_abs = abs(diffTs);
    Serial.println("RTC is Off by " + String(diffTs_abs) + " seconds");

    //Display old and new time stamps
    Serial.print("Updating RTC, old = " + String(oldTs));
    Serial.println(" new = " + String(newTs));

    //Update the rtc
    rtc.setEpoch(newTs);
  }
}


void setup()
{
  //Start Serial for serial monitor
  Serial.begin(57600);
  while (!Serial) ; // wait until Arduino Serial Monitor opens
  Serial.println("Running sketch: Example_04_Mayfly_setRTC.ino");
}

// This makes the date look all pretty
String weekDay[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
                     "Friday", "Saturday" };
String charMonth[] = {"January", "February", "March", "April", "May", "June",
                       "July", "August", "September", "October", "November",
                       "December" };
String add02d(uint16_t val)
{
  if (val < 10)
    {return "0" + String(val);}
  else
    {return String(val);}
}


void loop()
{
  //Print out current date/time
  DateTime now = rtc.now(); //get the current date-time
  uint32_t ts = now.getEpoch();
  Serial.print("Current RTC Date/Time: ");
  Serial.print(weekDay[now.dayOfWeek()-1]);
  Serial.print(", ");
  Serial.print(charMonth[now.month()-1]);
  Serial.print(' ');
  Serial.print(now.date());
  Serial.print(", ");
  Serial.print(now.year());
  Serial.print(' ');
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(add02d(now.minute()));
  Serial.print(':');
  Serial.print(add02d(now.second()));
  Serial.println(" (" + String(ts) + ")");

  if (Serial.available())
  {
    //Sync time
    syncRTCwithBatch();
  }
  // Empty the serial buffer
  while (Serial.available() > 0)
  {
    Serial.read();
  }

  delay(1000);
}
