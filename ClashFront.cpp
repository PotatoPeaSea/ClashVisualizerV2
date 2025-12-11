// ClashFront.cpp
// Tracker Royale Frontend
// Program to track the deck cycle

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

// window size
int width = 640;
int height = 600;

// the 8 cards
vector<string> cards = {
    "Hog Rider", "MightyMiner", "Earthquake", "Firecracker",
    "Ice Spirit", "Cannon", "The Log", "Skeletons"
};

// for clicking
Rect box1, box2, box3, box4;

void draw(Mat& img);
void click(int event, int x, int y, int flags, void* userdata);
void play(int pos);

int main() {
    namedWindow("Tracker Royale", WINDOW_AUTOSIZE);
    setMouseCallback("Tracker Royale", click, nullptr);

    Mat img(height, width, CV_8UC3);

    // main loop
    while (true) {
        // background color
        img = Scalar(40, 40, 40);
        
        draw(img);
        
        imshow("Tracker Royale", img);

        int k = waitKey(33);
        if (k == 27 || k == 'q') break;  // quit
        if (k == 'r') {
            // reset cards
            cards[0] = "Hog Rider";
            cards[1] = "MightyMiner";
            cards[2] = "Earthquake";
            cards[3] = "Firecracker";
            cards[4] = "Ice Spirit";
            cards[5] = "Cannon";
            cards[6] = "The Log";
            cards[7] = "Skeletons";
        }
    }

    destroyAllWindows();
    return 0;
}

void draw(Mat& img) {
    // title
    putText(img, "TRACKER ROYALE", Point(170, 50), FONT_HERSHEY_DUPLEX, 1.5, Scalar(0, 200, 255), 2);

    // hand label
    putText(img, "Opponent Hand:", Point(30, 100), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 1);

    // draw hand cards (first 4)
    int x1 = 30;
    int y1 = 120;
    int w = 130;
    int h = 160;
    
    // card 1
    box1 = Rect(x1, y1, w, h);
    rectangle(img, box1, Scalar(60, 100, 60), -1);
    rectangle(img, box1, Scalar(0, 215, 255), 3);
    putText(img, cards[0], Point(x1 + 20, y1 + 90), FONT_HERSHEY_SIMPLEX, 0.45, Scalar(255, 255, 255), 1);
    putText(img, "[CLICK]", Point(x1 + 35, y1 + 140), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 200, 200), 1);

    // card 2
    int x2 = x1 + w + 15;
    box2 = Rect(x2, y1, w, h);
    rectangle(img, box2, Scalar(60, 100, 60), -1);
    rectangle(img, box2, Scalar(0, 215, 255), 3);
    putText(img, cards[1], Point(x2 + 20, y1 + 90), FONT_HERSHEY_SIMPLEX, 0.45, Scalar(255, 255, 255), 1);
    putText(img, "[CLICK]", Point(x2 + 35, y1 + 140), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 200, 200), 1);

    // card 3
    int x3 = x2 + w + 15;
    box3 = Rect(x3, y1, w, h);
    rectangle(img, box3, Scalar(60, 100, 60), -1);
    rectangle(img, box3, Scalar(0, 215, 255), 3);
    putText(img, cards[2], Point(x3 + 20, y1 + 90), FONT_HERSHEY_SIMPLEX, 0.45, Scalar(255, 255, 255), 1);
    putText(img, "[CLICK]", Point(x3 + 35, y1 + 140), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 200, 200), 1);

    // card 4
    int x4 = x3 + w + 15;
    box4 = Rect(x4, y1, w, h);
    rectangle(img, box4, Scalar(60, 100, 60), -1);
    rectangle(img, box4, Scalar(0, 215, 255), 3);
    putText(img, cards[3], Point(x4 + 20, y1 + 90), FONT_HERSHEY_SIMPLEX, 0.45, Scalar(255, 255, 255), 1);
    putText(img, "[CLICK]", Point(x4 + 35, y1 + 140), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 200, 200), 1);

    // deck label
    putText(img, "In Deck:", Point(30, 320), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(150, 150, 150), 1);

    // draw deck cards (last 4) - gray color
    int y2 = 340;

    // card 5
    Rect d1(x1, y2, w, h);
    rectangle(img, d1, Scalar(50, 50, 50), -1);
    rectangle(img, d1, Scalar(80, 80, 80), 2);
    putText(img, cards[4], Point(x1 + 20, y2 + 90), FONT_HERSHEY_SIMPLEX, 0.45, Scalar(120, 120, 120), 1);

    // card 6
    Rect d2(x2, y2, w, h);
    rectangle(img, d2, Scalar(50, 50, 50), -1);
    rectangle(img, d2, Scalar(80, 80, 80), 2);
    putText(img, cards[5], Point(x2 + 20, y2 + 90), FONT_HERSHEY_SIMPLEX, 0.45, Scalar(120, 120, 120), 1);

    // card 7
    Rect d3(x3, y2, w, h);
    rectangle(img, d3, Scalar(50, 50, 50), -1);
    rectangle(img, d3, Scalar(80, 80, 80), 2);
    putText(img, cards[6], Point(x3 + 20, y2 + 90), FONT_HERSHEY_SIMPLEX, 0.45, Scalar(120, 120, 120), 1);

    // card 8
    Rect d4(x4, y2, w, h);
    rectangle(img, d4, Scalar(50, 50, 50), -1);
    rectangle(img, d4, Scalar(80, 80, 80), 2);
    putText(img, cards[7], Point(x4 + 20, y2 + 90), FONT_HERSHEY_SIMPLEX, 0.45, Scalar(120, 120, 120), 1);

    // help text
    putText(img, "R = Reset   Q = Quit", Point(220, height - 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(100, 100, 100), 1);
}

void play(int pos) {
    string played = cards[pos];

    // move card to end
    cards.erase(cards.begin() + pos);
    cards.push_back(played);
}

void click(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        // check which card was clicked
        if (box1.contains(Point(x, y))) {
            play(0);
        }
        else if (box2.contains(Point(x, y))) {
            play(1);
        }
        else if (box3.contains(Point(x, y))) {
            play(2);
        }
        else if (box4.contains(Point(x, y))) {
            play(3);
        }
    }
}