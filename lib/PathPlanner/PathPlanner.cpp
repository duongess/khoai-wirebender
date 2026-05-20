#include "PathPlanner.h"
#include "Utils.h"
#include <math.h>

// Gia lap thu vien va ham doc Flash cho moi truong may tinh
#ifndef ARDUINO
    #define PROGMEM
    inline float pgm_read_float(const float* addr) {
        return *addr;
    }
#endif

// Kiem tra file data
#if __has_include("PathData.h")
    #include "PathData.h"
    const bool hasPathData = true;
#else
    const bool hasPathData = false;
    // Khai bao gia de chong loi bien dich neu thieu file
    const int PATH_POINT_COUNT = 0;
    const float pathX[1] = {0.0f};
    const float pathY[1] = {0.0f};
#endif

PathPlanner::PathPlanner() {
    pointCount = 0;
}

void PathPlanner::generatePath() {
    if (!hasPathData) {
        printLn("ERR: Thieu PathData.h");
        return;
    }

    float x[MAX_POINTS];
    float y[MAX_POINTS];
    
    // Gioi han so diem doc de bao ve mang RAM
    int pointsToRead = PATH_POINT_COUNT;
    if (pointsToRead > MAX_POINTS) {
        pointsToRead = MAX_POINTS;
        printLn("WARN: Du lieu bi cat xen vi vuot MAX_POINTS");
    }

    // Vong lap dung chung cho ca PC va Arduino thong qua macro
    for (int i = 0; i < pointsToRead; i++) {
        x[i] = pgm_read_float(&pathX[i]);
        y[i] = pgm_read_float(&pathY[i]);
    }

    calculateVectors(x, y, pointsToRead);
    printLn("LOAD_OK: PathData");
}

void PathPlanner::calculateVectors(float x[], float y[], int count) {
    if (count < 2) {
        pointCount = 0;
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        float dx = x[i+1] - x[i];
        float dy = y[i+1] - y[i];
        
        path[i].length = sqrt(dx*dx + dy*dy);
        
        if (i > 0) {
            float prevDx = x[i] - x[i-1];
            float prevDy = y[i] - y[i-1];
            
            // Tinh goc tuyet doi bang atan2 
            float angle1 = atan2(prevDy, prevDx) * 180.0f / M_PI;
            float angle2 = atan2(dy, dx) * 180.0f / M_PI;
            
            // Tinh goc be lai kem dau (Trai/Phai)
            float turnAngle = angle2 - angle1;
            
            // Chuan hoa goc ve bien do [-180, 180]
            while (turnAngle <= -180.0f) turnAngle += 360.0f;
            while (turnAngle > 180.0f) turnAngle -= 360.0f;
            
            path[i].angle = turnAngle;
        } else {
            // Doan thang dau tien: Xac dinh huong goc tren he toa do
            path[i].angle = atan2(dy, dx) * 180.0f / M_PI;
        }
    }
    pointCount = count - 1;
}

int PathPlanner::getPointCount() {
    return pointCount;
}

Vector PathPlanner::getVector(int index) {
    if (index >= 0 && index < pointCount) {
        return path[index];
    }
    Vector emptyVec = {0.0f, 0.0f};
    return emptyVec;
}