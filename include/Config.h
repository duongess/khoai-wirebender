#ifndef CONFIG_H
#define CONFIG_H

#ifndef ARDUINO
    #define A0 14
    #define A1 15
    #define A2 16
    #define A3 17
    #define A4 18
    #define A5 19
#endif

enum CommandType {
    CMD_FEED, 
    CMD_BEND  
};

struct Instruction {
    CommandType type;
    float value; // Chieu dai (mm) hoac Goc uon (do)
};

struct Vector {
    float length; 
    float angle;  
};

// ==========================================
// CAU HINH CHAN PHAN CUNG (PINOUT)
// ==========================================
const int PIN_SERVO_FEED = A1;   // PWM dieu khien servo day day
const int PIN_SERVO_BEND = A0;  // PWM dieu khien servo uon day
const int PIN_BTN_START  = 2;   // Nut bam Start/Stop

// ==========================================
// CAU HINH QUY DOI HE QUY CHIEU (SCALE FACTORS)
// ==========================================
// Gia su ban ve 500x500 px. Muon kich thuoc phoi toi da la 100mm.
// Ty le quy doi = 100 / 500 = 0.2
const float PIXEL_TO_MM_RATIO = 0.2f; 

// Ty le chuyen doi tu MM sang thoi gian cua dong co Feed
const float SPEED_MM_PER_SEC = 10.0f; 

// Toc đo toi da di chuyen
const float MAX_SPEED = 180.0f; // (90-180]

// ==========================================
// GIOI HAN CO KHI (MECHANICAL CONSTRAINTS)
// ==========================================
const float MAX_FEED_STROKE_MM = 30.0f; // Hanh trinh day toi da cua 1 nhat ep (mm)
const int BEND_HOME_ANGLE = 0;          // Goc nghi mac dinh de thep di qua khong bi ket (do)
const int BEND_MAX_ANGLE = 180;         // Gioi han chong va dap co khi cua servo uon (do)
const int MAX_POINTS = 50; // Gioi han diem

// ==========================================
// BU TRU SAI SO (ERROR COMPENSATION)
// ==========================================
const float SPRINGBACK_FACTOR = 1.05f;  // He so dan hoi (Vi du: Uon 90 do he thong se tu nhan 90 * 1.05 = 94.5 do)
const float FEED_SLIP_FACTOR  = 1.02f;  // Bu truot ma sat khi day thep (Day 10mm thanh 10.2mm)
const unsigned long DEBOUNCE_DELAY_MS = 50; // Thoi gian tre chong nhieu nut bam (ms)

#endif