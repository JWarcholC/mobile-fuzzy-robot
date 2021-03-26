#ifndef SENDDATA_T_H
#define SENDDATA_T_H

#include <QDataStream>

extern "C" {
    struct SendData_t {
        double m_leftDistance;
        double m_rightDistance;
        double m_straightDistance;
        double m_distanceToEnd;
        double m_stopSimulation;
    };
}

inline QDataStream &operator<<(QDataStream &str, const SendData_t &sendData) {
    str.setFloatingPointPrecision(QDataStream::DoublePrecision);
    str << sendData.m_leftDistance      << sendData.m_rightDistance
        << sendData.m_straightDistance  << sendData.m_distanceToEnd
        << sendData.m_stopSimulation;

    return str;
}

#endif // SENDDATA_T_H
