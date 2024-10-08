// PluginKinect.hpp
// Evan Murray (evan@auraaudio.io)
#pragma once
#ifndef KINECT_H
#define KINECT_H
#include "WUserInput.hpp"
#include "WUserOutput.hpp"

// SuperCollider plug-in API
#include "SC_PlugIn.hpp"

namespace Kinect {

class Kinect : public SCUnit {
public:
    Kinect();

private:
    // Calc function
    void next(int nSamples);

    // Member variables
};

} // namespace Kinect

#endif // KINECT_H
