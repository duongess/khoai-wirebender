#include "Kinematics.h"
#include "Config.h"

Kinematics::Kinematics() {
}

float Kinematics::calcFeed(float rawPixelLength) {
    // Buoc 1: Chuyen doi tu do dai Pixel tren ban ve sang Milimet thuc te
    float realLengthMM = rawPixelLength * PIXEL_TO_MM_RATIO;
    
    // Buoc 2: Nhan voi he so bu truot ma sat cua banh ty
    return realLengthMM * FEED_SLIP_FACTOR;
}

float Kinematics::calcBend(float rawAngle) {
    // Nhan voi he so dan hoi cua thep thay vi cong mot so co dinh
    float compensatedAngle = rawAngle * SPRINGBACK_FACTOR;
    
    // Gioi han khong cho vuot qua goc co khi toi da cua may
    if (compensatedAngle > BEND_MAX_ANGLE) {
        return BEND_MAX_ANGLE;
    }
    return compensatedAngle;
}

float Kinematics::getMaxStroke() {
    return MAX_FEED_STROKE_MM;
}