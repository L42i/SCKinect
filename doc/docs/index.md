# SCKinect: Motion Tracking for Sound Generation

SCKinect is a SuperCollider plugin that bridges physical movement with sound generation. It allows you to use a Kinect sensor to track human body movements and map them to sound parameters in SuperCollider.

!!! warning "CUDA Requirement"
    The current implementation requires CUDA for effective real-time performance. The CPU-only implementation is in progress but not yet fully functional. You will need a CUDA-capable NVIDIA GPU to use this plugin as described in this guide. This is because in order for the current pose estimation model (OpenPose) to achieve efficient performance, you need to GPU accelerate it unless you have a CPU which can process ridiculous amounts of data like a GPU (some CPU's do have capabilities like that and separate cores for doing this processing i.e. Apple Silicon). But for building OpenPose, it is easiest to build it with CUDA. For this reason, alternative methods other than using OpenPose for CPU users are being explored.

## What is SCKinect?

SCKinect integrates multiple technologies to create an interactive system where physical movements can control sound:

1. [**Kinect v2 Sensor**](https://en.wikipedia.org/wiki/Kinect#Kinect_for_Xbox_One_(2013)) - A camera device originally designed for Xbox that can track movements and depth.
2. [**Libfreenect2**](https://github.com/OpenKinect/libfreenect2) - A tool that grabs raw data from the Kinect sensor and makes it available to your computer. It handles the direct communication with the hardware.
3. [**OpenPose**](https://github.com/CMU-Perceptual-Computing-Lab/openpose) - Computer vision software that can detect human body poses from camera images.
4. [**SuperCollider**](https://github.com/supercollider/supercollider) - A platform for audio synthesis and algorithmic composition.

The plugin enables you to map specific body joint positions (like hands, knees, or head) to sound parameters, creating an intuitive and embodied way to generate and control sound.

## Key Features

- Track up to 24 body joints in real-time
- Map any joint's X/Y position to sound parameters
- Choose from different processing pipelines for libfreenect2 (CPU, OpenGL, CUDA, and CUDAKDE)
- Multiple configuration options for tracking quality and performance
- Simple SuperCollider interface for easy integration with your sound design

## Next Steps

See the [Installation](installation.md) page for instructions on installing SCKinect. 
