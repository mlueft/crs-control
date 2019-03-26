# CRS-control
Software for Arduino based control system for continous rotation servos. Each arduino is determined to control one H-Bridge which is connected to one DC-motor. The motor has to give feedback about relative movement using a rotary encoder directly to the arduino.

CRS-control is designed to be a subsystem used in a CNC-machine. The CNC-machine is determined to be build of a central unit that coordinates movement of all motors by communicating to each subsystem(CRS-control) via I2C(twi-Two wire communication).

## TWI-Protocol

cmd | param | description
--- | ----- | -----------
01 | -   | get current position.
02 | -   | get current speed.
03 | int | set target position
04 | int | set max speed
05 | -   | get max speed
06 | int | set max acceleration
07 | -   | get max acceleration
08 | -   | get max absolute position
09 | int | set max absolute position
10 | -   | get min absolute position
11 | int | set min absolute position
12 | int | start rotating left
13 | int | start rotating right
14 | -   | stop rotating

## Modes
The system offers three modes to control the connected motor.
To reduce risk of damage each mode has to be enabled in the config file and requires reflashing of the firmware.

Mode I - Free continuous rotation without position control
------------------------------------------------------------
Requirements: minPosition == null && maxPosition == null
Mode has to be enabled in config.
 
Mode II - Absolute positioning with min and max
------------------------------------------------
Requirements: minPosition != null && maxPosition != null
Mode has to be enabled in config.

Mode III - Absolute position without min and max
------------------------------------------------
Requirements: MinPosition == null && maxPosition == null
Mode has to be enabled in config.


