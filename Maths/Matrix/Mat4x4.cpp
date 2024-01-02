#include <fstream>
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

Mat4x4 MakeIdentity() {
    Mat4x4 mat;
    mat.m[0][0] = 1.0f;
    mat.m[1][1] = 1.0f;
    mat.m[2][2] = 1.0f;
    mat.m[3][3] = 1.0f;
    return mat;
}

Mat4x4 MakeRotationX(float angleRad) {
    Mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = cosf(angleRad);
    matrix.m[1][2] = sinf(angleRad);
    matrix.m[2][1] = -sinf(angleRad);
    matrix.m[2][2] = cosf(angleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

Mat4x4 MakeRotationY(float angleRad) {
    Mat4x4 matrix;
    matrix.m[0][0] = cosf(angleRad);
    matrix.m[0][2] = sinf(angleRad);
    matrix.m[2][0] = -sinf(angleRad);
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = cosf(angleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

Mat4x4 MakeRotationZ(float angleRad) {
    Mat4x4 matrix;
    matrix.m[0][0] = cosf(angleRad);
    matrix.m[0][1] = sinf(angleRad);
    matrix.m[1][0] = -sinf(angleRad);
    matrix.m[1][1] = cosf(angleRad);
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

Mat4x4 MakeTranslation(float x, float y, float z) {
    Mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;
    return matrix;
}

Mat4x4 MakeProjection(float fov_degrees, float aspect_ratio, float near_plane, float far_plane) {
    float fov_rad = 1.0f / tanf(fov_degrees * 0.5f / 180.0f * PI);
    Mat4x4 matrix;
    matrix.m[0][0] = aspect_ratio * fov_rad;
    matrix.m[1][1] = fov_rad;
    matrix.m[2][2] = far_plane / (far_plane - near_plane);
    matrix.m[3][2] = (-far_plane * near_plane) / (far_plane - near_plane);
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;
    return matrix;
}

Mat4x4 MultiplyMatrix(Mat4x4& m1, Mat4x4& m2) {
    Mat4x4 matrix;
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
    return matrix;
}

Mat4x4 PointAt(Vector3d& pos, Vector3d& target, Vector3d& up) {
    // Calculate new forward direction, u
    Vector3d u = VectorSub(target, pos);
    Normalize(u);

    // Set up new up direction, v
    Vector3d a = VectorMul(u, DotProduct(up, u));
    Vector3d v = VectorSub(up, a);
    Normalize(v);

    // New Right, w
    Vector3d w = CrossProduct(v, u);

    Mat4x4 matrix;
    // NEED CHECK
    matrix.m[0][0] = w.x; matrix.m[0][1] = w.y; matrix.m[0][2] = w.z; matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = v.x; matrix.m[1][1] = v.y; matrix.m[1][2] = v.z; matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = u.x; matrix.m[2][1] = u.y; matrix.m[2][2] = u.z; matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = pos.x; matrix.m[3][1] = pos.y; matrix.m[3][2] = pos.z; matrix.m[3][3] = 1.0f;
    return matrix;
}

Mat4x4 Inverse(Mat4x4& m) {
    Mat4x4 matrix;
    matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
    matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
    matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
    matrix.m[3][3] = 1.0f;
    return matrix;
}
