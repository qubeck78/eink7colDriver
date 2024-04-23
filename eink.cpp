#include <Arduino.h>
#include <SPI.h>

#include "eink.h"

SPIClass *einkSPI;

static uint32_t einkWaitReady()
{

  while( digitalRead( _EINKBusyPin ) == 0 );

  return 0;
}

static uint32_t einkSend( uint8_t txb, uint8_t dataOrCMD )
{

  if( dataOrCMD )
  {
    digitalWrite( _EINKDATACMDPin, 1 );
  }
  else
  {
    digitalWrite( _EINKDATACMDPin, 0 );
  }

  einkSPI->beginTransaction( SPISettings( _EINKSPIClockSpeed, MSBFIRST, SPI_MODE0 ) );

  digitalWrite( _EINKSSPin, _EINKSSAsserted );

  einkSPI->transfer( txb );

  einkSPI->endTransaction();
  
  digitalWrite( _EINKSSPin, _EINKSSNegated );

  return 0;

}

uint32_t einkInit( void )
{
  einkSPI = new SPIClass( VSPI );
  
  if( einkSPI == NULL )
  {
    return 1;
  }

  einkSPI->begin( _EINKSCKPin, _EINKMISOPin, _EINKMOSIPin, _EINKSSPin );

  pinMode( _EINKSSPin, OUTPUT);
  digitalWrite( _EINKSSPin, _EINKSSNegated );

  pinMode( _EINKDATACMDPin, OUTPUT);
  digitalWrite( _EINKDATACMDPin, 0 );

  pinMode( _EINKBusyPin, INPUT );
  
  pinMode( _EINKResetPin, OUTPUT);

  digitalWrite( _EINKResetPin, _EINKResetNegated );

  delay( 20 );
  
  digitalWrite( _EINKResetPin, _EINKResetAsserted );

  delay( 5 );

  digitalWrite( _EINKResetPin, _EINKResetNegated );

  delay( 20 );

  einkWaitReady();

  delay( 30 );

  einkSend( 0xaa, _EINKCommand );
  einkSend( 0x49, _EINKData );
  einkSend( 0x55, _EINKData );
  einkSend( 0x20, _EINKData );
  einkSend( 0x08, _EINKData );
  einkSend( 0x09, _EINKData );
  einkSend( 0x18, _EINKData );

  einkSend( 0x01, _EINKCommand );
  einkSend( 0x3f, _EINKData );
  einkSend( 0x00, _EINKData );
  einkSend( 0x32, _EINKData );
  einkSend( 0x2a, _EINKData );
  einkSend( 0x0e, _EINKData );
  einkSend( 0x2a, _EINKData );

  einkSend( 0x00, _EINKCommand );
  einkSend( 0x5f, _EINKData );
  einkSend( 0x69, _EINKData );

  einkSend( 0x03, _EINKCommand );
  einkSend( 0x00, _EINKData );
  einkSend( 0x54, _EINKData );
  einkSend( 0x00, _EINKData );
  einkSend( 0x44, _EINKData );

  einkSend( 0x05, _EINKCommand );
  einkSend( 0x40, _EINKData );
  einkSend( 0x1f, _EINKData );
  einkSend( 0x1f, _EINKData );
  einkSend( 0x2c, _EINKData );
  
  einkSend( 0x06, _EINKCommand );
  einkSend( 0x6f, _EINKData );
  einkSend( 0x1f, _EINKData );
  einkSend( 0x1f, _EINKData );
  einkSend( 0x22, _EINKData );
  
  einkSend( 0x13, _EINKCommand );   //IPC
  einkSend( 0x00, _EINKData );
  einkSend( 0x04, _EINKData );

  einkSend( 0x30, _EINKCommand );   
  einkSend( 0x3c, _EINKData );

  einkSend( 0x41, _EINKCommand );   //TSE
  einkSend( 0x00, _EINKData );

  einkSend( 0x50, _EINKCommand );   
  einkSend( 0x3f, _EINKData );

  einkSend( 0x60, _EINKCommand );   
  einkSend( 0x02, _EINKData );
  einkSend( 0x00, _EINKData );

  einkSend( 0x61, _EINKCommand );   
  einkSend( 0x03, _EINKData );
  einkSend( 0x20, _EINKData );
  einkSend( 0x01, _EINKData );
  einkSend( 0xe0, _EINKData );

  einkSend( 0x82, _EINKCommand );   
  einkSend( 0x1e, _EINKData );

  einkSend( 0x84, _EINKCommand );   
  einkSend( 0x00, _EINKData );

  einkSend( 0x86, _EINKCommand );   //AGID
  einkSend( 0x00, _EINKData );

  einkSend( 0xe3, _EINKCommand );   
  einkSend( 0x2f, _EINKData );

  einkSend( 0xe0, _EINKCommand );   //CCSET
  einkSend( 0x00, _EINKData );

  einkSend( 0xe6, _EINKCommand );   //TSSET
  einkSend( 0x00, _EINKData );

  return 0;
}

uint32_t einkTurnOnDisplay()
{ 
  einkSend( 0x04, _EINKCommand );   //Power on

  einkWaitReady();
  
  einkSend( 0x12, _EINKCommand );   //Display refresh
  einkSend( 0x00, _EINKData );

  einkWaitReady();
  
  einkSend( 0x02, _EINKCommand );   //Power off
  einkSend( 0x00, _EINKData );

  einkWaitReady();

  return 0;
}

uint32_t einkDisplayImage( uint8_t *imgData )
{
  uint32_t x;
  uint32_t y;

  einkSend( 0x10, _EINKCommand );   //Send display data

  for( y = 0; y < 480; y++ )
  {
    for( x = 0; x < 400; x++ )
    {
      einkSend( *imgData++, _EINKData );
    }
  }

  return einkTurnOnDisplay();
}

uint32_t einkSleep()
{
  einkSend( 0x07, _EINKCommand );   //deep sleep
  einkSend( 0xa5, _EINKData );

  return 0;
}

uint32_t einkClearDisplay()
{
  uint32_t x;
  uint32_t y;

  einkSend( 0x10, _EINKCommand );   //Send display data

  for( y = 0; y < 480; y++ )
  {
    for( x = 0; x < 400; x++ )
    {
      einkSend( 0x11, _EINKData );
    }
  }

  return einkTurnOnDisplay();
}
