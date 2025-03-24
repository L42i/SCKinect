# User Input Component

The `WUserInput` class is a critical part of SCKinect that handles the acquisition of frames from the Kinect sensor and prepares them for body tracking. This component acts as a bridge between the Kinect hardware (via libfreenect2) and the OpenPose body tracking system.

## Overview

`WUserInput` is derived from the OpenPose `WorkerProducer` class, which is designed to produce data to be processed by OpenPose. In this case, it produces frames from the Kinect sensor.

## Class Definition

```cpp
// From WUserInput.hpp
class WUserInput : public op::WorkerProducer<std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>>
{
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

## Key Methods

### Constructor and Destructor

```cpp
WUserInput();
~WUserInput();
```

The constructor initializes the frame listener, and the destructor ensures proper cleanup of resources.

### setDevice

```cpp
void setDevice(libfreenect2::Freenect2Device* device);
```

This method connects the class to a specific Kinect device. The device is provided by the main Kinect UGen after it has been opened.

### initializationOnThread

```cpp
void initializationOnThread();
```

This method is called when the OpenPose thread starts. It initializes the necessary resources for frame acquisition.

### workProducer

```cpp
std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>> workProducer();
```

This is the main processing method that:

1. Waits for new frames from the Kinect sensor
2. Converts them from the Kinect format to OpenCV format (which OpenPose uses)
3. Creates OpenPose Datum objects with the frames
4. Returns the Datum objects to OpenPose for processing

## Technical Details

### Frame Acquisition

The `WUserInput` class uses a `SyncMultiFrameListener` to receive frames from the Kinect. This listener can handle multiple types of frames:

- **Color (RGB) frames**: Used for visual display and body detection
- **Depth frames**: Can be used for 3D tracking (though not fully implemented in the current version)
- **IR frames**: Infrared data (not used in current implementation)

### Frame Conversion

The frames from the Kinect are in a custom format provided by libfreenect2. The `workProducer` method converts these frames to OpenCV `cv::Mat` format, which is required by OpenPose.

### Threading Model

`WUserInput` runs on a separate thread as part of the OpenPose pipeline. This allows frame acquisition to occur in parallel with body tracking, improving performance.

## Usage

The `WUserInput` class is not meant to be used directly by SuperCollider code. Instead, it is created and managed by the main Kinect UGen. The UGen sets up the OpenPose pipeline and connects the `WUserInput` to the appropriate Kinect device.

## Relationship to Other Components

- **Kinect UGen**: Creates and manages the `WUserInput`
- **WUserOutput**: Receives the processed frames after OpenPose has detected body poses
- **libfreenect2**: Provides the raw frame data from the Kinect sensor
- **OpenPose**: Processes the frames to detect body poses 
