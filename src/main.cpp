// src/main.cpp
#include "Config.h"
#include "Utils.h"
#include "PathPlanner.h"
#include "Kinematics.h"
#include "CommandQueue.h"

// Chi nap thu vien phan cung khi o moi truong mach that
#ifdef ARDUINO
#include <Arduino.h>
#include "HardwareController.h"
HardwareController machine;
#endif

PathPlanner planner;
Kinematics engine;
CommandQueue cmdQueue;

void setup() {
#ifdef ARDUINO
    Serial.begin(9600);
    machine.init();
#endif

    planner.generatePath();
    
    for (int i = 0; i < planner.getPointCount(); i++) {
        Vector currentVec = planner.getVector(i);
        
        float lengthWithError = engine.calcFeed(currentVec.length);
        float maxS = engine.getMaxStroke();
        
        // Tinh so lan day toi da va xu ly phan du chieu dai
        int strokes = (int)(lengthWithError / maxS);
        float remainder = lengthWithError - (strokes * maxS);
        
        for(int s = 0; s < strokes; s++) {
            cmdQueue.push({CMD_FEED, maxS});
        }
        
        // Neu con du chieu dai chua day het, phai day not
        if (remainder > 0.0f) {
            cmdQueue.push({CMD_FEED, remainder});
        }
        
        float bendAngleWithError = engine.calcBend(currentVec.angle);
        cmdQueue.push({CMD_BEND, bendAngleWithError});
    }
    
    printLog("Da nap lenh vao hang doi.");
}

void loop() {
    if (!cmdQueue.isEmpty()) {
        Instruction currentCmd = cmdQueue.pop();
        
        if (currentCmd.type == CMD_FEED) {
            printLog("Day day (mm): ");
            printLog(currentCmd.value);
            
#ifdef ARDUINO
            machine.executeFeed(currentCmd.value);
#endif
        } 
        else if (currentCmd.type == CMD_BEND) {
            printLog("Uon day (do): ");
            printLog(currentCmd.value);
            
#ifdef ARDUINO
            machine.executeBend(currentCmd.value);
#endif
        }
    } else {
#ifndef ARDUINO
        // Tren PC, khi hang doi het lenh thi dung phan mem
        exit(0);
#endif
    }
}

// Mo phong diem khoi chay cho moi truong C++ may tinh (Native)
#ifndef ARDUINO
#include <cstdlib>
#include <thread>
#include <chrono>

int main() {
    setup();
    
    // Vong lap tren may tinh
    while(true) {
        loop();
        // Ham toc do vong lap (nghi 100ms) de khong treo CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
#endif