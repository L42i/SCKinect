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
    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device* device;
    libfreenect2::PacketPipeline* pipeline;
    libfreenect2::SyncMultiFrameListener listener{libfreenect2::Frame::Color};
    libfreenect2::FrameMap frames;
};

KinectData gKinectData;

} // namespace KinectStream

#endif // KINECTSTREAM_HPP
