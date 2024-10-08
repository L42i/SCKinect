#include "WUserInput.hpp"

WUserInput::WUserInput() : mListener{libfreenect2::Frame::Color}
{
    mPipeline = new libfreenect2::CudaKdePacketPipeline(0); // Run on the GPU
    mDev = mFreenect2.openDefaultDevice(mPipeline); // Initialize first Kinect
}

WUserInput::~WUserInput()
{
    mDev->stop();
    mDev->close();
    delete mDev;
    delete mPipeline;
}

void WUserInput::initializationOnThread()
{
    mDev->start();
    mDev->setColorFrameListener(&mListener);
    std::cout << "device serial: " << mDev->getSerialNumber() << std::endl;
    std::cout << "device firmware: " << mDev->getFirmwareVersion() << std::endl;
}

std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>> WUserInput::workProducer()
{
    try
    {
        // Create matrix for input
        cv::Mat rgbmat;

        // Create new datum
        auto datumsPtr = std::make_shared<std::vector<std::shared_ptr<op::Datum>>>();
        datumsPtr->emplace_back();
        auto& datumPtr = datumsPtr->at(0);
        datumPtr = std::make_shared<op::Datum>();

        // Fill the matrix from the Kinect frame
        mListener.waitForNewFrame(mFrames);
        libfreenect2::Frame *rgb = mFrames[libfreenect2::Frame::Color];
        cv::Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);
        cv::cvtColor(rgbmat, rgbmat, cv::COLOR_RGB2BGR);

        // Fill datum
        datumPtr->cvInputData = OP_CV2OPCONSTMAT(rgbmat);

        // If empty frame -> return nullptr
        if (datumPtr->cvInputData.empty())
        {
            op::opLog("Empty frame detected. Closing program.", op::Priority::High);
            this->stop();
            datumsPtr = nullptr;
        }

        // Release frame and return the datum
        mListener.release(mFrames);
        return datumsPtr;
    }
    catch (const std::exception& e)
    {
        this->stop();
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        return nullptr;
    }
}
