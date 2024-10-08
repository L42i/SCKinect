#ifndef WUSEROUTPUT_HPP
#define WUSEROUTPUT_HPP

// Import OpenPose
#include <openpose/flags.hpp>
#include <openpose/headers.hpp>

// Output from the OpenPose model
class WUserOutput : public op::WorkerConsumer<std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>>
{
public:
    void workConsumer(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr);
};

#endif // WUSEROUTPUT_HPP
