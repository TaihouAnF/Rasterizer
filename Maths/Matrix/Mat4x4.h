#pragma once
#include "../Vector/Vector3d.h"

/**
 * @brief A 4 by 4 matrix, initialize with 0.0f.
 */
struct Mat4x4 {
    float m[4][4] = { 0 };
};

/**
 * @brief This method helps to do the matrix calculation with matrix and vector,
 * it uses pass-in by reference to save some space.
 * @param i The input vector.
 * @param out The result vector.
 * @param matrix The matrix.
 */
void MultiplyMatrixVector(Vector3d& i, Vector3d& out, Mat4x4& matrix);

/** 
 * @brief Overloaded version of vec mat multiplication
 * @param i The vector
 * @param matrix The matrix
 * @return The result vector
 */
Vector3d MultiplyMatrixVector(Vector3d& i, Mat4x4& matrix);