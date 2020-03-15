#include <iostream>
#include <opencv2/highgui.hpp>

#include "MiniCubeVisualizer.h"
#include "MiniCubeSolver.h"
#include "MiniCubeFunctions.h"

int main()
{
    MiniCubeVisualizer vizer;
    
    cv::Mat image(500,1000,CV_8UC3);
    image = cv::Scalar(255,255,255);
    cv::Mat image_b = image.colRange(0,500);
    cv::Mat image_f = image.colRange(500,1000);
    
    vizer.getImage(0).copyTo(image_b);
    vizer.getImage(1).copyTo(image_f);
    cv::imshow("MiniCube",image);
    
    std::vector<int> initCube = vizer.getCurrentState();
    
    while(1){
        char key = cv::waitKey();
        if(key == '1') vizer.vizFrontL();
        else if(key == '3') vizer.vizFrontR();
        else if(key == '4') vizer.vizRightL();
        else if(key == '6') vizer.vizRightR();
        else if(key == '9') vizer.vizTopL();
        else if(key == '7') vizer.vizTopR();
        else if(key == 's') {
            std::vector<int> cube = vizer.getCurrentState();
            MiniCubeSolver solver;
            //solver.setMaxDepth(10);
            bool isSolved = solver.singlePathSolve_recursion(cube,initCube);
            
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
        
        vizer.getImage(0).copyTo(image_b);
        vizer.getImage(1).copyTo(image_f);
        cv::imshow("MiniCube",image);
    }
    
    return 0;
}
