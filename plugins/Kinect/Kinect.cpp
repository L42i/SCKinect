// PluginKinect.cpp
// Evan Murray (evan@auraaudio.io)

#include "SC_PlugIn.hpp"
#include "Kinect.hpp"

static InterfaceTable* ft;

namespace Kinect {

Kinect::Kinect() : mListener{libfreenect2::Frame::Color} {
    mCalcFunc = make_calc_function<Kinect, &Kinect::next>();
    next(1);
    mPipeline = new libfreenect2::CudaKdePacketPipeline(0);
    mDev = mFreenect2.openDevice(mFreenect2.getDefaultDeviceSerialNumber(), mPipeline);
    std::cout << "device serial: " << mDev->getSerialNumber() << std::endl;
    std::cout << "device firmware: " << mDev->getFirmwareVersion() << std::endl;
}

Kinect::~Kinect() {
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
    ft = inTable;
    registerUnit<Kinect::Kinect>(ft, "Kinect", false);
}
