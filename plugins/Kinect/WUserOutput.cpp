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
            // Display rendered output image
            const cv::Mat cvMat = OP_OP2CVCONSTMAT(datumsPtr->at(0)->cvOutputData);
            if (isDisplayEnabled && !cvMat.empty())
            {
                cv::imshow(OPEN_POSE_NAME_AND_VERSION + "C++ API", cvMat);
                // Display image and sleeps at least 1 ms (it usually sleeps ~5-10 msec to display the image)
                const char key = (char)cv::waitKey(1);
                if (cv::getWindowProperty(OPEN_POSE_NAME_AND_VERSION + "C++ API", cv::WND_PROP_AUTOSIZE) == -1)
                    isDisplayEnabled = false;
            }
        }
    }
    catch (const std::exception& e)
    {
        this->stop();
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}
