#include <cmath>
#include "Vector3d.h"

Vector3d::Vector3d() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

Vector3d::Vector3d(float new_x, float new_y, float new_z)  : x(new_x), y(new_y), z(new_z), w(1.0f) {}

void VectorAdd(Vector3d& input_a, Vector3d& input_b, Vector3d& out) {
    out.x = input_a.x + input_b.x;
    out.y = input_a.y + input_b.y;
    out.z = input_a.z + input_b.z;
}

Vector3d VectorAdd(Vector3d& input_a, Vector3d& input_b) {
    return {input_a.x + input_b.x, 
            input_a.y + input_b.y, 
            input_a.z + input_b.z};
}

void VectorSub(Vector3d& input_a, Vector3d& input_b, Vector3d& out) {
    out.x = input_a.x - input_b.x;
    out.y = input_a.y - input_b.y;
    out.z = input_a.z - input_b.z;
}

Vector3d VectorSub(Vector3d& input_a, Vector3d& input_b) {
    return {input_a.x - input_b.x,
            input_a.y - input_b.y,
            input_a.z - input_b.z};
}

void VectorMul(Vector3d& input_a, float m, Vector3d& out) {
    out.x = input_a.x * m;
    out.y = input_a.y * m;
    out.z = input_a.z * m;
}

Vector3d VectorMul(Vector3d& input_a, float m) {
    return {input_a.x * m,
            input_a.y * m,
            input_a.z * m};
}

void VectorDiv(Vector3d& input_a, float d, Vector3d& out) {
    if (d == 0.0f) {
        out.x = input_a.x;
        out.y = input_a.y;
        out.z = input_a.z;
    }
    else {
        out.x = input_a.x / d;
        out.y = input_a.y / d;
        out.z = input_a.z / d;
    }
    
}

Vector3d VectorDiv(Vector3d& input_a, float d) {
    if (d == 0) {
        return input_a;
    }
    return {input_a.x / d,
            input_a.y / d,
            input_a.z / d};
}

float DotProduct(Vector3d input_a, Vector3d input_b) {
    return input_a.x * input_b.x +
           input_a.y * input_b.y +
           input_a.z * input_b.z;
}

float DotProductRef(Vector3d& input_a, Vector3d& input_b) {
    return input_a.x * input_b.x +
           input_a.y * input_b.y +
           input_a.z * input_b.z;
}

float VectorLength(Vector3d& vec) {
    return std::sqrtf(DotProduct(vec, vec));
}

void CrossProduct(Vector3d& input_a, Vector3d& input_b, Vector3d& out) {
    out.x = input_a.y * input_b.z - input_a.z * input_b.y;
    out.y = input_a.z * input_b.x - input_a.x * input_b.z;
    out.z = input_a.x * input_b.y - input_a.y * input_b.x;
}

Vector3d CrossProduct(Vector3d& input_a, Vector3d& input_b) {
    Vector3d res{};
    res.x = input_a.y * input_b.z - input_a.z * input_b.y;
    res.y = input_a.z * input_b.x - input_a.x * input_b.z;
    res.z = input_a.x * input_b.y - input_a.y * input_b.x;
    return res;
}

void Normalize(Vector3d& vec) {
    float l = VectorLength(vec);
    vec.x /= l;
    vec.y /= l;
    vec.z /= l;
}

Vector3d NormalizeToNew(Vector3d& vec) {
    float l = VectorLength(vec);
    return { vec.x / l, vec.y / l, vec.z / l };
}

Vector3d IntersectPlane(Vector3d& plane_p, Vector3d& plane_n, Vector3d& line_start, Vector3d& line_end) {
    return Vector3d();
}


