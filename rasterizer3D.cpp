// rasterizer3D.cpp : This file contains the 'main' function. Program execution begins and ends there.
// #include <iostream>


/**
 * This code is following the tutorial created by @javidx9, 
 * Follow the link here: https://youtu.be/ih20l3pJoeU?si=3iTC-grtO-zyhjuu 
 */

#include <fstream>
#include <strstream>
#include <algorithm>
#include "olcConsoleGameEngine.h"
#include "Maths/Vector/Vector3d.h"
#include "Maths/Matrix/Mat4x4.h"

/**
 * @brief A Triangle object with three Vector3d points. 
 */
struct Triangle {
    Vector3d pts[3]; // Three vertices

    wchar_t sym;     // Symbol to the color
    short col;       // Color value? 
};

/**
 * @brief A Mesh of multiple Triangles, use this to represent arbitrary type of objects.
 */
struct Mesh {
    std::vector<Triangle> tris;

    /**
     * @brief Load obj file using the string "filename".
     * @param filename The string representing the obj file
     * @return true if successfully loaded, otherwise false
    */
    bool LoadFromObjFile(std::string filename) {
        std::ifstream f(filename);

        // If the file cannot be opened
        if (!f.is_open()) {
            std::cerr << "The file cannot be opened.\n";
            return false;
        }

        // A cache for vertices
        std::vector<Vector3d> vertices;
        while (!f.eof()) {
            
            // String buffer
            char line[128];
            
            // Read the line
            f.getline(line, 128);

            // String stream buffer
            std::strstream s;

            // Input the line
            s << line;

            // Junk, as we don't need the first character to be stored
            char junk;

            // If currently the line represent a vertex
            if (line[0] == 'v') {
                Vector3d vec{};
                s >> junk >> vec.x >> vec.y >> vec.z;
                vertices.push_back(vec);
            }
            // If currently the line represent a triangle
            if (line[0] == 'f') {
                int f[3]{};
                s >> junk >> f[0] >> f[1] >> f[2];
                tris.push_back({ vertices[f[0] - 1], vertices[f[1] - 1], vertices[f[2] - 1] });
            }
        }

        return true;
    }
};

/**
 * @brief A new class inherit from olcConsoleGameEngine
 */
class NewEngine : public olcConsoleGameEngine {
public:
    NewEngine() {
        m_sAppName = L"3D Demo";
    }

    /**
     * @brief A function to handle creating object by the user.
     * @return true if successfully created, otherwise false.
     */
    bool OnUserCreate() override {
        // Using load file
        mesh_cube_.LoadFromObjFile("VideoShip.obj");
        
        // Projection Matrix
        float near_plane = 0.1f;
        float far_plane = 1000.0f;
        float fov = 90.0f;  // FOV as usual
        float aspect_ratio = (float)ScreenHeight() / (float)ScreenWidth();
        cam_ = { 0.0f, 0.0f, 0.0f };    // The cam are set to origin for simplicity

        // Setting up the projection matrix
        mat_projection_ = MakeProjection(fov, aspect_ratio, near_plane, far_plane);

        // Return true to indicate it works without error.
        return true;
    }

    /**
     * @brief Update method, being called every frame
     * @param delta_time The time difference between two frame updates.
     * @return true if successfully called, otherwise false.
     */
    bool OnUserUpdate(float delta_time) override {

        // Fill the background With color, only works on first project
        Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

        // Setting Up Rotation Matrices
        Mat4x4 mat_rot_z, mat_rot_x;
        theta_ += 1.0f * delta_time;

        // Rotation Z
        mat_rot_z = MakeRotationZ(theta_);

        // Rotation X
        mat_rot_x = MakeRotationX(theta_ * 0.5f);

        // In order to use painter algorithm, we need a new array to cache the triangles
        std::vector<Triangle> sort_tri_raster;

        // Draw Triangles/Mesh, so far we only have a vector array of Triangles.
        // thus, we use for loop
        for (auto& tri : mesh_cube_.tris) {
            Triangle triangle_proj{}, triangle_trans{}, triangle_rotate_z{}, triangle_rotate_zx{};
            
            // Rotate on Z axis
            for (int i = 0; i < 3; ++i) {
                MultiplyMatrixVector(tri.pts[i], triangle_rotate_z.pts[i], mat_rot_z);
            }

            // Rotate on X axis
            for (int i = 0; i < 3; ++i) {
                MultiplyMatrixVector(triangle_rotate_z.pts[i], triangle_rotate_zx.pts[i], mat_rot_x);
            }

            // Translate, struct can have curly brace initialization here since C++11, and 
            // we only put an offset of 3.0f further. Now, as the ship is bigger, we need to put it further away 
            // to avoid stuttering.
            Vector3d translation = { 0.0f, 0.0f, 8.0f };
            // triangle_trans = triangle_rotate_zx; this line is redundant
            for (int i = 0; i < 3; ++i) {
                VectorAdd(triangle_rotate_zx.pts[i], translation, triangle_trans.pts[i]);
            }

            // Calculate the normal first before the projection
            Vector3d normal{}, line_1{}, line_2{};
            VectorSub(triangle_trans.pts[1], triangle_trans.pts[0], line_1);
            VectorSub(triangle_trans.pts[2], triangle_trans.pts[0], line_2);
            CrossProduct(line_1, line_2, normal);
            Normalize(normal);
            
            /**
             * Now we actually have a camera, we want to make sure the normal is facing the camera direction, 
             * so we introduce a dot product here. 
             * And we can take any points on the triangle as they are all on the same plane.
             */
            if (DotProduct(normal, VectorSub(triangle_trans.pts[0], cam_)) < 0) {

                // Illumination before projection, temporarily
                Vector3d light_dir = { 0.0f, 0.0f, -1.0f };
                Normalize(light_dir);
                // Get the color by using the dot product.
                CHAR_INFO c = GetColor(DotProduct(light_dir, normal));
                triangle_trans.sym = c.Char.UnicodeChar;
                triangle_trans.col = c.Attributes;


                // Projection from 3D ---> 2D
                for (int i = 0; i < 3; ++i) {
                    MultiplyMatrixVector(triangle_trans.pts[i], triangle_proj.pts[i], mat_projection_);
                }
                triangle_proj.sym = triangle_trans.sym;
                triangle_proj.col = triangle_trans.col;

                // Scaling the Triangle
                for (int i = 0; i < 3; ++i) {
                    triangle_proj.pts[i].x += 1.0f;
                    triangle_proj.pts[i].y += 1.0f;
                    triangle_proj.pts[i].x *= 0.5f * (float)ScreenWidth();
                    triangle_proj.pts[i].y *= 0.5f * (float)ScreenHeight();
                }

                // Push them into the triangle cache
                sort_tri_raster.push_back(triangle_proj);
            }
        }

        // sort them using painter algo
        std::sort(sort_tri_raster.begin(), sort_tri_raster.end(), [](Triangle& t_1, Triangle& t_2) {
            float z1 = (t_1.pts[0].z + t_1.pts[1].z + t_1.pts[2].z) / 3.0f;
            float z2 = (t_2.pts[0].z + t_2.pts[1].z + t_2.pts[2].z) / 3.0f;
            return z1 > z2;
        });

        for (auto& tri : sort_tri_raster) {
            // Rasterize triangle, Now the olc console engine has the function call fillTriangle
            FillTriangle(tri.pts[0].x, tri.pts[0].y,
                tri.pts[1].x, tri.pts[1].y,
                tri.pts[2].x, tri.pts[2].y,
                tri.sym, tri.col);

            // For Debug purpose
            DrawTriangle(tri.pts[0].x, tri.pts[0].y,
                tri.pts[1].x, tri.pts[1].y,
                tri.pts[2].x, tri.pts[2].y,
                PIXEL_SOLID, FG_BLACK);
        }

        // Return true to indicate it works without error.
        return true;
    }

private:
    Mesh mesh_cube_;        // A Mesh used in default
    Mat4x4 mat_projection_; // A project matrix
    Vector3d cam_;          // A temporary camera currently, we set it to the origin first
    float theta_;           // Rotation angle


    // ===================== Things are getting messy, maybe I should make this into another file ================ //

    // =========== Color code from Other Library ========= //
    // Taken From Command Line Webcam Video
    CHAR_INFO GetColor(float lum)
    {
        short bg_col, fg_col;
        wchar_t sym;
        int pixel_bw = (int)(13.0f * lum);
        switch (pixel_bw)
        {
        case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;

        case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
        case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
        case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
        case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID; break;

        case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER; break;
        case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF; break;
        case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS; break;
        case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_SOLID; break;

        case 9:  bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
        case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
        case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
        case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
        default:
            bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID;
        }

        CHAR_INFO c;
        c.Attributes = bg_col | fg_col;
        c.Char.UnicodeChar = sym;
        return c;
    }
};

/**
 * @brief The Main function
 * @return 0 if successfully run, else 1.
*/
int main()
{
    NewEngine demo;
    if (demo.ConstructConsole(256, 240, 4, 4)) {
        demo.Start();
    }
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
