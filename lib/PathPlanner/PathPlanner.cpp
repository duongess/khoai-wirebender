#include "PathPlanner.h"
#include <math.h>

// Chi dung thu vien C++ tieu chuan tren moi truong may tinh
#ifndef ARDUINO
#include <iostream>
#include <fstream>
#endif

PathPlanner::PathPlanner() {
    pointCount = 0;
}

void PathPlanner::generatePath() {
#ifndef ARDUINO
    std::ifstream file("master/data/coordinates.txt");
    
    if (!file.is_open()) {
        std::cout << "ERR_FILE_OPEN\n";
        return;
    }

    float x[MAX_POINTS + 1];
    float y[MAX_POINTS + 1];
    int rawPoints = 0;
    float currentX, currentY;

    // Doc tat ca toa do tu file
    while (file >> currentX >> currentY && rawPoints <= MAX_POINTS) {
        x[rawPoints] = currentX;
        y[rawPoints] = currentY;
        rawPoints++;
    }
    file.close();

    // Noi suy tu toa do (X,Y) thanh Vector (Do dai, Goc)
    for (int i = 0; i < rawPoints - 1; i++) {
        float dx = x[i+1] - x[i];
        float dy = y[i+1] - y[i];
        
        // Tinh do dai vector
        path[i].length = sqrt(dx*dx + dy*dy);
        
        // Tinh goc uon so voi vector truoc do
        if (i > 0) {
            float prevDx = x[i] - x[i-1];
            float prevDy = y[i] - y[i-1];
            
            float dotProduct = (dx * prevDx) + (dy * prevDy);
            float len1 = path[i-1].length;
            float len2 = path[i].length;
            
            float cosTheta = dotProduct / (len1 * len2);
            // Gioi han sai so toan hoc
            if (cosTheta > 1.0f) cosTheta = 1.0f;
            if (cosTheta < -1.0f) cosTheta = -1.0f;
            
            // Chuyen tu Radian sang Do
            path[i].angle = acos(cosTheta) * 180.0f / M_PI;
        } else {
            path[i].angle = 0.0f;
        }
    }
    
    pointCount = rawPoints - 1;
    std::cout << "LOAD_OK\n";
#endif
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