(
// Set the number of audio channels to match the hardware I/O
s.options.numOutputBusChannels = 48;
s.options.numInputBusChannels = 48;

s.waitForBoot({
	// Define a SynthDef for a wind sound and send it to the server
	SynthDef(\wind_sound,
		{
			// define arguments of the SynthDef
			|out = 0, amplitude = 1|

			// calculate a sine wave with frequency and amplitude
			var x = BrownNoise.ar(amplitude);

			// send the signal to the output bus '0'
			Out.ar(out, x);

	}).send(s);

	// Define a bus for the Kinect
	~kinectBus = Bus.control(s, 1);

	// Write the x position of the mid-hip detected by the kinect from left to right to a control bus
	{Out.kr(~kinectBus.index, Kinect.kr)}.play;

	// Scope this control bus to see what the output looks like
	s.scope(1, 0, rate: 'control');

	s.sync;

	// Create a wind sound and send it to speaker #4
	~windSound = Synth(\wind_sound, [\out, 3, \amplitude, 1]);

	// Map the control bus to the amplitude
	~windSound.map(\amplitude, ~kinectBus);
})
)
