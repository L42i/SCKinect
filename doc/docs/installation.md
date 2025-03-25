# Installation Guide

## Step 1: Install SuperCollider

1. Download and install from the [SuperCollider website](https://supercollider.github.io/downloads)
2. For building SCKinect, you also need the SuperCollider source code:

```bash
git clone https://github.com/supercollider/supercollider.git
```

## Step 2: Install CUDA

SCKinect requires CUDA for effective performance:

1. Download and install the CUDA Toolkit from [NVIDIA's website](https://developer.nvidia.com/cuda-downloads). You actually don't want to download the latest and should aim for 11.6 or 11.7 (for compatibility reasons).
2. Make sure to install the drivers. Try to install the latest one if you can. Although the CUDA toolkit will be an older version than the drivers, you can skip the driver install when installing the toolkit since you've already installed the newer drivers. Do this combination for the best compatibility. Higher CUDA versions will consume too much memory or have compilation issues with OpenPose on Linux, and older GPU drivers won't work on a newer GPU :/.
3. Next, you should install cuDNN which is a neural net extension to CUDA: https://developer.nvidia.com/cudnn. The install process is pretty similar to that of the CUDA install. Follow the instructions on the Nvidia website.
4. Clone the CUDA samples and make note of where the samples folder is located. Make sure to also checkout the version tag which matches the version of your CUDA install. If they don't match, try to pick the closest version below (i.e. 11.6 if you have CUDA 11.7)

```bash
git clone https://github.com/NVIDIA/cuda-samples.git
cd cuda-samples
git checkout tags/v11.6
```

5. Copy this folder to your CUDA toolkit folder

```bash
sudo cp -r cuda-samples /usr/local/cuda-11.7
```

6. Verify the installation by running:

```bash
nvcc --version
```

## Step 3: Install libfreenect2

[libfreenect2](https://github.com/OpenKinect/libfreenect2) is the library that enables communication with the Kinect device.

```bash
git clone https://github.com/OpenKinect/libfreenect2.git
cd libfreenect2
```

Before you install this, you'll want to edit the `CMakeLists.txt` file in the root directory, `libfreenect2` to include the CUDA samples folder downloaded in the previous step.

Change the line `"${CUDA_TOOLKIT_ROOT_DIR}/samples/common/inc"` to `"${CUDA_TOOLKIT_ROOT_DIR}/cuda-samples/Common"`. This will allow CMake to find the include files for these which are very important. Ever since CUDA 11.6, the samples are not shipped with the toolkit which is why we need to install them separately. Otherwise you will get some error about missing a math_helper.h file and a bunch of other files. If you have a version of CUDA which ships with the samples, then you don't need to worry about this step.

Now once that's done, begin installing `libfreenect2`.

```bash
sudo apt-get install libusb-1.0-0-dev libturbojpeg0-dev libglfw3-dev
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr
make -j`nproc`
sudo make install
sudo ldconfig
```

For detailed installation instructions and troubleshooting, see the [libfreenect2 installation guide](https://github.com/OpenKinect/libfreenect2#installation).

## Step 4: Install OpenPose with CUDA Support

[OpenPose](https://github.com/CMU-Perceptual-Computing-Lab/openpose) is required for body tracking. Ensure you build it with CUDA support. Nothing else works at the moment, but future plans are in progress to support even more pose tracking methods besides OpenPose. For now, OpenPose works best with CUDA though.

!!! important
    Remember the location where you installed OpenPose, as you'll need to reference the models directory when configuring SCKinect.

Because the link to the OpenPose models isn't working in the original repo, a fork is linked in the clone command below with working downloads for these models. Thanks Alec Dusheck!
```bash
git clone https://github.com/AlecDusheck/openpose.git
cd openpose
sudo apt-get install libopencv-dev && sudo bash ./scripts/ubuntu/install_deps.sh
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DGPU_MODE=CUDA -DBUILD_PYTHON=OFF
make -j`nproc`
sudo make install
sudo ldconfig
```

For detailed installation instructions and troubleshooting, see the [OpenPose installation guide](https://github.com/CMU-Perceptual-Computing-Lab/openpose/blob/master/doc/installation/README.md).

## Step 5: Build and Install SCKinect

Now you can build the SCKinect plugin:

```bash
git clone https://github.com/L42i/SCKinect.git
cd SCKinect
mkdir build && cd build
```

Configure the build with CMake:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DSC_PATH=/path/to/supercollider/source -DCMAKE_INSTALL_PREFIX=/path/to/supercollider/extensions
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

This should list your connected Kinect device with its serial number in the post window.

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
   - Ensure you are using the right netResolution and only tracking as many people as you need to

### Getting Help

If you encounter issues not covered here, check:
- The [GitHub repository](https://github.com/L42i/SCKinect) for open issues
