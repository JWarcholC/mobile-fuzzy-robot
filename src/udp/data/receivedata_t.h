#ifndef RECEIVEDATA_T_H
#define RECEIVEDATA_T_H

#include <QDataStream>
#include <QDebug>

extern "C" {
    struct ReceiveData_t {
        enum Direction {
            Straight,
            Right,
            Left
        };

        double m_velocity;
        Direction m_direction;
        bool m_regulatorWorks;
    };
}
#endif // RECEIVEDATA_T_H
