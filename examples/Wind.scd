s.options.numOutputBusChannels = 6;
s.waitForBoot({
	// Setup settings for a Kinect UGen
	Kinect.findAvailable;
	Kinect.setPipeline(4);
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
Kinect.closeDevice;
