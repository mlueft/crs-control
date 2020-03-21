#ifndef PROCEDUREFUNCTION_H
#define PROCEDUREFUNCTION_H

/**
*   State codes for the currently running procedure functions.
*/
class ProcedureFunction
{
public:

	const static int NONE = 0x01;
	const static int RELEASE = 0x02;
	const static int DETENTION = 0x04;
};
#endif
