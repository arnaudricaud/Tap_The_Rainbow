#include "camera.h"

Camera::Camera()
{

}


Mat Camera::getFrame(){
Mat frame;
        cap >> frame; // get a new frame from camera
        flip(frame,frame, 1);
        return frame;
}

