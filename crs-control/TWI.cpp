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
      *
      *
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
      *
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
      *
      *
      */    
    long returnValue = 0;
    
    public:    
    /**
      *
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
      *
      *
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
      *
      *
      */
    virtual void decodeMessage(byte data[])
    {
        
    }
        

};
    
