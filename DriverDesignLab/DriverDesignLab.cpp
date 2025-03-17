// DriverDesignLab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>

#include "Lab1.h"

int main()
{
    Lab1 obj;
    auto paths = obj.getImagePaths();
    for (const auto& pair : paths) {
        std::wcout << pair.first << L": " << pair.second << std::endl;
    }
    return 0;
}