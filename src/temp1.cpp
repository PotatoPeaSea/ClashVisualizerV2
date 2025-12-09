#include <iostream>
#include "opencv2/opencv.hpp"
#include <windows.h>
#include <vector>
int main(){
 
    cv::Mat card = cv::imread("../../sprites2/TheLogCard.png");
    cv::Mat frame = cv::imread("../../images/emptyHand.png");

    std::cout << "Card -\n"
          << "Rows: " << card.rows << "\n"
          << "Cols: " << card.cols << "\n"
          << "Channels: " << card.channels() << "\n"
          << "Depth: " << card.depth() << "\n"
          << "Is Continuous: " << card.isContinuous() << "\n";
    
    std::cout << "\nFrame -\n"
          << "Rows: " << frame.rows << "\n"
          << "Cols: " << frame.cols << "\n"
          << "Channels: " << frame.channels() << "\n"
          << "Depth: " << frame.depth() << "\n"
          << "Is Continuous: " << frame.isContinuous() << "\n";

    
    
}