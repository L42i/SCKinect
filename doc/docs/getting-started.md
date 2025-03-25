# Getting Started with SCKinect

This guide will walk you through the basic steps of setting up and using SCKinect to connect body movement with sound generation.

!!! warning "CUDA Requirement"
    The current implementation requires CUDA for effective real-time performance. The CPU-only implementation is in progress but not yet fully functional. You will need a CUDA-capable NVIDIA GPU to use this plugin as described in this guide. This is because in order for the current pose estimation model (OpenPose) to achieve efficient performance, you need to GPU accelerate it unless you have a CPU which can process ridiculous amounts of data like a GPU (some CPU's do have capabilities like that and separate cores for doing this processing i.e. Apple Silicon). But for building OpenPose, it is easiest to build it with CUDA. For this reason, alternative methods other than using OpenPose for CPU users are being explored.

## Basic Workflow

Working with SCKinect follows this general workflow:

1. **Boot The Server**: Boot the SuperCollider server (scsynth)
2. **Setup**: Find and connect to the Kinect device
3. **Configure**: Set tracking parameters for OpenPose
4. **Track**: Start body tracking
5. **Map**: Map joint positions to sound parameters
6. **Cleanup**: Stop tracking and close the device when done

## Step 1: Setting Up the Kinect

First, make sure your Kinect is connected to your computer. Then, in SuperCollider:

```supercollider
// Boot the server if it's not already running
s.boot;

// Find available Kinect devices
Kinect.findAvailable;
```

This will print the serial number(s) of any connected Kinect devices to the SuperCollider post window.

## Step 2: Connect to the Kinect

Once you have the serial number, you can connect to the device:

```supercollider
// Choose a processing pipeline (CUDA is recommended)
Kinect.setPipeline("CUDAKDE");  // "CUDA" is also an option but "CUDAKDE" looks nice

// Open the device using its serial number
Kinect.openDevice("YOUR_DEVICE_SERIAL");

// Start the device
Kinect.start;
```

!!! note
    Replace `"YOUR_DEVICE_SERIAL"` with the actual serial number of your Kinect device.

## Step 3: Configure Body Tracking

Before you can start tracking, you need to configure the tracking parameters:

```supercollider
Kinect.configureTracking(
  // Basic settings
  loggingLevel: 3,
  maxPeople: 1,
  modelFolder: "/path/to/openpose/models",
  
  // GPU settings
  numGpu: 1,             // Number of GPUs to use
  gpuStartIndex: 0,      // Starting GPU index (usually 0)
  
  // Tracking settings
  numAverages: 1,
  averageGap: 0.25,
  
  // Display settings
  renderPose: 0,
  outputSize: "-1x-1",
  netInputSize: "-1x256",
  
  // Model settings
  poseMode: 1,
  poseModel: "BODY_25",
  alphaPose: 0.5,
  
  // Advanced settings
  alphaHeatmap: 0.5,
  partToShow: 0,
  renderThreshold: 0.05,
  fpsMax: -1,
  upSamplingRatio: 0.0
);
```

!!! important
    The `modelFolder` parameter must point to your OpenPose models directory. This is usually found in the OpenPose installation directory.

## Step 4: Start Tracking

Once configured, you can start tracking:

```supercollider
Kinect.startTracking;
```

If you want to see what the Kinect is seeing:

```supercollider
Kinect.showDisplay;  // Show the camera view with pose overlay
// Later, to hide it:
Kinect.hideDisplay;
```

## Step 5: Use Joint Positions in Your Sound

Now you can use the `Kinect.kr` UGen to map joint positions to sound parameters:

```supercollider
// Create a SynthDef that uses Kinect data
SynthDef(\kinect_controlled_synth, {
  // Map right wrist Y position (up/down) to frequency
  var freq = Kinect.kr(200, 800, "RWrist", "Y");
  
  // Map left wrist X position (left/right) to amplitude
  var amp = Kinect.kr(0, 1, "LWrist", "X");
  
  // Create a sound using these parameters
  var sound = SinOsc.ar(freq) * amp;
  
  // Output the sound
  Out.ar(0, sound ! 2);
}).add;

// Create an instance of the synth
x = Synth(\kinect_controlled_synth);

// Later, to stop it:
x.free;
```

## Step 6: Cleanup

When you're done using the Kinect:

```supercollider
// Stop tracking
Kinect.stopTracking;

// Stop the device
Kinect.stop;

// Close the device
Kinect.closeDevice("YOUR_DEVICE_SERIAL");
```

## Next Steps

Now that you understand the basics, you can also explore the [API Reference](api/kinect-class.md) for more detailed information about all available functions. 
