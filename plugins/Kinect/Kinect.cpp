// PluginKinect.cpp
// Evan Murray (evan@auraaudio.io)
#include "Kinect.hpp"

// Import OpenPose
#include <openpose/headers.hpp>

static InterfaceTable* ft;

namespace Kinect {
// OpenPose Wrapper
op::WrapperT<op::Datum> opWrapperT;
// Initializing the user custom classes
// Frames producer (e.g., video, webcam, ...)
//auto wUserInput = std::make_shared<WUserInput>();
// GUI (Display)
//auto wUserOutput = std::make_shared<WUserOutput>();

void configureWrapper(op::WrapperT<op::Datum>& opWrapperT)
{
     try
     {
         // Configuring OpenPose

         // logging_level
         const int defaultLoggingLevel = 3; // Varies 0 to 4 for OpenPose (3=fairly important)
         op::checkBool(0 <= defaultLoggingLevel && defaultLoggingLevel <= 255,
                       "Wrong logging_level value.",
                       __LINE__, __FUNCTION__, __FILE__);
         op::ConfigureLog::setPriorityThreshold((op::Priority)defaultLoggingLevel);

         // Add custom input
         const auto workerInputOnNewThread = false;
         opWrapperT.setWorker(op::WorkerType::Input, wUserInput, workerInputOnNewThread);
         // Add custom output
         const auto workerOutputOnNewThread = true;
         opWrapperT.setWorker(op::WorkerType::Output, wUserOutput, workerOutputOnNewThread);

         // Hardcoded stuff (no flags)
         const auto keypointScaleMode = op::ScaleMode::ZeroToOne;
         const int numPeopleMax = -1;
         const auto flagsFolder = op::String("/home/emurray49/openpose/models");
         const int numGpu = -1; // All GPU's used
         const int gpuStartIndex = 0;
         const int numAverages = 1;
         const float averageGap = 0.25;
         const int renderPose = 0; // Don't render anything on the display
         const auto outputSize = op::flagsToPoint(op::String("-1x-1"), "-1x-1");
         const auto netInputSize = op::flagsToPoint(op::String("-1x368"), "-1x368");
         const auto poseMode = op::flagsToPoseMode(1);
         const auto poseModel = op::flagsToPoseModel(op::String("BODY_25"));
         const auto heatMapTypes = op::flagsToHeatMaps(false,
                                                       false,
                                                       false);
         const auto heatMapScaleMode = op::flagsToHeatMapScaleMode(2);
         const bool multipleView = false; // >1 camera view?
         const bool enableGoogleLogging = true;
         // These don't really matter since we're not displaying an image
         const bool disableBlending = true; // Don't render the background (only the skeleton)
         const float alphaPose = 0.0; // Hide the pose (completely transparent)
         const float alphaHeatMap = 0.0; // Hide the heatmap
         const int partToShow = 1; // Don't show any parts
         const bool recordPartCandidates = false; // No, only interested in the final output
         const float renderThreshold = 1.0; // Don't render anything, no matter how confident
         const bool maximizePositives = false; // High standards for accepting candidates = higher precision
         const int fpsMax = -1; // Process frames as quickly as possible
         const auto prototxtPath = op::String(""); // Use default path
         const auto caffeModelPath = op::String(""); // Use default path
         const float upSamplingRatio = 0.0; // Use default upsampling ratio for the output


         // Pose configuration (use WrapperStructPose{} for default and recommended configuration)
         const op::WrapperStructPose wrapperStructPose{
             poseMode, netInputSize, outputSize, keypointScaleMode, numGpu, gpuStartIndex,
             numAverages, averageGap, op::flagsToRenderMode(renderPose, multipleView),
             poseModel, !disableBlending, alphaPose, alphaHeatMap,
             partToShow, flagsFolder, heatMapTypes, heatMapScaleMode, recordPartCandidates,
             renderThreshold, numPeopleMax, maximizePositives, fpsMax,
             prototxtPath, caffeModelPath, upSamplingRatio, enableGoogleLogging
         };
         opWrapperT.configure(wrapperStructPose);

         // Face configuration (use op::WrapperStructFace{} to disable it)
         const op::WrapperStructFace wrapperStructFace{};
         opWrapperT.configure(wrapperStructFace);
         // Hand configuration (use op::WrapperStructHand{} to disable it)
         const op::WrapperStructHand wrapperStructHand{};
         opWrapperT.configure(wrapperStructHand);
         // Extra functionality configuration (use op::WrapperStructExtra{} to disable it)
         const op::WrapperStructExtra wrapperStructExtra{};
         opWrapperT.configure(wrapperStructExtra);
         // Output (comment or use default argument to disable any output)
         const op::WrapperStructOutput wrapperStructOutput{};
         opWrapperT.configure(wrapperStructOutput);
     }
     catch (const std::exception& e)
     {
         op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
     }
}

void cmdStartTracking(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    //opWrapperT.start(); // Start processing OpenPose in the background
}
void cmdStopTracking(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
{
    //opWrapperT.stop(); // Stop processing OpenPose in the background
}

Kinect::Kinect() {
    mCalcFunc = make_calc_function<Kinect, &Kinect::next>();
    next(1);
}

void Kinect::next(int nSamples) {
    // Output buffer
    float* outbuf = out(0);
    float minval = in0(0);
    float maxval = in0(1);
    *outbuf = zapgremlins((maxval - minval) * wUserOutput->controlValue + minval);
}

} // namespace Kinect

PluginLoad(KinectUGens) {
    // Kinect and OpenPose setup
    //Kinect::configureWrapper(Kinect::opWrapperT);

    // Plugin magic
    ft = inTable;
    registerUnit<Kinect::Kinect>(ft, "Kinect", false);

    DefinePlugInCmd("cmdStartTracking", Kinect::cmdStartTracking, (void*)nullptr);
    DefinePlugInCmd("cmdStopTracking", Kinect::cmdStopTracking, (void*)nullptr);
}
