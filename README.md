# SCKinect: Motion Tracking for Sound Generation

Author: Evan Murray

- Full documentation (with installation instructions): [https://l42i.github.io/SCKinect](https://l42i.github.io/SCKinect)
- Old installation instructions (helpful for debugging): [https://emurray2.github.io/spatial-auditory-feedback/kinect/](https://emurray2.github.io/spatial-auditory-feedback/kinect/)
- **Note**: Ubuntu 20.04 and CUDA 11.7 Toolkit (with NVIDIA GPU) currently recommended for building OpenPose, but CPU support + Windows + macOS coming soon :) ! Can experiment with other stuff, but it hasn't specifically been tested with this project so it might not work :/ . See [OpenPose Installation](https://github.com/CMU-Perceptual-Computing-Lab/openpose/blob/master/doc/installation/0_index.md) for more details on what specific setup works best (including hardware and software).

## What is This Project?

SCKinect is a bridge between physical movement and sound generation. It allows you to:

1. Track human body movements using a Kinect sensor (a special camera that can detect depth and movement)
2. Use those movements to control sound parameters in SuperCollider (a programming environment for creating audio)

In simpler terms: Move your body, make music!

## How It Works

This project consists of several interconnected technologies:

1. [**Kinect v2 Sensor**](https://en.wikipedia.org/wiki/Kinect#Kinect_for_Xbox_One_(2013)) - A camera device originally designed for Xbox that can track movements and depth.
2. [**Libfreenect2**](https://github.com/OpenKinect/libfreenect2) - A tool that grabs raw data from the Kinect sensor and makes it available to your computer. It handles the direct communication with the hardware.
3. [**OpenPose**](https://github.com/CMU-Perceptual-Computing-Lab/openpose) - Computer vision software that can detect human body poses from camera images.
4. [**SuperCollider**](https://github.com/supercollider/supercollider) - A platform for audio synthesis and algorithmic composition.
5. **This Plugin** - The software in this repository that connects these technologies together.

## Repository Structure

Here's what each folder and file in this project contains:

- `/plugins/Kinect/` - The core code that allows SuperCollider to communicate with the Kinect sensor and OpenPose.
  - Files like `Kinect.cpp` and `Kinect.sc` define how body tracking data is processed and made available to SuperCollider.

- `/examples/` - Sample code showing how to use this plugin.
  - `Wind.scd` - An example that uses hand movements to control wind-like sounds. Moving your right hand up/down controls volume, while moving your left hand left/right changes the sound's tone.

- `/doc/` - Documentation and additional information about the project.

- `/cmake_modules/` - Technical files needed to build the software.

## Using the Plugin (Basic Example)

Here's a simple example of what code for this plugin looks like:

```supercollider
// Boot the server (needs to be running for the whole UGen to work)
s.boot;
// Find and set up your Kinect
Kinect.findAvailable;  // Searches for connected Kinect devices
Kinect.setPipeline("CUDAKDE");  // Sets processing method (for faster tracking)
Kinect.openDevice("065915234247");  // Opens a specific Kinect (your ID will differ)
Kinect.start;  // Starts the Kinect

// Configure body tracking
Kinect.configureTracking(
	3, 1, "/path/to/openpose/models",  // Important: point to your models folder
	1, 0, 1, 0.25,
	0, "-1x-1", "-1x256",
	1, "BODY_25", 0.5,
	0.5, 0, 0.05, -1, 0.0
);
Kinect.startTracking;  // Begin tracking body movements

// Later, when you're done:
Kinect.stopTracking;
Kinect.stop;
Kinect.closeDevice("065915234247");  // Close the Kinect
```

Check the `examples` folder for more complete demonstrations.
