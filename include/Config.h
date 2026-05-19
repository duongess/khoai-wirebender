#ifndef CONFIG_H
#define CONFIG_H

// Dinh nghia cac loai hanh dong cua may
enum CommandType {
    CMD_FEED, 
    CMD_BEND  
};

// Cau truc cua 1 lenh dieu khien nam trong Queue
struct Instruction {
    CommandType type;
    float value; // Dung cho ca chieu dai day (mm) hoac goc uon (do)
};

// Cau truc vector co ban
struct Vector {
    float length; // Chieu dai vector (mm)
    float angle;  // Goc cua vector (do)
};

#endif