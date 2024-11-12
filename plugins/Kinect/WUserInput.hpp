// WUserInput.hpp
// Evan Murray (evan@auraaudio.io)
#pragma once
#ifndef WUSERINPUT_HPP
#define WUSERINPUT_HPP

// Import OpenCV, OpenPose, and Libfreenect2
#include <opencv2/opencv.hpp>
#include <openpose/headers.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>

// Input for the OpenPose model
class WUserInput : public op::WorkerProducer<std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>>
{
public:
    WUserInput();
    ~WUserInput();

    void setDevice(libfreenect2::Freenect2Device* device);
    void initializationOnThread(); // Runs to initialize the thread
    std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>> workProducer(); // Process an input frame
private:
    libfreenect2::Freenect2Device* mDev;
    libfreenect2::SyncMultiFrameListener mListener;
    libfreenect2::FrameMap mFrames;
};

#endif // WUSERINPUT_HPP
