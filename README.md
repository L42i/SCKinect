# SCKinect: Motion Tracking for Sound Generation

Author: Evan Murray

## Citation

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.15270938.svg)](https://doi.org/10.5281/zenodo.15270938)
```
@inproceedings{murray_sckinect_2025,
    address = {Johns Hopkins University Bloomberg Center},
    title = {{SCKinect}: {A} {SuperCollider} plugin for the {Kinect}},
    copyright = {Creative Commons Attribution 4.0 International},
    shorttitle = {{SCKinect}},
    url = {https://zenodo.org/doi/10.5281/zenodo.15270938},
    doi = {10.5281/ZENODO.15270938},
    abstract = {SCKinect is a SuperCollider plugin that allows users to interact with a Kinect sensor. Its core implementation contains a unit generator called ’Kinect’, designed to output motion-tracking data to control buses. The plugin also includes commands, facilitating interaction with Kinect devices through the interpreter. The interpreted nature of SuperCollider and the server-language duality allow multimedia enthusiasts to efficiently communicate with technical rendering systems. This is perfect for live performances and interactive installations. With the addition of this plugin, performers can interact with the Kinect directly in SuperCollider with low latency. This paper will cover the implementation of the plugin and its potential applications.},
    language = {en},
    urldate = {2025-04-23},
    publisher = {Zenodo},
    author = {Murray, Evan and von Coler, Henrik},
    month = apr,
    year = {2025},
    note = {Publisher: Zenodo
Version Number: 1.0.0},
    keywords = {Audio-visual presentation, Computer vision, Feedback, Sensory, Machine Learning/classification, Motion Capture},
}
```

## What is This Project?

SCKinect is a bridge between physical movement and sound generation. It allows you to:

1. Track human body movements using a Kinect sensor (a special camera that can detect depth and movement)
2. Use those movements to control sound parameters in SuperCollider (a programming environment for creating audio)

In simpler terms: Move your body, make music!

## Repository Structure

Here's what each folder and file in this project contains:

- `/plugins/Kinect/` - The core code that allows SuperCollider to communicate with the Kinect sensor and OpenPose.
- `/doc/` - Documentation and additional information about the project.
- `/cmake_modules/` - Technical files needed to build the software.

## Documentation

- Full documentation (with installation instructions and examples): [https://l42i.github.io/SCKinect](https://l42i.github.io/SCKinect)
