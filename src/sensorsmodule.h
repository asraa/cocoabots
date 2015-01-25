#ifndef SENSORSMODULE_H
#define SENSORSMODULE_H

#include "sensors/camera.h"
#include "sensors/encoder.h"
#include "sensors/encoderquadrature.h"
#include "sensors/gyroscope.h"
#include "sensors/shortIR.h"
#include "sensors/ultraShortIR.h"
#include "sensors/ultrasonic.h"
#include "sensors/jankycolorsensor.h"
#include "sensors/sensorssuperclass.h"
#include "cmath"
#include "configFile.h"

class sensorsModule
{
public:
    sensorsModule();
    ~sensorsModule();

    volatile double frontUltrasonicData;
    volatile double rightUltrasonicData;
    volatile double leftUltrasonicData;

    volatile double rightEncoderMovement;
    volatile double leftEncoderMovement;
    volatile double encoderAngle;

    volatile double frontUltraShortIRData;
    volatile double rightUltraShortIRData;
    volatile double leftUltraShortIRData;
    volatile double backUltraShortIRData;

    volatile double frontShortIRData;
    volatile double rightShortIRData;
    volatile double leftShortIRData;
    volatile double backShortIRData;

    volatile double colorSensorData;


    volatile double ultrasonicAlpha;
    volatile double ultraShortIRAlpha;
    volatile double shortIRAlpha;
    volatile double encoderAlpha;
    volatile double encoderAngleAlpha;
    volatile double gyroscopeTotalAlpha;
    volatile double gyroscopeReadingAlpha;
    volatile double colorSensorAlpha;

    volatile double gyroscopeAngle;
    volatile double gyroscopeReading;

    long long int timeMicrosecondsSinceEpoch;


    int running;
    std::thread *runThread;

    static void run(sensorsModule * sensors);
    static void updateSensor(sensorsSuperClass *sensor, volatile double * data, float alpha, int started);
    static void updateData(volatile double* previousData, double newData, float alpha, int started);
    static void updateTime(sensorsModule * sensors);

    static double kalmanFilter(double previousData, double newData, float alpha);

#if GYROSCOPE
gyroscope mygyroscope;
#endif


#if ENC_2_WIRES

#if RIGHT_ENCODER
encoderQuadrature rightEncoder;
#endif

#if LEFT_ENCODER
encoderQuadrature leftEncoder;
#endif

#else//ENC_2_WIRES

#if RIGHT_ENCODER
encoder rightEncoder;
#endif

#if LEFT_ENCODER
encoder leftEncoder;
#endif
#endif//ENC_2_WIRES
private:
#if FRONT_ULTRASONIC
ultrasonic frontUltrasonic;
#endif


#if RIGHT_ULTRASONIC
ultrasonic rightUltrasonic;
#endif

#if LEFT_ULTRASONIC
ultrasonic leftUltrasonic;
#endif


#if FRONT_ULTRASHORTIR
ultraShortIR frontUltraShortIR;
#endif

#if BACK_ULTRASHORTIR
ultraShortIR backUltraShortIR;
#endif

#if RIGHT_ULTRASHORTIR
ultraShortIR rightUltraShortIR;
#endif

#if LEFT_ULTRASHORTIR
ultraShortIR leftUltraShortIR;
#endif



#if FRONT_SHORTIR
shortIR frontShortIR;
#endif

#if BACK_SHORTIR
shortIR backShortIR;
#endif

#if RIGHT_SHORTIR
shortIR rightShortIR;
#endif

#if LEFT_SHORTIR
shortIR leftShortIR;
#endif

#if COLOR_DETECTOR
jankyColorSensor colorSensor;
#endif


};

#endif // SENSORSMODULE_H
