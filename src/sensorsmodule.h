#ifndef SENSORSMODULE_H
#define SENSORSMODULE_H

#include "sensors/camera.h"
#include "sensors/encoder.h"
#include "sensors/gyroscope.h"
#include "sensors/sensorutils.h"
#include "sensors/shortIR.h"
#include "sensors/ultraShortIR.h"
#include "sensors/ultrasonic.h"
#include "configFile.h"

class sensorsModule
{
public:
    sensorsModule();
    void run();
    double frontUltrasonicData;
    double rightUltrasonicData;
    double leftUltrasonicData;

    long long int rightEncoderData;
    long long int leftEncoderData;

    double frontUltraShortIRData;
    double rightUltraShortIRData;
    double leftUltraShortIRData;
    double backUltraShortIRData;

    double frontShortIRData;
    double rightShortIRData;
    double leftShortIRData;
    double backShortIRData;
private:

#if FRONT_ULTRASONIC
#endif

#if RIGHT_ULTRASONIC
#endif

#if LEFT_ULTRASONIC
#endif



#if RIGHT_ENCODER
#endif

#if LEFT_ENCODER
#endif



#if FRONT_ULTRASHORTIR
#endif

#if BACK_ULTRASHORTIR
#endif

#if RIGHT_ULTRASHORTIR
#endif

#if LEFT_ULTRASHORTIR
#endif



#if FRONT_SHORTIR
#endif

#if BACK_SHORTIR
#endif

#if RIGHT_SHORTIR
#endif

#if LEFT_SHORTIR
#endif


};

#endif // SENSORSMODULE_H
