#include "TWI.cpp"
#include "WireCommands.cpp"
#include "Procedures.cpp"
#include "ProcedureFunctions.cpp"

class AxActuator: public TWI
{
    
    private:
    int PIN_ENABLE_0;
    int PIN_ENABLE_1;
    int PIN_PWN_SPEED;
    int PIN_FEEDBACK_0;
    int PIN_FEEDBACK_1;
	int PIN_HOME_SWITCH;

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
	int speed = 0;

	/**
	*
	*/
	long lastposition = 0;

	/**
	*
	*/
	int procedure = Procedures::NONE;
	
	/**
	*
	*/
	int procedureFunction = ProcedureFunctions::NONE;
	
	/**
	*
	*/
	int echoValue = 0;

	/*
	*  stores the current speed if speed
	*  is temp set to a different value.
	*/
	int speedBackup;











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
    int minSpeed            = 60;

	/**
	*
	*/
    int maxSpeed            = 255;
    



	public:
    /**
      *
      *
      */    
    AxActuator(
        int pin0,
        int pin1,
        int pinPWN,
        int fbPin0,
        int fbPin1,
		int pinHome
    )
    {
        PIN_ENABLE_0 = pin0;
        PIN_ENABLE_1 = pin1;
        PIN_PWN_SPEED = pinPWN;
        PIN_FEEDBACK_0 = fbPin0;
        PIN_FEEDBACK_1 = fbPin1;
		PIN_HOME_SWITCH = pinHome;
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
	void setMinSpeed(int value)
	{
		this->minSpeed = max(0, min(255, value));
	}

	/**
	*
	*/
	int getMinSpeed()
	{
		return this->minSpeed;
	}

	/**
	*
	*/
    void setMaxSpeed( int value)
    {
        maxSpeed = max(0,min(255,value));
    }

	/**
	*
	*/
    bool isBusy()
    {
        /*
        Serial.print("---------------------------\n");
        Serial.print( String( position )+"\n" );
        Serial.print( String( targetPosition)+"\n" );
        Serial.print( String( getCurrentSpeed() )+"\n" );
        Serial.print("---------------------------\n");
        /**/
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
    
		if (procedure == Procedures::HOMING)
		{

			if (procedureFunction == ProcedureFunctions::RELEASE)
			{
				if (digitalRead(PIN_HOME_SWITCH) == HIGH) {
					if( !this->isBusy() )
					{
						this->moveTo(this->position - int(this->tolerance * 2));
					}
				}
				else
				{
					procedureFunction = ProcedureFunctions::DETENTION;
				}
			}
			else if (procedureFunction == ProcedureFunctions::DETENTION)
			{
				if (digitalRead(PIN_HOME_SWITCH) == LOW) {
					if (!this->isBusy())
					{
						this->moveTo(this->position + int(this->tolerance * 2));
					}
				}
				else
				{
					procedure = Procedures::NONE;
					procedureFunction = ProcedureFunctions::NONE;

					this->position = 0;
					this->targetPosition = 0;
					this->speed = speedBackup;
				}
			}
		}




        if(targetPosition < position)
        {
#if INVERSE_DIRECTION == 0
            digitalWrite( PIN_ENABLE_0, LOW);
            digitalWrite( PIN_ENABLE_1, HIGH);
#endif
#if INVERSE_DIRECTION == 1
			digitalWrite(PIN_ENABLE_0, HIGH);
			digitalWrite(PIN_ENABLE_1, LOW);
#endif
        }
        else if(targetPosition > position)
        {
#if INVERSE_DIRECTION == 0
            digitalWrite( PIN_ENABLE_0, HIGH);
            digitalWrite( PIN_ENABLE_1, LOW);
#endif
#if INVERSE_DIRECTION == 1
			digitalWrite(PIN_ENABLE_0, LOW);
			digitalWrite(PIN_ENABLE_1, HIGH);
#endif
        }
        /*
        Serial.print("targetPosition:"+String(targetPosition)+"\n");
        Serial.print("position:"+String(position)+"\n");
        /**/
        if( abs(targetPosition-position) > tolerance )
        {
            int speed = getCurrentSpeed();
            //Serial.print("speed: "+String(speed)+"\n");
            turnSpeed( speed );
        }
        else
        {
            //Serial.print("0*\n");
            turnSpeed(0);
        }

		speed = position - lastposition;

		

		//Serial.print( String(speed) +"\n");
		lastposition = position;

    }

	/**
	*
	*/
    unsigned int getCurrentSpeed()
    {
        //return 50;
        
        // we are breaking
        if( abs(targetPosition-position) < rampe )
        {
            //Serial.print("a\n");
            /*
            Serial.print("----------------------------\n");
            Serial.print(String(abs(rampe-abs(targetPosition-position)-rampe)/(float)rampe)+"\n");
            Serial.print(String(abs(rampe))+"\n");
            Serial.print(String(abs(position))+"\n");
            Serial.print(String(abs(targetPosition))+"\n");
            Serial.print("----------------------------\n");
            //Serial.print( "b\n");
            /**/
            return max(minSpeed,maxSpeed*(abs(rampe-abs(targetPosition-position)-rampe)/(float)rampe));
        }
        
        // we are accelerating
        if( abs(position-startPosition) < rampe )
        {
            //Serial.print("b\n");
            /*
            Serial.print("----------------------------\n");
            Serial.print(String(abs(rampe-position-startPosition-rampe))+"\n");
            Serial.print(String(abs(rampe))+"\n");
            Serial.print(String(abs(position))+"\n");
            Serial.print(String(abs(startPosition))+"\n");
            //Serial.print( "b\n");
            /**/
            return max(minSpeed,maxSpeed*abs(rampe-abs(position-startPosition)-rampe)/(float)rampe);
        }
        
        // we are at full speed
        //Serial.print( "c\n");
        return maxSpeed;
        
    }

	/**
	*
	*/
    void fb0()
    {
        if( digitalRead(PIN_FEEDBACK_0) == digitalRead(PIN_FEEDBACK_1))
        {
            position++;
        }else{
            position--;
        }
    }

	/**
	*
	*/
    void fb1()
    {
        if( digitalRead(PIN_FEEDBACK_0) != digitalRead(PIN_FEEDBACK_1))
        {
            position++;
        }else{
            position--;
        }
    }

	/**
	*
	*/
    void stop()
    {
		procedure = Procedures::NONE;
		procedureFunction = ProcedureFunctions::NONE;
		targetPosition = position;
        turnSpeed(0);
        digitalWrite( PIN_ENABLE_0, LOW);
        digitalWrite( PIN_ENABLE_1, LOW);
    }

	/**
	*
	*/
    void turnSpeed(int value = 0)
    {
        
        value = min( value,255 );
        value = max( value,0 );
        analogWrite( PIN_PWN_SPEED, value );
        
    }
	
	/**
	*
	*/
	void calibrate()
	{
		speedBackup = this->speed;
		this->speed = 125;

		procedure = Procedures::HOMING;
		procedureFunction = ProcedureFunctions::RELEASE;
		
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
			case WireCommands::SET_MIN_SPEED:
				this->minSpeed = this->toInt(data, 1, 3);
				break;
			case WireCommands::GET_MIN_SPEED:
				this->returnValue = this->minSpeed;
				break;
			case WireCommands::GET_TOLERANCE:
				this->returnValue = this->tolerance;
				break;
            default:

            break; 
        }

    }
    
};