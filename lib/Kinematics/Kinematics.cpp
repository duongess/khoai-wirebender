#include "Kinematics.h"

Kinematics::Kinematics() {
    // Thiet lap cac hang so vat ly (Can tinh chinh thuc te)
    maxStrokeMM = 30.0f;    // Chieu dai toi da tay don servo day duoc
    feedMultiplier = 1.8f;  // 1mm day tuong ung voi 1.8 do servo
    bendSpringback = 5.0f;  // Kim loai nay lai 5 do sau khi uon
}

float Kinematics::calcFeed(float length) {
    // Tra ve so do servo can quay de day chieu dai nay
    return length * feedMultiplier;
}

float Kinematics::calcBend(float angle) {
    if (angle == 0) return 0;
    
    // Goc uon thuc te cua may = phan bu cua goc vector + he so dan hoi
    float realAngle = 180.0f - angle + bendSpringback;
    if (realAngle > 180.0f) realAngle = 180.0f;
    return realAngle;
}

float Kinematics::getMaxStroke() {
    return maxStrokeMM;
}