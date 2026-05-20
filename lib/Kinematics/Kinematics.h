#ifndef KINEMATICS_H
#define KINEMATICS_H

class Kinematics {
private:
    float maxStrokeMM;      // Chieu dai toi da cua 1 nhat day
    float feedMultiplier;   // He so quy doi: 1mm -> ? do Servo
    float bendSpringback;   // Goc bu tru dan hoi (do)

public:
    Kinematics();
    
    // Tinh toan chieu dai day thuc te sau khi bu tru
    float calcFeed(float length);
    
    // Tinh toan goc uon thuc te sau khi bu tru
    float calcBend(float angle);    
};

#endif