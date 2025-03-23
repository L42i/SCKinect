# SCKinect: Motion Tracking for Sound Generation

Author: Evan Murray

## What is This Project?

SCKinect is a bridge between physical movement and sound generation. It allows you to:

1. Track human body movements using a Kinect sensor (a special camera that can detect depth and movement)
2. Use those movements to control sound parameters in SuperCollider (a programming environment for creating audio)

In simpler terms: Move your body, make music!

## How It Works

This project consists of several interconnected technologies:

1. **Kinect Sensor** - A camera device originally designed for Xbox that can track movements and depth.
2. **Libfreenect2** - A tool that grabs raw data from the Kinect sensor and makes it available to your computer. It handles the direct communication with the hardware.
3. **OpenPose** - Computer vision software that can detect human body poses from camera images.
4. **SuperCollider** - A platform for audio synthesis and algorithmic composition.
5. **This Plugin** - The software in this repository that connects these technologies together.

## Repository Structure

Here's what each folder and file in this project contains:

- `/plugins/Kinect/` - The core code that allows SuperCollider to communicate with the Kinect sensor and OpenPose.
  - Files like `Kinect.cpp` and `Kinect.sc` define how body tracking data is processed and made available to SuperCollider.

- `/examples/` - Sample code showing how to use this plugin.
  - `Wind.scd` - An example that uses hand movements to control wind-like sounds. Moving your right hand up/down controls volume, while moving your left hand left/right changes the sound's tone.

- `/doc/` - Documentation and additional information about the project.

- `/cmake_modules/` - Technical files needed to build the software.

## Getting Started (For Beginners)

### What You'll Need

1. A Kinect sensor (version 2 or compatible device)
2. A computer with enough processing power to run motion tracking software
3. The software dependencies listed in the "Requirements" section below

### Basic Workflow

Once set up, the typical workflow is:

1. Connect your Kinect to your computer
2. Start SuperCollider
3. Load the SCKinect plugin
4. Write code (or use existing examples) to map body movements to sound parameters
5. Move your body to create and control sounds!

## Requirements

- CMake >= 3.5 (a tool for building software)
- [SuperCollider source code](https://github.com/supercollider/supercollider) (the audio programming platform)
- [libfreenect2](https://github.com/OpenKinect/libfreenect2) (software that helps communicate with the Kinect)
- [OpenPose](https://github.com/CMU-Perceptual-Computing-Lab/openpose) (body tracking software)

## Building the Software

Clone the project:

    git clone https://github.com/L42i/SCKinect.git
    cd SCKinect
    mkdir build
    cd build

Then, use CMake to configure and build it:

    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build . --config Release
    cmake --build . --config Release --target install

You may want to manually specify the install location in the first step to point it at your
SuperCollider extensions directory: add the option `-DCMAKE_INSTALL_PREFIX=/path/to/extensions`.

It's expected that the SuperCollider repo is cloned at `../supercollider` relative to this repo. If
it's not: add the option `-DSC_PATH=/path/to/sc/source`.

## Using the Plugin (Basic Example)

Here's a simple example of what code for this plugin looks like:

```supercollider
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
