// rasterizer3D.cpp : This file contains the 'main' function. Program execution begins and ends there.
// #include <iostream>


/**
 * This code is following the tutorial created by @javidx9, 
 * Follow the link here: https://youtu.be/ih20l3pJoeU?si=3iTC-grtO-zyhjuu 
 */

#include "olcConsoleGameEngine.h"

constexpr float PI = 3.14159f;

/**
 * @brief A 3D vector in space, x y and z represents coordinate in 3D space
 */
struct vector3d {
    float x, y, z;
};

/**
 * @brief A triangle object with three points. 
 */
struct triangle {
    vector3d pts[3];
};

/**
 * @brief A mesh of multiple triangles, use this to represent arbitrary type of objects
 */
struct mesh {
    std::vector<triangle> tris;
};

/**
 * @brief A 4 by 4 matrix, initialize with 0
 */
struct mat4x4 {
    float m[4][4] = { 0 };
};

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
        
        // Create a Cube with 6 faces. Here we use struct initialization here
        mesh_cube_.tris = {
            // SOUTH
            { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

            // EAST                                                      
            { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

            // NORTH                                                     
            { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

            // WEST                                                      
            { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

            // TOP                                                       
            { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

            // BOTTOM                                                    
            { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
        };
        
        // Projection Matrix
        float near_plane = 0.1f;
        float far_plane = 1000.0f;
        float fov = 90.0f;  // FOV as usual
        float aspect_ratio = (float)ScreenHeight() / (float)ScreenWidth();
        float fov_radius = 1.0f / tanf(fov * 0.5f / 180.0f * PI);

        // Setting up the projection matrix
        mat_projection_.m[0][0] = aspect_ratio * fov_radius;
        mat_projection_.m[1][1] = fov_radius;
        mat_projection_.m[2][2] = far_plane / (fov_radius - near_plane);
        mat_projection_.m[3][2] = (-far_plane * near_plane) / (far_plane - near_plane);
        mat_projection_.m[2][3] = 1.0f;
        mat_projection_.m[3][3] = 0.0f;

        // Return true to indicate it works without error
        return true;
    }

    /**
     * @brief 
     * @param fDeltaTime The time difference between two frame updates
     * @return true if successfully called, otherwise false.
     */
    bool OnUserUpdate(float fDeltaTime) override {

        // Fill the background With color, only works on first project
        Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLUE);

        // Draw triangles/mesh, so far we only have a vector array of triangle
        // thus, we use for loop
        for (auto& tri : mesh_cube_.tris) {

        }


        // Return true to indicate it works without error
        return true;
    }

private:
    mesh mesh_cube_;
    mat4x4 mat_projection_;

    float delta_time_;
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
