#include "MiniCubeSolver.h"
#include "MiniCubeFunctions.h"

#include <iostream>
#include <random>
#include <opencv2/highgui.hpp>

MiniCubeSolver::MiniCubeSolver()
{
    m_cubeVecLen = g_cubeVecLen;
    
    m_hashCode = 472917414;
}

float MiniCubeSolver::calcOverlapRatio(const std::vector<int> &cubeState1, const std::vector<int> &cubeState2)
{
    int ncorrect = 0;
    
    for(int i=0;i<m_cubeVecLen;i++){
        if(cubeState1[i] == cubeState2[i])
            ncorrect++;
    }
    
    float overlapRatio = ncorrect/float(m_cubeVecLen);
    
    return overlapRatio;
}

std::vector<int> MiniCubeSolver::getCmdList()
{
    return m_cmdList;
}

std::string MiniCubeSolver::getCubeStateStr(const std::vector<int> &cube)
{
    std::string str = "";
    for(int i=0;i<m_cubeVecLen;i++)
        str += std::to_string(cube[i]);
    
    return str;
}

/*
bool MiniCubeSolver::breadthFirstSolve(const std::vector<int> &cubeState1, 
                                       const std::vector<int> &cubeState2, 
                                       int maxDepth, int maxNodesPerLayer,
                                       std::vector<int> &cmdPath)
{
    //初始状态集
    std::vector<std::vector<int>> states;
    states.push_back(cubeState1);
    
    //搜索次数
    int searchStep = 0;
    
    //目标状态所在层数
    int targetLayer = 0;
    
    bool isSolved = false;
    while (1) {
        //若达到最大深度，搜索次数加一，层数清零，状态置零
        if(targetLayer > maxDepth){
            searchStep ++;
            targetLayer = 0;
            
            states.clear();
            states.push_back(cubeState1);
        }
        
        std::vector<std::vector<int>> nextStates;
        
        //由states生成nextStates
        for(int i=0;i<states.size();i++){
            std::vector<std::vector<int>> tmpStates;
            genNext6States(states[i],tmpStates);
            nextStates.insert(nextStates.end(),tmpStates.begin(),tmpStates.end());
        }
        
        //排序
        std::vector<int> cmdIds(nextStates.size());
        bool overlap = sortStatesByOverlapRatio(cubeState2,nextStates,cmdIds);
        if(overlap){
            isSolved = true;
            break;
        }
        
        //选出第searchStep*maxNodesPerLayer到第(searchStep+1)*maxNodesPerLayer的状态
        int startPos = searchStep*maxNodesPerLayer;
        int endPos = (searchStep+1)*maxNodesPerLayer;
        
        if(startPos > nextStates.size()){
            startPos = 0;
            endPos = nextStates.size();
        }
        else if(endPos > nextStates.size()){
            
        }
        
        states.assign(nextStates.begin()+startPos,nextStates.begin()+endPos);
        
        targetLayer++;
    }
    
    //根据searchStep和targetLayer可以推算出整个过程的cmdPath
    
}
*/

bool MiniCubeSolver::singlePathSolve_recursion(const std::vector<int> &cubeState1, const std::vector<int> &cubeState2)
{
    if(m_blackListStates[getCubeStateStr(cubeState1)] == m_hashCode)
        return false;
    
    m_blackListStates[getCubeStateStr(cubeState1)] = m_hashCode;
    
    std::vector<std::vector<int>> nextStates;
    genNext6States(cubeState1,nextStates);
    
    //指令id列表
    std::vector<int> cmdIds(6);
    for(int i=0;i<6;i++) 
        cmdIds[i]=i;
    
    bool isSolved = sortStatesByOverlapRatio(cubeState2,nextStates,cmdIds);
    if(isSolved){
        m_cmdList.push_back(cmdIds[0]);
        return true;
    }
    
    for(int i=0;i<nextStates.size();i++){
        m_cmdList.push_back(cmdIds[i]);
        
        //对该状态进行递归
        bool isSolved = singlePathSolve_recursion(nextStates[i],cubeState2);
        
        //若此分支下存在解，返回true
        if(isSolved) {
            return true;
        }
        
        //若此分支没有成功求解，删除刚刚添加的指令
        m_cmdList.pop_back();
    }
    
    return false;
}

void MiniCubeSolver::genNext6States(const std::vector<int> &cube, std::vector<std::vector<int>> &nextStates)
{
    nextStates.resize(6);
    
    //正面向左旋转
    nextStates[0] = cube;
    frontL(nextStates[0]);
    //正面向右旋转
    nextStates[1] = cube;
    frontR(nextStates[1]);
    
    //侧面向左旋转
    nextStates[2] = cube;
    rightL(nextStates[2]);
    //侧面向右旋转
    nextStates[3] = cube;
    rightR(nextStates[3]);
    
    //上面向左旋转
    nextStates[4] = cube;
    topL(nextStates[4]);
    //上面向右旋转
    nextStates[5] = cube;
    topR(nextStates[5]);
}

bool MiniCubeSolver::sortStatesByOverlapRatio(const std::vector<int> &targetState, 
                              std::vector<std::vector<int>> &states, std::vector<int> &cmdIds)
{
    //计算与目标状态的重合率
    std::vector<float> overlapRatios(states.size());
    for(int i=0;i<overlapRatios.size();i++){
        overlapRatios[i] = calcOverlapRatio(states[i],targetState);
    }
    
    //按重合率排序
    for(int i=0;i<states.size();i++){
        for(int j=i+1;j<states.size();j++){
            if(overlapRatios[j] > overlapRatios[i]){
                float tmpRatio = overlapRatios[i];
                overlapRatios[i] = overlapRatios[j];
                overlapRatios[j] = tmpRatio;
                
                int tmpId = cmdIds[i];
                cmdIds[i] = cmdIds[j];
                cmdIds[j] = tmpId;
                
                std::vector<int> tmpState = states[i];
                states[i] = states[j];
                states[j] = tmpState;
            }
        }
    }
    
    //若完全重合则成功求解，返回true
    if(overlapRatios[0] == 1)
        return true;
    else
        return false;
}

void MiniCubeSolver::backStack()
{
    m_stateStack.pop_back();
    m_cmdIdsStack.pop_back();
    m_stateIdStack.pop_back();
    
    m_curStates = m_stateStack[m_stateStack.size()-1];
    m_curCmdIds = m_cmdIdsStack[m_cmdIdsStack.size()-1];
    m_curStateId = m_stateIdStack[m_stateIdStack.size()-1];
}
void MiniCubeSolver::pushStack()
{
    m_stateStack.push_back(m_curStates);
    m_cmdIdsStack.push_back(m_curCmdIds);
    m_stateIdStack.push_back(m_curStateId);
}

bool MiniCubeSolver::singlePathSolve(const std::vector<int> &cubeState1, const std::vector<int> &cubeState2)
{
    m_blackListStates.clear();
    
    //初始状态
    m_curStates.push_back(cubeState1);
    m_curCmdIds.push_back(-1);
    m_curStateId = 0;
    pushStack();
    
    int maxDepth = 999999;
    bool isSolved = false;
    
    while(maxDepth--){
        if(m_blackListStates[getCubeStateStr(m_curStates[m_curStateId])] == m_hashCode){
            if(m_curStateId < m_curStates.size()-1){
                m_curStateId++;
                m_stateIdStack[m_stateIdStack.size()-1]++;
            }
            else{
                do{backStack();}
                while(m_curStateId == m_curStates.size()-1);
                backStack();
                m_curStateId++;
                m_stateIdStack[m_stateIdStack.size()-1]++;
            }
            
            continue;
        }
        
        m_blackListStates[getCubeStateStr(m_curStates[m_curStateId])] = m_hashCode;
        
        std::vector<std::vector<int>> nextStates;
        genNext6States(m_curStates[m_curStateId],nextStates);
        
        //指令id列表
        std::vector<int> cmdIds(6);
        for(int i=0;i<6;i++) 
            cmdIds[i]=i;
        
        bool overlap = sortStatesByOverlapRatio(cubeState2,nextStates,cmdIds);
        
        m_curStates = nextStates;
        m_curCmdIds = cmdIds;
        m_curStateId = 0;
        pushStack();
        
        if(overlap){
            isSolved = true;
            break;
        }
    }
    
    for(int i=0;i<m_stateIdStack.size();i++){
        //3个相同方向变为一个反方向
        if(m_cmdList.size() >= 2){
            int a = m_cmdList[m_cmdList.size()-2];
            int b = m_cmdList[m_cmdList.size()-1];
            int c = m_cmdIdsStack[i][m_stateIdStack[i]];
            if(a==b && b==c){
                m_cmdList.pop_back();
                m_cmdList.pop_back();
                
                if(a==0) m_cmdList.push_back(1);
                else if(a==1) m_cmdList.push_back(0);
                else if(a==2) m_cmdList.push_back(3);
                else if(a==3) m_cmdList.push_back(2);
                else if(a==4) m_cmdList.push_back(5);
                else if(a==5) m_cmdList.push_back(4);
                
                continue;
            }
        }
        
        m_cmdList.push_back(m_cmdIdsStack[i][m_stateIdStack[i]]);
    }
    
    return isSolved;
}

void MiniCubeSolver::optimizePath(const std::vector<std::vector<int>> &statePath, 
                                  std::vector<int> &cmdPath)
{
    
}
