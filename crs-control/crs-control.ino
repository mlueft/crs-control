#include <Wire.h>
#include "Configuration.h"
#include "AxActuator.cpp"


AxActuator actuator = AxActuator(
    
    PIN_ENABLE_A,   // enable pin 0
    PIN_ENABLE_B,   // enable pin 1
    PIN_PWM,        // pwn pin
    PIN_ENCODER_A,  // encoder feedback pin 0
    PIN_ENCODER_B   // encoder feedback pin 1
);

/**
  *
  *
  */
void setup(void) {
  
    Serial.begin(9600);  
    
    Wire.begin(TWI_ADDR);
    Wire.onReceive( onTWIData );
    Wire.onRequest( onTWIRequest );
    
    pinMode( PIN_ENABLE_A, OUTPUT );
    pinMode( PIN_ENABLE_B, OUTPUT );
    pinMode( PIN_PWM, OUTPUT);
    pinMode( PIN_ENCODER_A, INPUT );
    pinMode( PIN_ENCODER_B, INPUT );
    
    attachInterrupt( digitalPinToInterrupt(PIN_ENCODER_A), fb0, CHANGE );
    attachInterrupt( digitalPinToInterrupt(PIN_ENCODER_B), fb1, CHANGE );

    actuator.setMinSpeed(100);
    actuator.setMaxSpeed(255);
    
}

/**
  *
  *
  */
void fb0(){
    actuator.fb0();
}

/**
  *
  *
  */
void fb1(){
    actuator.fb1();
}
 
/**
  *
  *
  */
void onTWIData(int qty){
    Serial.print("receive");
    actuator.TWIOnData(qty);
}

/**
  *
  *
  */
void onTWIRequest(){
    Serial.print("request");
    actuator.TWIOnRequest();
}

unsigned long  time = 0;

 /**
  *
  *
  */
void loop() {

    /**/
    if( !actuator.isBusy() && actuator.position < 500)
    {
        //Serial.print( "0\n" );
		//Serial.print("0 position: " + String(actuator.position) + "\n");
        actuator.moveTo(5000);
    }
    
    if( !actuator.isBusy() && actuator.position > 500)
    {
        //Serial.print( "1\n" );
		//Serial.print("1 position: " + String(actuator.position) + "\n");
        actuator.moveTo(0);
    }
    /**/

    /**/
    if( millis()-time > 50 )
    {
        actuator.main();
        
        //Serial.print( String(actuator.isBusy())+"="+String( actuator.position)+"\n" );
		//Serial.print("position: " + String(actuator.position) + "\n");

		time = millis();
    }
	/**/
}
 
