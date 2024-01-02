#include "Mat4x4.h"

void MultiplyMatrixVector(Vector3d& i, Vector3d& out, Mat4x4& matrix) {
    out.x = i.x * matrix.m[0][0] + i.y * matrix.m[1][0] + i.z * matrix.m[2][0] + matrix.m[3][0];
    out.y = i.x * matrix.m[0][1] + i.y * matrix.m[1][1] + i.z * matrix.m[2][1] + matrix.m[3][1];
    out.z = i.x * matrix.m[0][2] + i.y * matrix.m[1][2] + i.z * matrix.m[2][2] + matrix.m[3][2];
    out.w = i.x * matrix.m[0][3] + i.y * matrix.m[1][3] + i.z * matrix.m[2][3] + matrix.m[3][3];
}

Vector3d MultiplyMatrixVector(Vector3d& i, Mat4x4& matrix) {
    Vector3d res{};
    res.x = i.x * matrix.m[0][0] + i.y * matrix.m[1][0] + i.z * matrix.m[2][0] + matrix.m[3][0];
    res.y = i.x * matrix.m[0][1] + i.y * matrix.m[1][1] + i.z * matrix.m[2][1] + matrix.m[3][1];
    res.z = i.x * matrix.m[0][2] + i.y * matrix.m[1][2] + i.z * matrix.m[2][2] + matrix.m[3][2];
    res.w = i.x * matrix.m[0][3] + i.y * matrix.m[1][3] + i.z * matrix.m[2][3] + matrix.m[3][3];
    return res;
}
