#include "TWI.cpp"
#include "WireCommand.h"
#include "Procedure.h"
#include "Configuration.h"
#include "ProcedureFunction.h"

class AxActuator: public TWI
{

    private:

	/**
	*
	*/
	volatile long position = 0;

	/**
	*
	*/
	long targetPosition = 0;

	/**
	*
	*/
    long startPosition = 0;

	/**
	*
	*/
	long lastPosition = 0;

	/**
	 *
	 */
	unsigned long  lastTime = 0;

	/**
	*
	*/
	int procedure = Procedure::NONE;
	
	/**
	*
	*/
	int procedureFunction = ProcedureFunction::NONE;
	
	/**
	*
	*/
	int echoValue = 0;

	/**
	*
	*/
    unsigned long tolerance = 2;

	/**
	*
	*/
    int rampe               = 1000;

	/**
	*
	*/
    int maxSpeed            = 1000;
    
    /**
     *
     */
    float currentSpeed        = 0;

    /**
     *
     */
    int currentPWMSpeed     = 0;

	public:
    /**
      *
      *
      */    
    AxActuator()
    {

    }

	/**
	*
	*/
	void setTolerance(int value)
	{
		this->tolerance = value;
	}

	/**
	*
	*/
	int getTolerance()
	{
		return this->tolerance;
	}

	/**
	*
	*/
	void setRampe(int value)
	{
		this->rampe = value;
	}

	/**
	*
	*/
	int getRampe()
	{
		return this->rampe;
	}

	/**
	*
	*/
    void setMaxSpeed( int value)
    {
    	this->maxSpeed = max(0,min(255,value));
    }

	/**
	*
	*/
    void setPosition(long value)
    {
    	position = value;
    }

	/**
	*
	*/
    long getPosition()
    {
    	return position;
    }

	/**
	*
	*/
    void setCurrentSpeed(long value)
    {
    	currentSpeed = value;
    }

	/**
	*
	*/
    long getCurrentSpeed()
    {
    	return currentSpeed;
    }







	/**
	*
	*/
    bool isBusy( int excludeProcedures = 0 )
    {


    	if (
    			(procedure&~(excludeProcedures|Procedure::NONE)) > 0
		)
    		return true;

        if( abs(targetPosition-position) <= tolerance /*&& getCurrentSpeed() < 30*/)
            return false;
        
        return true;
    }

	/**
	*
	*/
    void moveTo( unsigned long value)
    {
        startPosition = position;
        targetPosition = value;
    }

	/**
	*
	*/
    void main()
    {
    
    	//
    	// Determine current movement speed.
    	//
    	int deltaTime = millis() - lastTime ;
        if( deltaTime > 50 )
        {

        	int deltaPosition = abs(position-lastPosition);

        	currentSpeed = deltaPosition*1000/(float)deltaTime;

            lastTime = millis();
            lastPosition = position;

        }

        //
        // executing procedures
        //
		if (procedure == Procedure::HOMING)
		{


			if (procedureFunction == ProcedureFunction::RELEASE)
			{

				// homingswitch is already pressed
				if (digitalRead(PIN_HOME) == HIGH) {

					// if machine isn't busy
					// move tool to release home switch
					if( !this->isBusy(Procedure::HOMING) )
					{
						this->moveTo(this->position + int(this->tolerance * 2));
					}
				}
				else
				{
					// if homeing switch isn't pressed
					// start homeing movement
					procedureFunction = ProcedureFunction::DETENTION;
				}
			}
			else if (procedureFunction == ProcedureFunction::DETENTION)
			{

				// homeing switch isn't pressed.
				if (digitalRead(PIN_HOME) == LOW) {

					// if machine isn't busy
					// start moving towarts homing switch
					if (!this->isBusy(Procedure::HOMING))
					{
						this->moveTo(this->position - int(this->tolerance * 2));
					}
				}
				else
				{
					// Homing switch reached
					// set position to offset and end
					// procedure
					procedure = Procedure::NONE;
					procedureFunction = ProcedureFunction::NONE;

					this->position = ZERO_OFFSET;
					this->targetPosition = ZERO_OFFSET;
				}
			}
		}



		//
		// direction control
		//
		/**/
        if(targetPosition < position)
        {
            digitalWrite( PIN_ENABLE_A, LOW);
            digitalWrite( PIN_ENABLE_B, HIGH);
        }
        else if(targetPosition > position)
        {
            digitalWrite( PIN_ENABLE_A, HIGH);
            digitalWrite( PIN_ENABLE_B, LOW);
        }
		/**/


        //
        // speed control
        //
        int deltaPosition = abs(targetPosition-position);
        if( deltaPosition > tolerance )
        {

        	// currentSpeed
        	referenceSpeed = getCurrentReferenceSpeed();

        	deltaSpeed = currentSpeed-referenceSpeed;

        	accelerationHisteresis = getAccelerationHisteresis(deltaPosition);

        	acceleration = 0;
        	if( deltaSpeed > accelerationHisteresis )
        	{
        		// acceleration
        		acceleration = -2;
        	}
        	else if( deltaSpeed < -accelerationHisteresis )
        	{
        		// break
        		acceleration = 2;

        	}else{
        		// just running

        	}

        	currentPWMSpeed += acceleration;
        	currentPWMSpeed = turnSpeed( currentPWMSpeed );

        }
        else
        {
            turnSpeed(0);
        }


    }

    int referenceSpeed;
    int deltaSpeed;
    float accelerationHisteresis;
    float acceleration;


    void trace()
    {


    	Serial.print( position );
    	Serial.print( ";" );

    	Serial.print( targetPosition );
    	Serial.print( ";" );

    	Serial.print( currentSpeed );
    	Serial.print( ";" );

    	Serial.print( deltaSpeed );
    	Serial.print( ";" );

    	Serial.print( referenceSpeed );
    	Serial.print( ";" );

    	Serial.print( accelerationHisteresis);
    	Serial.print( ";" );

    	Serial.print( acceleration );
    	Serial.print( ";" );

    	Serial.print( currentPWMSpeed );
    	Serial.print( ";" );

    	Serial.println( "" );

    	/*
    	Serial.write( "current speed:" );
    	Serial.println( currentSpeed );

    	Serial.write( "deltaSpeed:" );
    	Serial.println( deltaSpeed );

    	Serial.write( "deltaSpeed:" );
    	Serial.println( referenceSpeed );

    	Serial.write( "accelerationHisteresis:" );
    	Serial.println( accelerationHisteresis);

    	Serial.write( "acceleration:" );
    	Serial.println( acceleration );

    	Serial.write( "currentPWMSpeed:" );
    	Serial.println( currentPWMSpeed );

    	Serial.write( "================\n" );
    	/**/

    }

	/**
	*
	*/
    float getAccelerationHisteresis(int deltaPosition)
    {

    	float steigung = 0.01;
    	return deltaPosition*steigung;

    }

	/**
	*
	*/
    unsigned int getCurrentReferenceSpeed()
    {

        //return 50;
        int minSpeed = 10;

        // We are in slow down phase at the end of the way
        int deltaTarget = abs(targetPosition-position);
        if( deltaTarget < rampe )
        {

            return max(

				minSpeed,

				maxSpeed
				*
				deltaTarget/(float)rampe

			);

        }
        
        // we are at full speed
        return maxSpeed;
        
    }





	/**
	*
	*/
    void stop()
    {
		procedure = Procedure::NONE;
		procedureFunction = ProcedureFunction::NONE;
		targetPosition = position;
        turnSpeed(0);
        digitalWrite( PIN_ENABLE_A, LOW);
        digitalWrite( PIN_ENABLE_B, LOW);
    }

	/**
	*
	*/
    int  turnSpeed(int value = 0)
    {
        
        value = min( value,255 );
        value = max( value,0 );
        analogWrite( PIN_PWM, value );
        return value;
    }
	
	/**
	*
	*/
	void calibrate()
	{
		procedure = Procedure::HOMING;
		procedureFunction = ProcedureFunction::RELEASE;
	}

	/**
	*
	*/
    virtual void decodeMessage(byte data[])
    {

        switch( this->toInt(data,0,1) ){
			case WireCommands::ZERO:
				this->position = 0;
				this->targetPosition = 0;
				break;
			case WireCommands::INC_POSITION:
				this->moveTo(this->position + this->toInt(data, 1, 3));
				break; 
			case WireCommands::DEC_POSITION:
				this->moveTo(this->position - this->toInt(data, 1, 3));
				break; 
			case WireCommands::SET_TOLERANCE:
				this->tolerance = this->toInt(data, 1, 3);
				break;
			case WireCommands::MOVE_TO:
				this->moveTo(this->toInt(data, 1, 3));
				break;
			case WireCommands::IS_BUSY:
				this->returnValue = (int)this->isBusy();
				break;
			case WireCommands::GET_POSITION:
				this->returnValue = this->position;
				break;
			case WireCommands::SET_ECHO:
				this->echoValue = this->toInt(data, 1, 3);
				break;
			case WireCommands::GET_ECHO:
				this->returnValue = this->echoValue;
				break;
			case WireCommands::SET_MAX_SPEED:
				this->maxSpeed = this->toInt(data, 1, 3);
				break;
			case WireCommands::GET_MAX_SPEED:
				this->returnValue = this->maxSpeed;
				break;
			case WireCommands::HOME:
				this->calibrate();
				break;
			case WireCommands::STOP:
				this->stop();
				break;
			case WireCommands::SET_RAMPE:
				this->rampe = this->toInt(data, 1, 3);
				break;
			case WireCommands::GET_RAMPE:
				this->returnValue = this->rampe;
				break;
			case WireCommands::GET_CURRENT_SPEED:
				this->returnValue = this->currentSpeed;
				break;
			case WireCommands::GET_TOLERANCE:
				this->returnValue = this->tolerance;
				break;
            default:

            break; 
        }

    }
    
};

