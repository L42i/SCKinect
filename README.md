# SCKinect: Motion Tracking for Sound Generation

Author: Evan Murray

## What is This Project?

SCKinect is a bridge between physical movement and sound generation. It allows you to:

1. Track human body movements using a Kinect sensor (a special camera that can detect depth and movement)
2. Use those movements to control sound parameters in SuperCollider (a programming environment for creating audio)

In simpler terms: Move your body, make music!

## Repository Structure

Here's what each folder and file in this project contains:

- `/plugins/Kinect/` - The core code that allows SuperCollider to communicate with the Kinect sensor and OpenPose.
  - Files like `Kinect.cpp` and `Kinect.sc` define how body tracking data is processed and made available to SuperCollider.

- `/doc/` - Documentation and additional information about the project.

- `/cmake_modules/` - Technical files needed to build the software.

# Documentation

- Full documentation (with installation instructions): [https://l42i.github.io/SCKinect](https://l42i.github.io/SCKinect)
