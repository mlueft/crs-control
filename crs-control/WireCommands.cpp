
class WireCommands
{
public:

	const static int ZERO          = 0x01;
	const static int INC_POSITION  = 0x02;
	const static int DEC_POSITION  = 0x03;
	const static int SET_TOLERANCE = 0x04;
	const static int MOVE_TO       = 0x05;
	const static int IS_BUSY       = 0x06;
	const static int GET_POSITION  = 0x07;
	const static int SET_ECHO      = 0x08;
	const static int GET_ECHO      = 0x09;
	const static int SET_MAX_SPEED = 0x0A;
	const static int GET_MAX_SPEED = 0x0B;
	const static int HOME          = 0x0C;
	const static int STOP          = 0x0D;
};