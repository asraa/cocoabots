#ifndef SENSORSDEF_H
#define SENSORSDEF_H

#include "sensors/ultraShortIR.h"
#include "sensors/shortIR.h"
#include "sensors/camera.h"
#include "sensors/gyroscope.h"
#include "sensors/ultrasonic.h"

#define R_UIR_P 0
#define L_UIR_P 0
#define B_UIR_P 0
#define F_UIR_P 0

#define R_SIR_P 0
#define L_SIR_P 0

#define F_US_EC 0
#define F_US_TR 0

#define B_US_EC 0
#define B_US_TR 0

struct sensors{
  gyroscope mygyroscope;

  ultraShortIR rUIR; //right Ultra Short IR
  ultraShortIR lUIR;
  ultraShortIR bUIR;
  ultraShortIR fUIR;

  shortIR rSIR; //righ  Short IR
  shortIR lSIR;

  ultrasonic fUS; //front  Ultrassonic
  ultrasonic bUS;

  ///Struct initializer

  sensors(int rUIRP =R_UIR_P,int lUIRP=L_UIR_P, int fUIRP=F_UIR_P, int bUIRP=B_UIR_P, int rSIRP=R_UIR_P, int lSIRP=L_UIR_P, int fUSTR=F_UIR_P, int fUSEC=F_US_EC, int bUSTR=B_US_TR, int bUSEC=B_US_EC):
      mygyroscope(),

      rUIR(rUIRP),
      lUIR(lUIRP),
      fUIR(fUIRP),
      bUIR(bUIRP),

      rSIR(rSIRP),
      lSIR(lSIRP),

      fUS(fUSTR, fUSEC),
      bUS(bUSTR, bUSEC)
      {}
};

class sensorsDef
{
public:
    sensorsDef();
};

#endif // SENSORSDEF_H
