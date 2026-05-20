#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include "Config.h"

class PathPlanner {
private:
    // Gioi han toi da so luong diem de bao ve RAM Arduino
    Vector path[MAX_POINTS];
    int pointCount;

public:
    PathPlanner();
    
    // Tao lo trinh vector mau
    void generatePath();
    
    // Tra ve so luong vector dang co
    int getPointCount();
    
    // Lay vector cu the tai 1 vi tri
    Vector getVector(int index);

    void calculateVectors(float x[], float y[], int count);
};

#endif