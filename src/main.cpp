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
        
        // Bo qua thuc thi servo neu net ve di thang (goc lech xap xi 0)
        if (abs(currentVec.angle) > 0.1f) {
            
            // Tinh goc co bu tru dan hoi (VD: -45 do * 1.05 = -47.25 do)
            float bendAngleWithError = engine.calcBend(currentVec.angle);
            
            // MAP GOC SERVO:
            // -90 do (be phai) -> 0 do Servo
            // 0 do (di thang) -> 90 do Servo (Vi tri home moi)
            // +90 do (be trai) -> 180 do Servo
            float servoTarget = 90.0f + bendAngleWithError;
            
            // Gioi han chong va dap the chat cho servo
            if (servoTarget > 180.0f) servoTarget = 180.0f;
            if (servoTarget < 0.0f) servoTarget = 0.0f;
            
            cmdQueue.push({CMD_BEND, servoTarget});
        }
        
        // Day phoi
        float lengthWithError = engine.calcFeed(currentVec.length);
        float maxS = engine.getMaxStroke();
        
        int strokes = (int)(lengthWithError / maxS);
        float remainder = lengthWithError - (strokes * maxS);
        
        for(int s = 0; s < strokes; s++) {
            cmdQueue.push({CMD_FEED, maxS});
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