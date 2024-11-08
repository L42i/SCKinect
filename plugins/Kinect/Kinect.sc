Kinect : UGen {
	*kr { |minval=0, maxval=1|
		^this.multiNew('control', minval, maxval)
	}
	*startTracking {
		Server.default.sendMsg(\cmd, \cmdStartTracking);
	}
	*stopTracking {
		Server.default.sendMsg(\cmd, \cmdStopTracking);
	}
	*setPipeline { |pipeline = 1|
		Server.default.sendMsg(\cmd, \setPipeline, pipeline);
	}
	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
