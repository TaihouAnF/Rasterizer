// rasterizer3D.cpp : This file contains the 'main' function. Program execution begins and ends there.
// #include <iostream>


/**
 * This code is following the tutorial created by @javidx9, 
 * Follow the link here: https://youtu.be/ih20l3pJoeU?si=3iTC-grtO-zyhjuu 
 */

#include <algorithm>
#include "olcConsoleGameEngine.h"
#include "Maths/Vector/Vector3d.h"
#include "Maths/Matrix/Mat4x4.h"
#include "Primitive/Triangle.h"
#include "Primitive/Mesh.h"

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

        // Now we move the transformation outside the for loop, and make it a whole transform matrix

        theta_ += 1.0f * delta_time;
        
        // Rotation Z and X matrices
        Mat4x4 mat_rot_z, mat_rot_x;
        mat_rot_z = MakeRotationZ(theta_ * 0.5f);
        mat_rot_x = MakeRotationX(theta_);

        // Translation matrix
        Mat4x4 mat_trans = MakeTranslation(0.0f, 0.0f, 16.0f);

        // World matrix, Final transformation matrix
        Mat4x4 mat_world = MakeIdentity();
        mat_world = MultiplyMatrix(mat_rot_z, mat_rot_x);
        mat_world = MultiplyMatrix(mat_world, mat_trans);
        
        // In order to use painter algorithm, we need a new array to cache the triangles
        std::vector<Triangle> sort_tri_raster;

        // Draw Triangles/Mesh, so far we only have a vector array of Triangles.
        // thus, we use for loop
        for (auto& tri : mesh_cube_.tris) {
            Triangle triangle_proj{}, triangle_transform{};

            // Make the Transform
            for (int i = 0; i < 3; ++i) {
                triangle_transform.pts[i] = MultiplyMatrixVector(tri.pts[i], mat_world);
            }

            // Calculate the normal first before the projection
            Vector3d normal{}, line_1{}, line_2{};
            VectorSub(triangle_transform.pts[1], triangle_transform.pts[0], line_1);
            VectorSub(triangle_transform.pts[2], triangle_transform.pts[0], line_2);
            CrossProduct(line_1, line_2, normal);
            Normalize(normal);
            
            /**
             * Now we actually have a camera, we want to make sure the normal is facing the camera direction, 
             * so we introduce a dot product here. 
             * And we can take any points on the triangle as they are all on the same plane.
             */

            Vector3d cam_ray = VectorSub(triangle_transform.pts[0], cam_);

            if (DotProduct(normal, cam_ray) < 0) {

                // Illumination before projection, temporarily
                Vector3d light_dir = { 0.0f, 1.0f, -1.0f };
                Normalize(light_dir);

                // How "aligned" are light direction and triangle surface normal?
                float dp = max(0.1f, DotProduct(light_dir, normal));

                // Get the color by using the dot product.
                CHAR_INFO c = GetColor(dp);
                triangle_transform.sym = c.Char.UnicodeChar;
                triangle_transform.col = c.Attributes;


                // Projection from 3D ---> 2D
                for (int i = 0; i < 3; ++i) {
                    // projection
                    MultiplyMatrixVector(triangle_transform.pts[i], triangle_proj.pts[i], mat_projection_);
                    
                    // normalize
                    triangle_proj.pts[i] = VectorDiv(triangle_proj.pts[i], triangle_proj.pts[i].w);
                }
                triangle_proj.sym = triangle_transform.sym;
                triangle_proj.col = triangle_transform.col;

                // Scaling the Triangle
                // Offset vector
                Vector3d offset = { 1, 1, 0 };
                for (int i = 0; i < 3; ++i) {
                    triangle_proj.pts[i] = VectorAdd(triangle_proj.pts[i], offset);
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
