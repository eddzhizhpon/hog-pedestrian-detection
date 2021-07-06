#include "../header_files/header.hpp"

int main(int, char**) {
    // VideoCapture video("../resources/peatones_video_3.mp4");
    HogVideoController hogVideoController("../resources/peatones_video_3.mp4", "../resources/screenshots_rectangular/");
    hogVideoController.startDetection();
    return 0;
}
