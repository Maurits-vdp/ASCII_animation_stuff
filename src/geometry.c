#include "geometry.h"
#include <math.h>
#include <stdio.h>

#define PI 3.14159265359
#define FOV 90

//Subtracts v2 components from v1 and returns the result
Vec3 SubtractVec3s(Vec3* v1, Vec3* v2){ //Generalise?
    Vec3 vRes = {.vec = {0,0,0}};
    for (int i=0; i<3; i++){
        vRes.vec[i] = v1->vec[i] - v2->vec[i];
    }
    return vRes;
}

Vec3 CrossProductVec3(Vec3* vL, Vec3* vR){
    Vec3 vOut = {.vec = 
        {   vL->vec[1]*vR->vec[2] - vL->vec[2]*vR->vec[1],
            vL->vec[2]*vR->vec[0] - vL->vec[0]*vR->vec[2],
            vL->vec[0]*vR->vec[1] - vL->vec[1]*vR->vec[0]
        }
    };
    return vOut;
}

void NormaliseVec3(Vec3* vector){//Probably a better way to do this
    float magnitude = 0;
    for (int i=0; i<3; i++){
        magnitude += vector->vec[i]*vector->vec[i];
    }
    magnitude = sqrt(magnitude);
    for (int i=0; i<3; i++){
        vector->vec[i] /= magnitude;
    }
}

void CreateMat4x4(Mat4x4* matrix, float diagonal){
    for (int row=0; row < 4; row++){
        for (int col=0; col < 4; col++){
            if (col == row){
                matrix->mat[row][col] = diagonal;
                continue;   
            }
            matrix->mat[row][col] = 0.0f;
        }
    }
}

Mat4x4 CreateAndReturnMat4x4(float diagonal){
    Mat4x4 matrix;
    for (int row=0; row < 4; row++){
        for (int col=0; col < 4; col++){
            if (col == row){
                matrix.mat[row][col] = diagonal;
                continue;   
            }
            matrix.mat[row][col] = 0.0f;
        }
    }
    return matrix;
}

void Multiply4x4Matrices(const Mat4x4 *matL, Mat4x4 *matR){
    Mat4x4 matOut;
    for (size_t row=0; row < 4; row++){
        for (size_t col=0; col<4; col++){
            for (size_t i=0; i<4; i++){
                matOut.mat[row][col] += matL->mat[row][i]*matR->mat[i][col];
            }
        }
    }
    *matR = matOut;
}

Mat4x4 Multiply4x4MatricesAndReturnMat(Mat4x4* matL, Mat4x4* matR){
    Mat4x4 matOut;
    CreateMat4x4(&matOut, 0.0);
    for (size_t row=0; row < 4; row++){
        for (size_t col=0; col<4; col++){
            for (size_t i=0; i<4; i++){
                matOut.mat[row][col] += matL->mat[row][i]*matR->mat[i][col];
            }
        }
    }
    return matOut;
}

void VecMatrixProduct(const Mat4x4 *matL, Vec4 *vecR){
    Vec4 vecOut = {.vec = {0,0,0,0}};
    for (size_t i=0; i < 4; i++){
        for (size_t j=0; j<4; j++){
            vecOut.vec[i] += matL->mat[i][j]*vecR->vec[j];
        }
    }
    for (int i=0; i<4; i++){
        vecR->vec[i] = vecOut.vec[i];
    }
}

void CreateTranslationMatrix(Mat4x4 *transMat, const Vec4 *vTrans){
    transMat->mat[0][3] = vTrans->vec[0];
    transMat->mat[1][3] = vTrans->vec[1];
    transMat->mat[2][3] = vTrans->vec[2];
}

void CreateScaleMatrix(Mat4x4 *mat, const Vec3 *vScale){ //Check if this needs to be updated
    Mat4x4 scaleMat;
    CreateMat4x4(&scaleMat, 1.0f);
    for (size_t i=0; i<3; i++){
        scaleMat.mat[i][i]=vScale->vec[i];
    }
    Multiply4x4Matrices(&scaleMat, mat);
}

//All rotations for counter clockwise
void CreateRotationMatrix(Mat4x4* matrix, float theta, const Vec4* axis){ //Check if this needs to be updated
    float c = cosf(theta); float s = sinf(theta);
    float rxx = axis->vec[0]*axis->vec[0]; float rxy = axis->vec[0]*axis->vec[1]; float rxz = axis->vec[0]*axis->vec[2];
    float ryy = axis->vec[1]*axis->vec[1]; float ryz = axis->vec[1]*axis->vec[2]; float rzz = axis->vec[2]*axis->vec[2];

    Mat4x4 rMat = {
        .mat = {
        {c + rxx*(1-c), rxy*(1-c)-axis->vec[2]*s, rxz*(1-c)+axis->vec[1]*s, 0.0f},
        
        {rxy*(1-c)+axis->vec[2]*s, c + ryy*(1-c), ryz*(1-c)-axis->vec[0]*s, 0.0f},

        {rxz*(1-c)-axis->vec[1]*s, ryz*(1-c)+axis->vec[0]*s, c + rzz*(1-c), 0.0f},

        {0.0f, 0.0f, 0.0f, 1.0f}
        }
    };

    Multiply4x4Matrices(&rMat, matrix); //is passing &rMat possible?
}

void OrthoProj(Mat4x4* matrix, float xMax, float xMin, float yMax, float yMin, float zMax, float zMin){
    matrix->mat[0][0]=2/(xMax-xMin); //right-left
    matrix->mat[1][1]=2/(yMax-yMin); //top-bottom
    matrix->mat[2][2]=2/(zMax-zMin); //far-near
    matrix->mat[0][3]=-(xMax+xMin)/(xMax-xMin);
    matrix->mat[1][3]=-(yMax+yMin)/(yMax-yMin);
    matrix->mat[2][3]=-(zMax+zMin)/(zMax-zMin);
}

//https://johannesugb.github.io/gpu-programming/setting-up-a-proper-vulkan-projection-matrix/
//https://www.songho.ca/opengl/gl_projectionmatrix.html
void VFOVPerspectiveProj(Mat4x4* matrix, float vertFOV, float aspectRatio, float near, float far){
    float tanTheta = tan(vertFOV/2);
    float top = near*tanTheta;
    float right = top * aspectRatio;

    CreateMat4x4(matrix, 0);
    matrix->mat[0][0] = near/right;
    matrix->mat[1][1] = near/top;
    matrix->mat[2][2] = -(far+near)/(far-near);
    matrix->mat[2][3] = -(2*far*near)/(far-near);
    matrix->mat[3][2] = -1;
    matrix->mat[3][3] = 0;
};

void ConvertClipToNDC(Vec4* vector){
    if (vector->vec[3] != 0.0f){
        vector->vec[0] /= vector->vec[3];
        vector->vec[1] /= vector->vec[3];
        vector->vec[2] /= vector->vec[3];
    }
}

void ViewportTransform(Vec4* vecNDC, float width, float height){
    vecNDC->vec[0] = (vecNDC->vec[0]+1)*0.5*width;
    vecNDC->vec[1] = (1-vecNDC->vec[1])*0.5*height;
    vecNDC->vec[2] = (vecNDC->vec[2]+1)*0.5;
}

void SymInfPerspectiveProj(Mat4x4* matrix, float near, float width, float height){
    float r = width/2;
    float t = height/2;

    CreateMat4x4(matrix, 0);
    matrix->mat[0][0] = near/r;
    matrix->mat[1][1] = near/t;
    matrix->mat[2][2] = -1;
    matrix->mat[2][3] = -2*near;
    matrix->mat[3][2] = -1;
    matrix->mat[0][0] = 0;
};

Mat4x4 LookAtMatrix(Vec3* position, Vec3* target, Vec3* upVec){
    Vec3 viewDir = SubtractVec3s(target, position); //Note direction matters
    NormaliseVec3(&viewDir);

    Vec3 viewRight = CrossProductVec3(&viewDir, upVec);
    NormaliseVec3(&viewRight);

    Vec3 viewUp = CrossProductVec3(&viewRight, &viewDir);

    Mat4x4 matRight = CreateAndReturnMat4x4(1.0f);
    matRight.mat[0][3] = -position->vec[0];
    matRight.mat[1][3] = -position->vec[1];
    matRight.mat[2][3] = -position->vec[2];

    Mat4x4 matLeft = CreateAndReturnMat4x4(1.0f);
    matLeft.mat[0][0] = viewRight.vec[0];
    matLeft.mat[0][1] = viewRight.vec[1];
    matLeft.mat[0][2] = viewRight.vec[2];

    matLeft.mat[1][0] = viewUp.vec[0];
    matLeft.mat[1][1] = viewUp.vec[1];
    matLeft.mat[1][2] = viewUp.vec[2];

    matLeft.mat[2][0] = viewDir.vec[0];
    matLeft.mat[2][1] = viewDir.vec[1];
    matLeft.mat[2][2] = viewDir.vec[2];

    //Change to not multiplying but just numerically calculating the matrix instead?
    return Multiply4x4MatricesAndReturnMat(&matLeft, &matRight); 
}