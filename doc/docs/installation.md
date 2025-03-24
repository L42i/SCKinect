# Installation Guide

Installing SCKinect requires setting up several dependencies before building the plugin. This guide will walk you through each step of the process.

## Requirements

Before installing SCKinect, make sure you have the following:

- A Kinect v2 sensor device
- **CUDA-capable NVIDIA GPU** (required for effective performance)
- CMake (version 3.5 or higher)
- C++ compiler with C++17 support (gcc, clang, or MSVC)
- [SuperCollider](https://github.com/supercollider/supercollider) (with source code)
- [libfreenect2](https://github.com/OpenKinect/libfreenect2)
- [OpenPose](https://github.com/CMU-Perceptual-Computing-Lab/openpose) built with CUDA support (CPU solution other than OpenPose coming soon!)

!!! warning "Hardware Requirements"
    The current implementation requires CUDA for effective real-time body tracking. The CPU-only implementation is in progress but not yet fully functional for real-time applications. For best results, you need:
    
    - NVIDIA GPU with CUDA capability 3.0 or higher
    - At least 8GB of GPU memory (8GB+ recommended because cuDNN uses a lot)
    - Recent NVIDIA drivers

## Step 1: Install SuperCollider

If you don't already have SuperCollider installed:

1. Download and install from the [SuperCollider website](https://supercollider.github.io/downloads)
2. For building SCKinect, you also need the SuperCollider source code:

```bash
git clone https://github.com/supercollider/supercollider.git
```

## Step 2: Install CUDA

SCKinect requires CUDA for effective performance:

1. Download and install the CUDA Toolkit from [NVIDIA's website](https://developer.nvidia.com/cuda-downloads). You actually don't want to download the latest and should aim for 11.6 or 11.7 (for compatibility reasons).
2. Make sure to install the drivers. Try to install the latest one if you can. Although the CUDA toolkit will be an older version than the drivers, you can skip the driver install when installing the toolkit since you've already installed the newer drivers. Do this combination for the best compatibility. Higher CUDA versions will consume too much memory or have compilation issues with OpenPose on Linux, and older GPU drivers won't work on a newer GPU :/.
3. Verify the installation by running:

```bash
nvcc --version
```

## Step 3: Install libfreenect2

[libfreenect2](https://github.com/OpenKinect/libfreenect2) is the library that enables communication with the Kinect device.

```bash
git clone https://github.com/OpenKinect/libfreenect2.git
cd libfreenect2
mkdir build && cd build
cmake .. -DCUDA_PROPAGATE_HOST_FLAGS=off -DENABLE_CUDA=ON
make
sudo make install
```

For detailed installation instructions and troubleshooting, see the [libfreenect2 installation guide](https://github.com/OpenKinect/libfreenect2#installation).

## Step 4: Install OpenPose with CUDA Support

[OpenPose](https://github.com/CMU-Perceptual-Computing-Lab/openpose) is required for body tracking. Ensure you build it with CUDA support. Nothing else works at the moment, but future plans are in progress to support even more pose tracking methods besides OpenPose. For now, OpenPose works best with CUDA though:

```bash
git clone https://github.com/CMU-Perceptual-Computing-Lab/openpose.git
cd openpose
mkdir build && cd build
cmake .. -DGPU_MODE=CUDA -DBUILD_PYTHON=OFF
make -j`nproc`
sudo make install
```

For detailed installation instructions and troubleshooting, see the [OpenPose installation guide](https://github.com/CMU-Perceptual-Computing-Lab/openpose/blob/master/doc/installation/README.md).

!!! important
    Remember the location where you installed OpenPose, as you'll need to reference the models directory when configuring SCKinect.

## Step 5: Build and Install SCKinect

Now you can build the SCKinect plugin:

```bash
git clone https://github.com/L42i/SCKinect.git
cd SCKinect
mkdir build && cd build
```

Configure the build with CMake:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DSC_PATH=/path/to/supercollider/source
```

Or to install directly to your SuperCollider extensions directory:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DSC_PATH=/path/to/supercollider/source -DCMAKE_INSTALL_PREFIX=/path/to/extensions
```

Build and install:

```bash
cmake --build . --config Release
cmake --build . --config Release --target install
```

## Step 6: Verify Installation

1. Connect your Kinect sensor to your computer
2. Start SuperCollider and run:

```supercollider
// Always need to run the server before operating on UGen!
s.boot;

// Now server should be booted and we can run this command
Kinect.findAvailable;
```

This should list your connected Kinect device with its serial number.

## Troubleshooting

### Common Issues

1. **Kinect not detected**
   - Did you boot the server?
   - Make sure your Kinect is properly connected (i.e. loose USB, missing drivers, missing udev rules, etc.)
   - Check if libfreenect2 can detect the device using the libfreenect2 examples

2. **Build errors**
   - Ensure all dependencies are properly installed
   - Check that your compiler supports C++17
   - Verify path to SuperCollider source is correct

3. **OpenPose errors**
   - Make sure the path to OpenPose models is correctly specified when calling `Kinect.configureTracking()`

4. **Performance issues**
   - Ensure you are using the "CUDAKDE" or "CUDA" pipeline for best performance
   - The "CPU" pipeline is still in development and may not work effectively
   - Check your GPU meets the minimum requirements

### Getting Help

If you encounter issues not covered here, check:
- The [GitHub repository](https://github.com/L42i/SCKinect) for open issues
- [SuperCollider forum](https://scsynth.org/) for community support 
