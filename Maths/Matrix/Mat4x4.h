#pragma once

#include "../Vector/Vector3d.h"


constexpr float PI = 3.14159f;

/**
 * @brief A 4 by 4 matrix, initialize with 0.0f.
 */
struct Mat4x4 {
    float m[4][4] = { 0 };
};

/* -------------------------------
 -------- Matrix & Vector --------
 ------------------------------- */

/**
 * @brief This method helps to do the matrix calculation with matrix and vector,
 * it uses pass-in by reference to save some space.
 * @param i The input vector.
 * @param out The result vector.
 * @param matrix The matrix.
 */
void MultiplyMatrixVector(Vector3d& i, Vector3d& out, Mat4x4& matrix);

/** 
 * @brief Overloaded version of vec mat multiplication.
 * @param i The vector
 * @param matrix The matrix
 * @return The result vector
 */
Vector3d MultiplyMatrixVector(Vector3d& i, Mat4x4& matrix);

/* ---------------------------------
 ------------ Identity -------------
 --------------------------------- */

/**
 * @brief Make a Identity matrix.
 * @return The identity matrix
 */
Mat4x4 MakeIdentity();

/* ---------------------------------
 ------------ Rotation -------------
 --------------------------------- */

/**
 * @brief Make a rotation on X-axis.
 * @param angleRad angle to rotate in radius
 * @return The rotation matrix
 */
Mat4x4 MakeRotationX(float angleRad);

/**
 * @brief Make a rotation on Y-axis.
 * @param angleRad angle to rotate in radius
 * @return The rotation matrix
 */
Mat4x4 MakeRotationY(float angleRad);

/**
 * @brief Make a rotation on Z-axis.
 * @param angleRad angle to rotate in radius
 * @return The rotation matrix
 */
Mat4x4 MakeRotationZ(float angleRad);

/* ---------------------------------
 ---------- Translation ------------
 --------------------------------- */

/**
 * @brief Make a translation matrix.
 * @param x X coordinate that need to add
 * @param y Y coordinate that need to add
 * @param z Z coordinate that need to add
 * @return A translation matrix
 */
Mat4x4 MakeTranslation(float x, float y, float z);

/* -------------------------------
 ---------- Projection -----------
 ------------------------------- */

/**
 * @brief Construct a projection matrix
 * @param fov_degrees FOV degrees
 * @param aspect_ratio Ratio between height and width
 * @param near_plane The near plane of the frustum
 * @param far_plane The far plane of the frustum
 * @return The projection matrix
 */
Mat4x4 MakeProjection(float fov_degrees, float aspect_ratio, float near_plane, float far_plane);

/* -------------------------------
 ---- Matrix Multiplication ------
 ------------------------------- */

/**
 * @brief Matrix multiplication, this one has possible optimization
 * @param m1 First matrix
 * @param m2 Second matrix
 * @return The result matrix
 */
Mat4x4 MultiplyMatrix(Mat4x4& m1, Mat4x4& m2);

/* ------------------------
 ---------- Utils ---------
 ------------------------ */

/**
 * @brief This looks like setting up the camera coordinate/space
 * @param pos 
 * @param target 
 * @param up 
 * @return 
 */
Mat4x4 PointAt(Vector3d& pos, Vector3d& target, Vector3d& up);

/**
 * @brief Inverse the given matrix.
 * @param m The input matrix
 * @return A new matrix after inversed
 */
Mat4x4 Inverse(Mat4x4& m);