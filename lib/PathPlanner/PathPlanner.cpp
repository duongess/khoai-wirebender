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
        printLog("ERR: Thieu PathData.h");
        return;
    }

    float x[MAX_POINTS];
    float y[MAX_POINTS];
    
    // Gioi han so diem doc de bao ve mang RAM
    int pointsToRead = PATH_POINT_COUNT;
    if (pointsToRead > MAX_POINTS) {
        pointsToRead = MAX_POINTS;
        printLog("WARN: Du lieu bi cat xen vi vuot MAX_POINTS");
    }

    // Vong lap dung chung cho ca PC va Arduino thong qua macro
    for (int i = 0; i < pointsToRead; i++) {
        x[i] = pgm_read_float(&pathX[i]);
        y[i] = pgm_read_float(&pathY[i]);
    }

    calculateVectors(x, y, pointsToRead);
    printLog("LOAD_OK: PathData");
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
            
            float dotProduct = (dx * prevDx) + (dy * prevDy);
            float len1 = path[i-1].length;
            float len2 = path[i].length;
            
            float cosTheta = dotProduct / (len1 * len2);
            
            if (cosTheta > 1.0f) cosTheta = 1.0f;
            if (cosTheta < -1.0f) cosTheta = -1.0f;
            
            path[i].angle = acos(cosTheta) * 180.0f / M_PI;
        } else {
            path[i].angle = 0.0f;
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