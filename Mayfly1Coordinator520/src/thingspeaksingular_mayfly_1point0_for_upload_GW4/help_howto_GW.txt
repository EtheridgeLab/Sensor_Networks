thingspeaksingular_mayfly_1point0_for_upload_GW(1-5).ino

Hi there! This script is uploaded to the Mayfly with the purpose of recording, logging, and transmitting data to ThingSpeak.


A few general notes:
1. This script can be uploaded to the Arduino.

2. This script has two types of labels: Z labels and code labels.

Z labels provide comments on the functionality of certain parts of the script.

code labels provide reference points for where certain types of the code are located.

Below is a legend of the types of Z labels and code labels found in the script.

-----------------------------------------------------------------------------

Z labels

The following labels will be helpful editing the code later.
Ctrl+F the how_to_thingspeaksingular_mayfly_1point0_GW#.ino script for these labels.

ZGW = shows the things that need to be changed in the Arduino file
if a new GW station needs to be added

Zgeneral = generally tells what code does

Zarduinolabels = labels the variables section, setup function, and loop function
in the Arduino code for easy reference. The setup and loop functions are two
key functions that MUST be used in order for the program to run.

Zbrace = Critical braces (   {   }   ) that are easy to miss in the setup and
loop functions.


-------------------------------------------------------------------------------

code labels

The following labels indicate where particular code sections
come from. Ctrl+F the how_to_thingspeaksingular_mayfly_1point0_GW# for these
labels to see what C++ code is dependent on certain libraries.

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

This document will be updated if any additional labels or information is needed.