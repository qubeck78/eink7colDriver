 
#include <WiFi.h>

#include "eink.h"

uint8_t *image;

#include "img4.h"

void setup() 
{
  uint32_t i;
   
  Serial.begin( 115200 );
  

  Serial.printf( "\neinkTest\n" );

  einkInit();

  image = ( uint8_t * ) ps_malloc( 400 * 480 );

  if( image == NULL )
  {
    Serial.printf( "ERROR: frame buffer alloc fail!\n" );
    
    return;
  }
  
  for( i = 0; i < 400 * 480; i++ )
  {
    image[i] = rawData[i];
  }

  einkDisplayImage( image );

  //einkClearDisplay();
  
  einkSleep();

  Serial.printf( "done\n" );
}

void loop() 
{
  // put your main code here, to run repeatedly:

}
