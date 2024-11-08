Kinect : UGen {
	classvar <openDevices, <deviceCount;
	*initClass {
		openDevices = Dictionary.new;
		deviceCount = 0;
	}
	*kr { |minval=0, maxval=1|
		^this.multiNew('control', minval, maxval);
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
	*findAvailable {
		Server.default.sendMsg(\cmd, \findAvailable);
	}
	*openDevice { |serial|
		if (openDevices.includesKey(serial) != true)
		{
			deviceCount = deviceCount + 1;
			openDevices.put(serial, deviceCount);
			Server.default.sendMsg(\cmd, \openDevice, serial);
		}
	}
	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
