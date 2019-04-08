#include <iostream>
#include <stdlib.h>
#include <cstdlib>

#include "opencv2/opencv.hpp"
#include <iostream>
 
using namespace std;
using namespace cv;

/*
 * Simple main program that demontrates how access
 * CMake definitions (here the version number) from source code.
 */
int main() {
    std::cout << "C++ OpenCV 2.4.9 v"
//                 << PROJECT_VERSION_MAJOR
//                 << "."
//                 << PROJECT_VERSION_MINOR
//                 << "."
//                 << PROJECT_VERSION_PATCH
//                 << "."
//                 << PROJECT_VERSION_TWEAK
                << std::endl;
//     std::system("cat ../LICENSE");
 
  // Create a VideoCapture object and open the input file
  // If the input is the web camera, pass 0 instead of the video file name
//   VideoCapture cap("chaplin.mp4");
  VideoCapture cap("/home/pazzari/Videos/1080 60fps.m4v"); 
    
  // Check if camera opened successfully
  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }
  
  // Default resolution of the frame is obtained.The default resolution is system dependent. 
  int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
  int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 
  
  // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
  VideoWriter video("/home/pazzari/Downloads/outcpp.avi",CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height)); 
     
  while(1){
 
    Mat frame;
    // Capture frame-by-frame
    cap >> frame;
  
    // If the frame is empty, break immediately
    if (frame.empty())
      break;
 
    // Display the resulting frame
    imshow( "Frame", frame );
    
     // Write the frame into the file 'outcpp.avi'
    video.write(frame);
 
    // Press  ESC on keyboard to exit
    char c=(char)waitKey(25);
    if(c==27)
      break;
  }
  
  // When everything done, release the video capture object
  cap.release();
  video.release();
 
  // Closes all the frames
  destroyAllWindows();
     
  return 0;
}
