#include <iostream>
#include <opencv2/highgui.hpp>

#include "MiniCubeVisualizer.h"
#include "MiniCubeSolver.h"
#include "MiniCubeFunctions.h"

int main()
{
    MiniCubeVisualizer vizer_srcCube;
    MiniCubeVisualizer vizer_dstCube;
    
    cv::Mat srcImage(500,1000,CV_8UC3);
    srcImage = cv::Scalar(255,255,255);
    cv::Mat srcImage_b = srcImage.colRange(0,500);
    cv::Mat srcImage_f = srcImage.colRange(500,1000);
    vizer_srcCube.getImage(0).copyTo(srcImage_b);
    vizer_srcCube.getImage(1).copyTo(srcImage_f);
    cv::imshow("init state",srcImage);
    
    cv::Mat dstImage(500,1000,CV_8UC3);
    dstImage = cv::Scalar(255,255,255);
    cv::Mat dstImage_b = dstImage.colRange(0,500);
    cv::Mat dstImage_f = dstImage.colRange(500,1000);
    vizer_dstCube.getImage(0).copyTo(dstImage_b);
    vizer_dstCube.getImage(1).copyTo(dstImage_f);
    cv::imshow("target state",dstImage);
    
    while(1){
        char key = cv::waitKey();
        if(key == '1') vizer_srcCube.vizFrontL();
        else if(key == '3') vizer_srcCube.vizFrontR();
        else if(key == '4') vizer_srcCube.vizRightL();
        else if(key == '6') vizer_srcCube.vizRightR();
        else if(key == '9') vizer_srcCube.vizTopL();
        else if(key == '7') vizer_srcCube.vizTopR();
        else if(key == 'a') vizer_dstCube.vizFrontL();
        else if(key == 's') vizer_dstCube.vizFrontR();
        else if(key == 'd') vizer_dstCube.vizRightL();
        else if(key == 'e') vizer_dstCube.vizRightR();
        else if(key == 'w') vizer_dstCube.vizTopL();
        else if(key == 'q') vizer_dstCube.vizTopR();
        else if(key == ' ') {
            std::vector<int> srcCube = vizer_srcCube.getCurrentState();
            std::vector<int> dstCube = vizer_dstCube.getCurrentState();
            
            MiniCubeSolver solver;
            bool isSolved = solver.depthFirstSolve_recursion(srcCube,dstCube,13);
            
            if(isSolved){
                std::cout<<"success"<<std::endl;
                
                std::vector<int> cmdList = solver.getCmdList();
                std::string cmdStr = "";
                for(int i=0;i<cmdList.size();i++){
                    int cmd = cmdList[i];
                    if(cmd == 0) cmdStr += "1 ";
                    else if(cmd == 1) cmdStr += "3 ";
                    else if(cmd == 2) cmdStr += "4 ";
                    else if(cmd == 3) cmdStr += "6 ";
                    else if(cmd == 4) cmdStr += "9 ";
                    else if(cmd == 5) cmdStr += "7 ";
                }
                std::cout<<"command list:"<<cmdStr<<std::endl;
            }
            else{
                std::cout<<"failed"<<std::endl;
            }
        }
        else if(key == 27) break;
        
        vizer_srcCube.getImage(0).copyTo(srcImage_b);
        vizer_srcCube.getImage(1).copyTo(srcImage_f);
        cv::imshow("init state",srcImage);
        
        vizer_dstCube.getImage(0).copyTo(dstImage_b);
        vizer_dstCube.getImage(1).copyTo(dstImage_f);
        cv::imshow("target state",dstImage);
    }
    
    return 0;
}
