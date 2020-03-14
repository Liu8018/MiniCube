#ifndef CUBEFUNCTIONS_H
#define CUBEFUNCTIONS_H

#include <vector>

//复制魔方状态
void copyCubeState(const int cube1[6][4], int cube2[6][4]);

//翻转魔方
void frontL(int cube[6][4]);
void frontR(int cube[6][4]);
void rightL(int cube[6][4]);
void rightR(int cube[6][4]);
void topL(int cube[6][4]);
void topR(int cube[6][4]);

#endif // CUBEFUNCTIONS_H
