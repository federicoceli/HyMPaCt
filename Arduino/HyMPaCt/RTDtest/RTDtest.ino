#include <SPI.h>
#include <MAX31865.h>

// Define digital pins for RTD-to-digital sensors
#define RTD_CS_PIN1   11
#define RTD_CS_PIN2   12
#define RTD_CS_PIN3   13

MAX31865_RTD rtd1(MAX31865_RTD::RTD_PT100, RTD_CS_PIN1);
MAX31865_RTD rtd2(MAX31865_RTD::RTD_PT100, RTD_CS_PIN2);
MAX31865_RTD rtd3(MAX31865_RTD::RTD_PT100, RTD_CS_PIN3);

int readTemprature(MAX31865_RTD rtd) { 
  rtd.read_all( );

  if( rtd.status( ) == 0 )
  {
    double temperature = rtd.temperature( );
    Serial.print( " T = ");
    Serial.print( temperature, 1 );
    Serial.println(" deg C" );
  }
  else 
  {
    Serial.print( "RTD fault register: " );
    Serial.print( rtd.status( ) );
    Serial.print( ": " );
    if (rtd.status() == 255)
    {
      Serial.println("Probably not connected");
    }
    else if( rtd.status( ) & MAX31865_FAULT_HIGH_THRESHOLD )
    {
      Serial.println( "RTD high threshold exceeded" );
    }
    else if( rtd.status( ) & MAX31865_FAULT_LOW_THRESHOLD )
    {
      Serial.println( "RTD low threshold exceeded" );
    }
    else if( rtd.status( ) & MAX31865_FAULT_REFIN )
    {
      Serial.println( "REFIN- > 0.85 x V_BIAS" );
    }
    else if( rtd.status( ) & MAX31865_FAULT_REFIN_FORCE )
    {
      Serial.println( "REFIN- < 0.85 x V_BIAS, FORCE- open" );
    }
    else if( rtd.status( ) & MAX31865_FAULT_RTDIN_FORCE )
    {
      Serial.println( "RTDIN- < 0.85 x V_BIAS, FORCE- open" );
    }
    else if( rtd.status( ) & MAX31865_FAULT_VOLTAGE )
    {
      Serial.println( "Overvoltage/undervoltage fault");
    }
    else
    {
      Serial.println( "Unknown fault; check connection" );
    }
  }
}

void setup() {
    Serial.begin(9600);

    /* Initialize SPI communication. */
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    SPI.setDataMode(SPI_MODE3);
    
    /* Allow the MAX31865 to warm up. */
    delay(100);
    
    /* Configure:
        V_BIAS enabled
        Auto-conversion
        1-shot disabled
        3-wire enabled
        Fault detection:  automatic delay
        Fault status:  auto-clear
        50 Hz filter
        Low threshold:  0x0000
        High threshold:  0x7fff
    */
    
    rtd1.configure(true, true, false, true, MAX31865_FAULT_DETECTION_NONE,
                    true, true, 0x0000, 0x7fff);
    rtd2.configure(true, true, false, true, MAX31865_FAULT_DETECTION_NONE,
                    true, true, 0x0000, 0x7fff);
    rtd3.configure(true, true, false, true, MAX31865_FAULT_DETECTION_NONE,
                    true, true, 0x0000, 0x7fff);               
 }

 void loop() 
{
  readTemprature(rtd2);

  delay( 3000 );
}
