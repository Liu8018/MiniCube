#ifndef CUBEFUNCTIONS_H
#define CUBEFUNCTIONS_H

//复制魔方状态
void copyCubeState(const int * cube1, int * cube2);

//翻转魔方
void frontL(int * cube);
void frontR(int * cube);
void rightL(int * cube);
void rightR(int * cube);
void topL(int * cube);
void topR(int * cube);

#endif // CUBEFUNCTIONS_H
