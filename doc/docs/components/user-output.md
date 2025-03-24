# User Output Component

The `WUserOutput` class is responsible for processing the results of body tracking from OpenPose and making this data available to the Kinect UGen. This component acts as the bridge between the OpenPose body tracking system and SuperCollider.

## Overview

`WUserOutput` is derived from the OpenPose `WorkerConsumer` class, which is designed to consume data processed by OpenPose. In this case, it consumes the body pose data detected from Kinect frames.

## Class Definition

```cpp
// From WUserOutput.hpp
class WUserOutput : public op::WorkerConsumer<std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>>
{
public:
    WUserOutput();
    ~WUserOutput();

    void initializationOnThread();
    void workConsumer(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr);

    op::Array<float> poseKeypoints;
    bool isDisplayEnabled = true;
};
```

## Key Methods

### Constructor and Destructor

```cpp
WUserOutput();
~WUserOutput();
```

These methods handle initialization and cleanup of resources used by the class.

### initializationOnThread

```cpp
void initializationOnThread();
```

This method is called when the OpenPose thread starts. It initializes any necessary resources for processing the output from OpenPose.

### workConsumer

```cpp
void workConsumer(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr);
```

This is the main processing method that:

1. Receives processed `Datum` objects from OpenPose
2. Extracts the body pose keypoints from the data
3. Stores these keypoints for access by the Kinect UGen
4. Optionally displays the processed frames with pose overlays

## Technical Details

### Pose Data Storage

The `poseKeypoints` member variable stores the latest body pose data from OpenPose. This is a 3D array that contains:

- **Person index**: For multiple people tracking (first dimension)
- **Joint index**: For different body parts like hands, shoulders, etc. (second dimension)
- **Coordinate values**: X, Y coordinates and confidence value (third dimension)

The format is:

```
poseKeypoints[person_idx][joint_idx][0] = X coordinate
poseKeypoints[person_idx][joint_idx][1] = Y coordinate
poseKeypoints[person_idx][joint_idx][2] = Confidence (0-1)
```

### Visual Display

The `isDisplayEnabled` flag controls whether the processed frames are displayed in a window. When enabled, this shows the camera view with body pose overlays, which is useful for debugging and visual feedback.

### Data Access Pattern

The `poseKeypoints` variable is accessed by the Kinect UGen to provide real-time pose data to SuperCollider. The UGen maps these raw coordinates to the range specified in the `Kinect.kr` method calls.

### Threading Model

Like `WUserInput`, the `WUserOutput` class runs on a separate thread as part of the OpenPose pipeline. This allows pose data processing to occur in parallel with audio synthesis in SuperCollider.

## Usage

The `WUserOutput` class is not meant to be used directly by SuperCollider code. Instead, it is created and managed by the main Kinect UGen. The UGen sets up the OpenPose pipeline and regularly accesses the pose data from the `WUserOutput` instance.

## Relationship to Other Components

- **Kinect UGen**: Creates and manages the `WUserOutput`, accesses its `poseKeypoints` data
- **WUserInput**: Provides the input frames that are eventually processed and passed to `WUserOutput`
- **OpenPose**: Processes the frames to detect body poses, passing results to `WUserOutput`
- **SuperCollider**: Receives the processed pose data through the Kinect UGen 
