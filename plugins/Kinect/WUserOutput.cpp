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
            // Get the mid-hip for person 0 from the BODY_25 model.
            // See: https://cmu-perceptual-computing-lab.github.io/openpose/web/html/doc/md_doc_02_output.html#pose-output-format-body_25
            const auto& poseKeypoints = datumsPtr->at(0)->poseKeypoints;
            if (poseKeypoints.getSize(0) > 0)
            {
                const auto baseIndex = poseKeypoints.getSize(2)*(8);
                const auto x = poseKeypoints[baseIndex];
                const auto y = poseKeypoints[baseIndex + 1];
                const auto score = poseKeypoints[baseIndex + 2];
                op::opLog("X: " + std::to_string(x));
                op::opLog("Y: " + std::to_string(y));
                op::opLog("Score: " + std::to_string(score));
            }
        }
    }
    catch (const std::exception& e)
    {
        this->stop();
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}
