
define( I2C_ADDRESS, 1 )

#include <Wire.h>

void setup() {
    //
    // I2C
    //
    Wire.begin( I2C_ADDRESS );
    Wire.onReceive( onI2CDataReceive );
  
}

void onI2CDataReceive( int byteCount){
    while(Wire.available()>0){
        int b = Wire.read();
      
    }
}

void loop() {
  

}
