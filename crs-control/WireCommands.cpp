
class WireCommands
{
public:

	const static int ZERO          = 0x01;  // sets the current position to 0.
	const static int INC_POSITION  = 0x02;  // increases the current position.
	const static int DEC_POSITION  = 0x03;  // decreases the current position.
	const static int SET_TOLERANCE = 0x04;  // sets the tolerance.
	const static int MOVE_TO       = 0x05;  // moves to the absolute position.
	const static int IS_BUSY       = 0x06;  // asks for isBusy value.
	const static int GET_POSITION  = 0x07;  // asks for the absolute position.
	const static int SET_ECHO      = 0x08;  // sets the echo value.
	const static int GET_ECHO      = 0x09;  // asks for the echo value.
	const static int SET_MAX_SPEED = 0x0A;  // sets the max speed value.
	const static int GET_MAX_SPEED = 0x0B;  // asks for the max speed value.
	const static int HOME          = 0x0C;  // starts the homing 
	const static int STOP          = 0x0D;
};