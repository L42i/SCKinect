# C++ Interface

The SCKinect plugin is implemented in C++ and provides a bridge between the Kinect sensor, OpenPose, and SuperCollider. This page documents the C++ components and how they interact with each other.

!!! warning "CUDA Requirement"
    The current implementation requires CUDA for effective real-time performance. The CPU-only implementation is in progress but not yet fully functional. The C++ code assumes CUDA availability for optimal OpenPose operation.

## Key Classes and Structures

### Kinect UGen Class

```cpp
namespace Kinect {
class Kinect : public SCUnit {
public:
    Kinect();
private:
    // Calc function
    void next(int nSamples);
    // Member variables
};
}
```

This is the main UGen class that SuperCollider interfaces with. It:

- Handles the audio rate processing in the `next` method
- Maps joint positions to output values
- Provides the interface for SuperCollider's `Kinect.kr` method

### KinectData Structure

```cpp
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
```

This structure maintains the state of the Kinect connection and OpenPose processing. It contains:

- The OpenPose wrapper
- Input and output handlers for the Kinect frames
- Libfreenect2 instances for Kinect communication
- Maps of connected devices
- Pipeline configuration

While the default pipeline is CPU, this is overridden in practice to use CUDA for effective performance.

### WUserInput Class

```cpp
class WUserInput : public op::WorkerProducer<std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>> {
public:
    WUserInput();
    ~WUserInput();
    void setDevice(libfreenect2::Freenect2Device* device);
    void initializationOnThread();
    std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>> workProducer();
private:
    libfreenect2::Freenect2Device* mDev;
    libfreenect2::SyncMultiFrameListener mListener;
    libfreenect2::FrameMap mFrames;
};
```

This class is responsible for acquiring frames from the Kinect and feeding them to OpenPose. See [User Input](../components/user-input.md) for more details.

### WUserOutput Class

```cpp
class WUserOutput : public op::WorkerConsumer<std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>> {
public:
    WUserOutput();
    ~WUserOutput();
    void initializationOnThread();
    void workConsumer(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr);
    op::Array<float> poseKeypoints;
    bool isDisplayEnabled = true;
};
```

This class processes the output from OpenPose and makes the pose data available to the Kinect UGen. See [User Output](../components/user-output.md) for more details.

## Enumerations

### PacketPipeline Enum

```cpp
enum PacketPipeline {
    Dump, CPU, OpenGL, CUDA, CUDAKDE
};
```

This enumeration defines the different processing pipelines available for the Kinect sensor:

- **Dump**: Minimal processing (for testing)
- **CPU**: CPU-based processing (slower, still in development)
- **OpenGL**: OpenGL-accelerated processing
- **CUDA**: CUDA GPU-accelerated processing  
- **CUDAKDE**: Enhanced CUDA processing with Kernel Density Estimation filtering (recommended)

!!! note
    While all these pipelines are defined, the current implementation works best with CUDA and CUDAKDE options. The CPU pipeline is still in development and may not provide adequate performance for real-time applications.

### JointName Enum

```cpp
enum JointName {
    Nose, Neck, RShoulder, RElbow, RWrist,
    LShoulder, LElbow, LWrist, MidHip, RHip,
    RKnee, RAnkle, LHip, LKnee, LAnkle, REye,
    LEye, REar, LEar, LBigToe, LSmallToe, LHeel,
    RBigToe, RSmallToe, RHeel
};
```

This enumeration defines the indices for different body joints that can be tracked.

### JointCoordinate Enum

```cpp
enum JointCoordinate {
    X, Y
};
```

This enumeration defines the available coordinates for joint positions.

## Command Handlers

The C++ interface exposes several command handlers that are called when SuperCollider sends commands to the plugin:

### Pipeline Commands

```cpp
bool KinectCmd_setPipeline2(World* world, void* inUserData)
void KinectCmd_setPipeline(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
```

These functions handle the `setPipeline` command from SuperCollider, configuring the appropriate libfreenect2 pipeline. The implementation creates the appropriate pipeline object based on the selected type:

```cpp
// Pseudocode from implementation
switch(pipeline) {
    case CUDA:
        kinectData->mPipeline = new libfreenect2::CudaPacketPipeline();
        break;
    case CUDAKDE:
        kinectData->mPipeline = new libfreenect2::CudaKdePacketPipeline();
        break;
    // other cases...
}
```

### Device Commands

```cpp
void KinectCmd_findAvailable(World* world, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
bool KinectCmd_openDevice2(World* world, void* inUserData)
void KinectCmd_openDevice(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
void KinectCmd_closeDevice(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
```

These functions handle device discovery and connection, allowing SuperCollider to find and connect to Kinect devices.

### Tracking Commands

```cpp
void KinectCmd_start(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
void KinectCmd_stop(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
void KinectCmd_configureTracking(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
void KinectCmd_startTracking(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
void KinectCmd_stopTracking(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
void KinectCmd_showDisplay(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
void KinectCmd_hideDisplay(World* inWorld, void* inUserData, struct sc_msg_iter* args, void* replyAddr)
```

These functions handle the various tracking commands, configuring and controlling the OpenPose tracking process. The `configureTracking` command sets up the OpenPose wrapper with GPU configuration:

```cpp
// Pseudocode from implementation
wrapperStructOutput.gpuNumber = numGpu;
wrapperStructOutput.gpuStart = gpuStartIndex;
// ...other configuration
```

## UGen Processing

```cpp
void Kinect::next(int nSamples) {
    // Get the input parameters
    float minval = in0(0);
    float maxval = in0(1);
    int joint = in0(2);
    int coord = in0(3);
    
    // Access poseKeypoints from WUserOutput
    // Map joint position to output range
    // Write output
}
```

The `next` method is called at the control rate to process and output the joint position data. It:

1. Reads the input parameters (min/max range, joint and coordinate indices)
2. Accesses the current pose data from the `WUserOutput`
3. Maps the raw coordinate values to the specified output range
4. Writes the result to the output buffer

## Plugin Registration

```cpp
PluginLoad(KinectUGens) {
    // Register the UGen
    registerUnit<Kinect::Kinect>("Kinect");
    
    // Register command handlers
    DefinePlugInCmd("setPipeline", KinectCmd_setPipeline, KinectCmd_setPipeline2);
    DefinePlugInCmd("findAvailable", KinectCmd_findAvailable);
    // ... additional command registrations
}
```

This section registers the Kinect UGen and all its command handlers with SuperCollider, making them available in the language.

## Implementation Notes

### Thread Safety

The plugin uses a thread-safe design to handle multiple concurrent operations:

- The UGen's `next` method runs in the audio thread
- The OpenPose processing runs in a separate thread
- Communication between threads is managed through thread-safe data structures

### Performance Considerations

- The `CUDAKDE` pipeline provides the best performance but requires CUDA-capable GPU
- Frame rate is affected by the `netInputSize` parameter in `configureTracking`
- Smaller image sizes result in faster processing but potentially less accurate tracking
- The CPU implementation is still in development and not recommended for real-time use

### Hardware Requirements

For optimal performance, the following hardware is recommended:

- NVIDIA GPU with CUDA capability 3.0 or higher
- At least 2GB of GPU memory (4GB+ recommended)
- Recent NVIDIA drivers
- USB 3.0 port for the Kinect sensor

### Memory Management

- The plugin uses smart pointers (`std::shared_ptr`) for safe resource management
- Device connections are tracked in a map to prevent duplicate connections
- Resources are properly cleaned up when devices are disconnected 