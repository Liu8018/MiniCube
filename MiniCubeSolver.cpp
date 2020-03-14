#include "MiniCubeSolver.h"
#include "MiniCubeFunctions.h"

#include <iostream>
#include <random>

MiniCubeSolver::MiniCubeSolver()
{
    m_hashCode = 472917414;
    
    for(int i=0;i<24;i++)
        m_hashMat[i] = std::rand()%100;
}

MiniCubeSolver::~MiniCubeSolver()
{
    
}

void MiniCubeSolver::setMaxDepth(int maxDepth)
{
    m_maxDepth = maxDepth;
}

float calcOverlapRatio(const int cubeState1[6][4], const int cubeState2[6][4])
{
    int ncorrect = 0;
    
    int *list1 = (int*)cubeState1;
    int *list2 = (int*)cubeState2;
    
    for(int i=0;i<24;i++){
        if(list1[i] == list2[i])
            ncorrect++;
    }
    
    float overlapRatio = ncorrect/float(24);
    
    return overlapRatio;
}

std::vector<int> MiniCubeSolver::getCmdList()
{
    return m_cmdList;
}

int MiniCubeSolver::getCubeStateHash(int *cube)
{
    int hash = 0;
    for(int i=0;i<24;i++)
        hash += cube[i]*m_hashMat[i];
    
    return hash;
}

bool MiniCubeSolver::solve(const int cubeState1[6][4], const int cubeState2[6][4])
{
    //若大于最大深度，则返回false
    if(m_cmdList.size() > m_maxDepth)
        return false;
    
    //从该节点往下有9个分支
    int nextStates[9][6][4];
    
    int tmpState[6][4];
    
    //正面旋转3次
    copyCubeState(cubeState1,tmpState);
    for(int i=0;i<3;i++){
        frontL(tmpState);
        copyCubeState(tmpState,nextStates[i]);
    }
    
    //侧面旋转3次
    copyCubeState(cubeState1,tmpState);
    for(int i=3;i<6;i++){
        rightL(tmpState);
        copyCubeState(tmpState,nextStates[i]);
    }
    
    //上面旋转3次
    copyCubeState(cubeState1,tmpState);
    for(int i=6;i<9;i++){
        topL(tmpState);
        copyCubeState(tmpState,nextStates[i]);
    }
    
    //计算与目标状态的重合率
    float overlapRatios[9];
    for(int i=0;i<9;i++){
        overlapRatios[i] = calcOverlapRatio(nextStates[i],cubeState2);
    }
    
    //按重合率排序
    int cmdIds[9];
    for(int i=0;i<9;i++) 
        cmdIds[i]=i;
    
    for(int i=0;i<9;i++){
        for(int j=i+1;j<9;j++){
            if(overlapRatios[j] > overlapRatios[i]){
                float tmpRatio = overlapRatios[i];
                overlapRatios[i] = overlapRatios[j];
                overlapRatios[j] = tmpRatio;
                
                int tmpId = cmdIds[i];
                cmdIds[i] = cmdIds[j];
                cmdIds[j] = tmpId;
                
                int tmpState[6][4];
                copyCubeState(nextStates[i],tmpState);
                copyCubeState(nextStates[j],nextStates[i]);
                copyCubeState(tmpState,nextStates[j]);
            }
        }
    }
    
    //若完全重合则成功求解，返回true
    if(overlapRatios[0] == 1){
        m_cmdList.push_back(cmdIds[0]);
        return true;
    }
    
    //递归，重合率高的先进行搜索
    for(int i=0;i<9;i++){
        //检查是否在黑名单内，若是就跳过
        int cubeStateHash = getCubeStateHash((int*)(nextStates[i]));
        if(m_blackListStates[cubeStateHash] == m_hashCode)
            continue;
        
        //不在黑名单内则加入黑名单并展开搜索
        m_blackListStates.insert(std::make_pair(cubeStateHash,m_hashCode));
        
        m_cmdList.push_back(cmdIds[i]);
        
        bool isSolved = solve(nextStates[i],cubeState2);
        
        //若此分支下存在解，返回true
        if(isSolved) {
            return true;
        }
        
        //若此分支没有成功求解，删除刚刚添加的指令
        m_cmdList.pop_back();
    }
    
    //若此状态下所有分支均求解失败，则返回false
    return false;
}
