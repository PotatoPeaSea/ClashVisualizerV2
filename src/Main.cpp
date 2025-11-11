// File: Main.cpp
// Simple OpenCV webcam example — opens camera 0 and shows live frames.
// Build via CMake (CMakeLists.txt in project root). Make sure OpenCV runtime
// DLLs are on PATH (e.g. C:/opencv/build/x64/vc16/bin) when running.

#include <iostream>
#include "opencv2/opencv.hpp"
#include <windows.h>

using namespace cv;


const int runMode = 2; // 1 for live video , 2 to read a video file
const std::string videoPath = "../../video/test3.mp4";

int findStarterBox(cv::Mat frame, Rect& dimensions);
void mouseCallback(int event, int x, int y, int flags, void* userdata);
Point starterBoxCordinates = Point(680, 971); // Top-left corner of the starter box

int mousex, mousey;

struct Card {
    std::string name;
    Mat sprite;
    int elixirCost;
};


int main()
{
    Mat cardSprites[8] = {
        imread("../../sprites/CannonCardEvolution2.png"), 
        imread("../../sprites/EarthquakeCard2.png"),
        imread("../../sprites/FirecrackerCardEvolution2.png"),
        imread("../../sprites/HogRiderCard2.png"),
        imread("../../sprites/IceSpiritCard2.png"),
        imread("../../sprites/MightyMinerCard2.png"),
        imread("../../sprites/SkeletonsCard2.png"),
        imread("../../sprites/TheLogCard2.png")
    };

    Card cards[8] = {
        {"Cannon", cardSprites[0], 3},
        {"Earthquake", cardSprites[1], 3},
        {"Firecracker", cardSprites[2], 3},
        {"Hog Rider", cardSprites[3], 4},
        {"Ice Spirit", cardSprites[4], 1},
        {"Mighty Miner", cardSprites[5], 4},
        {"Skeletons", cardSprites[6], 1},
        {"The Log", cardSprites[7], 2}
    };
    
    int cardsSize = sizeof(cardSprites) / sizeof(cardSprites[0]);

    double scale = 0.7;
    for(int i = 0; i < 8; i++){
        if(cardSprites[i].empty()){
            std::cerr << "Failed to load card image at index " << i << std::endl;
            return -1;
        }
         resize(cardSprites[i], cardSprites[i], cv::Size(), scale, scale, cv::INTER_LANCZOS4); // Resize card image
         cvtColor(cardSprites[i], cardSprites[i], COLOR_BGR2GRAY);
    }

    // Disable OpenCV's verbose logging
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    
    std::cout << "DEBUG: Program starting" << std::endl;
    std::cout << "DEBUG: OpenCV version: " << CV_VERSION << std::endl;

    std::cout << "Opening camera..." << std::endl;

        cv::VideoCapture cap;

    switch (runMode){

    case 1:
         cap.open(1);
        break;
    
    case 2:
        cap.open(videoPath);
        break;
    }
    
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
    dimensions = Rect(starterBoxCordinates.x, starterBoxCordinates.y, 633, 95); // Manually set dimensions of the starter box
    // while(!found){
    //     cap >> frame; // grab a frame
    //     if (frame.empty()) {
    //         std::cerr << "Warning: captured empty frame" << std::endl;
    //         break;
    //     }
    //     found = findStarterBox(frame, dimensions); // match template for starting box and find dimensions 
    // }

    


    while (true) {
        cap >> frame; // grab a frame
        if(runMode == 2){
            resize(frame, frame, cv::Size(), .666, .666, cv::INTER_LANCZOS4); // Resize frame if reading from video
        }
        if (frame.empty()) {
            std::cerr << "Warning: captured empty frame" << std::endl;
            break;
        }
        else{
            // std::cout << "frame grabbed" << std::endl;
        }
        
        cv::setWindowProperty(windowName, cv::WINDOW_AUTOSIZE, cv::WINDOW_AUTOSIZE);

        frame = frame(dimensions).clone(); // crop to starting box dimensions
        cvtColor(frame, frame, COLOR_BGR2GRAY);
     


        for(int i = 0; i < cardsSize; i++){
            if(cardSprites[i].empty()){
                std::cerr << "Card image at index " << i << " is empty!" << std::endl;
                continue;

            }

            if(frame.cols < cardSprites[i].cols || frame.rows < cardSprites[i].rows) {
                std::cerr << "Cropped frame is smaller than card image at index " << i << std::endl;
                continue;
            }


            int resultCols = frame.cols - cardSprites[i].cols + 1;
            int resultRows = frame.rows - cardSprites[i].rows + 1;

            Mat result;
            Mat resultMap;
            result.create(resultRows, resultCols, CV_32FC1);

            matchTemplate(frame, cardSprites[i], result, cv::TM_CCOEFF_NORMED);
            normalize(result, resultMap, 0, 255, NORM_MINMAX, -1);

            resultMap.convertTo(resultMap, CV_8U);
            applyColorMap(resultMap, resultMap, cv::COLORMAP_JET);

            double minVal, maxVal;
            Point minLoc, maxLoc, matchLoc;
            minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
            matchLoc = maxLoc;

            // rectangle(frame, matchLoc, Point(matchLoc.x + cardSprites[i].cols , matchLoc.y + cardSprites[i].rows), Scalar::all(0), 2, 8, 0);



            if(maxVal > 0.4){
                // std::cout << "Found card: " << cards[i].name << " at " << matchLoc << " with confidence: " << maxVal << std::endl;
                rectangle(frame, matchLoc, Point(matchLoc.x + cardSprites[i].cols , matchLoc.y + cardSprites[i].rows), Scalar(0,255,0), 2, 8, 0);
                putText(frame, cards[i].name + " (" + std::to_string(maxVal) + ")", Point(matchLoc.x, matchLoc.y + 5 + (i*10)), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
            }
            else{
                std::cout << "card not found: " << cards[i].name << " maxVal: " << maxVal << std::endl;
            }

        }

        
        
        cv::imshow(windowName, frame);
        setMouseCallback(windowName, mouseCallback, nullptr);
        // std::cout << "Displayed cropped frame" << std::endl;
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

    double scale = 0.7;
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

    cv::rectangle(displayImg, matchLoc, cv::Point(matchLoc.x + starterBar.cols , matchLoc.y + starterBar.rows), cv::Scalar::all(0), 2, 8, 0);\

    Rect imageRect(0, 0, frame.cols, frame.rows);
    Rect matchRect(matchLoc.x, matchLoc.y, starterBar.cols, starterBar.rows);    
    matchRect = matchRect & imageRect; // Ensure the rectangle is within image bounds
    dimensions = matchRect;
    imshow("cropped image", frame(dimensions));
    cv::imshow("Display Image", displayImg);
    cv::imshow("Result Image", resultMap);
    
    std::cout << "Max Val: " << maxVal << std::endl;
    
    int key = cv::waitKey(30);
    if(maxVal > 0.9 || key == 'k'){
        std::cout << "Starter box found at: " << matchLoc << " with confidence: " << maxVal << std::endl;
        std::cout << "Dimensions: " << dimensions << std::endl;
        // std::cout << "Startbox is at" << 
        return 1;
    }

            
            if (key == 27 || key == 'q' || key == 'Q') { // ESC or q to quit
                cv::destroyWindow("Display Image");
                cv::destroyWindow("Result Image");
                return 1;
            }
    return 0;
}


void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        mousex = x;
        mousey = y;
        std::cout << "Mouse clicked at: (" << mousex << ", " << mousey << ")\n";
    }
}

