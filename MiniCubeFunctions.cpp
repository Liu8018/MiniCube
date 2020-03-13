#include "MiniCubeFunctions.h"

void copyCubeState(const int * cube1, int * cube2)
{
    for(int i=0;i<8;i++){
        cube2[i] = cube1[i];
    }
}

void frontL(int * cube)
{
    int newCubeState[8];
    copyCubeState(cube,newCubeState);
    
    newCubeState[0] = cube[1];
    newCubeState[1] = cube[5];
    newCubeState[5] = cube[4];
    newCubeState[4] = cube[0];
    
    copyCubeState(newCubeState,cube);
}
void frontR(int * cube)
{
    int newCubeState[8];
    copyCubeState(cube,newCubeState);
    
    newCubeState[0] = cube[4];
    newCubeState[4] = cube[5];
    newCubeState[5] = cube[1];
    newCubeState[1] = cube[0];
    
    copyCubeState(newCubeState,cube);
}
void rightL(int * cube)
{
    int newCubeState[8];
    copyCubeState(cube,newCubeState);
    
    newCubeState[1] = cube[2];
    newCubeState[2] = cube[6];
    newCubeState[6] = cube[5];
    newCubeState[5] = cube[1];
    
    copyCubeState(newCubeState,cube);
}
void rightR(int * cube)
{
    int newCubeState[8];
    copyCubeState(cube,newCubeState);
    
    newCubeState[1] = cube[5];
    newCubeState[5] = cube[6];
    newCubeState[6] = cube[2];
    newCubeState[2] = cube[1];
    
    copyCubeState(newCubeState,cube);
}
void topL(int * cube)
{
    int newCubeState[8];
    copyCubeState(cube,newCubeState);
    
    newCubeState[0] = cube[1];
    newCubeState[1] = cube[2];
    newCubeState[2] = cube[3];
    newCubeState[3] = cube[0];
    
    copyCubeState(newCubeState,cube);
}
void topR(int * cube)
{
    int newCubeState[8];
    copyCubeState(cube,newCubeState);
    
    newCubeState[0] = cube[3];
    newCubeState[3] = cube[2];
    newCubeState[2] = cube[1];
    newCubeState[1] = cube[0];
    
    copyCubeState(newCubeState,cube);
}
