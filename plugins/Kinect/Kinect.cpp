// PluginKinect.cpp
// Evan Murray (evan@auraaudio.io)
#include "Kinect.hpp"

static InterfaceTable* ft;

namespace Kinect {

struct KinectData {
    op::WrapperT<op::Datum> opWrapperT; // OpenPose wrapper
    std::shared_ptr<WUserInput> wUserInput;
    std::shared_ptr<WUserOutput> wUserOutput;
    libfreenect2::Freenect2 mFreenect2;
    libfreenect2::PacketPipeline* mPipeline = new libfreenect2::CpuPacketPipeline();
    libfreenect2::Freenect2Device* mDevice;
    std::string selectedSerial;
    int selectedPipeline = 1;
};

KinectData gKinectData;

enum JointName {
    Nose, Neck, RShoulder, RElbow, RWrist,
    LShoulder, LElbow, LWrist, MidHip, RHip,
    RKnee, RAnkle, LHip, LKnee, LAnkle, REye,
    LEye, REar, LEar, LBigToe, LSmallToe, LHeel,
    RBigToe, RSmallToe, RHeel
};

enum JointCoordinate {
    X, Y
};

bool KinectCmd_setPipeline2(World* world, void* inUserData)
{
    KinectData* kinectData = (KinectData*)inUserData;
    // Check for existing pipeline and delete when new one is set
    if (kinectData->mPipeline != nullptr) {
        delete kinectData->mPipeline;
    }
    switch(kinectData->selectedPipeline) {
        case 0:
            kinectData->mPipeline = new libfreenect2::DumpPacketPipeline();
            break;
        case 1:
            kinectData->mPipeline = new libfreenect2::CpuPacketPipeline();
            break;
        case 2:
            kinectData->mPipeline = new libfreenect2::OpenGLPacketPipeline();
            break;
        case 3:
            kinectData->mPipeline = new libfreenect2::CudaPacketPipeline();
            break;
        case 4:
            kinectData->mPipeline = new libfreenect2::CudaKdePacketPipeline();
            break;
        default:
            break;
    }
    return true;
}

bool KinectCmd_setPipeline3(World* world, void* inUserData) { return true; }

bool KinectCmd_setPipeline4(World* world, void* inUserData) { return true; }

void KinectCmd_setPipelineCleanup(World* world, void* inUserData) {}

void KinectCmd_setPipeline(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->selectedPipeline = args->geti(0);

    int msgSize = args->getbsize();
    char* msgData = 0;
    if (msgSize) {
        msgData = (char*)RTAlloc(inWorld, msgSize);
        args->getb(msgData, msgSize);
    }
    DoAsynchronousCommand(inWorld, replyAddr, "KinectCmd_setPipeline", (void*)&gKinectData, (AsyncStageFn)KinectCmd_setPipeline2,
                          (AsyncStageFn)KinectCmd_setPipeline3, (AsyncStageFn)KinectCmd_setPipeline4, KinectCmd_setPipelineCleanup, msgSize, msgData);
}

void KinectCmd_findAvailable(World* world, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->mFreenect2.enumerateDevices();
}

bool KinectCmd_openDevice2(World* world, void* inUserData)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->mDevice = kinectData->mFreenect2.openDevice(kinectData->selectedSerial, kinectData->mPipeline);
    return true;
}

bool KinectCmd_openDevice3(World* world, void* inUserData) { return true; }

bool KinectCmd_openDevice4(World* world, void* inUserData) { return true; }

void KinectCmd_openDeviceCleanup(World* world, void* inUserData) {}

void KinectCmd_openDevice(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->selectedSerial = args->gets();

    int msgSize = args->getbsize();
    char* msgData = 0;
    if (msgSize) {
        msgData = (char*)RTAlloc(inWorld, msgSize);
        args->getb(msgData, msgSize);
    }
    DoAsynchronousCommand(inWorld, replyAddr, "KinectCmd_openDevice", (void*)&gKinectData, (AsyncStageFn)KinectCmd_openDevice2,
                          (AsyncStageFn)KinectCmd_openDevice3, (AsyncStageFn)KinectCmd_openDevice4, KinectCmd_openDeviceCleanup, msgSize, msgData);

}

void KinectCmd_closeDevice(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->mDevice->close();
}

void KinectCmd_start(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->mDevice->start();
}

void KinectCmd_stop(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->mDevice->stop();
}

void KinectCmd_configureTracking(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    // Initializing the user custom classes
    // Frames producer (e.g., video, webcam, ...)
    kinectData->wUserInput = std::make_shared<WUserInput>();
    kinectData->wUserOutput = std::make_shared<WUserOutput>();
    try
    {
        // Configuring OpenPose

        // logging_level
        const int defaultLoggingLevel = args->geti(3); // Varies 0 to 4 for OpenPose (3=fairly important)
        op::checkBool(0 <= defaultLoggingLevel && defaultLoggingLevel <= 255,
                      "Wrong logging_level value.",
                      __LINE__, __FUNCTION__, __FILE__);
        op::ConfigureLog::setPriorityThreshold((op::Priority)defaultLoggingLevel);

        // Add custom input
        const auto workerInputOnNewThread = false;
        kinectData->opWrapperT.setWorker(op::WorkerType::Input, kinectData->wUserInput, workerInputOnNewThread);
        // Add custom output
        const auto workerOutputOnNewThread = true;
        kinectData->opWrapperT.setWorker(op::WorkerType::Output, kinectData->wUserOutput, workerOutputOnNewThread);

        // Hardcoded stuff (no flags)
        const auto keypointScaleMode = op::ScaleMode::ZeroToOne;
        const int numPeopleMax = args->geti(-1);
        const auto flagsFolder = op::String(args->gets());
        const int numGpu = args->geti(-1);
        const int gpuStartIndex = args->geti(0);
        const int numAverages = args->geti(1);
        const float averageGap = args->getf(0.25);
        const int renderPose = args->geti(0);
        const auto outputSize = op::flagsToPoint(op::String(args->gets()), "-1x-1");
        const auto netInputSize = op::flagsToPoint(op::String(args->gets()), "-1x368");
        const auto poseMode = op::flagsToPoseMode(args->geti(1));
        const auto poseModel = op::flagsToPoseModel(op::String(args->gets()));
        const auto heatMapTypes = op::flagsToHeatMaps(false,
                                                      false,
                                                      false);
        const auto heatMapScaleMode = op::flagsToHeatMapScaleMode(2);
        const bool multipleView = false; // >1 camera view?
        const bool enableGoogleLogging = true;
        const bool disableBlending = false;
        const float alphaPose = args->getf(0.5);
        const float alphaHeatMap = args->getf(0.5);
        const int partToShow = args->geti(0);
        const bool recordPartCandidates = false;
        const float renderThreshold = args->getf(0.05);
        const bool maximizePositives = false;
        const int fpsMax = args->geti(-1); // Process frames as quickly as possible
        const auto prototxtPath = op::String(""); // Use default path
        const auto caffeModelPath = op::String(""); // Use default path
        const float upSamplingRatio = args->getf(0.0); // Use default upsampling ratio for the output

        // Pose configuration (use WrapperStructPose{} for default and recommended configuration)
        const op::WrapperStructPose wrapperStructPose{
            poseMode, netInputSize, outputSize, keypointScaleMode, numGpu, gpuStartIndex,
            numAverages, averageGap, op::flagsToRenderMode(renderPose, multipleView),
            poseModel, !disableBlending, alphaPose, alphaHeatMap,
            partToShow, flagsFolder, heatMapTypes, heatMapScaleMode, recordPartCandidates,
            renderThreshold, numPeopleMax, maximizePositives, fpsMax,
            prototxtPath, caffeModelPath, upSamplingRatio, enableGoogleLogging
        };
        kinectData->opWrapperT.configure(wrapperStructPose);

        // Face configuration (use op::WrapperStructFace{} to disable it)
        const op::WrapperStructFace wrapperStructFace{};
        kinectData->opWrapperT.configure(wrapperStructFace);
        // Hand configuration (use op::WrapperStructHand{} to disable it)
        const op::WrapperStructHand wrapperStructHand{};
        kinectData->opWrapperT.configure(wrapperStructHand);
        // Extra functionality configuration (use op::WrapperStructExtra{} to disable it)
        const op::WrapperStructExtra wrapperStructExtra{};
        kinectData->opWrapperT.configure(wrapperStructExtra);
        // Output (comment or use default argument to disable any output)
        const op::WrapperStructOutput wrapperStructOutput{};
        kinectData->opWrapperT.configure(wrapperStructOutput);
    }
    catch (const std::exception& e)
    {
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}

void KinectCmd_startTracking(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->wUserInput->setDevice(kinectData->mDevice);
    kinectData->opWrapperT.start(); // Start processing OpenPose in the background
}

void KinectCmd_stopTracking(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->opWrapperT.stop();
}

Kinect::Kinect() {
    mCalcFunc = make_calc_function<Kinect, &Kinect::next>();
    next(1);
}

void Kinect::next(int nSamples) {
    // Output buffer
    float* outbuf = out(0);

    // Arguments
    float minval = in0(0);
    float maxval = in0(1);
    JointName jointName = (JointName)in0(2);
    JointCoordinate jointCoordinate = (JointCoordinate)in0(3);

    // Initialization
    float controlValue = 0.0f;
    const auto& poseKeypoints = gKinectData.wUserOutput->poseKeypoints;

    // Handling
    if (poseKeypoints.getSize(0) > 0)
    {
        const auto baseIndex = poseKeypoints.getSize(2)*(jointName);
        const auto coordinate = poseKeypoints[baseIndex + jointCoordinate];
        const auto score = poseKeypoints[baseIndex + 2];
        controlValue = coordinate;
    }
    *outbuf = zapgremlins((maxval - minval) * controlValue + minval);
}

} // namespace Kinect

PluginLoad(KinectUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<Kinect::Kinect>(ft, "Kinect", false);

    DefinePlugInCmd("setPipeline", Kinect::KinectCmd_setPipeline, (void*)&Kinect::gKinectData);
    DefinePlugInCmd("findAvailable", Kinect::KinectCmd_findAvailable, (void*)&Kinect::gKinectData);
    DefinePlugInCmd("openDevice", Kinect::KinectCmd_openDevice, (void*)&Kinect::gKinectData);
    DefinePlugInCmd("closeDevice", Kinect::KinectCmd_closeDevice, (void*)&Kinect::gKinectData);
    DefinePlugInCmd("start", Kinect::KinectCmd_start, (void*)&Kinect::gKinectData);
    DefinePlugInCmd("stop", Kinect::KinectCmd_stop, (void*)&Kinect::gKinectData);
    DefinePlugInCmd("configureTracking", Kinect::KinectCmd_configureTracking, (void*)&Kinect::gKinectData);
    DefinePlugInCmd("startTracking", Kinect::KinectCmd_startTracking, (void*)&Kinect::gKinectData);
    DefinePlugInCmd("stopTracking", Kinect::KinectCmd_stopTracking, (void*)&Kinect::gKinectData);
}
