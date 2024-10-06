// PluginKinect.hpp
// Evan Murray (evan@auraaudio.io)

#pragma once

#include "SC_PlugIn.hpp"

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
