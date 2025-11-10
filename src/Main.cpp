// File: Main.cpp
// Simple OpenCV webcam example — opens camera 0 and shows live frames.
// Build via CMake (CMakeLists.txt in project root). Make sure OpenCV runtime
// DLLs are on PATH (e.g. C:/opencv/build/x64/vc16/bin) when running.

#include <iostream>
#include "opencv2/opencv.hpp"
#include <windows.h>
using namespace cv;
int findStarterBox(cv::Mat frame, Rect& dimensions);

int main()
{
    // Disable OpenCV's verbose logging
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);
    
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    
    std::cout << "DEBUG: Program starting" << std::endl;
    std::cout << "DEBUG: OpenCV version: " << CV_VERSION << std::endl;

    std::cout << "Opening camera..." << std::endl;
    cv::VideoCapture cap(1);
    if (!cap.isOpened()) {
        std::cout << "Failed to open camera!" << std::endl;
        return 1;
    }
    std::cout << "Camera opened successfully" << std::endl;

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);  // Set resolution width
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1200); // Set resolution height

    cv::Mat frame;
    const char* windowName = "Screen Capture";

    cap >> frame;
    bool found = false;
    Rect dimensions;
    while(!found){
        cap >> frame; // grab a frame
        if (frame.empty()) {
            std::cerr << "Warning: captured empty frame" << std::endl;
            break;
        }
        found = findStarterBox(frame, dimensions); 
    }

    


    while (true) {
        cap >> frame; // grab a frame
        if (frame.empty()) {
            std::cerr << "Warning: captured empty frame" << std::endl;
            break;
        }
        cv::setWindowProperty(windowName, cv::WINDOW_AUTOSIZE, cv::WINDOW_AUTOSIZE);

        frame = frame(dimensions).clone();

        
        

        cv::imshow(windowName, frame);
        int key = cv::waitKey(30);
        if (key == 27 || key == 'q' || key == 'Q') { // ESC or q to quit
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}



int findStarterBox(cv::Mat frame, Rect& dimensions) {
    cv::Mat starterBar = cv::imread("../../images/Starterbar.jpg");

    cv::cvtColor(starterBar, starterBar, cv::COLOR_BGR2GRAY);
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    
    if (starterBar.empty()) {
        std::cerr << "findStarterBox: template not found: images/Starterbar.jpg\n";
        return -1;
    }
    if (frame.empty()) {
        std::cerr << "findStarterBox: input frame is empty\n";
        return -1;
    }

    float scale = 0.7;
    cv::resize(starterBar, starterBar, cv::Size(), scale, scale, cv::INTER_LANCZOS4); // Resize template if needed
    cv::Mat displayImg = frame.clone();

    if(frame.cols < starterBar.cols || frame.rows < starterBar.rows) {
        std::cerr << "findStarterBox: input frame is smaller than template\n";
        return -1;
    }

    int resultCols = frame.cols - starterBar.cols + 1;
    int resultRows = frame.rows - starterBar.rows + 1;

    cv::Mat result;
    cv::Mat resultMap;

    result.create(resultRows, resultCols, CV_32FC1);


    cv::matchTemplate(frame, starterBar, result, cv::TM_CCORR_NORMED);
    cv::normalize(result, resultMap, 0, 255, cv::NORM_MINMAX);

    resultMap.convertTo(resultMap, CV_8U);
    cv::applyColorMap(resultMap, resultMap, cv::COLORMAP_JET);


    double minVal, maxVal;
    cv::Point minLoc, maxLoc, matchLoc;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

    matchLoc = maxLoc;

    cv::rectangle(displayImg, matchLoc, cv::Point(matchLoc.x + starterBar.cols , matchLoc.y + starterBar.rows), cv::Scalar::all(0), 2, 8, 0);
    Rect matchRect(matchLoc.x, matchLoc.y, starterBar.cols, starterBar.rows);    

    cv::imshow("Display Image", displayImg);
    cv::imshow("Result Image", resultMap);
    
    std::cout << "Max Val: " << maxVal << std::endl;
    
  
    if(maxVal > 0.822){
        std::cout << "Starter box found at: " << matchLoc << " with confidence: " << maxVal << std::endl;
        return 1;
    }

            int key = cv::waitKey(30);
            if (key == 27 || key == 'q' || key == 'Q') { // ESC or q to quit
                cv::destroyWindow("Display Image");
                cv::destroyWindow("Result Image");
                return 1;
            }
    return 0;
}


