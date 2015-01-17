#ifndef SENSORSMODULE_H
#define SENSORSMODULE_H

#include "sensors/camera.h"
#include "sensors/encoder.h"
#include "sensors/encoderquadrature.h"
#include "sensors/gyroscope.h"
#include "sensors/shortIR.h"
#include "sensors/ultraShortIR.h"
#include "sensors/ultrasonic.h"
#include "sensors/sensorssuperclass.h"
#include "configFile.h"

class sensorsModule
{
public:
    sensorsModule();
    ~sensorsModule();

    double frontUltrasonicData;
    double rightUltrasonicData;
    double leftUltrasonicData;

    double rightEncoderMovement;
    double leftEncoderMovement;

    double frontUltraShortIRData;
    double rightUltraShortIRData;
    double leftUltraShortIRData;
    double backUltraShortIRData;

    double frontShortIRData;
    double rightShortIRData;
    double leftShortIRData;
    double backShortIRData;


    double ultrasonicAlpha;
    double ultraShortIRAlpha;
    double shortIRAlpha;
    double encoderAlpha;
    double gyroscopeTotalAlpha;
    double gyroscopeReadingAlpha;

    double gyroscopeAngle;
    double gyroscopeReading;

    long long int timeMicrosecondsSinceEpoch;


    int running;
    std::thread *runThread;

    static void run(sensorsModule * sensors);
    static void updateSensor(sensorsSuperClass *sensor, double * data, float alpha, int started);
    static void updateData(double* previousData, double newData, float alpha, int started);
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



};

#endif // SENSORSMODULE_H
