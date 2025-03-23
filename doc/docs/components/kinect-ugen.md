# Kinect UGen

The `Kinect` UGen is the core of SCKinect, providing real-time access to body tracking data from the Kinect sensor. This component runs as a native SuperCollider UGen (Unit Generator), allowing direct integration with the audio synthesis engine.

!!! warning "CUDA Requirement"
    The current implementation requires CUDA for effective real-time performance. The CPU-only implementation is in progress but not yet fully functional. You will need a CUDA-capable NVIDIA GPU to use this UGen effectively.

## Overview

The Kinect UGen connects to the following components:

1. **libfreenect2** - For raw device communication
2. **OpenPose** - For body pose estimation (via CUDA)
3. **SuperCollider** - For audio control signals

It processes data in real-time, tracking up to 25 different body joints and making their positions available as control signals.

## Technical Implementation

The UGen consists of:

- A C++ class (`Kinect.cpp`, `Kinect.hpp`) handling the low-level communication and processing
- A SuperCollider class (`Kinect.sc`) providing a convenient interface
- Supporting classes for input/output handling (`WUserInput`, `WUserOutput`)

Under the hood, the UGen:

1. Receives raw data from the Kinect sensor through libfreenect2
2. Passes this data to OpenPose for body pose detection (using GPU acceleration)
3. Makes the pose data (joint positions) available as SuperCollider control signals
4. Provides methods for configuring and managing the Kinect connection

## Class Structure

### SuperCollider Class

The `Kinect` class in `Kinect.sc` provides the SuperCollider interface to the UGen. It includes:

- **Class variables**:
  - `openDevices`: Dictionary of connected devices
  - `deviceCount`: Number of connected devices
  - `packetPipelines`: Available processing pipelines (CPU, CUDA, etc.)
  - `jointNames`: Dictionary mapping joint names to indices
  - `jointCoordinates`: Dictionary mapping coordinate names to indices

- **UGen methods**:
  - `kr`: Creates a control rate UGen returning a specific joint coordinate

- **Device management methods**:
  - `findAvailable`: Scans for connected Kinect devices
  - `setPipeline`: Selects the processing pipeline (CUDA/CUDAKDE recommended)
  - `openDevice`: Connects to a specific Kinect device
  - `closeDevice`: Disconnects from a device
  - `start`/`stop`: Controls the Kinect operation

- **Tracking methods**:
  - `configureTracking`: Sets up body tracking parameters
  - `startTracking`/`stopTracking`: Controls the tracking process
  - `showDisplay`/`hideDisplay`: Controls the visual display

### C++ Implementation

The C++ side handles the actual device operation and data processing:

- **KinectData structure**: Maintains the state of connected devices and OpenPose configuration
- **WUserInput class**: Handles frame acquisition from the Kinect sensor
- **WUserOutput class**: Processes the OpenPose output and makes the data available to SuperCollider
- **Command handlers**: Implement the various commands sent from SuperCollider to control the device

## UGen Parameters

### Kinect.kr

```supercollider
Kinect.kr(minval, maxval, joint_name, joint_coordinate)
```

- `minval` (Float): Minimum value in the output range (default: 0)
- `maxval` (Float): Maximum value in the output range (default: 1)
- `joint_name` (String): Name of the joint to track (default: "Nose")
- `joint_coordinate` (String): "X" or "Y" coordinate (default: "X")

The UGen maps the raw joint position (which is in pixel coordinates) to the specified range. This makes it easy to control audio parameters based on body position.

## configureTracking Parameters

The `configureTracking` method accepts a large number of parameters that control the behavior of OpenPose:

```supercollider
Kinect.configureTracking(
  loggingLevel, maxPeople, modelFolder, numGpu,
  gpuStartIndex, numAverages, averageGap, renderPose,
  outputSize, netInputSize, poseMode, poseModel,
  alphaPose, alphaHeatmap, partToShow, renderThreshold,
  fpsMax, upSamplingRatio
)
```

Key parameters include:

- `loggingLevel`: Controls verbosity (0-3, with 3 being most verbose)
- `maxPeople`: Maximum number of people to track (-1 for no limit)
- `modelFolder`: Path to OpenPose models
- `numGpu`/`gpuStartIndex`: GPU settings for OpenPose (crucial for performance)
- `poseModel`: Model to use for tracking (default: "BODY_25")
- `renderThreshold`: Minimum confidence for detecting joints

!!! note "Performance Considerations"
    - The `netInputSize` parameter significantly affects performance. Smaller sizes (like "-1x128") are faster but less accurate.
    - The CUDA and CUDAKDE pipelines require a NVIDIA GPU but provide much better performance than the CPU pipeline.
    - Multi-person tracking (`maxPeople` > 1) requires more GPU resources.

See the [Getting Started](../getting-started.md) guide for more examples of how to use these parameters. 