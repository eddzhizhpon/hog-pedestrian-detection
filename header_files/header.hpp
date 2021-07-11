#include <iostream>
#include <cstdlib>
#include <cstring>

#include <cmath>
#include <experimental/filesystem>

#include <opencv2/core/core.hpp> // Contiene los elementos base de OpenCV (matrices, arreglos, etc.)
#include <opencv2/highgui/highgui.hpp> // Contiene estructuras para crear la interfaz gráfica
#include <opencv2/imgproc/imgproc.hpp> // Procesamiento de imágenes
#include <opencv2/imgcodecs/imgcodecs.hpp> // Códecs para leer determinados formatos de imágenes
#include <opencv2/video/video.hpp> // Procesamiento y escritura
#include <opencv2/videoio/videoio.hpp> // de video

#include <opencv2/objdetect/objdetect.hpp> // Librería para detección de objetos (contiene el HOG Descriptor)

using namespace std;

using namespace cv;

namespace fs =  std::experimental::filesystem;

class HogVideoController {
    private:
        HOGDescriptor hog;
        // HoughCircles

        Mat frame;
        Mat frameWithDetection;

        Mat gx, gy;
        Mat angulo, magnitud; //Dirección y magnitud del gradiente

        Mat decimal;

        int detectionSize;

        int winSize = 8;
        int paddingSize = 128;
        int imageIdToSave = 0;
        int frameIdToSave = 0;

        VideoCapture video;

        fs::path outPath;
        fs::path videoPath;
        
        int getMaxSize(int currentSize, int pStart, int totalSize);

        void takeScreenshot(Mat frame, Rect &r, string idName="0");

        int applyDetections(Mat frame, Mat &detectionOut);

    public:
        HogVideoController(fs::path videoPath, fs::path outPath);
        void startDetection();
};