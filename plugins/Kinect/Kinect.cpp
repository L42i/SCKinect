// PluginKinect.cpp
// Evan Murray (evan@auraaudio.io)
#include "Kinect.hpp"

static InterfaceTable* ft;

namespace Kinect {

enum PacketPipeline {
    Dump, CPU, OpenGL
};

struct KinectData {
    op::WrapperT<op::Datum> opWrapperT; // OpenPose wrapper
    std::shared_ptr<WUserInput> wUserInput;
    std::shared_ptr<WUserOutput> wUserOutput;
    libfreenect2::Freenect2 mFreenect2;
    libfreenect2::PacketPipeline* mPipeline = new libfreenect2::CpuPacketPipeline();
    std::unordered_map<std::string, libfreenect2::Freenect2Device*> mOpenDevices;
    std::string mSelectedSerial;
    PacketPipeline selectedPipeline = CPU;
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
        case Dump:
            kinectData->mPipeline = new libfreenect2::DumpPacketPipeline();
            break;
        case CPU:
            kinectData->mPipeline = new libfreenect2::CpuPacketPipeline();
            break;
        case OpenGL:
            kinectData->mPipeline = new libfreenect2::OpenGLPacketPipeline();
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
    kinectData->selectedPipeline = (PacketPipeline)args->geti(0);

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
    std::string serialNumber = kinectData->mSelectedSerial;
    kinectData->mOpenDevices[serialNumber] = kinectData->mFreenect2.openDevice(serialNumber, kinectData->mPipeline);
    return true;
}

bool KinectCmd_openDevice3(World* world, void* inUserData) { return true; }

bool KinectCmd_openDevice4(World* world, void* inUserData) { return true; }

void KinectCmd_openDeviceCleanup(World* world, void* inUserData) {}

void KinectCmd_openDevice(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->mSelectedSerial = args->gets();

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
    kinectData->mSelectedSerial = args->gets();
    kinectData->mOpenDevices.at(kinectData->mSelectedSerial)->close();
    kinectData->mOpenDevices.erase(kinectData->mSelectedSerial);
}

void KinectCmd_start(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->mOpenDevices.at(kinectData->mSelectedSerial)->start();
}

void KinectCmd_stop(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->mOpenDevices.at(kinectData->mSelectedSerial)->stop();
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
        double netInputSizeDynamicBehavior = -1; // * Zero or negative means that using `-1` in netInputSize will behave as explained in its flag description.
        const int numPeopleMax = args->geti(-1);
        const auto flagsFolder = op::String(args->gets());
        const int numGpu = args->geti(-1);
        const int gpuStartIndex = args->geti(0);
        const int numAverages = args->geti(1);
        const float averageGap = args->getf(0.25);
        const int renderPose = args->geti(-1);
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
            poseMode, netInputSize, netInputSizeDynamicBehavior, outputSize, keypointScaleMode, numGpu, gpuStartIndex,
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
    kinectData->wUserInput->setDevice(kinectData->mOpenDevices.at(kinectData->mSelectedSerial));
    kinectData->opWrapperT.start(); // Start processing OpenPose in the background
}

void KinectCmd_stopTracking(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->opWrapperT.stop();
}

void KinectCmd_showDisplay(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->wUserOutput->isDisplayEnabled = true;
}

void KinectCmd_hideDisplay(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    KinectData* kinectData = (KinectData*)inUserData;
    kinectData->wUserOutput->isDisplayEnabled = false;
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
    DefinePlugInCmd("showDisplay", Kinect::KinectCmd_showDisplay, (void*)&Kinect::gKinectData);
    DefinePlugInCmd("hideDisplay", Kinect::KinectCmd_hideDisplay, (void*)&Kinect::gKinectData);
}
