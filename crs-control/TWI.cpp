#include <Arduino.h>
#include "Wire.h"

class TWI{

    protected:
    /**
      *
      *
      */
    String splitString( String data, int index, char seperator = ' ' ){
      int i=0;
      int leftIndex = 0;
      int currentPart = 0;
      while( i++ <= data.length() ){
        if( ( data.charAt(i) == seperator || i == data.length() )  && currentPart != index){
          currentPart++;
          leftIndex = i+1;
        }else if( (data.charAt(i) == seperator || i == data.length() )  && currentPart == index){
          break;
        }
      }
      return data.substring( leftIndex, i );
    }
    
    /**
      * Reads the next [size] bytes beginning at [index] and
      * returns an integer containing the read bytes. Uses 
	  * little endian.
      */
    long toInt(byte data[], int index = 0, int size=3)
    {
        
        long result = 0;
        for( int i = 0; i < size; i++)
        {
            result = (result << 8) | data[index+i];
        }
        
        return result;
    }
    
    /**
      *  Reads the next [size] bytes from data and returns an byte array.
	  *  Uses little endian.
      *
      */    
    byte* toBytes(long data, int size = 3)
     {
        byte* result = new byte[size];
        
        for( int i = 0; i < size; i++)
        {
            int c = (data>>(i*8))&255;
            result[size-1-i] = c;
        }
        
        return result;
        
    }
    
    /**
      *  Stores the value returned at the next master request call.
      *
      */    
    long returnValue = 0;
    
    public:    
    /**
      *  Answers master's request call and sends the return value.
      *
      */
    void TWIOnRequest()
    {
        //Serial.println( "returnValue on request: "+ String(returnValue) );
        byte *d = toBytes( returnValue);
		Wire.write(d, sizeof(long));
        delete[] d;
    }    


    /**
      *  Receives the master send call and calls decodeMessage.
      *  DecodeMessage should be implemented be a child class.
      */
	void TWIOnData(int qty)
    {
        
        byte data[qty];
        int index = 0;
        while( Wire.available() > 0){
            char c = Wire.read();
			//Serial.println(c);
            data[index] = c;
            index++;
            delay(2);
        }

        this->decodeMessage(data);
      
    }    

    /**
      *  To be implemented by a child class to react on
      *  to receives message.
      */
    virtual void decodeMessage(byte data[])
    {
        
    }
        

};
    
