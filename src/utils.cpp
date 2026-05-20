#ifndef ARDUINO
#include <iostream>
#include <fstream>
#else
#include <Arduino.h>
#endif

void print(char* text) {
    #ifndef ARDUINO
    std::cout << text << std::endl;
    #else
    Serial.println(text);
    #endif
}