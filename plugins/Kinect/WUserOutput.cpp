// WUserOutput.cpp
// Evan Murray (evan@auraaudio.io)
#include "WUserOutput.hpp"

WUserOutput::WUserOutput() {}
WUserOutput::~WUserOutput() {}

void WUserOutput::initializationOnThread() {}
void WUserOutput::workConsumer(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr)
{
    try
    {
        // Check for no output
        if (datumsPtr != nullptr && !datumsPtr->empty())
        {
            poseKeypoints = datumsPtr->at(0)->poseKeypoints;
        }
    }
    catch (const std::exception& e)
    {
        this->stop();
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}
