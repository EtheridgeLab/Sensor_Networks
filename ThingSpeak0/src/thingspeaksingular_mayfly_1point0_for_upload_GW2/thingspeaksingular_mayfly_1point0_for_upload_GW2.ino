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

/*

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
#include <Arduino.h>

// Zgeneral
// EnableInterrupt.h is a library that allows for pins on the Mayfly to be
// changed for other uses.
#include <EnableInterrupt.h>

// Include the main header for ModularSensors
// Zgeneral // codeLoggerBase
#include <ModularSensors.h>
/** End [includes] */


// ==========================================================================
//  Data Logging Options
// ==========================================================================
/** Start [logging_options] */


// The name of this program file

// ZGW // codeLoggerBase
// Change the name of the Arduino file if necessary
const char* sketchName = "thingspeaksingular_mayfly_1point0_for_upload_GW2.ino";


// Logger ID, also becomes the prefix for the name of the data file on SD card
// Zgeneral // codeLoggerBase
const char* LoggerID = "logger";



// How frequently (in minutes) to log data
// ZGW // codeLoggerBase
// Change the length of the logging interval if necessary (in minutes)
const uint8_t loggingInterval = 1;


// Your logger's timezone.
// NOTE:  Daylight savings time will not be applied!  Please use standard time!
// ZGW // codeLoggerBase
const int8_t timeZone = -5;  // Eastern Standard Time

// Zgeneral // codeThisScript
const int32_t serialBaud = 115200;   // Baud rate for debugging

// Zgeneral // codeLoggerBase
const int8_t  greenLED   = 8;       // Pin for the green LED

// Zgeneral // codeThisScript
const int8_t  redLED     = 9;       // Pin for the red LED

// Zgeneral // codeLoggerBase
const int8_t  buttonPin  = 21;      // Pin for debugging mode (ie, button pin)


// Mayfly 0.x D31 = A7
// Set the wake pin to -1 if you do not want the main processor to sleep.
// In a SAMD system where you are using the built-in rtc, set wakePin to 1
// Zgeneral // codeLoggerBase
const int8_t  wakePin    = 31;  // MCU interrupt/alarm pin to wake from sleep

// Zgeneral // codeLoggerBase
const int8_t sdCardPwrPin   = -1;  // MCU SD card power pin

// Zgeneral // codeLoggerBase
const int8_t sdCardSSPin = 12;  // SD card chip select/slave select pin

// Zgeneral // codeLoggerBase
const int8_t sensorPowerPin = 22;  // MCU pin controlling main sensor power
/** End [logging_options] */


// ==========================================================================
//  Wifi/Cellular Modem Options
// ==========================================================================
/** Start [sim7080] */
// For almost anything based on the SIMCom SIM7080G
// Zgeneral // codeModem
#include <modems/SIMComSIM7080.h>

// Zgeneral // codeModem
#define modemSerial Serial1

// NOTE: Extra hardware and software serial ports are created in the "Settings
// for Additional Serial Ports" section
// Zgeneral // codeModem
const int32_t modemBaud =    9600;  //  SIM7080 does auto-bauding by default, but for simplicity we set to 9600


// codeModem
// Modem Pins - Describe the physical pin connection of your modem to your board
// NOTE:  Use -1 for pins that do not apply

// Zgeneral // codeModem
const int8_t modemVccPin     = 18;  // MCU pin controlling modem power --- Pin 18 is the power enable pin for the bee socket on Mayfly v1.0,
                                   //  use -1 if using Mayfly 0.5b or if the bee socket is constantly powered (ie you changed SJ18 on Mayfly1.0 to 3.3v)

// Zgeneral // codeModem
const int8_t modemStatusPin  = 19;  // MCU pin used to read modem status

// Zgeneral // codeModem
const int8_t modemSleepRqPin = 23;  // MCU pin for modem sleep/wake request

// Zgeneral // codeModem
const int8_t modemLEDPin = redLED;  // MCU pin connected an LED to show modem
                                    // status

// Network connection information
// ZGW // codeLoggerBase
// Change the name of the SIM card if you are not using a Hologram SIM card.
const char* apn = "hologram";  // APN connection name, typically Hologram unless you have a different provider's SIM card. Change as needed


// Create the modem object
// Zgeneral // codeModem
SIMComSIM7080 modem7080(&modemSerial, modemVccPin, modemStatusPin,
                        modemSleepRqPin, apn);

// Create an extra reference to the modem by a generic name
// Zgeneral // codeLoggerBase // codeModem
SIMComSIM7080 modem = modem7080;
/** End [sim7080] */
// ==========================================================================

// ==========================================================================
//  Using the Processor as a Sensor
// ==========================================================================
/** Start [processor_sensor] */
// Zgeneral
#include <sensors/ProcessorStats.h>


// Create the main processor chip "sensor" - for general metadata
// Zgeneral
const char*    mcuBoardVersion = "v0.5b";  // is only used for onboard battery voltage calculation,
                                          // "v0.5b" is safe to use on Mayfly v1.0 boards because the formula is the same

// Zgeneral
ProcessorStats mcuBoard(mcuBoardVersion);
/** End [processor_sensor] */


// ==========================================================================
//  Maxim DS3231 RTC (Real Time Clock)
// ==========================================================================
/** Start [ds3231] */
// Zgeneral // codeMeterHydros21
#include <sensors/MaximDS3231.h>


// Create a DS3231 sensor object
// Zgeneral
MaximDS3231 ds3231(1);
/** End [ds3231] */


// ==========================================================================
//  Meter Hydros 21 Conductivity, Temperature, and Depth Sensor
// ==========================================================================
/** Start [hydros21] */
// Zgeneral // codeMeterHydros21
#include <sensors/MeterHydros21.h>

// ZGW // codeMeterHydros21
const char*   hydrosSDI12address = "1";  // The SDI-12 Address of the Hydros 21

// Zgeneral // codeMeterHydros21
const uint8_t hydrosNumberReadings = 6;  // The number of readings to average

// Zgeneral // codeMeterHydros21
const int8_t  SDI12Power = sensorPowerPin;  // Power pin (-1 if unconnected)

// Zgeneral // codeMeterHydros21
const int8_t  SDI12Data  = 7;               // The SDI12 data pin

// Create a Meter Hydros 21 sensor object
// Zgeneral // codeMeterHydros21
MeterHydros21 hydros(*hydrosSDI12address, SDI12Power, SDI12Data,
                     hydrosNumberReadings);
/** End [hydros21] */

// Zgeneral // codeMeterHydros21
Variable *Hydros21_TempF = new MeterHydros21_Temp(&hydros, "12345678-abcd-1234-ef00-1234567890ab");

// Zgeneral // codeMeterHydros21
float calculateHydros21degF(void) {
  float MeterHydros21_TempC = Hydros21_TempF->getValue();
  float MeterHydros21_TempF = MeterHydros21_TempC*(1.8)+32;
  if (MeterHydros21_TempC == -9999) { //ensures temperature is not an error reading
      MeterHydros21_TempF = -9999;
  }
  return MeterHydros21_TempF;
}

// Zgeneral // codeMeterHydros21
const char* Hydros21TempFVarName = "Temperature";
const char *Hydros21TempVarFUnit = "Degree";
int Hydros21TempFVarResolution = 2;
const char *Hydros21TempFUUID = "12345678-abcd-1234-ef00-1234567890ab";
const char *Hydros21TempFVarCode = "Hydros21TempF";

// Zgeneral // codeMeterHydros21
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

// Zgeneral // codeMeterHydros21
const char* Hydros21DepthftVarName = "Depth";
const char *Hydros21DepthftVarUnit = "Feet";
int Hydros21DepthftVarResolution = 2;
const char *Hydros21DepthftUUID = "12345678-abcd-1234-ef00-1234567890ab";
const char *Hydros21DepthftVarCode = "Hydros21Depthft";


// Zgeneral // codeMeterHydros21
Variable *HydrosCalcDepthft = new Variable(calculateHydros21feet, Hydros21DepthftVarResolution,
  Hydros21DepthftVarName, Hydros21DepthftVarUnit, Hydros21DepthftVarCode, Hydros21DepthftUUID);

// ==========================================================================
//  Creating the Variable Array[s] and Filling with Variable Objects
// ==========================================================================
/** Start [variable_arrays] */
// Zgeneral // codeMeterHydros21 // codeVariableArray

// new MeterHydros21_Cond(&hydros)
// Units: Conductivity in uS/cm

// HydrosCalcTempF
// Units: Temperature in degrees Fahrenheit

// HydrosCalcDepthft
// Units: Depth in feet

// ProcessorStats_Battery(&mcuBoard)
// Units: Voltage in volts

Variable* variableList[] = {
    new MeterHydros21_Cond(&hydros),
    HydrosCalcTempF,
    HydrosCalcDepthft,
    new ProcessorStats_Battery(&mcuBoard),
};


// Count up the number of pointers in the array

// Zgeneral // codeVariableArray
int variableCount = sizeof(variableList) / sizeof(variableList[0]);

// Create the VariableArray object
// Zgeneral
// codeLoggerBase
// codeVariableArray
VariableArray varArray;
/** End [variable_arrays] */


// ==========================================================================
//  The Logger Object[s]
// ==========================================================================
/** Start [loggers] */

// Create a logger instance
// Zgeneral // codeLoggerBase
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


// ZGW // codeThingSpeakPublisher // codePubSubClient
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

// Create a data publisher for ThingSpeak
// Zgeneral // codeThingSpeakPublisher // codePubSubClient
#include <publishers/ThingSpeakPublisher.h>

// Zgeneral // codeThingSpeakPublisher // codePubSubClient
ThingSpeakPublisher TsMqtt;
/** End [loggers] */


// ==========================================================================
//  Working Functions
// ==========================================================================
/** Start [working_functions] */

// Flashes the LED's on the primary board
// Zgeneral // codeThisScript
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

// Reads the battery voltage
// NOTE: This will actually return the battery level from the previous update!

// codeThisScript
// Calls the value for the battery voltage
float getBatteryVoltage() {
    if (mcuBoard.sensorValues[0] == -9999) mcuBoard.update();
    return mcuBoard.sensorValues[0];
}
// Zarduinolabel

// ==========================================================================
// Arduino Setup Function
// ==========================================================================
/** Start [setup] */
// Zarduinolabels

void setup() { // Zbrace

    // Start the primary serial connection
    // Zgeneral // codeThisScript
    Serial.begin(serialBaud);

    // Zgeneral // codeThisScript
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

    // Start the serial connection with the modem
    // Zgeneral // codeModem
    modemSerial.begin(modemBaud);

    // Set up pins for the LED's
    // Zgeneral // codeThisScript
    pinMode(greenLED, OUTPUT);
    digitalWrite(greenLED, LOW);

    // Zgeneral // codeThisScript
    pinMode(redLED, OUTPUT);
    digitalWrite(redLED, LOW);

    // Blink the LEDs to show the board is on and starting up
    // Zgeneral // codeThisScript
    greenredflash();


    // Zgeneral
    pinMode(20, OUTPUT); //for proper operation of the onboard flash memory chip's ChipSelect (Mayfly v1.0 and later)


    // Set the timezones for the logger/data and the RTC
    // Logging in the given time zone
    // Zgeneral // codeLoggerBase
    Logger::setLoggerTimeZone(timeZone);

    // It is STRONGLY RECOMMENDED that you set the RTC to be in UTC (UTC+0)
    // Zgeneral // codeLoggerBase
    Logger::setRTCTimeZone(0);

    // Attach the modem and information pins to the logger
    // Zgeneral // codeLoggerBase
    dataLogger.attachModem(modem);

    // Zgeneral // codeLoggerBase
    modem.setModemLED(modemLEDPin);

    // Zgeneral // codeLoggerBase
    dataLogger.setLoggerPins(wakePin, sdCardSSPin, sdCardPwrPin, buttonPin,
                             greenLED);

   // Begin the variable array[s], logger[s], and publisher[s]
   // Zgeneral // codeVariableArray
   varArray.begin(variableCount, variableList);

   // Zgeneral // codeLoggerBase
   dataLogger.begin(LoggerID, loggingInterval, &varArray);

   // Zgeneral // codeThingSpeakPublisher // codePubSubClient // codeLoggerBase
   TsMqtt.begin(dataLogger, &modem.gsmClient,
                thingSpeakChannelID,
                thingSpeakMQTTusername,
              thingSpeakMQTTpassword,
              thingSpeakMQTTclientID);


    // Note:  Please change these battery voltages to match your battery
    // Set up the sensors, except at lowest battery level
    // Zgeneral // codeVariableArray
    if (getBatteryVoltage() > 3.4) {
        Serial.println(F("Setting up sensors..."));
        varArray.setupSensors();
    }

    /** Start [setup_sim7080] */
    // Zgeneral // codeModem
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

    // Sync the clock if it isn't valid or we have battery to spare
    // Zgeneral // codeLoggerBase
    if (getBatteryVoltage() > 3.55 || !dataLogger.isRTCSane()) {
        // Synchronize the RTC with NIST
        // This will also set up the modem
        dataLogger.syncRTC();
    }

    // Create the log file, adding the default header to it
    // Do this last so we have the best chance of getting the time correct and
    // all sensor names correct
    // Writing to the SD card can be power intensive, so if we're skipping
    // the sensor setup we'll skip this too.
    // Zgeneral // codeLoggerBase
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
    // Zgeneral // codeLoggerBase
    dataLogger.systemSleep();

} // Zbrace

// Zarduinolabels.

/** End [setup] */

// ==========================================================================
//  Arduino Loop Function
// ==========================================================================
/** Start [loop] */

// Zarduinolabels

// Use this short loop for simple data logging and sending

void loop() { // Zbrace
  // Note:  Please change these battery voltages to match your battery
  // At very low battery, just go back to sleep
  // Zgeneral // codeLoggerBase
  if (getBatteryVoltage() < 3.4) {
      dataLogger.systemSleep();
  }
  // At moderate voltage, log data but don't send it over the modem
  // Zgeneral // codeLoggerBase
  else if (getBatteryVoltage() < 3.55) {
      dataLogger.logData();
  }
  // If the battery is good, send the data to the world
  // Zgeneral // codeLoggerBase
  else {
      dataLogger.logDataAndPublish();
  }
} // Zbrace
/** End [loop] */
