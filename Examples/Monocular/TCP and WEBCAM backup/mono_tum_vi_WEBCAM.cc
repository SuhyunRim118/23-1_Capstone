#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// #include "etoeNet.h"

#include <opencv4/opencv2/opencv.hpp>

#include<System.h>


#include <fstream> 
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "termios.h"
#include <sys/signal.h>





#define _WEBCAM_BUILD_


using namespace std;

      

int main(int argc, char **argv)
{
#ifdef _WEBCAM_BUILD_
    if(argc != 3)
    {
        cerr << endl << "argc:" << argc << "!= 3"<< endl;
    }

    // etoeNet etoe;
    // etoe.loadOnnxFile(argv[3]);

    cv::VideoCapture cap(-1);
    // cv::VideoCapture cap(argv[3]);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1024);//Setting the width of the video
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 768);//Setting the height of the video
    // cap.set(cv::CAP_PROP_FPS, 30);//Setting the height of the video

    std::cout<< cap.get(cv::CAP_PROP_FPS)<<std::endl;
    // return -1;

    
    cout<<"WIDTH : " << cap.get(cv::CAP_PROP_FRAME_WIDTH)<<endl;
    cout<<"HEIGHT : " << cap.get(cv::CAP_PROP_FRAME_HEIGHT)<<endl;
    

    if (!cap.isOpened()) {
        cerr << endl << "Could not open camera feed." << endl;
        return -1;
    }
    ORB_SLAM3::System SLAM(argv[1], argv[2], ORB_SLAM3::System::MONOCULAR, true);
    cout << endl << "-------" << endl;
    cout << "Start processing sequence ..." << endl;


    // // SERIAL 통신파트 
    // SLAM.OpenSerialPort();
    // float currentAngle = 0.0;
    // int diffAngle = 0;
    // float actualAngle = 0.0;
    

#ifdef COMPILEDWITHC11
    std::chrono::steady_clock::time_point initT = std::chrono::steady_clock::now();
#else
    std::chrono::monotonic_clock::time_point initT = std::chrono::monotonic_clock::now();
#endif

    // Main loop
    while(true)//cv::waitKey(0) != 27)

    {
        //Create a new Mat
        cv::Mat frame;
        //Send the captured frame to the new Mat
        cap >> frame;
        cv::Mat temp;
        cv::resize(frame, temp, cv::Size(640,480));
        // actualAngle = etoe.runInference(temp, &SLAM.fd, &actualAngle);


        if(frame.empty())
            break;
#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point nowT = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point nowT = std::chrono::monotonic_clock::now();
#endif
        // Pass the image to the SLAM system
        SLAM.TrackMonocular(frame, std::chrono::duration_cast<std::chrono::duration<double> >(nowT-initT).count());
    }
    // Stop all threads
    SLAM.Shutdown();

    //slam->SaveSeperateKeyFrameTrajectoryTUM("KeyFrameTrajectory-1.txt", "KeyFrameTrajectory-2.txt", "KeyFrameTrajectory-3.txt");
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

#else
    
#endif
    return 0;
}
