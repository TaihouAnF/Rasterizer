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
        mesh_cube_.LoadFromObjFile("mountains.obj");
        
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

        // User Control using arrow keys
        if (GetKey(VK_UP).bHeld) {
            cam_.y += 8.0f * delta_time;
        }

        if (GetKey(VK_DOWN).bHeld) {
            cam_.y -= 8.0f * delta_time;
        }

        if (GetKey(VK_LEFT).bHeld) {
            cam_.x -= 8.0f * delta_time;
        }

        if (GetKey(VK_RIGHT).bHeld) {
            cam_.x += 8.0f * delta_time;
        }

        // FPS control
        // we make a forward velocity vector
        Vector3d forward = VectorMul(look_dir_, 8.0f * delta_time);

        if (GetKey(L'W').bHeld) {
            cam_ = VectorAdd(cam_, forward);
        }

        if (GetKey(L'S').bHeld) {
            cam_ = VectorSub(cam_, forward);
        }

        if (GetKey(L'A').bHeld) {
            yaw_ -= 2.0f * delta_time;
        }

        if (GetKey(L'D').bHeld) {
            yaw_ += 2.0f * delta_time;
        }

        // Now we move the transformation outside the for loop, and make it a whole transform matrix
        
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
        
        // Helper Up vector
        Vector3d up = { 0.0f, 1.0f, 0.0f };

        Vector3d target = { 0.0f, 0.0f, 1.0f };
        Mat4x4 mat_cam_rot = MakeRotationY(yaw_);

        look_dir_ = MultiplyMatrixVector(target, mat_cam_rot);
        target = VectorAdd(cam_, look_dir_);

        // Camera matrix
        Mat4x4 mat_cam = PointAt(cam_, target, up);

        // View matrix/Inverse
        Mat4x4 mat_view = Inverse(mat_cam);

        // In order to use painter algorithm, we need a new array to cache the triangles
        std::vector<Triangle> sort_tri_raster;

        // Draw Triangles/Mesh, so far we only have a vector array of Triangles.
        // thus, we use for loop
        for (auto& tri : mesh_cube_.tris) {
            // This represent 3 different stage of rendering pipeline
            Triangle triangle_proj{}, triangle_transform{}, triangle_view{};

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

                // Before projection, we want to Convert world space --> camera space/view space
                for (int i = 0; i < 3; ++i) {
                    triangle_view.pts[i] = MultiplyMatrixVector(triangle_transform.pts[i], mat_view);
                }

                triangle_view.col = triangle_transform.col;
                triangle_view.sym = triangle_transform.sym;

                // Clip views 
                int clipped_cnt = 0;
                Triangle clipped[2];
                clipped_cnt = ClipAgainstPlane({ 0.0f, 0.0f, 2.1f }, { 0.0f, 0.0f, 1.0f }, triangle_view, clipped[0], clipped[1]);


                for (int n = 0; n < clipped_cnt; ++n) {
                    // Projection from 3D ---> 2D
                    for (int i = 0; i < 3; ++i) {
                        // Projection
                        MultiplyMatrixVector(clipped[n].pts[i], triangle_proj.pts[i], mat_projection_);
                    }
                    triangle_proj.col = clipped[n].col;
                    triangle_proj.sym = clipped[n].sym;
                    

                    for (int i = 0; i < 3; ++i) {
                        // Normalize
                        triangle_proj.pts[i] = VectorDiv(triangle_proj.pts[i], triangle_proj.pts[i].w);
                    }

                    // The axis are upside down, change them back
                    for (int i = 0; i < 3; ++i) {
                        triangle_proj.pts[i].x *= -1.0f;
                        triangle_proj.pts[i].y *= -1.0f;
                    }

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
        }

        // Sort them using painter algo
        std::sort(sort_tri_raster.begin(), sort_tri_raster.end(), [](Triangle& t_1, Triangle& t_2) {
            float z1 = (t_1.pts[0].z + t_1.pts[1].z + t_1.pts[2].z) / 3.0f;
            float z2 = (t_2.pts[0].z + t_2.pts[1].z + t_2.pts[2].z) / 3.0f;
            return z1 > z2;
        });

        // Fill the background With color, only works on first project
        Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

        for (auto& tri : sort_tri_raster) {

            // Clipping triangles on the edge, we might possibly has some triangles to be clipped
            Triangle clipped[2];
            std::list<Triangle> triangle_list;
            triangle_list.push_back(tri);
            int new_tri = triangle_list.size();

            for (int i = 0; i < 4; ++i) {
                int tri_to_add = 0;
                while (new_tri > 0) {
                    Triangle test = triangle_list.front();
                    triangle_list.pop_front();
                    new_tri--;

                    // Clip against each plane, we only need to check subsequent plane
                    switch (i) {
                        case 0:
                            tri_to_add = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]);
                            break;
                        case 1:
                            tri_to_add = ClipAgainstPlane({ 0.0f, (float)ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]);
                            break;
                        case 2:
                            tri_to_add = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
                            break;
                        case 3:
                            tri_to_add = ClipAgainstPlane({ (float)ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
                            break;
                        default:
                            break;
                    }

                    // we might have new triangles for subsequent planes, and clipping may yield variable number of triangles
                    for (int k = 0; k < tri_to_add; ++k) {
                        triangle_list.push_back(clipped[k]);
                    }
                }

                // Update new tri size
                new_tri = triangle_list.size();
            }

            for (auto& t : triangle_list) {
                // Rasterize triangle, Now the olc console engine has the function call fillTriangle
                FillTriangle(t.pts[0].x, t.pts[0].y, t.pts[1].x, t.pts[1].y, t.pts[2].x, t.pts[2].y, t.sym, t.col);
            }
        }

        // Return true to indicate it works without error.
        return true;
    }

private:
    Mesh mesh_cube_;        // A Mesh used in default
    Mat4x4 mat_projection_; // A project matrix
    Vector3d cam_;          // A temporary camera currently, we set it to the origin first
    Vector3d look_dir_;     // The look at direction, should be unit length
    float theta_;           // Rotation angle
    float yaw_;             // An angle for FPS look direction

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

    /**
     * @brief This performs clipping triangles, checking whether the triangle should be renderred or not, or render parts of them
     * @param plane_p The frustum boarder plane point
     * @param normal The normal of the plane
     * @param in_tri The input triangle
     * @param out_tri_1 The placeholder for output
     * @param out_tri_2 The other placeholder 
     * @return Integer representing how many triangles are output
     */
    int ClipAgainstPlane(Vector3d plane_p, Vector3d normal, Triangle& in_tri, Triangle& out_tri_1, Triangle& out_tri_2) {
        // Normal should be normalized
        Normalize(normal);

        // Return signed shortest distance from point to plane, plane normal must be normalized
        auto dist = [&](Vector3d& p) {
            return (normal.x * p.x + normal.y * p.y + normal.z * p.z - DotProduct(normal, plane_p));
        };

        // Create two temporary storage arrays to classify points either side of plane
        // If distance sign is positive, point lies on "inside" of plane
        Vector3d* inside_pts[3]{};
        Vector3d* outside_pts[3]{};

        int cnt_inside = 0, cnt_outside = 0;

        // Get signed distance of each point in triangle to plane
        float d0 = dist(in_tri.pts[0]);
        float d1 = dist(in_tri.pts[1]);
        float d2 = dist(in_tri.pts[2]);

        if (d0 >= 0) {
            inside_pts[cnt_inside++] = &in_tri.pts[0];
        }
        else {
            outside_pts[cnt_outside++] = &in_tri.pts[0];
        }

        if (d1 >= 0) {
            inside_pts[cnt_inside++] = &in_tri.pts[1];
        }
        else {
            outside_pts[cnt_outside++] = &in_tri.pts[1];
        }

        if (d2 >= 0) {
            inside_pts[cnt_inside++] = &in_tri.pts[2];
        }
        else {
            outside_pts[cnt_outside++] = &in_tri.pts[2];
        }

        // 4 possible case
        if (cnt_inside == 0) {
            // The triangle is outside of the frustum
            return 0;
        }

        if (cnt_inside == 3) {
            // All points line inside of the frustum
            out_tri_1 = in_tri;

            return 1;
        }

        if (cnt_inside == 1 && cnt_outside == 2) {
            
            // Triangle should be clipped. As two points lie outside
            // the plane, the triangle simply becomes a smaller triangle

            out_tri_1.col = in_tri.col;
            out_tri_1.sym = in_tri.sym;

            out_tri_1.pts[0] = *inside_pts[0];
            out_tri_1.pts[1] = IntersectPlane(plane_p, normal, *inside_pts[0], *outside_pts[0]);
            out_tri_1.pts[2] = IntersectPlane(plane_p, normal, *inside_pts[0], *outside_pts[1]);
            return 1;
        }

        if (cnt_inside == 2 && cnt_outside == 1) {
            // Make quad to be two triangles
            out_tri_1.col = in_tri.col;
            out_tri_1.sym = in_tri.sym;
            out_tri_2.col = in_tri.col;
            out_tri_2.sym = in_tri.sym;

            out_tri_1.pts[0] = *inside_pts[0];
            out_tri_1.pts[1] = *inside_pts[1];
            out_tri_1.pts[2] = IntersectPlane(plane_p, normal, *inside_pts[0], *outside_pts[0]);

            out_tri_2.pts[0] = *inside_pts[1];
            out_tri_2.pts[1] = out_tri_1.pts[2];
            out_tri_2.pts[2] = IntersectPlane(plane_p, normal, *inside_pts[1], *outside_pts[0]);
            return 2;
        }
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
