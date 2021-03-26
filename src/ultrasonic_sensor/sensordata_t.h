#ifndef SENSORDATA_T_H
#define SENSORDATA_T_H

extern "C" {
    struct SensorData_t {
        double topDistance      = 0.;
        double straightDistance = 0.;
        double bottomDistance   = 0.;
    };
}

#endif // SENSORDATA_T_H
