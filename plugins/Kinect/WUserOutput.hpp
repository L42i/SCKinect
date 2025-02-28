// WUserOutput.hpp
// Evan Murray (evan@auraaudio.io)
#pragma once
#ifndef WUSEROUTPUT_HPP
#define WUSEROUTPUT_HPP

// Import OpenPose
#include <openpose/headers.hpp>

// Output from the OpenPose model
class WUserOutput : public op::WorkerConsumer<std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>>
{
public:
    WUserOutput();
    ~WUserOutput();

    void initializationOnThread(); // Runs to initialize the thread
    void workConsumer(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr); // Process an output frame

    op::Array<float> poseKeypoints;
};

#endif // WUSEROUTPUT_HPP
