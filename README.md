# Kinect

Author: Evan Murray

Interact with a Kinect sensor

### Requirements

- CMake >= 3.5
- [SuperCollider source code](https://github.com/supercollider/supercollider)
- [libfreenect2](https://github.com/OpenKinect/libfreenect2)
- [OpenPose](https://github.com/CMU-Perceptual-Computing-Lab/openpose)

### Building

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

### Developing

Use the command in `regenerate` to update CMakeLists.txt when you add or remove files from the
project. You don't need to run it if you only change the contents of existing files. You may need to
edit the command if you add, remove, or rename plugins, to match the new plugin paths. Run the
script with `--help` to see all available options.

### SuperCollider code example
```
Kinect.findAvailable;
Kinect.setPipeline(4);
Kinect.openDevice("065915234247");
Kinect.start;
Kinect.configureTracking(
	3, 1, "/home/emurray49/openpose/models",
	1, 0, 1, 0.25,
	0, "-1x-1", "-1x256",
	1, "BODY_25", 0.5,
	0.5, 0, 0.05, -1, 0.0
);
Kinect.startTracking;
Kinect.stopTracking;
Kinect.stop;
Kinect.closeDevice;
```
