/** =========================================================================
 * @file logging_to_ThingSpeak.ino
 * @brief Example logging data and publishing to ThingSpeak.
 *
 * @author Sara Geleskie Damiano <sdamiano@stroudcenter.org>
 * @copyright (c) 2017-2020 Stroud Water Research Center (SWRC)
 *                          and the EnviroDIY Development Team
 *            This example is published under the BSD-3 license.
 *
 * Build Environment: Visual Studios Code with PlatformIO
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
/** Start [defines] */
#ifndef TINY_GSM_RX_BUFFER
#define TINY_GSM_RX_BUFFER 64
#endif
#ifndef TINY_GSM_YIELD_MS
#define TINY_GSM_YIELD_MS 2
#endif
#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 240
#endif
#define TINY_GSM_MODEM_XBEE
String incomingstring; // for any incoming data
/** End [defines] */

// ==========================================================================
//  Include the libraries required for any data logger
// ==========================================================================
/** Start [includes] */
// The Arduino library is needed for every Arduino program.
#include <Arduino.h>

// EnableInterrupt is used by ModularSensors for external and pin change
// interrupts and must be explicitly included in the main program.
#include <EnableInterrupt.h>

// To get all of the base classes for ModularSensors, include LoggerBase.
// NOTE:  Individual sensor definitions must be included separately.

#include <LoggerBase.h>

#include <ModularSensors.h> //was <LoggerBase.h>

//#include <StreamDebugger.h>

/** End [includes] */


// ==========================================================================
//  Data Logging Options
// ==========================================================================
/** Start [logging_options] */
// The name of this program file
const char* sketchName = "zlogging_to_thingspeak5.ino";
// Logger ID, also becomes the prefix for the name of the data file on SD card
const char* LoggerID = "logger";
// How frequently (in minutes) to log data
const uint8_t loggingInterval = 1;
// Your logger's timezone.
const int8_t timeZone = -5;  // Eastern Standard Time
// NOTE:  Daylight savings time will not be applied!  Please use standard time!

// Set the input and output pins for the logger
// NOTE:  Use -1 for pins that do not apply
const int32_t serialBaud = 115200;  // Baud rate for debugging
const int8_t  greenLED   = 8;       // Pin for the green LED
const int8_t  redLED     = 9;       // Pin for the red LED
const int8_t  buttonPin  = 21;      // Pin for debugging mode (ie, button pin)
const int8_t  wakePin    = A7;  // MCU interrupt/alarm pin to wake from sleep
// Set the wake pin to -1 if you do not want the main processor to sleep.
// In a SAMD system where you are using the built-in rtc, set wakePin to 1
const int8_t sdCardPwrPin   = -1;  // MCU SD card power pin
const int8_t sdCardSSPin    = 12;  // SD card chip select/slave select pin
const int8_t sensorPowerPin = 22;  // MCU pin controlling main sensor power
/** End [logging_options] */


// ==========================================================================
//  Wifi/Cellular Modem Options
// ==========================================================================
/** Start [xbee_cell_transparent] */
// For any Digi Cellular XBee's
// NOTE:  The u-blox based Digi XBee's (3G global and LTE-M global) can be used
// in either bypass or transparent mode, each with pros and cons
// The Telit based Digi XBees (LTE Cat1) can only use this mode.
HardwareSerial& modemSerial = Serial1;  // Use hardware serial if possible
#if defined STREAMDEBUGGER_DBG
#include <StreamDebugger.h>
StreamDebugger modemDebugger(modemSerial, STANDARD_SERIAL_OUTPUT);
#define modemSerHw modemDebugger
#else
#define modemSerHw modemSerial
#endif  // STREAMDEBUGGER_DBG

#define MS_BUILD_MODEM_XBEE_LTE_B


#include <modems/DigiXBeeCellularTransparent.h>

const int32_t modemBaud = 9600;  // All XBee's use 9600 by default

// Modem Pins - Describe the physical pin connection of your modem to your board
// NOTE:  Use -1 for pins that do not apply
// The pin numbers here are for a Digi XBee with a Mayfly and LTE adapter
// For options https://github.com/EnviroDIY/LTEbee-Adapter/edit/master/README.md
const int8_t modemVccPin = -1;     // MCU pin controlling modem power
                                   // Option: modemVccPin = A5, if Mayfly SJ7 is
                                   // connected to the ASSOC pin
const int8_t modemStatusPin = 13;  // MCU pin used to read modem status
// NOTE:  If possible, use the <code>STATUS/SLEEP_not</code> (XBee pin 13) for status, but
// the CTS pin can also be used if necessary
const bool   useCTSforStatus = false;  // Flag to use the CTS pin for status
const int8_t modemResetPin   = 20;     // MCU pin connected to modem reset pin
const int8_t modemSleepRqPin = 23;     // MCU pin for modem sleep/wake request
const int8_t modemLEDPin = redLED;     // MCU pin connected an LED to show modem
                                       // status

// Network connection information
//const char* apn = "VZWINTERNET";  // https://dataplans.digikey.com/
const char* apn = "hologram";  // APN for GPRS connection


// Create the modem object
DigiXBeeCellularTransparent modemXBCT(&modemSerHw, modemVccPin, modemStatusPin,
                                      useCTSforStatus, modemResetPin,
                                      modemSleepRqPin, apn);
// Create an extra reference to the modem by a generic name
DigiXBeeCellularTransparent modem = modemXBCT;
/** End [xbee_cell_transparent] */
// ==========================================================================


// ==========================================================================
//  Using the Processor as a Sensor
// ==========================================================================
/** Start [processor_sensor] */
#include <sensors/ProcessorStats.h>

// Create the main processor chip "sensor" - for general metadata
const char*    mcuBoardVersion = "v0.5b";
ProcessorStats mcuBoard(mcuBoardVersion);
/** End [processor_sensor] */

// ==========================================================================
//  Maxim DS3231 RTC (Real Time Clock)
// ==========================================================================
/** Start [ds3231] */
#include <sensors/MaximDS3231.h>

// Create a DS3231 sensor object
MaximDS3231 ds3231(1);
/** End [ds3231] */


// ==========================================================================
//  Meter Hydros 21 Conductivity, Temperature, and Depth Sensor
// ==========================================================================
/** Start [decagon_ctd] */
#include <sensors/DecagonCTD.h>

const char*   CTDSDI12address   = "1";      // The SDI-12 Address of the CTD
const uint8_t CTDNumberReadings = 6;        // The number of readings to average
const int8_t  SDI12Power = sensorPowerPin;  // Power pin (-1 if unconnected)
const int8_t  SDI12Data  = 7;               // The SDI12 data pin

// Create a Decagon CTD sensor object
DecagonCTD ctd(*CTDSDI12address, SDI12Power, SDI12Data, CTDNumberReadings);
/** End [decagon_ctd] */


// ==========================================================================
//  Creating the Variable Array[s] and Filling with Variable Objects
// ==========================================================================
/** Start [variable_arrays] */
Variable* variableList[] = {
    new DecagonCTD_Cond(&ctd, "12345678-abcd-1234-ef00-1234567890ab"),
    new DecagonCTD_Temp(&ctd, "12345678-abcd-1234-ef00-1234567890ab"),
    new DecagonCTD_Depth(&ctd, "12345678-abcd-1234-ef00-1234567890ab"),
    new ProcessorStats_Battery(&mcuBoard,
                               "12345678-abcd-1234-ef00-1234567890ab"),
    new Modem_RSSI(&modem, "12345678-abcd-1234-ef00-1234567890ab")};
// Count up the number of pointers in the array
int variableCount = sizeof(variableList) / sizeof(variableList[0]);

// Create the VariableArray object
VariableArray varArray;
/** End [variable_arrays] */


// ==========================================================================
//  The Logger Object[s]
// ==========================================================================
/** Start [loggers] */
// Create a logger instance
Logger dataLogger;
/** End [loggers] */


// ==========================================================================
//  Creating Data Publisher[s]
// ==========================================================================
// Create a channel with fields on ThingSpeak in advance
// The fields will be sent in exactly the order they are in the variable array.
// Any custom name or identifier given to the field on ThingSpeak is irrelevant.
// No more than 8 fields of data can go to any one channel.  Any fields beyond
// the eighth in the array will be ignored.

const char* thingSpeakMQTTKey =
    "VE16X56XTCUQF66D";  // Your MQTT API Key from Account > MyProfile.
const char* thingSpeakChannelID =
    "1444268";  // The numeric channel id for your channel
const char* thingSpeakChannelKey =
    "M0EJX42C6OK0MTBL";  // The Write API Key for your channel


// Create a data publisher for ThingSpeak
#include <publishers/ThingSpeakPublisher.h>
ThingSpeakPublisher TsMqtt;
/** End [loggers] */


// ==========================================================================
//  Working Functions
// ==========================================================================
/** Start [working_functions] */
// Flashes the LED's on the primary board
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
float getBatteryVoltage() {
    if (mcuBoard.sensorValues[0] == -9999) mcuBoard.update();
    return mcuBoard.sensorValues[0];
}
/** End [working_functions] */


// ==========================================================================
//  Arduino Setup Function
// ==========================================================================
/** Start [setup] */
void setup() {
    // Start the primary serial connection
    Serial.begin(serialBaud);

    // Print a start-up note to the first serial port
    Serial.print(F("Now running "));
    Serial.print(sketchName);
    Serial.print(F(" on Logger "));
    Serial.println(LoggerID);
    Serial.println();

    Serial.print(F("Using ModularSensors Library version "));
    //Serial.println(MODULAR_SENSORS_VERSION);
    Serial.print(F("TinyGSM Library version "));
    Serial.println(TINYGSM_VERSION);
    Serial.println();

    // Start the serial connection with the modem
    modemSerial.begin(modemBaud);

    // Set up pins for the LED's
    pinMode(greenLED, OUTPUT);
    digitalWrite(greenLED, LOW);
    pinMode(redLED, OUTPUT);
    digitalWrite(redLED, LOW);
    // Blink the LEDs to show the board is on and starting up
    greenredflash();

    // Set the timezones for the logger/data and the RTC
    // Logging in the given time zone
    Logger::setLoggerTimeZone(timeZone);
    // It is STRONGLY RECOMMENDED that you set the RTC to be in UTC (UTC+0)
    Logger::setRTCTimeZone(0);

    // Attach the modem and information pins to the logger
    dataLogger.attachModem(modem);
    modem.setModemLED(modemLEDPin);
    dataLogger.setLoggerPins(wakePin, sdCardSSPin, sdCardPwrPin, buttonPin,
                             greenLED);

    // Begin the variable array[s], logger[s], and publisher[s]
    varArray.begin(variableCount, variableList);
    dataLogger.begin(LoggerID, loggingInterval, &varArray);
    TsMqtt.begin(dataLogger, &modem.gsmClient, thingSpeakMQTTKey,
                 thingSpeakChannelID, thingSpeakChannelKey);

    // Note:  Please change these battery voltages to match your battery
    // Set up the sensors, except at lowest battery level
    if (getBatteryVoltage() > 3.4) {
        Serial.println(F("Setting up sensors..."));
        varArray.setupSensors();
    }

    // Sync the clock if it isn't valid or we have battery to spare
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
    if (getBatteryVoltage() > 3.4) {
        Serial.println(F("Setting up file on SD card"));
        dataLogger.turnOnSDcard(
            true);  // true = wait for card to settle after power up
        dataLogger.createLogFile(true);  // true = write a new header
        dataLogger.turnOffSDcard(
            true);  // true = wait for internal housekeeping after write
    }

    // Call the processor sleep
    Serial.println(F("Putting processor to sleep"));
    dataLogger.systemSleep();
}
/** End [setup] */


// ==========================================================================
//  Arduino Loop Function
// ==========================================================================
/** Start [loop] */
// Use this short loop for simple data logging and sending
void loop() {
/* if (Serial.available()) {
    incomingstring = Serial.readString();

    Serial.print(incomingstring);

  }*/
    // Note:  Please change these battery voltages to match your battery
    // At very low battery, just go back to sleep
    if (getBatteryVoltage() < 3.4) {
        dataLogger.systemSleep();
    }
//     At moderate voltage, log data but don't send it over the modem
    else if (getBatteryVoltage() < 3.55) {
        dataLogger.logData();
    }
    // If the battery is good, send the data to the world
    else {
        dataLogger.logDataAndPublish();
      }
}
/** End [loop] */
