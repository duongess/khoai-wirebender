// src/main.cpp
#include <Arduino.h>
#include "Config.h"
#include "PathPlanner.h"
#include "Kinematics.h"
#include "CommandQueue.h"
#include "HardwareController.h"

PathPlanner planner;
Kinematics engine;
CommandQueue cmdQueue;
HardwareController machine;

void setup() {
    Serial.begin(9600);
    machine.init(); // Khoi tao servo
    
    // B1 & B2: Tao mang vector cho duong thang/hinh hoc
    planner.generatePath();
    
    // B3: Tinh toan thong so + sai so, day vao Queue
    // Duyet qua tung doan vector da tinh toan
    for (int i = 0; i < planner.getPointCount(); i++) {
        Vector currentVec = planner.getVector(i);
        
        // Tinh toan chieu dai day, chia nho nhieu nhat neu qua dai
        float lengthWithError = engine.calcFeed(currentVec.length);
        int strokes = lengthWithError / engine.getMaxStroke();
        
        for(int s = 0; s < strokes; s++) {
            cmdQueue.push({CMD_FEED, engine.getMaxStroke()});
        }
        
        // Tinh goc uon voi he so an dan hoi (Springback)
        float bendAngleWithError = engine.calcBend(currentVec.angle);
        cmdQueue.push({CMD_BEND, bendAngleWithError});
    }
    
    Serial.println("Da nap lenh vao hang doi.");
}

void loop() {
    // B4 & B5: Lay so trong Queue va thuc thi chuyen dong
    if (!cmdQueue.isEmpty()) {
        Instruction currentCmd = cmdQueue.pop();
        
        if (currentCmd.type == CMD_FEED) {
            Serial.print("Day day: ");
            Serial.println(currentCmd.value);
            machine.executeFeed(currentCmd.value);
        } 
        else if (currentCmd.type == CMD_BEND) {
            Serial.print("Uon day: ");
            Serial.println(currentCmd.value);
            machine.executeBend(currentCmd.value);
        }
    }
}