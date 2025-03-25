# Kinect Class API Reference

This page documents all methods and properties of the `Kinect` class in SuperCollider. This class provides the interface between SuperCollider and the Kinect sensor hardware.

## Class Variables

```supercollider
classvar <openDevices, <deviceCount, <packetPipelines, <jointNames, <jointCoordinates;
```

- **openDevices**: Dictionary mapping device serial numbers to device IDs
- **deviceCount**: Number of currently connected devices
- **packetPipelines**: Dictionary mapping pipeline names to internal IDs
- **jointNames**: Dictionary mapping joint names to indices 
- **jointCoordinates**: Dictionary mapping coordinate names to indices

## UGen Methods

### kr

```supercollider
*kr { |minval=0, maxval=1, joint_name="Nose", joint_coordinate="X"| }
```

Creates a control rate UGen that outputs the position of a specific joint.

**Parameters:**

- **minval** (Float): Minimum value of the output range (default: 0)
- **maxval** (Float): Maximum value of the output range (default: 1)
- **joint_name** (String): Name of the joint to track (default: "Nose")
- **joint_coordinate** (String): "X" or "Y" coordinate (default: "X")

**Returns:** A control rate UGen that maps the joint position to the specified range.

**Example:**

```supercollider
// Map the Y position of the right wrist (0-1 range)
~rightHandHeight = Kinect.kr(0, 1, "RWrist", "Y");

// Use it to control the frequency of a sine oscillator (200-800 Hz)
SinOsc.ar(~rightHandHeight);
```

## Device Management Methods

### findAvailable

```supercollider
*findAvailable { }
```

Scans for connected Kinect devices and prints their serial numbers to the post window.

**Example:**

```supercollider
Kinect.findAvailable;
// Posts something like: Found device with serial: 065915234247
```

### setPipeline

```supercollider
*setPipeline { |pipeline = "CPU"| }
```

Sets the processing pipeline used for Kinect data. Note this is different than what OpenPose uses.

**Parameters:**

- **pipeline** (String): Name of the pipeline to use for libfreenect2. Options are:
  - "Dump": Minimal processing (for testing)
  - "CPU": CPU-based processing
  - "OpenGL": OpenGL-accelerated processing
  - "CUDA": CUDA GPU-accelerated processing
  - "CUDAKDE": Enhanced CUDA processing with KDE filtering (recommended)

**Example:**

```supercollider
// Use CUDAKDE pipeline (KDE for Kernel Density Estimation - a fancy filter which makes the depth data look nice)
Kinect.setPipeline("CUDAKDE");
```

### openDevice

```supercollider
*openDevice { |serial| }
```

Opens a connection to a specific Kinect device.

**Parameters:**

- **serial** (String): Serial number of the device to open

**Example:**

```supercollider
Kinect.openDevice("065915234247");
```

### closeDevice

```supercollider
*closeDevice { |serial| }
```

Closes the connection to a specific Kinect device.

**Parameters:**

- **serial** (String): Serial number of the device to close

**Example:**

```supercollider
Kinect.closeDevice("065915234247");
```

### start

```supercollider
*start { }
```

Starts data acquisition from the Kinect device.

**Example:**

```supercollider
Kinect.start;
```

### stop

```supercollider
*stop { }
```

Stops data acquisition from the Kinect device.

**Example:**

```supercollider
Kinect.stop;
```

## Tracking Methods

### configureTracking

```supercollider
*configureTracking {|loggingLevel=3,
maxPeople= -1,
modelFolder="",
numGpu= -1,
gpuStartIndex=0,
numAverages=1,
averageGap=0.25,
renderPose= -1,
outputSize="-1x-1",
netInputSize="-1x368",
poseMode=1,
poseModel="BODY_25",
alphaPose=0.5,
alphaHeatmap=0.5,
partToShow=0,
renderThreshold=0.05,
fpsMax= -1,
upSamplingRatio=0.0| }
```

Configures the OpenPose body tracking system.

**Parameters:**

- **loggingLevel** (Integer): Verbosity level (0-3)
- **maxPeople** (Integer): Maximum number of people to track (-1 for no limit)
- **modelFolder** (String): Path to OpenPose model files
- **numGpu** (Integer): Number of GPUs to use (-1 for auto-detect)
- **gpuStartIndex** (Integer): Index of the first GPU to use
- **numAverages** (Integer): Number of frames to average for smoother tracking
- **averageGap** (Float): Gap between the netInputSize and outputSize which helps transition between the two
- **renderPose** (Integer): Whether to render pose overlay (-1 for auto render on CPU or GPU, 0 for don't render at all, 1 for CPU, 2 for GPU)
- **outputSize** (String): Output frame size in "WxH" format (-1 for native)
- **netInputSize** (String): Network input size in "WxH" format
- **poseMode** (Integer): Pose detection mode
- **poseModel** (String): Model to use (e.g., "BODY_25")
- **alphaPose** (Float): Alpha (transparency) of pose overlay
- **alphaHeatmap** (Float): Alpha of confidence heatmap overlay
- **partToShow** (Integer): Which body part to focus on (0 for all)
- **renderThreshold** (Float): Minimum confidence for a joint to be rendered
- **fpsMax** (Float): Maximum frames per second (-1 for no limit)
- **upSamplingRatio** (Float): Sampling ratio for confidence map

!!! tip "Performance Tip"
    For better performance on lower-end GPUs, try setting `netInputSize` to a smaller value like "-1x128" and limit `maxPeople` to 1 if you're only tracking yourself.

**Example:**

```supercollider
Kinect.configureTracking(
  3, 1, "/path/to/openpose/models",
  1, 0, 1, 0.25,
  0, "-1x-1", "-1x256",
  1, "BODY_25", 0.5,
  0.5, 0, 0.05, -1, 0.0
);
```

### startTracking

```supercollider
*startTracking { }
```

Starts the body tracking process.

**Example:**

```supercollider
Kinect.startTracking;
```

### stopTracking

```supercollider
*stopTracking { }
```

Stops the body tracking process.

**Example:**

```supercollider
Kinect.stopTracking;
```

### showDisplay

```supercollider
*showDisplay { }
```

Shows the visual display with camera feed and pose overlay.

**Example:**

```supercollider
Kinect.showDisplay;
```

### hideDisplay

```supercollider
*hideDisplay { }
```

Hides the visual display.

**Example:**

```supercollider
Kinect.hideDisplay;
```

## Available Joints

The following joint names are available for tracking with the "BODY_25" pose model:

| Joint Name   | Description      | Joint Name   | Description     |
|--------------|------------------|--------------|-----------------|
| "Nose"       | Nose             | "REye"       | Right eye       |
| "Neck"       | Base of neck     | "LEye"       | Left eye        |
| "RShoulder"  | Right shoulder   | "REar"       | Right ear       |
| "RElbow"     | Right elbow      | "LEar"       | Left ear        |
| "RWrist"     | Right wrist      | "LBigToe"    | Left big toe    |
| "LShoulder"  | Left shoulder    | "LSmallToe"  | Left small toe  |
| "LElbow"     | Left elbow       | "LHeel"      | Left heel       |
| "LWrist"     | Left wrist       | "RBigToe"    | Right big toe   |
| "MidHip"     | Middle of hips   | "RSmallToe"  | Right small toe |
| "RHip"       | Right hip        | "RHeel"      | Right heel      |
| "RKnee"      | Right knee       |              |                 |
| "RAnkle"     | Right ankle      |              |                 |
| "LHip"       | Left hip         |              |                 |
| "LKnee"      | Left knee        |              |                 |
| "LAnkle"     | Left ankle       |              |                 |

# Next Steps

Wanna go low level? Check out the [C++ Interface](cpp-interface.md) to better understand how this UGen works thanks to the SuperCollider plugin interface.
