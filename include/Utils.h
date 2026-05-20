#ifndef ARDUINO
#include <iostream>
#include <fstream>
#else
#include <Arduino.h>
#endif

// Su dung template de compiler tu dong tao ham tuong ung voi kieu du lieu truyen vao
template <typename T>
void printLog(T data) {
    #ifndef ARDUINO
    std::cout << data << std::endl;
    #else
    Serial.println(data);
    #endif
}