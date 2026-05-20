#include "HardwareController.h"
#include "Config.h"

HardwareController::HardwareController() {
    lastDebounceTime = 0;
    // Gia su nut bam noi voi GND nen dung INPUT_PULLUP (mac dinh la HIGH)
    buttonState = HIGH;
    lastButtonState = HIGH;
}

void HardwareController::init() {
#ifdef ARDUINO
    servoFeed.attach(PIN_SERVO_FEED);
    servoBend.attach(PIN_SERVO_BEND);
    
    // Kich hoat dien tro keo len cho nut bam
    pinMode(PIN_BTN_START, INPUT_PULLUP);
    
    servoFeed.write(0);
    servoBend.write(0);
    
    Serial.println("SYS_READY");
#endif
}

bool HardwareController::isStartStopPressed() {
#ifdef ARDUINO
    int reading = digitalRead(PIN_BTN_START);

    // Kiem tra neu trang thai vat ly vua thay doi
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    // Neu trang thai giu nguyen lau hon khoang thoi gian chong nhieu (50ms)
    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY_MS) {
        if (reading != buttonState) {
            buttonState = reading;
            
            // Phat hien su kien nhan xuong (HIGH xuong LOW)
            if (buttonState == LOW) {
                lastButtonState = reading;
                return true; 
            }
        }
    }
    lastButtonState = reading;
#endif
    return false;
}

void HardwareController::executeFeed(float distance) {
#ifdef ARDUINO
    // Kiem tra loai tru cac gia tri am hoac 0
    if (distance <= 0.0f) return;

    unsigned long runTimeMs = (unsigned long)((distance / SPEED_MM_PER_SEC) * 1000.0f);

    // 1. Phat lenh quay toi voi toc do toi da
    servoFeed.write(MAX_SPEED); 
    
    // 2. Cho doi cho den khi dat du chieu dai (Bao gom sai so sụt ap)
    delay(runTimeMs);
    
    // 3. Phat lenh dung dong co (Can kiem tra lai xem con servo cua ban dung o 90 hay so khac)
    servoFeed.write(90); 

    // Doi them mot chut de triet tieu quan tinh truoc khi uon
    delay(200);

    Serial.print("F_MM:");
    Serial.println(distance);
    Serial.print("TIME_MS:");
    Serial.println(runTimeMs);
#endif
}

void HardwareController::executeBend(float angle) {
#ifdef ARDUINO
    int targetAngle = (int)angle;
    if (targetAngle > 180) targetAngle = 180;
    if (targetAngle <= 0) return;
    
    servoBend.write(targetAngle);
    delay(600); 
    
    servoBend.write(0); 
    delay(500);
    
    Serial.print("B:");
    Serial.println(targetAngle);
#endif
}