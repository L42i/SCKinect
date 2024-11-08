// KinectStream.hpp
// Evan Murray (evan@auraaudio.io)
#pragma once
#ifndef KINECTSTREAM_HPP
#define KINECTSTREAM_HPP

// SuperCollider plug-in API
#include "SC_PlugIn.hpp"

// Third party API's
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>

namespace KinectStream {

struct KinectData {
    libfreenect2::Freenect2 mFreenect2;
    libfreenect2::Freenect2Device* mDev;
    libfreenect2::PacketPipeline* mPipeline;
    libfreenect2::SyncMultiFrameListener mListener;
    libfreenect2::FrameMap mFrames;
};

KinectData gKinectData;

class KinectStream
{
public:
    KinectStream();
    ~KinectStream()
};

} // namespace KinectStream

#endif // KINECTSTREAM_HPP
