#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "MiniCubeVisualizer.h"
#include "MiniCubeSolver.h"
#include "MiniCubeFunctions.h"

cv::Mat getMainView(cv::Mat srcImage_b, cv::Mat srcImage_f,
                    cv::Mat dstImage_b, cv::Mat dstImage_f)
{
    int leftSpaceCols = 300;
    cv::Mat canvas(900,1000+leftSpaceCols,CV_8UC3);
    canvas = cv::Scalar(255,255,255);
    
    srcImage_b.copyTo(canvas(cv::Range(0,500),cv::Range(leftSpaceCols,leftSpaceCols+500)));
    srcImage_f.copyTo(canvas(cv::Range(0,500),cv::Range(leftSpaceCols+500,canvas.cols)));
    dstImage_b.copyTo(canvas(cv::Range(400,900),cv::Range(leftSpaceCols,leftSpaceCols+500)));
    dstImage_f.copyTo(canvas(cv::Range(400,900),cv::Range(leftSpaceCols+500,canvas.cols)));
    
    cv::line(canvas,cv::Point(0,canvas.rows/2),cv::Point(canvas.cols,canvas.rows/2),cv::Scalar(0,0,0),2);
    cv::line(canvas,cv::Point(canvas.cols-500,0),cv::Point(canvas.cols-500,canvas.rows),cv::Scalar(0,0,0),2);
    cv::line(canvas,cv::Point(leftSpaceCols,0),cv::Point(leftSpaceCols,canvas.rows),cv::Scalar(0,0,0),2);
    cv::line(canvas,cv::Point(0,50),cv::Point(canvas.cols,50),cv::Scalar(0,0,0),2);
    
    cv::putText(canvas,"Initial State:",cv::Point(30,250),3,1.1,cv::Scalar(0,0,0),2);
    cv::putText(canvas,"Target State:",cv::Point(30,650),3,1.1,cv::Scalar(0,0,0),2);
    
    cv::putText(canvas,"Back",cv::Point(505,40),3,1.1,cv::Scalar(0,0,0),2);
    cv::putText(canvas,"Front",cv::Point(1000,40),3,1.1,cv::Scalar(0,0,0),2);
    
    canvas = canvas.rowRange(0,850);
    
    float resizeRatio = 600/float(canvas.rows);
    cv::resize(canvas,canvas,cv::Size(),resizeRatio,resizeRatio);
    
    return canvas;
}

int main()
{
    MiniCubeVisualizer vizer_srcCube;
    MiniCubeVisualizer vizer_dstCube;
    
    cv::Mat mainView = getMainView(vizer_srcCube.getImage(0),vizer_srcCube.getImage(1),
                                   vizer_dstCube.getImage(0),vizer_dstCube.getImage(1));
    cv::imshow("MiniCube",mainView);
    
    int method = 1;
    
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
            std::cout<<"Searching..."<<std::endl;
            
            std::vector<int> srcCube = vizer_srcCube.getCurrentState();
            std::vector<int> dstCube = vizer_dstCube.getCurrentState();
            
            MiniCubeSolver solver;
            bool isSolved;
            if(method)
                isSolved = solver.depthFirstSolve_multiThread(srcCube,dstCube,13);
            else
                isSolved = solver.singlePathSolve(srcCube,dstCube);
            
            if(isSolved){
                std::cout<<"Success"<<std::endl;
                
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
                std::cout<<"Failed"<<std::endl;
            }
        }
        else if(key == 't'){
            method = !method;
            if(method == 1)
                std::cout<<"现在使用 深度优先搜索+限制深度 方法"<<std::endl;
            else
                std::cout<<"现在使用 深度优先搜索+不限深度 方法"<<std::endl;
        }
        else if(key == 27) break;
        
        cv::Mat mainView = getMainView(vizer_srcCube.getImage(0),vizer_srcCube.getImage(1),
                                       vizer_dstCube.getImage(0),vizer_dstCube.getImage(1));
        cv::imshow("MiniCube",mainView);
    }
    
    return 0;
}
