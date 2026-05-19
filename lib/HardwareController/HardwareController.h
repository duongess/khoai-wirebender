#ifndef HARDWARECONTROLLER_H
#define HARDWARECONTROLLER_H

#include <Arduino.h>
#include <Servo.h>

class HardwareController {
private:
    Servo servoFeed;
    Servo servoBend;
    
    int pinFeed;
    int pinBend;

public:
    HardwareController();
    
    // Khoi tao cong ket noi va dua may ve trang thai 0
    void init();
    
    // Thuc thi 1 nhat day tuyen tinh
    void executeFeed(float distance);
    
    // Thuc thi 1 nhat uon goc
    void executeBend(float angle);
};

#endif