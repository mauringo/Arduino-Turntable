# Arduino-Turnatbale

The idea is to use an Arduino UNO to feed an inverter with three phase sPWM signals
in order to run a synchronous motor at constant spees. 

The Firmware for the Arduino UNO has been done starting from the Martin Nawrath's DDS 
software: 
http://interface.khm.de/index.php/lab/interfaces-advanced/arduino-dds-sinewave-generator/

Thank you very much!

The motor i am using is a modified Nidec 24H667H010 bldc motor: removing the integrated 
control circuit it's possible to run the motor like a synchronous motor.
