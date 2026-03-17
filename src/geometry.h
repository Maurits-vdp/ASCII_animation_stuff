typedef struct {
    float vec[2];
} Vec2;

typedef struct {
    float vec[4];
} Vec4;

typedef struct {
    float mat[4][4];
} Mat4x4;

typedef struct {
    float mat[2][2];
} Mat2x2;

void CreateMat4x4(Mat4x4* matrix, float diagonal);
void Translate(Mat4x4 *mat, const Vec4 *vTrans);
void Scale(Mat4x4 *mat, const Vec4 *vScale);
void VecMatrixProduct(const Mat4x4 *matL, Vec4 *vecR);
void VFOVPerspectiveProj(Mat4x4* matrix, float vertFOV, float aspectRatio, float front, float back);
void SymInfPerspectiveProj(Mat4x4* matrix, float near, float width, float height);