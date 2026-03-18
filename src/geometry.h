#ifndef GEOMETRY_H
#define GEOMETRY_H
typedef struct {
    float vec[2];
} Vec2;
typedef struct {
    float vec[3];
} Vec3;
typedef struct {
    float vec[4];
} Vec4;

typedef struct {
    float mat[4][4];
} Mat4x4;
typedef struct {
    float mat[2][2];
} Mat2x2;

//Vector functions
Vec3 SubtractVec3s(Vec3* v1, Vec3* v2);
void NormaliseVec3(Vec3* vector);
Vec3 CrossProductVec3(Vec3* vL, Vec3* vR);
void VecMatrixProduct(const Mat4x4 *matL, Vec4 *vecR);

//Matrix functions
void CreateMat4x4(Mat4x4* matrix, float diagonal);
Mat4x4 CreateAndReturnMat4x4(float diagonal);
void Multiply4x4Matrices(const Mat4x4 *matL, Mat4x4 *matR);
Mat4x4 Multiply4x4MatricesAndReturnMat(Mat4x4* matL, Mat4x4* matR);

//Transformation matrices
//Converting local to clip space
void CreateTranslationMatrix(Mat4x4 *transMat, const Vec4 *vTrans);
void CreateScaleMatrix(Mat4x4 *mat, const Vec3 *vScale);
void CreateRotationMatrix(Mat4x4* matrix, float theta, const Vec4* axis);
void VFOVPerspectiveProj(Mat4x4* matrix, float vertFOV, float aspectRatio, float near, float far);
Mat4x4 LookAtMatrix(Vec3* position, Vec3* target, Vec3* upVec);
void SymInfPerspectiveProj(Mat4x4* matrix, float near, float width, float height);

//Screen transform
void ConvertClipToNDC(Vec4* vector);
void ViewportTransform(Vec4* vector, float width, float height);

#endif