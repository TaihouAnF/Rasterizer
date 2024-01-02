#pragma once

/**
 * @brief A 3D vector in space, x y and z represents coordinate in 3D space.
 */
struct Vector3d {
    float x, y, z, w;

    /**
     * @brief Constructor
     */
    Vector3d();

    /**
     * @brief Constructor with new x y z
     * @param new_x 
     * @param new_y 
     * @param new_z 
     */
    Vector3d(float new_x, float new_y, float new_z);
};

/* ---------------------------------
 ------------ Addition -------------
 --------------------------------- */

/**
 * @brief This method performs vector addition, both vector should be the same size.
 * @param input_a One parameter vector
 * @param input_b The other parameter vector
 * @param out The result vector
 */
void VectorAdd(Vector3d& input_a, Vector3d& input_b, Vector3d& out);

/**
 * @brief Overloaded version of previous addition.
 * @param input_a One parameter vector
 * @param input_b The other parameter vector
 * @return The result vector
 */
Vector3d VectorAdd(Vector3d& input_a, Vector3d& input_b);

/* ---------------------------------
 ----------- Subtraction -----------
 --------------------------------- */

/**
 * @brief Similar to Addition.
 * @param input_a One parameter vector
 * @param input_b The other parameter vector
 * @param out The result vector
 */
void VectorSub(Vector3d& input_a, Vector3d& input_b, Vector3d& out);

/**
 * @brief This method is an overloaded version of previous subtraction.
 * @param input_a One parameter vector
 * @param input_b The other parameter vector
 * @return The result vector
 */
Vector3d VectorSub(Vector3d& input_a, Vector3d& input_b);

/* ---------------------------------
 ----------- Multiplication --------
 --------------------------------- */

/**
 * @brief Vector Multiplication with a float.
 * @param input_a The Vector to be multiplied
 * @param m The floating point number
 * @param out A result vector to store the final result
 */
void VectorMul(Vector3d& input_a, float m, Vector3d& out);

/**
 * @brief Overloaded version of VectorMul, this one would return a new Vector3d.
 * @param input_a The Vector to be multiplied
 * @param m The floating point number
 * @return A result vector
 */
Vector3d VectorMul(Vector3d& input_a, float m);

/* --------------------------------
 ----------- Division -------------
 -------------------------------- */

/**
 * @brief Performs Vector division with a float.
 * @param input_a Vector to be divided
 * @param d The float, MAKE SURE this != 0.0f
 * @param out The result Vector
 */
void VectorDiv(Vector3d& input_a, float d, Vector3d& out);

/**
 * @brief Performs Vector division with a float, This is overloaded version
 * @param input_a Vector to be divided
 * @param d The float, MAKE SURE this != 0.0f
 * @return The result Vector
 */
Vector3d VectorDiv(Vector3d& input_a, float d);

/* -----------------------------
 ----------- Utils -------------
 ----------------------------- */

/**
 * @brief Compute the dot product between two vectors in 3d.
 * @param input_a One parameter vector
 * @param input_b The other parameter vector
 * @return the final product of the dot product operation
 */
float DotProduct(Vector3d input_a, Vector3d input_b);

/**
 * @brief Compute the dot product between two vectors in 3d.
 * @param input_a One parameter vector
 * @param input_b The other parameter vector
 * @return the final product of the dot product operation
 */
float DotProductRef(Vector3d& input_a, Vector3d& input_b);

/**
 * @brief Takes in a vector and return its length.
 * @param vec Vector
 * @return The length of vector
 */
float VectorLength(Vector3d& vec);

/**
 * @brief This method performs a cross product of input_a and input_b in 3d.
 * @param input_a One parameter vector
 * @param input_b The other parameter vector
 * @param out The result vector
 */
void CrossProduct(Vector3d& input_a, Vector3d& input_b, Vector3d& out);

/**
 * @brief Overloaded version of CrossProduct which returns a new Vector.
 * @param input_a One parameter vector
 * @param input_b The other parameter vector
 * @return The result vector
 */
Vector3d CrossProduct(Vector3d& input_a, Vector3d& input_b);

/**
 * @brief Normalize the vector.
 * @param vec The Vector3d we need to normalize.
 */
void Normalize(Vector3d& vec);

/**
 * @brief Normalize the vector, this function would return a new instance.
 * @param vec The Vector3d we need to normalize.
 */
Vector3d NormalizeToNew(Vector3d& vec);

/**
 * @brief 
 * @param plane_p 
 * @param plane_n 
 * @param line_start 
 * @param line_end 
 * @return 
 */
Vector3d IntersectPlane(Vector3d& plane_p, Vector3d& plane_n, Vector3d& line_start, Vector3d& line_end);
