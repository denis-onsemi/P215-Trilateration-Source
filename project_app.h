#ifndef HEADER_H
#define HEADER_H




/************************************************
 * [1]
 * Add here header file
 *
 ************************************************/

/* custom */
#include "uartuartcontroller.h"
#include "uartspicontroller.h"
#include "uartssicontroller.h"
#include "dsi3master.h"
#include "asicmaster.h"
#include "rdummaster.h"
#include "dsi3slave.h"
#include "measurement.h"
#include "CFARsim.h"

/************************************************
 * [2]
 * register module here
 *
 ************************************************/
#define SPECIFIC_INIT \
  REGISTER_APPLET(UartUartController);\
  REGISTER_APPLET(UartSpiController);\
  REGISTER_APPLET(UartSsiController);\
  REGISTER_APPLET(DSI3Master);\
  REGISTER_APPLET(ASICMaster);\
  REGISTER_APPLET(RDUMMaster);\
  REGISTER_APPLET(DSI3Slave);\
  REGISTER_APPLET(Measurement);\
  REGISTER_APPLET(CFARsim);


#include "mainapp.h"
#include <customdialogs.h>


#define PROJECT_TYPES   0// "NAA","OAA"

#define APPLET_COUNT    (11)

static bool  ProjectApp_Load_Data()
{
    bool result = false;
    return result;
}



#endif // HEADER_H
