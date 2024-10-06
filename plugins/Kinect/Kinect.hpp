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

class Kinect : public SCUnit {
public:
    Kinect();
    ~Kinect();

private:
    // Calc function
    void next(int nSamples);

    // Configure the graphics pipeline to use for the Kinect
    void configure();

    // Member variables
    libfreenect2::Freenect2 mFreenect2;
    libfreenect2::FrameMap mFrames;
    libfreenect2::SyncMultiFrameListener mListener;
    libfreenect2::Freenect2Device *mDev = 0;
    libfreenect2::PacketPipeline *mPipeline = 0;
};

} // namespace Kinect
