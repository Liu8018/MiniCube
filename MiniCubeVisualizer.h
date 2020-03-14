#ifndef MINICUBEVISUALIZER_H
#define MINICUBEVISUALIZER_H

#include <opencv2/core.hpp>

class MiniCubeVisualizer
{
public:
    MiniCubeVisualizer();
    
    //翻转魔方
    void vizFrontL();
    void vizFrontR();
    void vizRightL();
    void vizRightR();
    void vizTopL();
    void vizTopR();
    
    //得到图片
    cv::Mat getImage(int isFront);
    
    //得到魔方当前状态
    void getCurrentState(int cubeState[6][4]);
    
private:
    //魔方状态
    int m_cube[6][4];
};

#endif // MINICUBEVISUALIZER_H
