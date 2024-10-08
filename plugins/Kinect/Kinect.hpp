// PluginKinect.hpp
// Evan Murray (evan@auraaudio.io)

#pragma once

#include "SC_PlugIn.hpp"

// Third-party libraries
#include <opencv2/opencv.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <openpose/flags.hpp>
#include <openpose/headers.hpp>

namespace Kinect {
libfreenect2::Freenect2 freenect2;
libfreenect2::PacketPipeline *mPipeline = new libfreenect2::CudaPacketPipeline(0);
libfreenect2::Freenect2Device *dev = freenect2.openDefaultDevice(mPipeline);
libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color);
libfreenect2::FrameMap frames;

// Process a frame from the Kinect
void processFrame() {
    while(true) {
        listener.waitForNewFrame(frames);
        libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
        cv::Mat rgbmat;
        cv::Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);
        listener.release(frames);
    }
}

class Kinect : public SCUnit {
public:
    Kinect();

private:
    // Calc function
    void next(int nSamples);

    // Member variables
};

} // namespace Kinect
