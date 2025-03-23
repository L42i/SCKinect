# Wind Sound Example

This example demonstrates how to use SCKinect to control a wind-like sound using hand movements. The right hand controls the volume (moving up/down) while the left hand controls the tone (moving left/right).

!!! warning "CUDA Requirement"
    This example requires CUDA for effective real-time performance. The CPU-only implementation is in progress but not yet fully functional. You will need a CUDA-capable NVIDIA GPU to run this example as intended.

## Overview

The example creates a simulated wind sound using filtered noise generators. Body tracking data from the Kinect is mapped to control parameters:

- **Right Wrist Y Position**: Controls the amplitude of the wind sound
- **Left Wrist X Position**: Controls the cutoff frequency of a lowpass filter

This creates an intuitive and physical way to "conduct" the wind sound, making it stronger or weaker, brighter or darker through natural hand movements.

## Full Example Code

```supercollider
s.options.numOutputBusChannels = 6;
s.waitForBoot({
	// Setup settings for a Kinect UGen
	Kinect.findAvailable;
	Kinect.setPipeline("CUDAKDE");  // CUDA pipeline is required for real-time performance
	Kinect.openDevice("065915234247");
	s.sync;
	Kinect.start;
	Kinect.configureTracking(
		3, -1, "/home/evanmurray/openpose/models", // Point this to your own models folder.
		-1, 0, 1, 0.25,
		-1, "-1x-1", "-1x256",
		1, "BODY_25", 0.5,
		0.5, 0, 0.05, -1, 0.0
	);
	Kinect.startTracking;

	// Define a SynthDef for a wind sound and send it to the server
	SynthDef(\wind_sound, {
		|out = 0, amplitude = 1, cutoff_frequency = 20000|

		var x = BLowPass4.ar(in: Klank.ar(`[[200, 671, 1153, 1723], nil, [1, 1, 1, 1]],
			PinkNoise.ar(0.004)*amplitude), freq: cutoff_frequency, rq: 0.1, mul: 0.8, add: 0.0);
		var y = BLowPass4.ar(in: BrownNoise.ar(0.2) * amplitude, freq: cutoff_frequency, rq: 0.07, mul: 1.0, add: 0.0);

		Out.ar(out, x);
		Out.ar(out+1, y);
		Out.ar(out+2, x);
		Out.ar(out+3, y);
		Out.ar(out+4, x);
		Out.ar(out+5, y);
	}).send(s);

	// Define some control buses for the Kinect
	~kinectAmplitudeBus = Bus.control(s, 1);
	~kinectFrequencyBus = Bus.control(s, 1);

	// Write the y position of the right wrist detected by the kinect to a control bus to control the amplitude
	{Out.kr(~kinectAmplitudeBus.index, Kinect.kr(0, 0.5, "RWrist", "Y"))}.play;

	// Now write the x position of the left wrist info to a frequency bus to control the lowpass filter
	{Out.kr(~kinectFrequencyBus.index, Kinect.kr(20, 10000, "LWrist", "X"))}.play;

	// Scope the first control bus to see what the output looks like
	s.scope(1, 0, rate: 'control');

	// Wait for the control buses to be created before proceeding
	s.sync;

	// Create a wind sound and send it to speaker #1
	~windSound = Synth(\wind_sound, [\out, 0, \amplitude, 0.5, \cutoff_frequency, 20000]);

	// Map the control buses to the amplitude and frequency
	~windSound.map(\amplitude, ~kinectAmplitudeBus);
	~windSound.map(\cutoff_frequency, ~kinectFrequencyBus);

	s.meter;
})
Kinect.stopTracking;
Kinect.stop;
Kinect.closeDevice("065915234247");
```

## Code Breakdown

Let's break down the example into key sections:

### 1. Server Configuration

```supercollider
s.options.numOutputBusChannels = 6;
s.waitForBoot({
    // Rest of the code...
})
```

This configures the SuperCollider server to use 6 output channels (for surround sound) and waits for the server to boot before executing the rest of the code.

### 2. Kinect Setup

```supercollider
// Setup settings for a Kinect UGen
Kinect.findAvailable;
Kinect.setPipeline("CUDAKDE");  // CUDA pipeline is required for real-time performance
Kinect.openDevice("065915234247");
s.sync;
Kinect.start;
```

This section:
1. Searches for available Kinect devices
2. Sets the processing pipeline to CUDAKDE (GPU-accelerated, required for real-time tracking)
3. Opens a specific Kinect device (you'll need to replace the serial number with your own)
4. Synchronizes with the server
5. Starts the Kinect data acquisition

!!! note
    Note the use of "CUDAKDE" pipeline, which requires a CUDA-capable GPU. The CPU pipeline is not recommended for this example as it may not provide adequate performance.

### 3. Tracking Configuration

```supercollider
Kinect.configureTracking(
    3, -1, "/home/evanmurray/openpose/models", // Point this to your own models folder.
    -1, 0, 1, 0.25,
    -1, "-1x-1", "-1x256",
    1, "BODY_25", 0.5,
    0.5, 0, 0.05, -1, 0.0
);
Kinect.startTracking;
```

This configures OpenPose for body tracking:
1. Sets logging level to 3 (verbose)
2. Tracks unlimited number of people (-1)
3. Points to the OpenPose models folder (you'll need to change this path)
4. Uses default GPU settings
5. Sets up tracking parameters
6. Starts the tracking process

### 4. Wind Sound Synthesis

```supercollider
SynthDef(\wind_sound, {
    |out = 0, amplitude = 1, cutoff_frequency = 20000|

    var x = BLowPass4.ar(in: Klank.ar(`[[200, 671, 1153, 1723], nil, [1, 1, 1, 1]],
        PinkNoise.ar(0.004)*amplitude), freq: cutoff_frequency, rq: 0.1, mul: 0.8, add: 0.0);
    var y = BLowPass4.ar(in: BrownNoise.ar(0.2) * amplitude, freq: cutoff_frequency, rq: 0.07, mul: 1.0, add: 0.0);

    Out.ar(out, x);
    Out.ar(out+1, y);
    Out.ar(out+2, x);
    Out.ar(out+3, y);
    Out.ar(out+4, x);
    Out.ar(out+5, y);
}).send(s);
```

This defines a synthesizer for wind sounds:
1. Creates two types of filtered noise:
   - `x`: Resonant filtered pink noise (for tonal wind components)
   - `y`: Lowpass filtered brown noise (for background wind)
2. Both sounds are controlled by the same parameters:
   - `amplitude`: Overall volume
   - `cutoff_frequency`: Brightness/darkness of the sound
3. Outputs the sounds to all 6 channels

### 5. Control Bus Setup

```supercollider
// Define some control buses for the Kinect
~kinectAmplitudeBus = Bus.control(s, 1);
~kinectFrequencyBus = Bus.control(s, 1);

// Write the y position of the right wrist detected by the kinect to a control bus to control the amplitude
{Out.kr(~kinectAmplitudeBus.index, Kinect.kr(0, 0.5, "RWrist", "Y"))}.play;

// Now write the x position of the left wrist info to a frequency bus to control the lowpass filter
{Out.kr(~kinectFrequencyBus.index, Kinect.kr(20, 10000, "LWrist", "X"))}.play;
```

This sets up control buses to route Kinect data to the synth:
1. Creates two control buses for amplitude and frequency
2. Maps right wrist Y position (up/down) to amplitude (range 0-0.5)
3. Maps left wrist X position (left/right) to filter cutoff frequency (range 20-10000 Hz)

### 6. Creating and Mapping the Synth

```supercollider
// Create a wind sound and send it to speaker #1
~windSound = Synth(\wind_sound, [\out, 0, \amplitude, 0.5, \cutoff_frequency, 20000]);

// Map the control buses to the amplitude and frequency
~windSound.map(\amplitude, ~kinectAmplitudeBus);
~windSound.map(\cutoff_frequency, ~kinectFrequencyBus);
```

This creates the synth and maps the control buses:
1. Creates a wind sound synth with default parameters
2. Maps the amplitude and cutoff_frequency parameters to the control buses

### 7. Cleanup

```supercollider
Kinect.stopTracking;
Kinect.stop;
Kinect.closeDevice("065915234247");
```

This stops tracking, stops the Kinect, and closes the device when you're done.

## Running the Example

To run this example:

1. Replace the serial number (`"065915234247"`) with your own Kinect's serial number
2. Update the path to OpenPose models to match your installation
3. Make sure you have a CUDA-capable GPU installed and properly configured
4. Run the code in SuperCollider
5. Stand in front of the Kinect and move your hands to control the wind sound:
   - Move your right hand up and down to control volume
   - Move your left hand left and right to control the tone

!!! warning "Performance Considerations"
    If you experience lag or stuttering in the tracking:
    - Try reducing the `netInputSize` parameter in `configureTracking()` (e.g., "-1x128")
    - Ensure you're using the "CUDAKDE" pipeline
    - Make sure other GPU-intensive applications are closed
    - Consider upgrading your GPU if problems persist

## Extending the Example

You can extend this example in several ways:

1. Map different body joints to different parameters
2. Add more sound parameters (like reverb, panning, or modulation)
3. Create more complex conditional responses (e.g., specific gestures trigger events)
4. Map multiple people's movements to different instruments 