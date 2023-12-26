// rasterizer3D.cpp : This file contains the 'main' function. Program execution begins and ends there.
// #include <iostream>


/**
 * This code is following the tutorial created by @javidx9, 
 * Follow the link here: https://youtu.be/ih20l3pJoeU?si=3iTC-grtO-zyhjuu 
 */

#include "olcConsoleGameEngine.h"

/**
 * @brief A new class inherit from olcConsoleGameEngine
 */
class newEngine : public olcConsoleGameEngine {
public:
    newEngine() {
        m_sAppName = L"3D Demo";
    }

    /**
     * @brief A function to handle creating object by the user.
     * @return true if successfully created, otherwise false.
     */
    bool OnUserCreate() override {

        // Return true to indicate it works without error
        return true;
    }

    /**
     * @brief 
     * @param fDeltaTime The time difference between two frame updates
     * @return true if successfully called, otherwise false.
     */
    bool OnUserUpdate(float fDeltaTime) override {


        // Return true to indicate it works without error
        return true;
    }
};


int main()
{
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
