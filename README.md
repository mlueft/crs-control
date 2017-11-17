# CRS-control
Software for Arduino based control system for continous rotation servos. Each arduino is determined to control one H-Bridge which is connected to one DC-motor. The motor has to give feedback about relative movement using a rotary encoder directly to the arduino.

CRS-control is designed to be a subsystem used in a CNC-machine. The CNC-machine is determined to be build of a central unit that coordinates movement of all motors by communicating to each subsystem(CRS-control) via I2C(twi-Two wire communication).

## TWI-Protocol

cmd | param | description
--- | ----- | -----------
01 | - | get current position.
02 | - | get current speed.
03 |  | set target position
04 |  | set max speed
05 |  | set max acceleration
