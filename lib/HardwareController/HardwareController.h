#ifndef HARDWARECONTROLLER_H
#define HARDWARECONTROLLER_H

#include <Arduino.h>
#include <Servo.h>

class HardwareController {
private:
    Servo servoFeed;
    Servo servoBend;

    // Bien dung cho bo loc nhieu nut bam (Debounce)
    unsigned long lastDebounceTime;
    int buttonState;
    int lastButtonState;

public:
    HardwareController();
    
    void init();
    bool isStartStopPressed();
    
    void executeFeed(float distance);
    void executeBend(float angle);
};

#endif