/** =========================================================================
 * @file DRWI_sim7080LTE.ino
 * @brief Example for DRWI CitSci LTE sites.
 *
 * This example shows proper settings for the following configuration:
 *
 * Mayfly v1.0 board
 * EnviroDIY sim7080 LTE module (with Hologram SIM card)
 * Hydros21 CTD sensor
 * Campbell Scientific OBS3+ Turbidity sensor
 *
 * @author Sara Geleskie Damiano <sdamiano@stroudcenter.org>
 * @copyright (c) 2017-2021 Stroud Water Research Center (SWRC)
 *                          and the EnviroDIY Development Team
 *            This example is published under the BSD-3 license.
 *
 * Hardware Platform: EnviroDIY Mayfly Arduino Datalogger
 *
 * DISCLAIMER:
 * THIS CODE IS PROVIDED "AS IS" - NO WARRANTY IS GIVEN.
 * ======================================================================= */

// ==========================================================================
//  Defines for the Arduino IDE
//  NOTE:  These are ONLY needed to compile with the Arduino IDE.
//         If you use PlatformIO, you should set these build flags in your
//         platformio.ini
// ==========================================================================


 /* Comments from Zeke based on edits

 -------------------------------------------------------------------------------
 -------------------------------------------------------------------------------

This code was edited in order to transmit to Thingspeak mqtt3.thingspeak.com
using the EnviroDIY Mayfly 1.0 board and a Hydros-21 CTD sensor. This code relies
on code and libraries developed by EnviroDIY (Stroud Research Center),
and many other contributors. To allow for this code to continue to be used in
the future, I've added some comments to show the process of how data is uploaded
to ThingSpeak. Finally, the other file that needs to mentioned here is the
platformio.ini. This file is used by plaformio.ini to set up a lot of stuff
on the front end. This is located at the bottom of the Mayfly1Coordinator520
folder (the very last item in the folder when you open it initially).

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

Reference:

Using the Ctrl+F function in different files, the following words will help
direct edits of the code for later use

ZGW = shows the things that need to be changed in the Arduino file
if a new GW station needs to be added

Znecessary = find pertinent changes in .h or .cpp files
that allow the Mayfly to function properly

Zgeneral = generally tells what code does

Zarduinolabels = labels the variables section, setup function, and loop function
in the Arduino code for easy reference. The setup and loop functions are two
key functions that MUST be used in order for the program to run.

Zbrace = Critical braces (   {   }   ) that are easy to miss in the setup and
loop functions.


-------------------------------------------------------------------------------

Code Reference: The following labels indicate where particular code sections
come from. Ctrl+F this document for these labels to see what C++ code is
dependent on certain libraries.

codeLoggerBase = Directly dependent on LoggerBase (LoggerBase.h/LoggerBase.cpp)

codeModem = Directly used in the modem setup

codeThingSpeakPublisher = Directly dependent on ThingSpeakPublisher
(ThingSpeakPublisher.h/ThingSpeakPublisher.cpp)

codePubSubClient = Direclty dependent on PubSubClient
(PubSubClient.h / PubSubClient.cpp)

codeMeterHydros21 = Directly dependent on MeterHydros21.h

codeVariableArray = Directly dependent on VariableArray
(VariableArray.h / VariableArray.cpp)

codeThisScript = Code that is primarily used in this script. This code
may be used in other code bases as well, but it's main uses are in this
script.

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

The following code files and libraries are pertinent to the functions of
this project and are included below:

-------------------------------------------------------------------------------

1. how_to_thingspeaksingular_mayfly_1point0_GW#.ino

Where is how_to_thingspeaksingular_mayfly_1point0_GW#.ino found?:
Mayfly1Coordinator520->src->how_to_thingspeaksingular_mayfly_1point0_GW#

File type: Arduino file

Summary of the functions of how_to_thingspeaksingular_mayfly_1point0_GW#.ino:
how_to_thingspeaksingular_mayfly_1point0_GW#.ino uses many of the libraries in the
this folder to upload water level data to ThingSpeak. Once necessary connections
have been made to the EnviroDIY Mayfly 1.0 microcontroller, this file is
uploaded to the EnviroDIY Mayfly 1.0.

Before uploading this file to the Mayfly, make sure you connect the following
to the Mayfly.

a. A microSD card is in the microSD card slot.
b. A coin battery is in the coin battery slot.
c. A battery is connected to one of the Lipo battery connector on the board.
d. A solar panel is connected to the solar panel port at the top of the board.
e. A Hydros-21 CTD sensor (which has already been connected to a Grove adapter)
   is connected to the SDI-12 D6-7 Z? port on the board.
f. One USB-C data + power cable is connected to the USB-C port on the board.

A different how_to_thingspeaksingular_mayfly_1point0_GW#.ino file will be needed for
each Mayfly. For each file change, add 1 to the 'GW1' (GW2, GW3, etc.).

Since many code sections used in how_to_thingspeaksingular_mayfly_1point0_GW#.ino
are dependent on code non-Arduino code, the following descriptions of libraries
and code will include a list of the code sections throughout
how_to_thingspeaksingular_mayfly_1point0_GW#.ino that are directly dependent on
functions or variables found in other code scripts.

-------------------------------------------------------------------------------

2. LoggerBase.h

Where is LoggerBase.h found?:
Mayfly1Coordinator520->pio->libdeps->mayfly->EnviroDIY_ModularSensors->src->LoggerBase.h

File type: C++ header file

Summary of the functions of LoggerBase.h:
LoggerBase.h is a C++ header file that handles the logging of data before
uploading that data to ThingSpeak. LoggerBase.h is the header file for
implementation file LoggerBase.cpp. The .h file sets up the necessary variables
and functions, and the .cpp file works to log the data.

Things in how_to_thingspeaksingular_mayfly_1point0_GW#.ino directly dependent on
LoggerBase.h:

Most of the code in this document is dependent on LoggerBase. The code sections.
Listed below are the code sections from LoggerBase that are important in
the logging process.

Ctrl+F the code sections to find them a-y in the
how_to_thingspeaksingular_mayfly_1point0_GW#.ino file. Select beginning at the first
character in the line to ensure Ctrl+F recognizes the line. If using Atom,
set your search results to case sensitive.

a. const char* sketchName = "how_to_thingspeaksingular_mayfly_1point0_GW#.ino";

b. const char* LoggerID = "logger";

c. const uint8_t loggingInterval = 1;

d. const int8_t timeZone = -5;

e. const int8_t  greenLED   = 8;

f. const int8_t  buttonPin  = 21;

g. const int8_t  wakePin    = 31;

h. const int8_t sdCardPwrPin   = -1;

i. const int8_t sdCardSSPin = 12;

j. const int8_t sensorPowerPin = 22;

k. const char* apn = "hologram";

l. Logger dataLogger(LoggerID, loggingInterval, &varArray);

m. Logger::setLoggerTimeZone(timeZone);

n. Logger::setRTCTimeZone(0);

o. dataLogger.attachModem(modem);

p. dataLogger.setLoggerPins(wakePin, sdCardSSPin, sdCardPwrPin, buttonPin, greenLED);

q. dataLogger.begin(LoggerID, loggingInterval, &varArray);

r. dataLogger.syncRTC();

s. dataLogger.turnOnSDcard(true);

t. dataLogger.createLogFile(true);

u. dataLogger.turnOffSDcard(true);

v. dataLogger.systemSleep();

w. dataLogger.systemSleep();

x. dataLogger.logData();

y. dataLogger.logDataAndPublish();

-------------------------------------------------------------------------------

3. LoggerBase.cpp

Where is LoggerBase.cpp found?:
Mayfly1Coordinator520->pio->libdeps->mayfly->EnviroDIY_ModularSensors->src->LoggerBase.cpp

File type: C++ implementation file

Summary of the functions of LoggerBase.h:
LoggerBase.cpp is a C++ implementation file that handles the logging of data
before uploading that data to ThingSpeak. LoggerBase.cpp depends on the
variables and functions in LoggerBase.h. Many of the functions in
LoggerBase.cpp are used used in the Arduino file
how_to_thingspeaksingular_mayfly_1point0_GW#.ino.

Things in how_to_thingspeaksingular_mayfly_1point0_GW#.ino directly dependent on
LoggerBase.cpp:

Ctrl+F the code sections a-y to find them in the
how_to_thingspeaksingular_mayfly_1point0_GW#.ino file. Select beginning at the first
character in the line to ensure Ctrl+F recognizes the line. If using Atom,
set your search results to case sensitive.

a. const char* sketchName = "how_to_thingspeaksingular_mayfly_1point0_GW#.ino";

b. const char* LoggerID = "logger";

c. const uint8_t loggingInterval = 1;

d. const int8_t timeZone = -5;

e. const int8_t  greenLED   = 8;

f. const int8_t  buttonPin  = 21;

g. const int8_t  wakePin    = 31;

h. const int8_t sdCardPwrPin   = -1;

i. const int8_t sdCardSSPin = 12;

j. const int8_t sensorPowerPin = 22;

k. const char* apn = "hologram";

l. Logger dataLogger(LoggerID, loggingInterval, &varArray);

m. Logger::setLoggerTimeZone(timeZone);

n. Logger::setRTCTimeZone(0);

o. dataLogger.attachModem(modem);

p. dataLogger.setLoggerPins(wakePin, sdCardSSPin, sdCardPwrPin, buttonPin, greenLED);

q. dataLogger.begin(LoggerID, loggingInterval, &varArray);

r. dataLogger.syncRTC();

s. dataLogger.turnOnSDcard(true);

t. dataLogger.createLogFile(true);

u. dataLogger.turnOffSDcard(true);

v. dataLogger.systemSleep();

w. dataLogger.systemSleep();

x. dataLogger.logData();

y. dataLogger.logDataAndPublish();

-------------------------------------------------------------------------------

4. ThingSpeakPublisher.h
Where is ThingSpeakPublisher.h found?:
Mayfly1Coordinator520->pio->libdeps->mayfly->EnviroDIY_ModularSensors->src->publishers->ThingSpeakPublisher.h

File type: C++ header file

Summary of the functions of ThingSpeakPublisher.h:
ThingSpeakPublisher.h is a C++ header file that formats data correctly to send
it to ThingSpeak. ThingSpeak depends on the MQTT data transmitting protocol.
ThingSpeakPublisher.h is the header file for the implementation file
ThingSpeakPublisher.cpp, and sets up the functions and variables that are
used in ThingSpeakPublisher.cpp.

Things in how_to_thingspeaksingular_mayfly_1point0_GW#.ino directly dependent on
ThingSpeakPublisher.h:

Ctrl+F the code sections a-g to find them in the
how_to_thingspeaksingular_mayfly_1point0_GW#.ino file. Select beginning at the first
character in the line to ensure Ctrl+F recognizes the line.

a. #include <publishers/ThingSpeakPublisher.h>

b. ThingSpeakPublisher TsMqtt

c. const char* thingSpeakChannelID =

d. const char* thingSpeakMQTTusername =

e. const char* thingSpeakMQTTpassword =

f. const char* thingSpeakMQTTclientID =

g. TsMqtt.begin(dataLogger, &modem.gsmClient, thingSpeakChannelID,
thingSpeakMQTTusername, thingSpeakMQTTpassword, thingSpeakMQTTclientID);

-------------------------------------------------------------------------------

5. ThingSpeakPublisher.cpp
Where is ThingSpeakPublisher.cpp found?:
Mayfly1Coordinator520->pio->libdeps->mayfly->EnviroDIY_ModularSensors->src->publishers->ThingSpeakPublisher.cpp

File type: C++ implementation file

Summary of the functions of ThingSpeakPublisher.cpp:
ThingSpeakPublisher.cpp is a C++ header file that formats data correctly to
send it to ThingSpeak. ThingSpeak depends on the MQTT data transmitting
protocol. ThingSpeakPublisher.cpp depends on the variables set up in the
header file ThingSpeakPublisher.h.

Things in how_to_thingspeaksingular_mayfly_1point0_GW#.ino directly dependent on
ThingSpeakPublisher.cpp:

Ctrl+F the code sections a-g to find them in the
how_to_thingspeaksingular_mayfly_1point0_GW#.ino file. Select beginning at the first
character in the line to ensure Ctrl+F recognizes the line. If using Atom,
set your search results to case sensitive.

a. #include <publishers/ThingSpeakPublisher.h>

b. ThingSpeakPublisher TsMqtt

c. const char* thingSpeakChannelID =

d. const char* thingSpeakMQTTusername =

e. const char* thingSpeakMQTTpassword =

f. const char* thingSpeakMQTTclientID =

g. TsMqtt.begin(dataLogger, &modem.gsmClient, thingSpeakChannelID,
thingSpeakMQTTusername, thingSpeakMQTTpassword, thingSpeakMQTTclientID);

-------------------------------------------------------------------------------

6. PubSubClient.h

Where is PubSubClient.h found?:
Mayfly1Coordinator520->pio->libdeps->mayfly->PubSubClient->src->PubSubClient.h

File type: C++ header file

Summary of the functions of PubSubClient.h:
PubSubClient.h is a C++ header file that handles the MQTT connection
of the MQTT process. It sets up the variables and functions that are used in
PubSubClient.cpp. PubSubClient.h is also dependent on ThingSpeakPublisher.cpp
and ThingSpeakPublisher.h.

Things in how_to_thingspeaksingular_mayfly_1point0_GW#.ino directly dependent on
PubSubClient.h:

Ctrl+F the code sections a-g to find them in the
how_to_thingspeaksingular_mayfly_1point0_GW#.ino file. Select beginning of the first
character in the line to ensure Ctrl+F recognizes the line. If using Atom,
set your search results to case sensitive.

a. #include <publishers/ThingSpeakPublisher.h>

b. ThingSpeakPublisher TsMqtt

c. const char* thingSpeakChannelID =

d. const char* thingSpeakMQTTusername =

e. const char* thingSpeakMQTTpassword =

f. const char* thingSpeakMQTTclientID =

g. TsMqtt.begin(dataLogger, &modem.gsmClient, thingSpeakChannelID,
thingSpeakMQTTusername, thingSpeakMQTTpassword, thingSpeakMQTTclientID);

-------------------------------------------------------------------------------

7. PubSubClient.cpp

Where is PubSubClient.cpp found?:
Mayfly1Coordinator520->pio->libdeps->mayfly->PubSubClient->src->PubSubClient.cpp

File type: C++ implementation file

Summary of the functions of PubSubClient.cpp:
PubSubClient.cpp is a C++ implementation file that handles the MQTT connection
of the MQTT process. It contains the variables and functions that are set up
in PubSubClient.h

Things in how_to_thingspeaksingular_mayfly_1point0_GW#.ino directly dependent on
PubSubClient.h:

Ctrl+F the code sections a-g to find them in the
how_to_thingspeaksingular_mayfly_1point0_GW#.ino file. Select beginning of the first
character in the line to ensure Ctrl+F recognizes the line. If using Atom,
set your search results to case sensitive.

a. #include <publishers/ThingSpeakPublisher.h>

b. ThingSpeakPublisher TsMqtt

c. const char* thingSpeakChannelID =

d. const char* thingSpeakMQTTusername =

e. const char* thingSpeakMQTTpassword =

f. const char* thingSpeakMQTTclientID =

g. TsMqtt.begin(dataLogger, &modem.gsmClient, thingSpeakChannelID,
thingSpeakMQTTusername, thingSpeakMQTTpassword, thingSpeakMQTTclientID);

-------------------------------------------------------------------------------

8. MeterHydros21.h

Where is MeterHydros21.h found?:
Mayfly1Coordinator520->pio->libdeps->mayfly->EnviroDIY_ModularSensors->src->sensors->MeterHydros21.h

File type: C++ header file

Summary of the functions of MeterHydros21.h:
MeterHydros21.h is a header file that is used to make data measurements
using the Hydros-21 CTD sensor (conductivity, temperature, and depth). This
library is developed by METER Group.

Things in how_to_thingspeaksingular_mayfly_1point0_GW#.ino directly dependent on
MeterHydros21.h:

a. #include <sensors/MeterHydros21.h>

b. const char*   hydrosSDI12address = "1";

c. const uint8_t hydrosNumberReadings = 6;

d. const int8_t  SDI12Power = sensorPowerPin;

e. const int8_t  SDI12Data  = 7;

f. MeterHydros21 hydros(*hydrosSDI12address, SDI12Power, SDI12Data,
                     hydrosNumberReadings);

g. Hydros-21 Temperature Conversion Code

h. Hydros-21 Depth Conversion Code


i. Variable* variableList[] = {
    new MeterHydros21_Cond(&hydros),
    HydrosCalcTempF,
    HydrosCalcDepthft,
    new ProcessorStats_Battery(&mcuBoard),
};

j. int variableCount = sizeof(variableList) / sizeof(variableList[0]);

k. VariableArray varArray;

l. Logger dataLogger(LoggerID, loggingInterval, &varArray);

-------------------------------------------------------------------------------

9. VariableArray.h

Where is VariableArray.h found?:
Mayfly1Coordinator520->pio->libdeps->mayfly->EnviroDIY_ModularSensors->src->sensors->MeterHydros21.h

File type: C++ header file

Summary of the functions of VariableArray.h:
VariableArray.h is a header file that is used to set up a variable array
containing the parameters that are logged on the Mayfly in LoggerBase. The
variables and functions set up in this file are used in VariableArray.cpp.

Things in how_to_thingspeaksingular_mayfly_1point0_GW#.ino directly dependent on
VariableArray.h:

a. Variable* variableList[] = {
      new MeterHydros21_Cond(&hydros),
      HydrosCalcTempF,
      HydrosCalcDepthft,
      new ProcessorStats_Battery(&mcuBoard),
};

b. int variableCount = sizeof(variableList) / sizeof(variableList[0]);

c. VariableArray varArray;

d. Logger dataLogger(LoggerID, loggingInterval, &varArray);

e. varArray.begin(variableCount, variableList);

f. dataLogger.begin(LoggerID, loggingInterval, &varArray);

g. varArray.setupSensors();

-------------------------------------------------------------------------------

10. VariableArray.cpp

Where is VariableArray.cpp found?:
Mayfly1Coordinator520->pio->libdeps->mayfly->EnviroDIY_ModularSensors->src->sensors->MeterHydros21.h

File type: C++ implementation file

Summary of the functions of VariableArray.cpp:
VariableArray.cpp is an implementation file that is used to arrange the
variables being recorded on the Mayfly. The variables and functions in this file
are initialized in VariableArray.h.

Things in how_to_thingspeaksingular_mayfly_1point0_GW#.ino directly dependent on
VariableArray.cpp:

a. Variable* variableList[] = {
      new MeterHydros21_Cond(&hydros),
      HydrosCalcTempF,
      HydrosCalcDepthft,
      new ProcessorStats_Battery(&mcuBoard),
};

b. int variableCount = sizeof(variableList) / sizeof(variableList[0]);

c. VariableArray varArray;

d. Logger dataLogger(LoggerID, loggingInterval, &varArray);

e. varArray.begin(variableCount, variableList);

f. dataLogger.begin(LoggerID, loggingInterval, &varArray);

g. varArray.setupSensors();

-------------------------------------------------------------------------------

// Zarduinolabels
// This begins the variables section of the code. This section sets up variables
// that will be used in the setup and loop functions.


/** Start [defines] */
#ifndef TINY_GSM_RX_BUFFER
#define TINY_GSM_RX_BUFFER 64
#endif
#ifndef TINY_GSM_YIELD_MS
#define TINY_GSM_YIELD_MS 2
#endif
#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 256
#endif
/** End [defines] */



// ==========================================================================
//  Include the libraries required for any data logger
// ==========================================================================


/** Start [includes] */
// The Arduino library is needed for every Arduino program.

// Zgeneral
// Arduino.h is a library that is required to allow any Arduino code to run
#include <Arduino.h>

// Zgeneral
// EnableInterrupt.h is a library that is required to allow any Arduino code to run

// EnableInterrupt is used by ModularSensors for external and pin change
// interrupts and must be explicitly included in the main program.

// Zgeneral
// EnableInterrupt.h is a library that allows for pins on the Mayfly to be
// changed for other uses.
#include <EnableInterrupt.h>

// Include the main header for ModularSensors

// Zgeneral
// codeLoggerBase
// ModularSensors.h is a library that contains many different code bases
// which are pertinent for using the Mayfly.
#include <ModularSensors.h>
/** End [includes] */


// ==========================================================================
//  Data Logging Options
// ==========================================================================
/** Start [logging_options] */


// Original comment
// The name of this program file

// ZGW
// codeLoggerBase
// If the name of the Arduino file has been changed, change the sketchName below.
const char* sketchName = "how_to_thingspeaksingular_mayfly_1point0_GW#.ino";


// Original comment
// Logger ID, also becomes the prefix for the name of the data file on SD card

// Zgeneral
// codeLoggerBase
// This is used by LoggerBase in order to set the prefix on the SD card and
// also notify logger base of the LoggerID
const char* LoggerID = "logger";


// Original comment
// How frequently (in minutes) to log data

// ZGW
// codeLoggerBase
// Use loggingInterval to change the interval between data readings. If it is
// necessary to change the logging interval away from 15 minutes, change the
// number below.
const uint8_t loggingInterval = 1;


// Original comment
// Your logger's timezone.
// NOTE:  Daylight savings time will not be applied!  Please use standard time!

// ZGW
// codeLoggerBase
// For the sake of this project, this can stay the same. This can be changed
// based on the time zone.
const int8_t timeZone = -5;  // Eastern Standard Time


// Original
// Set the input and output pins for the logger
// NOTE:  Use -1 for pins that do not apply

// Zgeneral
// codeThisScript
// serialBaud = sets the serial communication rate in bits per second
// of the Mayfly
const int32_t serialBaud = 115200;   // Baud rate for debugging

// Zgeneral
// codeLoggerBase
// greenLED = sets the pin value for the green LED on the Mayfly
const int8_t  greenLED   = 8;       // Pin for the green LED

// Zgeneral
// codeThisScript
// redLED = sets the pin value for the red LED on the Mayfly
const int8_t  redLED     = 9;       // Pin for the red LED


// Zgeneral
// codeLoggerBase
// buttonPin = sets the pin value for the button on the Mayfly
const int8_t  buttonPin  = 21;      // Pin for debugging mode (ie, button pin)


// Original comment
// Mayfly 0.x D31 = A7
// Set the wake pin to -1 if you do not want the main processor to sleep.
// In a SAMD system where you are using the built-in rtc, set wakePin to 1

// Zgeneral
// codeLoggerBase
// wakePin = sets the wake pin value which allow the Mayfly to wake from being
// asleep
const int8_t  wakePin    = 31;  // MCU interrupt/alarm pin to wake from sleep

// Zgeneral
// codeLoggerBase
// sdCardPwrPin = sets the power pin value for the SD card on the Mayfly
const int8_t sdCardPwrPin   = -1;  // MCU SD card power pin

// Zgeneral
// codeLoggerBase
// sdCardSSPin = sets the select pin value on the Mayfly for the SD card chip
const int8_t sdCardSSPin = 12;  // SD card chip select/slave select pin

// Zgeneral
// codeLoggerBase
// sensorPowerPin = sets the pin value on the Mayfly for the sensor power
const int8_t sensorPowerPin = 22;  // MCU pin controlling main sensor power
/** End [logging_options] */


// ==========================================================================
//  Wifi/Cellular Modem Options
// ==========================================================================
/** Start [sim7080] */
// For almost anything based on the SIMCom SIM7080G

// Zgeneral
// codeModem
// This is the modem library for the EnviroDIY custom cellular modem
#include <modems/SIMComSIM7080.h>

// Zgeneral
// codeModem
// Sets up the second serial connection for the EnviroDIY custom cellular modem
#define modemSerial Serial1

// NOTE: Extra hardware and software serial ports are created in the "Settings
// for Additional Serial Ports" section

// Zgeneral
// codeModem
// modemBaud = sets the serial communication rate in bits per second of the
// EnviroDIY custom cellular modem
const int32_t modemBaud =    9600;  //  SIM7080 does auto-bauding by default, but for simplicity we set to 9600


// Original comment
// codeModem
// Modem Pins - Describe the physical pin connection of your modem to your board
// NOTE:  Use -1 for pins that do not apply

// Zgeneral
// codeModem
// modemVccPin = sets the power pin value of the EnviroDIY custom cellular modem
const int8_t modemVccPin     = 18;  // MCU pin controlling modem power --- Pin 18 is the power enable pin for the bee socket on Mayfly v1.0,
                                   //  use -1 if using Mayfly 0.5b or if the bee socket is constantly powered (ie you changed SJ18 on Mayfly1.0 to 3.3v)

// Zgeneral
// codeModem
// modemStatusPin = sets the pin value that is used to read in the modem status
const int8_t modemStatusPin  = 19;  // MCU pin used to read modem status

// Zgeneral
// codeModem
// modemSleepPin = sets the pin value that is used put the modem to sleep
const int8_t modemSleepRqPin = 23;  // MCU pin for modem sleep/wake request

// Zgeneral
// codeModem
// modemLEDPin = sets the pin value that is used to control the LED to show the status
// of the modem
const int8_t modemLEDPin = redLED;  // MCU pin connected an LED to show modem
                                    // status


// Original comment
// Network connection information

// ZGW
// codeLoggerBase
// apn = sets the type of SIM card being used. If you are not using a
// Hologram SIM, this will need to be changed.
const char* apn = "hologram";  // APN connection name, typically Hologram unless you have a different provider's SIM card. Change as needed


// Original comment
// Create the modem object

// Zgeneral
// codeModem
// SIMComSIM7080 modem7080 = sets up the modem using the parameters used earlier
SIMComSIM7080 modem7080(&modemSerial, modemVccPin, modemStatusPin,
                        modemSleepRqPin, apn);


// Original comment
// Create an extra reference to the modem by a generic name

// Zgeneral
// codeLoggerBase
// codeModem
// Sets up a second reference to the EnviroDIY custom cellular modem.
SIMComSIM7080 modem = modem7080;
/** End [sim7080] */
// ==========================================================================

// ==========================================================================
//  Using the Processor as a Sensor
// ==========================================================================
/** Start [processor_sensor] */

// Zgeneral
// This line calls the ProcessorStats.h library, which is used to call the
// voltage level the board is running on.
#include <sensors/ProcessorStats.h>


// Original Comment
// Create the main processor chip "sensor" - for general metadata

// Zgeneral
// mcuBoardVersion = sets the board version of the processor stats
// It's okay to use this as opposed to 1.0 because it functions the same.
const char*    mcuBoardVersion = "v0.5b";  // is only used for onboard battery voltage calculation,
                                          // "v0.5b" is safe to use on Mayfly v1.0 boards because the formula is the same

// Zgeneral
// ProcessorStats mcuBoard = sets up the processor stats of the board
ProcessorStats mcuBoard(mcuBoardVersion);
/** End [processor_sensor] */


// ==========================================================================
//  Maxim DS3231 RTC (Real Time Clock)
// ==========================================================================
/** Start [ds3231] */

// Zgeneral
// codeMeterHydros21
// Sets up the RTC (real-time clock) on the Mayfly board in order to tell time
#include <sensors/MaximDS3231.h>


// Original Comment
// Create a DS3231 sensor object

// Zgeneral
// MaximDS3231 ds3231 = makes the object for the RTC (real-time clock)
MaximDS3231 ds3231(1);
/** End [ds3231] */


// ==========================================================================
//  Meter Hydros 21 Conductivity, Temperature, and Depth Sensor
// ==========================================================================
/** Start [hydros21] */

// Zgeneral
// codeMeterHydros21
// The library MeterHydros21.h is used to set up the Hydros-21 CTD sensor
// (conductivity, temperature, and depth sensor) for later use
#include <sensors/MeterHydros21.h>

// ZGW
// codeMeterHydros21
// hydrosSDI12address = sets up the SDI-12 of the Hydros-21 CTD sensor
// For this project, this should be set to 1.
const char*   hydrosSDI12address = "1";  // The SDI-12 Address of the Hydros 21

// Zgeneral
// codeMeterHydros21
// hydrosNumberReadings = takes and averages the number of readings listed below.
// This allows for better accuracy of readings.
const uint8_t hydrosNumberReadings = 6;  // The number of readings to average

// Zgeneral
// codeMeterHydros21
// SDI12Power = sets the power pin for SDI-12
const int8_t  SDI12Power = sensorPowerPin;  // Power pin (-1 if unconnected)

// Zgeneral
// codeMeterHydros21
// SDI12 = sets the data pin for the SDI-12
const int8_t  SDI12Data  = 7;               // The SDI12 data pin


// Original Comment
// Create a Meter Hydros 21 sensor object

// Zgeneral
// codeMeterHydros21
// MeterHydros21 hydros = sets up the Hydros-21 CTD sensor based on the
// parameters set above
MeterHydros21 hydros(*hydrosSDI12address, SDI12Power, SDI12Data,
                     hydrosNumberReadings);
/** End [hydros21] */

// Zgeneral
// codeMeterHydros21
// Hydros-21 CTD sensor temperature conversion
// The Hydros-21 CTDsensor measures temperature in degrees Celsius by default.
// The code below converts the temperature to degrees Fahrenheit
// to be used later.

// Zgeneral
// Sets up a new variable to allow for temperature conversion
Variable *Hydros21_TempF = new MeterHydros21_Temp(&hydros, "12345678-abcd-1234-ef00-1234567890ab");

// Zgeneral
// codeMeterHydros21
// calculateHydros21degF = calculates Hydros-21 CTD sensor temperature
// in degrees Fahrenheit by pulling the value for the temperature degrees Celsius
// from the Meter Hydros-21
float calculateHydros21degF(void) {
  float MeterHydros21_TempC = Hydros21_TempF->getValue();
  float MeterHydros21_TempF = MeterHydros21_TempC*(1.8)+32;
  if (MeterHydros21_TempC == -9999) { //ensures temperature is not an error reading
      MeterHydros21_TempF = -9999;
  }
  return MeterHydros21_TempF;
}

// Zgeneral
// codeMeterHydros21
// Hydros21TempFVarName =
// Hydros21TempVarFUnit =
// Hydros21TempFVarResolution =
// Hydros21TempFUUID =
// Hydros21TempFVarCode =

// These 5 variables are parameters used to initialize the calculated
// Hydros-21 CTD sensor temperature in degrees Fahrenheit variable
const char* Hydros21TempFVarName = "Temperature";
const char *Hydros21TempVarFUnit = "Degree";
int Hydros21TempFVarResolution = 2;
const char *Hydros21TempFUUID = "12345678-abcd-1234-ef00-1234567890ab";
const char *Hydros21TempFVarCode = "Hydros21TempF";


// Zgeneral
// codeMeterHydros21
//HydrosCalcTempF = creates a new calculated temperature variable
// based on the calculated temperature in degrees Fahrenheit for the
// Hydros-21 CTD sensor.
Variable *HydrosCalcTempF = new Variable(calculateHydros21degF, Hydros21TempFVarResolution,
  Hydros21TempFVarName, Hydros21TempVarFUnit, Hydros21TempFVarCode, Hydros21TempFUUID);




Variable *Hydros21_Depthft = new MeterHydros21_Depth(&hydros, "12345678-abcd-1234-ef00-1234567890ab");
//Hydros temperature in degrees Fahrenheit

float calculateHydros21feet(void) {
  float MeterHydros21_Depth_mm = Hydros21_Depthft->getValue();
  float MeterHydros21_Depth_ft = MeterHydros21_Depth_mm/304.8;
  if (MeterHydros21_Depth_mm == -9999) {
      MeterHydros21_Depth_ft = -9999;
  }
  return MeterHydros21_Depth_ft;
}

// Zgeneral
// codeMeterHydros21
//Hydros21DepthftVarName =
//Hydros21DepthftVarUnit =
//Hydros21DepthftVarResolution =
//Hydros21DepthftUUID =
//Hydros21DepthftVarCode =
// These 5 variables are parameters used to initialize the calculated
// Hydros-21 CTD sensor temperature in degrees Fahrenheit variable
const char* Hydros21DepthftVarName = "Depth";
const char *Hydros21DepthftVarUnit = "Feet";
int Hydros21DepthftVarResolution = 2;
const char *Hydros21DepthftUUID = "12345678-abcd-1234-ef00-1234567890ab";
const char *Hydros21DepthftVarCode = "Hydros21Depthft";


// Zgeneral
// codeMeterHydros21
// HydrosCalcDepthft = creates a new calculated temperature variable
// based on the calculated temperature in degrees Fahrenheit for the
// Hydros-21 CTD sensor.

Variable *HydrosCalcDepthft = new Variable(calculateHydros21feet, Hydros21DepthftVarResolution,
  Hydros21DepthftVarName, Hydros21DepthftVarUnit, Hydros21DepthftVarCode, Hydros21DepthftUUID);

// ==========================================================================
//  Creating the Variable Array[s] and Filling with Variable Objects
// ==========================================================================
/** Start [variable_arrays] */

// Zgeneral
// codeMeterHydros21
// codeVariableArray
// Variable* variableList[] = sets up the variable array used by the
// VariableArray code to create variables for the Hydros-21 CTD parameters
// and the voltage the Mayfly is running on. In depth descriptions for each
// 4 variables are listed below.

// new MeterHydros21_Cond(&hydros)
// Name: Conductivity
// Units: uS/cm

// HydrosCalcTempF
// Name: Temperature
// Units: degrees Fahrenheit

// HydrosCalcDepthft
// Name: Depth
// Units: Feet

// ProcessorStats_Battery(&mcuBoard)
// Name: Battery Voltage
// Units: Volts

Variable* variableList[] = {
    new MeterHydros21_Cond(&hydros),
    HydrosCalcTempF,
    HydrosCalcDepthft,
    new ProcessorStats_Battery(&mcuBoard),
};


// Original Comment
// Count up the number of pointers in the array

// Zgeneral
// codeVariableArray
//int variableCount = sets up the number of variables in the variable array
//based on the number of variables found in the array above.
int variableCount = sizeof(variableList) / sizeof(variableList[0]);


// Original Comment
// Create the VariableArray object

// Zgeneral
// codeLoggerBase
// codeVariableArray
//VariableArray varArray = sets up the variable array
VariableArray varArray;
/** End [variable_arrays] */


// ==========================================================================
//  The Logger Object[s]
// ==========================================================================
/** Start [loggers] */


// Original Comment
// Create a logger instance

// Zgeneral
// codeLoggerBase
// Logger dataLogger = sets up the data logger in Logger Base
// This is dependent on the LoggerID, the logging interval in minutes, and the
// variable array.
Logger dataLogger(LoggerID, loggingInterval, &varArray);
/** End [loggers] */


// ==========================================================================
//  Creating Data Publisher[s]
// ==========================================================================
// Create a channel with fields on ThingSpeak in advance
// The fields will be sent in exactly the order they are in the variable array.
// Any custom name or identifier given to the field on ThingSpeak is irrelevant.
// No more than 8 fields of data can go to any one channel.  Any fields beyond
// the eighth in the array will be ignored.


// ZGW
// codeThingSpeakPublisher
// codePubSubClient
// const char* thingSpeakChannelID =
// const char* thingSpeakMQTTusername =
// const char* thingSpeakMQTTpassword =
// const char* thingSpeakMQTTclientID =
// These 4 variables are integral to uploading data to ThingSpeak.

// ZGW
// codeThingSpeakPublisher
// codePubSubClient
// These 4 variables are provided by ThingSpeak. Copy and paste them between
// the quotes in this code in order to upload them ThingSpeak. This code
// can only upload to one ThingSpeak channel currently. However, 8 fields can
// be uploaded to ThingSpeak. In this code, 4 fields are being uploaded
// to ThingSpeak (conductivity, temperature, depth, and battery voltage)

const char* thingSpeakChannelID =
    "";  // The numeric channel id for your channel
const char* thingSpeakMQTTusername =
    ""; //Your MQTT username
const char* thingSpeakMQTTpassword =
    ""; //Your MQTT password
const char* thingSpeakMQTTclientID =
    ""; //Your MQTT client ID


// Original Comment
// Create a data publisher for ThingSpeak

// Zgeneral
// codeThingSpeakPublisher
// codePubSubClient
// ThingSpeakPublisher.h is the header file for the ThingSpeakPublisher
#include <publishers/ThingSpeakPublisher.h>

// Zgeneral
// codeThingSpeakPublisher
// codePubSubClient
// ThingSpeakPublisher TsMqtt = sets up the ThingSpeakPublisher variable
ThingSpeakPublisher TsMqtt;
/** End [loggers] */


// ==========================================================================
//  Working Functions
// ==========================================================================
/** Start [working_functions] */


// Original Comment
// Flashes the LED's on the primary board

// Zgeneral
// codeThisScript
// void greenredflash( = used to flash the green and red lights on the board
void greenredflash(uint8_t numFlash = 4, uint8_t rate = 75) {
    for (uint8_t i = 0; i < numFlash; i++) {
        digitalWrite(greenLED, HIGH);
        digitalWrite(redLED, LOW);
        delay(rate);
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        delay(rate);
    }
    digitalWrite(redLED, LOW);
}


// Original Comment
// Reads the battery voltage
// NOTE: This will actually return the battery level from the previous update!

// Zgeneral
// codeThisScript
// Calls the value for the battery voltage
float getBatteryVoltage() {
    if (mcuBoard.sensorValues[0] == -9999) mcuBoard.update();
    return mcuBoard.sensorValues[0];
}

// Zarduinolabels
// This ends the variables section of the code.

// ==========================================================================
// Arduino Setup Function
// ==========================================================================
/** Start [setup] */



// Zarduinolabels
// This begins the setup section of the code. This section sets up the Mayfly
// based on the variables defined in the variable section above. There are two
// function braces that are critical to the function uploading correctly
// that can be easy to miss when uploading code.


void setup() { // Zbrace


    // Original Comment
    // Start the primary serial connection

    // Zgeneral
    // codeThisScript
    // Serial.begin = sets the baud rate as defined earlier in the code
    // as serialBaud
    Serial.begin(serialBaud);


    // Original Comment
    // Print a start-up note to the first serial port

    // Zgeneral
    // Prints some comments to the serial monitor
    // codeThisScript
    Serial.print(F("Now running "));
    Serial.print(sketchName);
    Serial.print(F(" on Logger "));
    Serial.println(LoggerID);
    Serial.println();

    Serial.print(F("Using ModularSensors Library version "));
    Serial.println(MODULAR_SENSORS_VERSION);
    Serial.print(F("TinyGSM Library version "));
    Serial.println(TINYGSM_VERSION);
    Serial.println();


    // Original comment
    // Start the serial connection with the modem

    // Zgeneral
    // codeModem
    // modemSerial.begin = sets up the baud rate of the EnviroDIY custom
    // cellular modem based on the modemBaud set earlier in the code.
    modemSerial.begin(modemBaud);


    // Original Comment
    // Set up pins for the LED's

    // Zgeneral
    // codeThisScript
    // pinMode(greenLED = sets whether the greenLED as an output
    // digitalWrite(greenLED = sets the initial state of the the greenLED LOW
    pinMode(greenLED, OUTPUT);
    digitalWrite(greenLED, LOW);

    // Zgeneral
    // codeThisScript
    // pinMode(redLED = sets whether the redLED as an output
    // digitalWrite(redLED = sets the initial state of the the redLED LOW
    pinMode(redLED, OUTPUT);
    digitalWrite(redLED, LOW);


    // Original Comment
    // Blink the LEDs to show the board is on and starting up

    // Zgeneral
    // codeThisScript
    // greenredflash( = calls the greenredflash() function made earlier to
    // flash the green and red LEDs
    greenredflash();


    // Zgeneral
    // pinMode( = sets the flash memory chip on the Mayfly for the Mayfly 1.0
    // Previous versions of the Mayfly (0.5b and before) do not have flash
    // memory.
    pinMode(20, OUTPUT); //for proper operation of the onboard flash memory chip's ChipSelect (Mayfly v1.0 and later)


    // Original Comment
    // Set the timezones for the logger/data and the RTC
    // Logging in the given time zone

    // Zgeneral
    // codeLoggerBase
    // Logger::setLoggerTimeZone(timeZone) = uses the timeZone variable
    // set the RTC time zone. Based on LoggerBase.
    Logger::setLoggerTimeZone(timeZone);


    // Original Comment
    // It is STRONGLY RECOMMENDED that you set the RTC to be in UTC (UTC+0)


    // Zgeneral
    // codeLoggerBase
    // Logger::setRTCTimeZone(0); = sets the RTC (real-time clock)
    // to UTC+0. EnviroDIY recommends to keep this as 0.
    // It is STRONGLY RECOMMENDED that you set the RTC to be in UTC (UTC+0)
    Logger::setRTCTimeZone(0);


    // Original Comment
    // Attach the modem and information pins to the logger

    // Zgeneral
    // codeLoggerBase
    // dataLogger.attachModem(
    // This line setups up the modem on the Mayfly based on the requirements
    // of the EnviroDIY custom cellular modem defined earlier.
    dataLogger.attachModem(modem);


    // Zgeneral
    // codeLoggerBase
    // dataLogger.setModemLED( = sets up up the light that
    // corresponds to the modem
    modem.setModemLED(modemLEDPin);

    // Zgeneral
    // codeLoggerBase
    // dataLogger.setLoggerPins( = sets up the logger pins based on
    // the pins defined earlier in the code
    dataLogger.setLoggerPins(wakePin, sdCardSSPin, sdCardPwrPin, buttonPin,
                             greenLED);


   // Original Comment
   // Begin the variable array[s], logger[s], and publisher[s]

   // Zgeneral
   // codeVariableArray
   // varArray.begin( = sets up the variables in the VariableArray code
   // based on previous defined items in this script
   varArray.begin(variableCount, variableList);

   // Zgeneral
   // codeLoggerBase
   // dataLogger.begin( = sets up the variables in the LoggerBase code
   // based on previously defined items this script
   dataLogger.begin(LoggerID, loggingInterval, &varArray);

   // Zgeneral
   // codeThingSpeakPublisher
   // codePubSubClient
   // codeLoggerBase
   // TsMqtt.begin( = sets up the MQTT and ThingSpeak parameters
   // to allow for uploading to a ThingSpeak channel
   TsMqtt.begin(dataLogger, &modem.gsmClient,
                thingSpeakChannelID,
                thingSpeakMQTTusername,
              thingSpeakMQTTpassword,
              thingSpeakMQTTclientID);


    // Original Comment
    // Note:  Please change these battery voltages to match your battery
    // Set up the sensors, except at lowest battery level

    // Zgeneral
    // codeVariableArray
    // These code sections check to see if the batter voltage is below
    // 3.4 volts. If that is the case, it will setup the sensors
    // within the VariableArray code
    if (getBatteryVoltage() > 3.4) {
        Serial.println(F("Setting up sensors..."));
        varArray.setupSensors();
    }

    /** Start [setup_sim7080] */

    // Zgeneral
    // codeModem
    // This code initializes the EnviroDIY modem with parameters included earlier
    modem.setModemWakeLevel(HIGH);   // ModuleFun Bee inverts the signal
    modem.setModemResetLevel(HIGH);  // ModuleFun Bee inverts the signal
    Serial.println(F("Waking modem and setting Cellular Carrier Options..."));
    modem.modemWake();  // NOTE:  This will also set up the modem
    modem.gsmModem.setBaud(modemBaud);   // Make sure we're *NOT* auto-bauding!
    modem.gsmModem.setNetworkMode(38);   // set to LTE only
                                         // 2 Automatic
                                         // 13 GSM only
                                         // 38 LTE only
                                         // 51 GSM and LTE only
    modem.gsmModem.setPreferredMode(1);  // set to CAT-M
                                         // 1 CAT-M
                                         // 2 NB-IoT
                                         // 3 CAT-M and NB-IoT
    /** End [setup_sim7080] */


    // Original Commment
    // Sync the clock if it isn't valid or we have battery to spare

    // Zgeneral
    // codeLoggerBase
    // This section checks to see if the battery voltage is above 3.55 volts
    // or if the RTC (real-time clock) is out of sync. If either of these is
    // the case, the RTC will be synced.
    if (getBatteryVoltage() > 3.55 || !dataLogger.isRTCSane()) {
        // Synchronize the RTC with NIST
        // This will also set up the modem
        dataLogger.syncRTC();
    }


    // Original Comment
    // Create the log file, adding the default header to it
    // Do this last so we have the best chance of getting the time correct and
    // all sensor names correct
    // Writing to the SD card can be power intensive, so if we're skipping
    // the sensor setup we'll skip this too.

    // Zgeneral
    // codeLoggerBase
    // This code section checks to see if the battery voltage is above
    // 3.4 volts. If this is case, the SD card will be set up.
    if (getBatteryVoltage() > 3.4) {
        Serial.println(F("Setting up file on SD card"));
        dataLogger.turnOnSDcard(
            true);  // true = wait for card to settle after power up
        dataLogger.createLogFile(true);  // true = write a new header
        dataLogger.turnOffSDcard(
            true);  // true = wait for internal housekeeping after write
    }

    // Call the processor sleep
    Serial.println(F("Putting processor to sleep\n"));

    // Zgeneral
    // codeLoggerBase
    // dataLogger.systemSleep( = This puts the data logger to sleep.
    dataLogger.systemSleep();

} // Zbrace

// Zarduinolabels
// This ends the setup section of the code.

/** End [setup] */


// ==========================================================================
//  Arduino Loop Function
// ==========================================================================
/** Start [loop] */

// Zarduinolabels
// This begins the loop section of the code. This section loops the main
// logging functions of the Mayfly. There are two function braces that are
// critical to the function uploading correctly that can be easy to miss
// when uploading code.


// Original Comment
// Use this short loop for simple data logging and sending

void loop() { // Zbrace


  // Original Comment
  // Note:  Please change these battery voltages to match your battery
  // At very low battery, just go back to sleep

  // Zgeneral
  // codeLoggerBase
  // This code section checks to see if the battery voltage is less than
  // 3.4 volts. If this is the case, it will put the system to sleep so
  // it does not waste the battery.
  if (getBatteryVoltage() < 3.4) {
      dataLogger.systemSleep();
  }


  // Original Comment
  // At moderate voltage, log data but don't send it over the modem

  // Zgeneral
  // codeLoggerBase
  // This code section checks to see if the battery voltage is less than 3.55
  // volts. If that is the case, the Mayfly will still log data,
  // but it will not send it over to the modem.
  else if (getBatteryVoltage() < 3.55) {
      dataLogger.logData();
  }


  // Original Comment
  // If the battery is good, send the data to the world

  // Zgeneral
  // codeLoggerBase
  // This code section checks logs and publishes data to ThingSpeak if
  // the battery voltage is at a good level.
  else {
      dataLogger.logDataAndPublish();
  }

} // Zbrace
/** End [loop] */
