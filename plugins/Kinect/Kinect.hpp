// PluginKinect.hpp
// Evan Murray (evan@auraaudio.io)

#pragma once

#include "SC_PlugIn.hpp"

// Third-party libraries
#include <opencv2/opencv.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <openpose/flags.hpp>

namespace Kinect {

class Kinect : public SCUnit {
public:
    Kinect();

    // Destructor
    // ~Kinect();

private:
    // Calc function
    void next(int nSamples);

    // Member variables
};

} // namespace Kinect
