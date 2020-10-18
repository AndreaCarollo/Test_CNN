#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

#include "./lib/followme.h"
// my libraries
#include "./lib/rs_utility.h"

using namespace std; // standard
using namespace cv;  // openCV
using namespace dnn; // deep neural network

int main()
{
    //* video capture setting
    // cv::VideoCapture cap(0);

    // --- start realsense camera ----
    rs2::context ctx;

    std::cout << "hello from librealsense - " << RS2_API_VERSION_STR << std::endl;
    std::cout << "You have " << ctx.query_devices().size() << " RealSense devices connected" << std::endl;

    // Create a Pipeline - this serves as a top-level API for streaming and processing frames
    rs2::pipeline pipe;

    // Configure and start the pipeline
    pipe.start();

    // Block program until frames arrive
    rs2::frameset frames = pipe.wait_for_frames();
    // -----

    //* basic parameters
    //* path to load model
    // std::string model  = "/home/andrea/openvino_models/ir/intel/face-detection-0106/FP32/face-detection-0106.bin";
    // std::string config = "/home/andrea/openvino_models/ir/intel/face-detection-0106/FP32/face-detection-0106.xml";

    std::string model = "/home/andrea/Documents/Model_yolov3/yolov3-tiny.weights";
    std::string config = "/home/andrea/Documents/Model_yolov3/yolov3-tiny.cfg";

    // std::string config = "/home/andrea/openvino_models/ir/intel/person-detection-retail-0002/FP16/person-detection-retail-0002.bin";
    // std::string model = "/home/andrea/openvino_models/ir/intel/person-detection-retail-0002/FP16/person-detection-retail-0002.xml";
    //* load neural network and config
    // cv::dnn::Net network = cv::dnn::readNet(model, config, "darknet");

    cv::dnn::Net network = cv::dnn::readNet(model,config);

    //* Set backend and target to execute network
    network.setPreferableBackend(DNN_BACKEND_INFERENCE_ENGINE);
    network.setPreferableTarget(DNN_TARGET_OPENCL);
    //     set the preferable computation to speed up the execution of the network
    //     DNN_TARGET_CPU
    //     DNN_TARGET_OPENCL
    //     DNN_TARGET_OPENCL_FP16
    //     DNN_TARGET_MYRIAD
    //     DNN_TARGET_FPGA

    for (;;)

    {
        Mat img;

        rs2::frameset frames = pipe.wait_for_frames();
        img = frame_to_mat(frames.get_color_frame(), 0);

        //* create blob from image
        // The yolo3-tiny.cfg using the input layer of scale 416 x 416
        static Mat blobFromImg;
        bool swapRB = true; // to swap RGB -> BGR
        blobFromImage(img, blobFromImg, 1, Size(416, 416), Scalar(), swapRB, false);
        cout << blobFromImg.size() << endl;

        float scale = 1.0 / 255.0;
        Scalar mean = 0;

        //* Set blob as input of neural network.
        network.setInput(blobFromImg, "", scale, mean);

        //* perform network.forward evaluation
        Mat outMat;
        network.forward(outMat);
        //* process the output layer evaluation
        // rows represent number of detected object (proposed region)
        int rowsNoOfDetection = outMat.rows;

        // The coluns looks like this, The first is region center x, center y, width
        // height, The class 1 - N is the column entries, which gives you a number,
        // where the biggist one corrsponding to most probable class.
        // [x ; y ; w; h; class 1 ; class 2 ; class 3 ;  ; ;....]
        int colsCoordinatesPlusClassScore = outMat.cols;
        // Loop over number of detected object.
        for (int j = 0; j < rowsNoOfDetection; ++j)
        {
            // for each row, the score is from element 5 up
            // to number of classes index (5 - N columns)
            Mat scores = outMat.row(j).colRange(5, colsCoordinatesPlusClassScore);

            Point PositionOfMax;
            double confidence;

            // This function find indexes of min and max confidence and related index of element.
            // The actual index is match to the concrete class of the object.
            // First parameter is Mat which is row [5fth - END] scores,
            // Second parameter will gives you min value of the scores. NOT needed
            // confidence gives you a max value of the scores. This is needed,
            // Third parameter is index of minimal element in scores
            // the last is position of the maximum value.. This is the class!!
            minMaxLoc(scores, 0, &confidence, 0, &PositionOfMax);

            if (confidence > 0.0001)
            {
                // thease four lines are
                // [x ; y ; w; h;
                int centerX = (int)(outMat.at<float>(j, 0) * img.cols);
                int centerY = (int)(outMat.at<float>(j, 1) * img.rows);
                int width = (int)(outMat.at<float>(j, 2) * img.cols + 20);
                int height = (int)(outMat.at<float>(j, 3) * img.rows + 100);

                int left = centerX - width / 2;
                int top = centerY - height / 2;

                stringstream ss;
                ss << PositionOfMax.x;
                string clas = ss.str();
                int color = PositionOfMax.x * 10;
                putText(img, clas, Point(left, top), 1, 2, Scalar(color, 255, 255), 2, false);
                stringstream ss2;
                ss << confidence;
                string conf = ss.str();

                rectangle(img, Rect(left, top, width, height), Scalar(color, 0, 0), 2, 8, 0);
            }
        }

        namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
        imshow("Display window", img);
        waitKey(25);
    }
    return 0;

    //  region proposal (most difficult step)
    //* display image
}
