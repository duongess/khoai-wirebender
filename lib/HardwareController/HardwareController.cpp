#include "HardwareController.h"

HardwareController::HardwareController() {
    // Chot cong tren Arduino Uno
    pinFeed = A1;
    pinBend = A0;
}

void HardwareController::init() {
#ifdef ARDUINO
    servoFeed.attach(pinFeed);
    servoBend.attach(pinBend);
    
    servoFeed.write(0);
    servoBend.write(0);
    
    Serial.println("SYS_READY");
#endif
}

void HardwareController::executeFeed(float distance) {
#ifdef ARDUINO
    // Ep kieu tu float sang int cho Servo
    int targetAngle = (int)distance;
    if (targetAngle > 180) targetAngle = 180;
    
    servoFeed.write(targetAngle);
    delay(400); // Thoi gian tay don day ra
    
    servoFeed.write(0); // Thu tay don ve de ngam 1 chieu giu lai
    delay(400);
    
    Serial.print("F:");
    Serial.println(targetAngle);
#endif
}

void HardwareController::executeBend(float angle) {
#ifdef ARDUINO
    int targetAngle = (int)angle;
    if (targetAngle > 180) targetAngle = 180;
    if (targetAngle <= 0) return;
    
    servoBend.write(targetAngle);
    delay(600); // Thoi gian uon
    
    servoBend.write(0); // Thu chot uon ve vi tri nghi
    delay(500);
    
    Serial.print("B:");
    Serial.println(targetAngle);
#endif
}