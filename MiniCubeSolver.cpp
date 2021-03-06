#include "MiniCubeSolver.h"
#include "MiniCubeFunctions.h"

#include <iostream>
#include <random>
#include <future>
#include <opencv2/highgui.hpp>

MiniCubeSolver::MiniCubeSolver()
{
    m_cubeVecLen = g_cubeVecLen;
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

bool MiniCubeSolver::depthFirstSolve_recursion(const std::vector<int> &cubeState1, 
                                               const std::vector<int> &cubeState2, 
                                               int maxDepth)
{
    if(m_blackListStates[getCubeStateStr(cubeState1)] == true)
        return false;
    
    if(m_cmdList.size() > maxDepth)
        return false;
    
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
    
    m_blackListStates[getCubeStateStr(cubeState1)] = true;
    
    for(int i=0;i<nextStates.size();i++){
        m_cmdList.push_back(cmdIds[i]);
        
        //对该状态进行递归
        bool isSolved = depthFirstSolve_recursion(nextStates[i],cubeState2,maxDepth);
        
        //若此分支下存在解，返回true
        if(isSolved) {
            return true;
        }
        
        //若此分支没有成功求解，删除刚刚添加的指令
        m_cmdList.pop_back();
    }
    
    m_blackListStates[getCubeStateStr(cubeState1)] = 0;
    
    return false;
}

void pushStack_thread(std::vector<std::vector<int>> &curStates,
                      std::vector<int> &curCmdIds,
                      int &curStateId,
                      std::vector<std::vector<std::vector<int>>> &stateStack,
                      std::vector<std::vector<int>> &cmdIdsStack,
                      std::vector<int> &stateIdStack)
{
    stateStack.push_back(curStates);
    cmdIdsStack.push_back(curCmdIds);
    stateIdStack.push_back(curStateId);
}

void backStack_thread(std::vector<std::vector<int>> &curStates,
                      std::vector<int> &curCmdIds,
                      int &curStateId,
                      std::vector<std::vector<std::vector<int>>> &stateStack,
                      std::vector<std::vector<int>> &cmdIdsStack,
                      std::vector<int> &stateIdStack)
{
    stateStack.pop_back();
    cmdIdsStack.pop_back();
    stateIdStack.pop_back();
    
    curStates = stateStack[stateStack.size()-1];
    curCmdIds = cmdIdsStack[cmdIdsStack.size()-1];
    curStateId = stateIdStack[stateIdStack.size()-1];
}

bool MiniCubeSolver::depthFirstSolve_thread(const std::vector<int> &cubeState1, 
                                            const std::vector<int> &cubeState2, 
                                            int maxDepth,
                                            std::map<std::string,bool> &blackList,
                                            std::vector<int> &cmdPath)
{
    std::vector<std::vector<int>> curStates;
    std::vector<int> curCmdIds;
    int curStateId;
    std::vector<std::vector<std::vector<int>>> stateStack;
    std::vector<std::vector<int>> cmdIdsStack;
    std::vector<int> stateIdStack;
    curStates.push_back(cubeState1);
    curCmdIds.push_back(-1);
    curStateId = 0;
    pushStack_thread(curStates,curCmdIds,curStateId,stateStack,cmdIdsStack,stateIdStack);
    
    bool isSolved = false;
    
    while(1){
        m_mutexLock.lock();
        bool isSolved_multiThread = m_isSolved_multiThread;
        m_mutexLock.unlock();
        if(isSolved_multiThread)
            return false;
        
        if(stateStack.size() > maxDepth){
            do{
                backStack_thread(curStates,curCmdIds,curStateId,stateStack,cmdIdsStack,stateIdStack);
                blackList[getCubeStateStr(curStates[curStateId])] = 0;
            }while(curStateId == curStates.size()-1);
            curStateId++;
            stateIdStack[stateIdStack.size()-1]++;
        }
        
        if(blackList[getCubeStateStr(curStates[curStateId])] == true){
            if(curStateId < curStates.size()-1){
                curStateId++;
                stateIdStack[stateIdStack.size()-1]++;
            }
            else{
                do{
                    backStack_thread(curStates,curCmdIds,curStateId,stateStack,cmdIdsStack,stateIdStack);
                    blackList[getCubeStateStr(curStates[curStateId])] = 0;
                    if(stateStack.empty())
                        return false;
                }while(curStateId == curStates.size()-1);
                curStateId++;
                stateIdStack[stateIdStack.size()-1]++;
            }
            
            continue;
        }
        
        blackList[getCubeStateStr(curStates[curStateId])] = true;
        
        std::vector<std::vector<int>> nextStates;
        genNext6States(curStates[curStateId],nextStates);
        
        //指令id列表
        std::vector<int> cmdIds(6);
        for(int i=0;i<6;i++) 
            cmdIds[i]=i;
        
        bool overlap = sortStatesByOverlapRatio(cubeState2,nextStates,cmdIds);
        
        curStates = nextStates;
        curCmdIds = cmdIds;
        curStateId = 0;
        pushStack_thread(curStates,curCmdIds,curStateId,stateStack,cmdIdsStack,stateIdStack);
        
        if(overlap){
            isSolved = true;
            m_mutexLock.lock();
            m_isSolved_multiThread = true;
            m_mutexLock.unlock();
            break;
        }
    }
    
    if(isSolved){
        for(int i=0;i<stateIdStack.size();i++){
            //3个相同方向变为一个反方向
            if(cmdPath.size() >= 2){
                int a = cmdPath[cmdPath.size()-2];
                int b = cmdPath[cmdPath.size()-1];
                int c = cmdIdsStack[i][stateIdStack[i]];
                if(a==b && b==c){
                    cmdPath.pop_back();
                    cmdPath.pop_back();
                    
                    if(a==0) cmdPath.push_back(1);
                    else if(a==1) cmdPath.push_back(0);
                    else if(a==2) cmdPath.push_back(3);
                    else if(a==3) cmdPath.push_back(2);
                    else if(a==4) cmdPath.push_back(5);
                    else if(a==5) cmdPath.push_back(4);
                    
                    continue;
                }
            }
            cmdPath.push_back(cmdIdsStack[i][stateIdStack[i]]);
        }
    }
    
    return isSolved;
} 

bool MiniCubeSolver::depthFirstSolve_multiThread(const std::vector<int> &cubeState1, 
                                                 const std::vector<int> &cubeState2, int maxDepth)
{
    //由cubeState1生成下一级的6种状态
    std::vector<std::vector<int>> nextStates;
    genNext6States(cubeState1,nextStates);
    
    std::vector<int> cmdIds(6);
    for(int i=0;i<6;i++) 
        cmdIds[i]=i;
    
    bool isSolved = sortStatesByOverlapRatio(cubeState2,nextStates,cmdIds);
    if(isSolved){
        m_cmdList.push_back(cmdIds[0]);
        return true;
    }
    
    m_blackListStates[getCubeStateStr(cubeState1)] = true;
    
    //开6线程同时从6个状态出发进行搜索
    maxDepth--;
    m_isSolved_multiThread = false;
    
    {
        std::future<void> ft0 = async(std::launch::async, [&]{
            std::map<std::string,bool> blackList;
            std::vector<int> cmdPath;
            bool isSolved = depthFirstSolve_thread(nextStates[0],cubeState2,maxDepth,blackList,cmdPath);
            if(isSolved){
                m_mutexLock.lock();
                m_cmdList.clear();
                m_cmdList.push_back(cmdIds[0]);
                m_cmdList.insert(m_cmdList.end(),cmdPath.begin(),cmdPath.end());
                m_mutexLock.unlock();
            }
        });
        std::future<void> ft1 = async(std::launch::async, [&]{
            std::map<std::string,bool> blackList;
            std::vector<int> cmdPath;
            bool isSolved = depthFirstSolve_thread(nextStates[1],cubeState2,maxDepth,blackList,cmdPath);
            if(isSolved){
                m_mutexLock.lock();
                m_cmdList.clear();
                m_cmdList.push_back(cmdIds[1]);
                m_cmdList.insert(m_cmdList.end(),cmdPath.begin(),cmdPath.end());
                m_mutexLock.unlock();
            }
        });
        std::future<void> ft2 = async(std::launch::async, [&]{
            std::map<std::string,bool> blackList;
            std::vector<int> cmdPath;
            bool isSolved = depthFirstSolve_thread(nextStates[2],cubeState2,maxDepth,blackList,cmdPath);
            if(isSolved){
                m_mutexLock.lock();
                m_cmdList.clear();
                m_cmdList.push_back(cmdIds[2]);
                m_cmdList.insert(m_cmdList.end(),cmdPath.begin(),cmdPath.end());
                m_mutexLock.unlock();
            }
        });
        std::future<void> ft3 = async(std::launch::async, [&]{
            std::map<std::string,bool> blackList;
            std::vector<int> cmdPath;
            bool isSolved = depthFirstSolve_thread(nextStates[3],cubeState2,maxDepth,blackList,cmdPath);
            if(isSolved){
                m_mutexLock.lock();
                m_cmdList.clear();
                m_cmdList.push_back(cmdIds[3]);
                m_cmdList.insert(m_cmdList.end(),cmdPath.begin(),cmdPath.end());
                m_mutexLock.unlock();
            }
        });
        std::future<void> ft4 = async(std::launch::async, [&]{
            std::map<std::string,bool> blackList;
            std::vector<int> cmdPath;
            bool isSolved = depthFirstSolve_thread(nextStates[4],cubeState2,maxDepth,blackList,cmdPath);
            if(isSolved){
                m_mutexLock.lock();
                m_cmdList.clear();
                m_cmdList.push_back(cmdIds[4]);
                m_cmdList.insert(m_cmdList.end(),cmdPath.begin(),cmdPath.end());
                m_mutexLock.unlock();
            }
        });
        std::future<void> ft5 = async(std::launch::async, [&]{
            std::map<std::string,bool> blackList;
            std::vector<int> cmdPath;
            bool isSolved = depthFirstSolve_thread(nextStates[5],cubeState2,maxDepth,blackList,cmdPath);
            if(isSolved){
                m_mutexLock.lock();
                m_cmdList.clear();
                m_cmdList.push_back(cmdIds[5]);
                m_cmdList.insert(m_cmdList.end(),cmdPath.begin(),cmdPath.end());
                m_mutexLock.unlock();
            }
        });
    }
    
    if(m_isSolved_multiThread)
        return true;
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
        if(m_blackListStates[getCubeStateStr(m_curStates[m_curStateId])] == true){
            if(m_curStateId < m_curStates.size()-1){
                m_curStateId++;
                m_stateIdStack[m_stateIdStack.size()-1]++;
            }
            else{
                do{backStack();}
                while(m_curStateId == m_curStates.size()-1);
                m_curStateId++;
                m_stateIdStack[m_stateIdStack.size()-1]++;
            }
            
            continue;
        }
        
        m_blackListStates[getCubeStateStr(m_curStates[m_curStateId])] = true;
        
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
    
    //状态路径简化
    std::vector<std::vector<int>> statePath;
    for(int i=0;i<m_stateIdStack.size();i++){
        //3个相同方向变为一个反方向
        if(m_cmdList.size() >= 2){
            int a = m_cmdList[m_cmdList.size()-2];
            int b = m_cmdList[m_cmdList.size()-1];
            int c = m_cmdIdsStack[i][m_stateIdStack[i]];
            if(a==b && b==c){
                m_cmdList.pop_back();
                m_cmdList.pop_back();
                statePath.pop_back();
                statePath.pop_back();
                
                if(a==0) m_cmdList.push_back(1);
                else if(a==1) m_cmdList.push_back(0);
                else if(a==2) m_cmdList.push_back(3);
                else if(a==3) m_cmdList.push_back(2);
                else if(a==4) m_cmdList.push_back(5);
                else if(a==5) m_cmdList.push_back(4);
                
                statePath.push_back(m_stateStack[i][m_stateIdStack[i]]);
                
                continue;
            }
        }
        
        m_cmdList.push_back(m_cmdIdsStack[i][m_stateIdStack[i]]);
        statePath.push_back(m_stateStack[i][m_stateIdStack[i]]);
    }
    
    //test
    //for(int i=0;i<statePath.size();i++){
    //    for(int j=0;j<statePath[i].size();j++)
    //        std::cout<<statePath[i][j]<<",";
    //    std::cout<<"\n";
    //}
    
    //std::cout<<"before:"<<m_cmdList.size()<<std::endl;
    //optimizePath(statePath,m_cmdList);
    //std::cout<<"after:"<<m_cmdList.size()<<std::endl;
    
    return isSolved;
}

void MiniCubeSolver::optimizePath(std::vector<std::vector<int>> &statePath, 
                                  std::vector<int> &cmdPath)
{
    int tryDepth = 3;
    int optimizeTimes = 9999;
    
    //所有状态加入map
    std::map<std::string,bool> blackList;
    for(int i=0;i<statePath.size();i++)
        blackList[getCubeStateStr(statePath[i])] = true;
    
    std::vector<std::vector<int>> newStatePath;
    
    for(int i=0;i<statePath.size()-tryDepth;i++){
        //std::cout<<"t1"<<std::endl;
        
        //此状态及后tryDepth个状态移出map
        for(int j=0;j<tryDepth+1;j++){
            blackList[getCubeStateStr(statePath[i+j])] = 0;
        }
        
        //std::cout<<"t2"<<std::endl;
        
        //从statePath[i]出发搜索tryDepth层内的所有状态，若有跟后续状态重合的则直接连接
        std::vector<std::vector<int>> tmpStates;
        tmpStates.push_back(statePath[i]);
        for(int t=0;t<tryDepth;t++){
            std::vector<std::vector<int>> nextStates;
            for(int t1=0;t1<tmpStates.size();t1++){
                std::vector<std::vector<int>> next6States;
                genNext6States(tmpStates[t1],next6States);
                nextStates.insert(nextStates.end(),next6States.begin(),next6States.end());
            }
            tmpStates.assign(nextStates.begin(),nextStates.end());
        }
        
        //std::cout<<"t3"<<std::endl;
        
        std::map<std::string,bool> sameStateStrMap;
        for(int t=0;t<tmpStates.size();t++){
            std::string stateStr = getCubeStateStr(tmpStates[t]);
            if( blackList[stateStr] == true ){
                std::cout<<"???"<<std::endl;
                sameStateStrMap[stateStr] = true;
            }
        }
        
        //std::cout<<"t4"<<std::endl;
        
        if(sameStateStrMap.empty())
            continue;
        
        int nearestStateId = i+1;
        for(int t=i;t<statePath.size();t++){
            std::string stateStr = getCubeStateStr(statePath[t]);
            if(blackList[stateStr] == true && 
               sameStateStrMap[stateStr] == true){
                nearestStateId = t;
                break;
            }
        }
        
        //std::cout<<"t5"<<std::endl;
        
        //连接i到nearestId
        std::cout<<"-------before:"<<statePath.size()<<std::endl;
        statePath.erase(statePath.begin()+i+1,statePath.begin()+nearestStateId);
        std::cout<<"-------after:"<<statePath.size()<<std::endl;
        cmdPath.erase(cmdPath.begin()+i+1,cmdPath.begin()+nearestStateId);
        
        //std::cout<<"t6"<<std::endl;
    }
}
