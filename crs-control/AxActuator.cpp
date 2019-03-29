#include "TWI.cpp"

class AxActuator: public TWI
{
    
    private:
    int PIN_ENABLE_0;
    int PIN_ENABLE_1;
    int PIN_PWN_SPEED;
    int PIN_FEEDBACK_0;
    int PIN_FEEDBACK_1;
    
    public:

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
    
    /**
      *
      *
      */    
    AxActuator(
        int pin0,
        int pin1,
        int pinPWN,
        int fbPin0,
        int fbPin1
    )
    {
        PIN_ENABLE_0 = pin0;
        PIN_ENABLE_1 = pin1;
        PIN_PWN_SPEED = pinPWN;
        PIN_FEEDBACK_0 = fbPin0;
        PIN_FEEDBACK_1 = fbPin1;
        
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
    void setMinSpeed( int value)
    {
        minSpeed = max(0,min(255,value));
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

	int speed = 0;
	long lastposition = 0;

	/**
	*
	*/
    void main()
    {
    
        if(targetPosition < position)
        {
            digitalWrite( PIN_ENABLE_0, LOW);
            digitalWrite( PIN_ENABLE_1, HIGH);
        }
        else if(targetPosition > position)
        {
            digitalWrite( PIN_ENABLE_0, HIGH);
            digitalWrite( PIN_ENABLE_1, LOW);
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

		

		Serial.print( String(speed) +"\n");
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
    void turnBreak()
    {
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
    virtual String decodeMessage(byte data[])
    {

        Serial.print("Received: "+String(this->toInt(data,0,1))+"\n" );
        
        switch( this->toInt(data,0,1) ){
            case 1:
                //value0 = toInt(data,1,3);
                
            break;
            case 2:
                
            break; 
            case 3:
                
            break; 
            case 4:
                
            break; 
            default:

            break; 
        }
        
        return "m";

    }
    
};