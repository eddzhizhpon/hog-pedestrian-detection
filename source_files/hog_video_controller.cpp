#include "../header_files/header.hpp"

HogVideoController::HogVideoController(fs::path videoPath, fs::path outPath){
    this->videoPath = videoPath;

    winSize = 16;
    paddingSize = 128;

    // Instancia a la clase HOGDescriptor.
    hog = HOGDescriptor( Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9, 1 );

    // Carga de la máquina de soporte vectorial ya entrenada 
    // por OpenCV
    hog.setSVMDetector( HOGDescriptor::getDefaultPeopleDetector() );
    cout << hog.svmDetector.size() << endl;
    cout << hog.getDescriptorSize() << endl;
    
    video = VideoCapture(videoPath.u8string());
    this->outPath = outPath;

    fs::create_directory( outPath.c_str() );
    
}

int HogVideoController::getMaxSize(int currentSize, int pStart, int totalSize){
    if (pStart + currentSize > totalSize) {
        return totalSize - pStart;
    }
    return currentSize;
}

void HogVideoController::takeScreenshot(Mat frame, Rect &r, string idName) {

    Point2i tl = r.tl();
    Point2i br = r.br();

    int w = br.x - tl.x;
    int h = br.y - tl.y;

    // Valida que el tamaño de la captura no exceda 
    // el tamaño del frame original.
    w = getMaxSize(w, tl.x, frame.cols);
    h = getMaxSize(h, tl.y, frame.rows);

    tl.x = tl.x < 0 ? 0 : tl.x;
    tl.y = tl.y < 0 ? 0 : tl.y;

    Rect frameRect(tl.x, tl.y, w, h);
    Mat image = frame(frameRect);
    string name = idName + ".jpg";
    fs::path outName = outPath / name;
    imwrite(outName.u8string(), image);
}

int HogVideoController::applyDetections(Mat frame, Mat &detectionOut) {
    vector<Rect> detectionList;
    
    // Detección de peatones usando HOG Descriptor
    hog.detectMultiScale(frame, detectionList, 0, Size(winSize, winSize), 
        Size(paddingSize, paddingSize), 1.05, 2, false);

    detectionOut = frame.clone();

    Rect r;
    int w = 0;
    int h = 0;
    int nDetections = (int) detectionList.size();
    for(int i = 0; i < detectionList.size(); i++){
        r = detectionList[i];

        // r.x += cvRound(r.width + 0.11);
        // r.width = cvRound(r.width * 0.8);
        // r.y += cvRound(r.height * 0.073);
        // r.height = cvRound(r.height * 0.8);

        w = r.width;
        h = r.height;

        takeScreenshot( frame, r, ( "frame_" + to_string(frameIdToSave) + "_image_" 
                            + to_string( imageIdToSave ) + "_ndetec_" 
                            + to_string( nDetections ) ) );

        rectangle(detectionOut, r.tl(), r.br(), Scalar(10, 10, 237), 2);
        
        imageIdToSave++;
    }
    frameIdToSave++;

    return nDetections;
}

void HogVideoController::printByCircleMethod(Mat &frame, vector<Vec3f> circleDetections){
    int nDetections = (int) circleDetections.size();
    for (size_t i = 0; i < circleDetections.size(); i++) {
        Vec3i c = circleDetections[i];
        Point center = Point(c[0], c[1]);
        int radius = c[2];

        circle(frame, center, radius, Scalar(0, 255, 0), 2, LINE_AA);
        Rect r(c[0] - radius, c[1] - radius, radius * 2, radius * 2);
        takeScreenshot(frame, r, ( "frame_" + to_string(frameIdToSave) + "_image_" 
                            + to_string( imageIdToSave ) + "_ndetec_" 
                            + to_string( nDetections ) ) );

        imageIdToSave++;
    }
    frameIdToSave++;
}

void HogVideoController::startDetection(bool makeCircles){
    vector<Vec3f> circleDetections;
    Mat frameByCircle;


    if (makeCircles) {
        namedWindow("Hough Circles", WINDOW_AUTOSIZE);
    } else {
        namedWindow("Peatones", WINDOW_AUTOSIZE);
        namedWindow("GradienteX", WINDOW_AUTOSIZE);
        namedWindow("GradienteY", WINDOW_AUTOSIZE);
        namedWindow("Magnitud", WINDOW_AUTOSIZE);
        namedWindow("Angulo", WINDOW_AUTOSIZE);
        namedWindow("Detecciones", WINDOW_AUTOSIZE);
    }

    if (video.isOpened()) {
        while (true) {
            video >> frame;
            if (frame.empty()) {
                break;
            } else {
                
                if (makeCircles) {
                    frameByCircle = frame.clone();

                    cvtColor(frameByCircle, frameByCircle, COLOR_BGR2GRAY);
                    HoughCircles(frameByCircle, circleDetections, HOUGH_GRADIENT, 1, frameByCircle.rows/2, 105, 30, 1, 50);
                    printByCircleMethod(frameByCircle, circleDetections);
                } else {
                    frame.convertTo(decimal, CV_32F, 1.0/255.0); // La imagen tendrá un nueva escala, donde se tiene el rango [0, 1]

                    Sobel(decimal, gx, CV_32F, 1, 0, 1);
                    Sobel(decimal, gy, CV_32F, 0, 1, 1);

                    cartToPolar(gx, gy, magnitud, angulo, true);

                    detectionSize = applyDetections(frame, frameWithDetection);
                }
                

                
                if (makeCircles) {
                    imshow("Hough Circles", frameByCircle);
                } else {
                    imshow("Peatones", frame);
                    imshow("GradienteX", gx);
                    imshow("GradienteY", gy);
                    imshow("Magnitud", magnitud);
                    imshow("Angulo", angulo);
                    imshow("Detecciones", frameWithDetection);
                }

                // waitKey(0);
                
                if (waitKey(23) == 27){
                    break;
                }

            }

        }
    }

    // waitKey(0);

    cv::destroyAllWindows();
}