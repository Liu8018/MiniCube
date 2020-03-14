#include "MiniCubeFunctions.h"

void copyCubeState(const int cube1[6][4], int cube2[6][4])
{
    int *list1 = (int*)cube1;
    int *list2 = (int*)cube2;
    
    for(int i=0;i<24;i++){
        list2[i] = list1[i];
    }
}

void halfBlockL(int cubeFace[6][4], int sideIdList[5], int idList[12])
{
    int s0=sideIdList[0], 
        s1=sideIdList[1], s2=sideIdList[2],
        s3=sideIdList[3], s4=sideIdList[4];
    
    int i0=idList[0], i1=idList[1], i2=idList[2],
        i3=idList[3], i4=idList[4], i5=idList[5],
        i6=idList[6], i7=idList[7], i8=idList[8],
        i9=idList[9], i10=idList[10], i11=idList[11];
    
    int tmpv1 = cubeFace[s0][i3];
    cubeFace[s0][i3] = cubeFace[s0][i4];
    cubeFace[s0][i4] = cubeFace[s0][i8];
    cubeFace[s0][i8] = cubeFace[s0][i7];
    cubeFace[s0][i7] = tmpv1;
    
    tmpv1 = cubeFace[s1][i0];
    int tmpv2 = cubeFace[s1][i1];
    cubeFace[s1][i0] = cubeFace[s4][i5];
    cubeFace[s1][i1] = cubeFace[s4][i9];
    cubeFace[s4][i5] = cubeFace[s3][i11];
    cubeFace[s4][i9] = cubeFace[s3][i10];
    cubeFace[s3][i11] = cubeFace[s2][i6];
    cubeFace[s3][i10] = cubeFace[s2][i2];
    cubeFace[s2][i6] = tmpv1;
    cubeFace[s2][i2] = tmpv2;
}
void halfBlockR(int cubeFace[6][4], int sideIdList[5], int idList[12])
{
    int s0=sideIdList[0], 
        s1=sideIdList[1], s2=sideIdList[2],
        s3=sideIdList[3], s4=sideIdList[4];
    
    int i0=idList[0], i1=idList[1], i2=idList[2],
        i3=idList[3], i4=idList[4], i5=idList[5],
        i6=idList[6], i7=idList[7], i8=idList[8],
        i9=idList[9], i10=idList[10], i11=idList[11];
    
    int tmpv1 = cubeFace[s0][i3];
    cubeFace[s0][i3] = cubeFace[s0][i7];
    cubeFace[s0][i7] = cubeFace[s0][i8];
    cubeFace[s0][i8] = cubeFace[s0][i4];
    cubeFace[s0][i4] = tmpv1;
    
    tmpv1 = cubeFace[s1][i0];
    int tmpv2 = cubeFace[s1][i1];
    cubeFace[s1][i0] = cubeFace[s2][i6];
    cubeFace[s1][i1] = cubeFace[s2][i2];
    cubeFace[s2][i6] = cubeFace[s3][i11];
    cubeFace[s2][i2] = cubeFace[s3][i10];
    cubeFace[s3][i11] = cubeFace[s4][i5];
    cubeFace[s3][i10] = cubeFace[s4][i9];
    cubeFace[s4][i5] = tmpv1;
    cubeFace[s4][i9] = tmpv2;
}

void frontL(int cube[6][4])
{
    int sideIdList[5] = {0,5,3,4,1};
    int idList[12] = {2,3,0,0,1,0,2,2,3,2,2,3};
    halfBlockL(cube,sideIdList,idList);
}
void frontR(int cube[6][4])
{
    int sideIdList[5] = {0,5,3,4,1};
    int idList[12] = {2,3,0,0,1,0,2,2,3,2,2,3};
    halfBlockR(cube,sideIdList,idList);
}
void rightL(int cube[6][4])
{
    int sideIdList[5] = {1,5,0,4,2};
    int idList[12] = {3,1,1,0,1,1,3,2,3,3,3,1};
    halfBlockL(cube,sideIdList,idList);
}
void rightR(int cube[6][4])
{
    int sideIdList[5] = {1,5,0,4,2};
    int idList[12] = {3,1,1,0,1,1,3,2,3,3,3,1};
    halfBlockR(cube,sideIdList,idList);
}
void topL(int cube[6][4])
{
    int sideIdList[5] = {5,2,3,0,1};
    int idList[12] = {0,1,1,0,1,1,0,2,3,0,0,1};
    halfBlockL(cube,sideIdList,idList);
}
void topR(int cube[6][4])
{
    int sideIdList[5] = {5,2,3,0,1};
    int idList[12] = {0,1,1,0,1,1,0,2,3,0,0,1};
    halfBlockR(cube,sideIdList,idList);
}
