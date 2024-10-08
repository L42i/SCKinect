// PluginKinect.cpp
// Evan Murray (evan@auraaudio.io)

#include "SC_PlugIn.hpp"
#include "Kinect.hpp"

static InterfaceTable* ft;

namespace Kinect {

Kinect::Kinect() {
    mCalcFunc = make_calc_function<Kinect, &Kinect::next>();
    next(1);
}

void Kinect::next(int nSamples) {
    // Audio rate input
    const float* input = in(0);

    // Control rate parameter: gain.
    const float gain = in0(1);

    // Output buffer
    float* outbuf = out(0);

    // simple gain function
    for (int i = 0; i < nSamples; ++i) {
        outbuf[i] = input[i] * gain;
    }
}

} // namespace Kinect

PluginLoad(KinectUGens) {
    // Plugin magic
    Kinect::dev->setColorFrameListener(&Kinect::listener);
    Kinect::dev->start();
    std::thread backgroundTask(Kinect::processFrame); // Start processing frames in the background
    backgroundTask.detach();
    ft = inTable;
    registerUnit<Kinect::Kinect>(ft, "Kinect", false);
}
