#include "MiniCubeFunctions.h"

int g_cubeVecLen = 14;

void rotateVec(int &v, int constV)
{
    if(v == constV)
        return;
    
    if(constV == 1){
        v = v==2?3:2;
    }
    else if(constV == 2){
        v = v==1?3:1;
    }
    else{
        v = v==1?2:1;
    }
}

void frontL(std::vector<int> &cube)
{
    int tmp = cube[0];
    cube[0] = cube[1];
    cube[1] = cube[5];
    cube[5] = cube[4];
    cube[4] = tmp;
    
    rotateVec(cube[7+0],1);
    rotateVec(cube[7+1],1);
    rotateVec(cube[7+5],1);
    rotateVec(cube[7+4],1);
    
    tmp = cube[7+0];
    cube[7+0] = cube[7+1];
    cube[7+1] = cube[7+5];
    cube[7+5] = cube[7+4];
    cube[7+4] = tmp;
}
void frontR(std::vector<int> &cube)
{
    int tmp = cube[0];
    cube[0] = cube[4];
    cube[4] = cube[5];
    cube[5] = cube[1];
    cube[1] = tmp;
    
    rotateVec(cube[7+0],1);
    rotateVec(cube[7+1],1);
    rotateVec(cube[7+5],1);
    rotateVec(cube[7+4],1);
    
    tmp = cube[7+0];
    cube[7+0] = cube[7+4];
    cube[7+4] = cube[7+5];
    cube[7+5] = cube[7+1];
    cube[7+1] = tmp;
}
void rightL(std::vector<int> &cube)
{
    int tmp = cube[1];
    cube[1] = cube[2];
    cube[2] = cube[6];
    cube[6] = cube[5];
    cube[5] = tmp;
    
    rotateVec(cube[7+1],2);
    rotateVec(cube[7+2],2);
    rotateVec(cube[7+6],2);
    rotateVec(cube[7+5],2);
    
    tmp = cube[7+1];
    cube[7+1] = cube[7+2];
    cube[7+2] = cube[7+6];
    cube[7+6] = cube[7+5];
    cube[7+5] = tmp;
}
void rightR(std::vector<int> &cube)
{
    int tmp = cube[1];
    cube[1] = cube[5];
    cube[5] = cube[6];
    cube[6] = cube[2];
    cube[2] = tmp;
    
    rotateVec(cube[7+1],2);
    rotateVec(cube[7+2],2);
    rotateVec(cube[7+6],2);
    rotateVec(cube[7+5],2);
    
    tmp = cube[7+1];
    cube[7+1] = cube[7+5];
    cube[7+5] = cube[7+6];
    cube[7+6] = cube[7+2];
    cube[7+2] = tmp;
}

void topL(std::vector<int> &cube)
{
    int tmp = cube[0];
    cube[0] = cube[3];
    cube[3] = cube[2];
    cube[2] = cube[1];
    cube[1] = tmp;
    
    rotateVec(cube[7+0],3);
    rotateVec(cube[7+3],3);
    rotateVec(cube[7+2],3);
    rotateVec(cube[7+1],3);
    
    tmp = cube[7+0];
    cube[7+0] = cube[7+3];
    cube[7+3] = cube[7+2];
    cube[7+2] = cube[7+1];
    cube[7+1] = tmp;
}
void topR(std::vector<int> &cube)
{
    int tmp = cube[0];
    cube[0] = cube[1];
    cube[1] = cube[2];
    cube[2] = cube[3];
    cube[3] = tmp;
    
    rotateVec(cube[7+0],3);
    rotateVec(cube[7+3],3);
    rotateVec(cube[7+2],3);
    rotateVec(cube[7+1],3);
    
    tmp = cube[7+0];
    cube[7+0] = cube[7+1];
    cube[7+1] = cube[7+2];
    cube[7+2] = cube[7+3];
    cube[7+3] = tmp;
}
