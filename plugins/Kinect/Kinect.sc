Kinect : UGen {
	classvar <openDevices, <deviceCount;
	*initClass {
		openDevices = Dictionary.new;
		deviceCount = 0;
	}
	*kr { |minval=0, maxval=1|
		^this.multiNew('control', minval, maxval);
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
	*start {
		Server.default.sendMsg(\cmd, \start);
	}
	*startTracking {
		Server.default.sendMsg(\cmd, \startTracking);
	}
	*stopTracking {
		Server.default.sendMsg(\cmd, \stopTracking);
	}
	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
