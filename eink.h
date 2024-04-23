#ifndef _EINK_H
#define _EINK_H

#define _EINKMOSIPin       23
#define _EINKMISOPin       19
#define _EINKSCKPin        18
#define _EINKSSPin         5
#define _EINKResetPin      33
#define _EINKSPIClockSpeed 10000000
#define _EINKResetAsserted 0
#define _EINKResetNegated  1
#define _EINKSSAsserted    0
#define _EINKSSNegated     1

#define _EINKDATACMDPin    26

#define _EINKCommand       0
#define _EINKData          1

#define _EINKBusyPin       35

uint32_t einkInit( void );
uint32_t einkTurnOnDisplay( void );
uint32_t einkSleep( void );
uint32_t einkDisplayImage( uint8_t *imgData );
uint32_t einkClearDisplay( void );

#endif
