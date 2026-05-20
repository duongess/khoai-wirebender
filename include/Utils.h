#ifndef ARDUINO
#include <iostream>
#include <fstream>
#else
#include <Arduino.h>
#endif

// Su dung template de compiler tu dong tao ham tuong ung voi kieu du lieu truyen vao
template <typename T>
void printLn(T data) {
    #ifndef ARDUINO
    std::cout << data << std::endl;
    #else
    Serial.println(data);
    #endif
}

template <typename T>
void print(T data) {
    #ifndef ARDUINO
    std::cout << data;
    #else
    Serial.print(data);
    #endif
}