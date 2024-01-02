#pragma once
#include "../Maths/Vector/Vector3d.h"

/**
 * @brief A Triangle object with three Vector3d points.
 */
struct Triangle {
    Vector3d pts[3]; // Three vertices

    wchar_t sym;     // Symbol to the color
    short col;       // Color value? 
};