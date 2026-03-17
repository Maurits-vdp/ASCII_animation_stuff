#include "geometry.h"
#include <math.h>

#define PI 3.14159265359
#define FOV 90

void CreateMat4x4(Mat4x4* matrix, float diagonal){
    for (int row; row < 4; row++){
        for (int col; col < 4; col++){
            if (col == row){
                matrix->mat[row][col] = diagonal;
                continue;   
            }
            matrix->mat[row][col] = 0.0f;
        }
    }
}

void Multiply4x4Matrices(const Mat4x4 *matL, Mat4x4 *matR){
    Mat4x4 matOut;
    for (size_t row=0; row < 4; row++){
        for (size_t col=0; col<4; col++){
            for (size_t i=0; i<4; i++){
                matOut.mat[row][col] += matL->mat[row + i][col]*matR->mat[row][col + i];
            }
        }
    }
    *matR = matOut;
}

void VecMatrixProduct(const Mat4x4 *matL, Vec4 *vecR){
    Vec4 vecOut;
    for (size_t i=0; i < 4; i++){
        for (size_t j=0; j<4; j++){
            vecOut.vec[i] += matL->mat[i][j]*vecR->vec[j];
        }
    }
    *vecR = vecOut;
}

void Translate(Mat4x4 *mat, const Vec4 *vTrans){
    Mat4x4 transMat; 
    CreateMat4x4(&transMat, 1.0f);
    for (size_t i=0; i<4; i++){
        transMat.mat[i][3]=vTrans->vec[i];
    }
    Multiply4x4Matrices(&transMat, mat);
}

void Scale(Mat4x4 *mat, const Vec4 *vScale){
    Mat4x4 scaleMat;
    CreateMat4x4(&scaleMat, 1.0f);
    for (size_t i=0; i<4; i++){
        scaleMat.mat[i][i]=vScale->vec[i];
    }
    Multiply4x4Matrices(&scaleMat, mat);
}

//All rotations for counter clockwise
void Rotate(Mat4x4* matrix, float theta, const Vec4* axis){
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
void VFOVPerspectiveProj(Mat4x4* matrix, float vertFOV, float aspectRatio, float front, float back){
    float tanTheta = tan(vertFOV/2);
    float top = front*tanTheta;
    float right = top * aspectRatio;

    CreateMat4x4(matrix, 0);
    matrix->mat[0][0] = front/right;
    matrix->mat[1][1] = front/top;
    matrix->mat[2][2] = -(back+front)/(back-front);
    matrix->mat[2][3] = -1;
    matrix->mat[3][2] = -(2*back*front)/(back-front);
    matrix->mat[3][3] = 0;
};

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
}