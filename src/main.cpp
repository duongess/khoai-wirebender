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
#else
#include <fstream>
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
        
        // Tham so chia nho goc
        float filletFeed = 5.0f; // Nhich 5mm de lay khong gian be tiep
        int extraFeeds = 0;      // Dem so lan da nhich
        
        if (abs(currentVec.angle) > 0.1f) {
            float remainingAngle = currentVec.angle;
            
            // Vong lap bam nho nhung goc gắt
            while (abs(remainingAngle) > 90.0f/SPRINGBACK_FACTOR) {
                float chunkAngle = (remainingAngle > 0) ? 90.0f/SPRINGBACK_FACTOR : -90.0f/SPRINGBACK_FACTOR;
                
                float servoTarget = 90.0f + engine.calcBend(chunkAngle);
                if (servoTarget > 180.0f) servoTarget = 180.0f;
                if (servoTarget < 0.0f) servoTarget = 0.0f;
                
                cmdQueue.push({CMD_BEND, servoTarget});
                cmdQueue.push({CMD_FEED, filletFeed});
                
                remainingAngle -= chunkAngle;
                extraFeeds++;
            }
            
            // Be not phan goc le con lai (chac chan nam trong nguong an toan)
            if (abs(remainingAngle) > 0.1f) {
                float finalServoTarget = 90.0f + engine.calcBend(remainingAngle);
                if (finalServoTarget > 180.0f) finalServoTarget = 180.0f;
                if (finalServoTarget < 0.0f) finalServoTarget = 0.0f;
                
                cmdQueue.push({CMD_BEND, finalServoTarget});
            }
        }
        
        // Tinh toan chieu dai di thang sau khi bẻ
        // Tru di luong hao phi chieu dai da nhich de bẻ goc
        float lengthWithError = engine.calcFeed(currentVec.length) - (extraFeeds * filletFeed);
        
        // Chan so am neu doan can day ngan hon ca phan da hao phi
        if (lengthWithError < 0.0f) {
            lengthWithError = 0.0f;
        }
                
        int strokes = (int)(lengthWithError / MAX_FEED_STROKE_MM);
        float remainder = lengthWithError - (strokes * MAX_FEED_STROKE_MM);
        
        // Bam nho lenh day chieu dai chinh
        for(int s = 0; s < strokes; s++) {
            cmdQueue.push({CMD_FEED, MAX_FEED_STROKE_MM});
        }
        
        if (remainder > 0.0f) {
            cmdQueue.push({CMD_FEED, remainder});
        }
    }
    
    printLn("Da nap lenh vao hang doi.");
}

void loop() {
#ifndef ARDUINO
    // Tao luong ghi file tinh, chi khoi tao 1 lan khi loop chay lan dau
    // Co ios::trunc de xoa trang du lieu cu moi khi chay lai phan mem
    static std::ofstream outFile("master/data/result.txt", std::ios::out | std::ios::trunc);
#endif

    if (!cmdQueue.isEmpty()) {
        Instruction currentCmd = cmdQueue.pop();
        
        if (currentCmd.type == CMD_FEED) {
            print("FEED ");
            printLn(currentCmd.value);
            
#ifndef ARDUINO
            if (outFile.is_open()) {
                outFile << "FEED " << currentCmd.value << "\n";
            }
#else
            machine.executeFeed(currentCmd.value);
#endif
        } 
        else if (currentCmd.type == CMD_BEND) {
            print("BEND ");
            printLn(currentCmd.value);
            
#ifndef ARDUINO
            if (outFile.is_open()) {
                outFile << "BEND " << currentCmd.value << "\n";
            }
#else
            machine.executeBend(currentCmd.value);
#endif
        }
    } else {
#ifndef ARDUINO
        // Dong file de xa toan bo bo dem xuong o cung truoc khi thoat
        if (outFile.is_open()) {
            outFile.close();
        }
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