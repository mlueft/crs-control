#include <Wire.h>
#include "Configuration.h"
#include "AxActuator.cpp"
#include <avr/io.h>
#include <avr/interrupt.h>


volatile int8_t  enc_delta;
static int8_t  last;

AxActuator actuator = AxActuator();


/**
  *
  *
  */
void setup(void) {
  
    Wire.begin(TWI_ADDR);
    Wire.onReceive( onTWIData );
    Wire.onRequest( onTWIRequest );
    
    pinMode( PIN_ENABLE_A, OUTPUT );
    pinMode( PIN_ENABLE_B, OUTPUT );
    pinMode( PIN_PWM, OUTPUT);
	pinMode( PIN_HOME, INPUT);

    actuator.setMaxSpeed(2000);
    actuator.setRampe(1000);
	actuator.setTolerance(100);




	int8_t newv;

	newv = 0;
	if( PHASE_A ) newv = 3;
	if( PHASE_B ) newv ^= 1;       // convert gray to binary
	last = newv;                   // power on state
	enc_delta = 0;

	Serial.begin(9600);

	Serial.write("init:");

}

/**
  *
  *
  */
/**/
void isr( )
{
	int8_t newv, diff;

	newv = 0;
	if( PHASE_A ) newv = 3;
	if( PHASE_B ) newv ^= 1;          // convert gray to binary
	diff = last - newv;               // difference last - new
	if( diff & 1 ) {                 // bit 0 = value (1)
		last = newv;                    // store new as next last
		enc_delta += (diff & 2) - 1;   // bit 1 = direction (+/-)
	}
}
/**/


int8_t encode_read4( void )         // read four step encoders
{
	int8_t val;

	cli();
	val = enc_delta;
	enc_delta = val & 3;
	sei();
	return val >> 2;
}


/**
  *
  *
  */
void onTWIData(int qty){
    actuator.TWIOnData(qty);
}

/**
  *
  *
  */
void onTWIRequest(){
    actuator.TWIOnRequest();
}

unsigned long  time = 0;
unsigned long  time1 = 0;

 /**
  *
  *
  */
void loop() {

  	isr();
	actuator.setPosition( actuator.getPosition() + encode_read4() );

    if( millis()-time > 50 )
    {
        actuator.main();
		time = millis();

    }

    if( millis()-time1 > 50 )
    {
    	actuator.trace();
		time1 = millis();

    }

}
 
