Kinect : UGen {
	*kr { |minval=0, maxval=1|
		^this.multiNew('control', minval, maxval)
	}
	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
