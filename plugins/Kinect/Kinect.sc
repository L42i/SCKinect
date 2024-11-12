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
	*closeDevice {
		Server.default.sendMsg(\cmd, \closeDevice);
	}
	*start {
		Server.default.sendMsg(\cmd, \start);
	}
	*stop {
		Server.default.sendMsg(\cmd, \stop);
	}
	*configureTracking {|loggingLevel=3,
	maxPeople= -1,
	modelFolder="",
	numGpu= -1,
	gpuStartIndex=0,
	numAverages=1,
	averageGap=0.25,
	renderPose=0,
	outputSize="-1x-1",
	netInputSize="-1x368",
	poseMode=1,
	poseModel="BODY_25",
	alphaPose=0.5,
	alphaHeatmap=0.5,
	partToShow=0,
	renderThreshold=0.05,
	fpsMax= -1,
	upSamplingRatio=0.0|
		Server.default.sendMsg(
		\cmd, \configureTracking,
		loggingLevel, maxPeople,
		modelFolder, numGpu,
		gpuStartIndex, numAverages,
		averageGap, renderPose,
		outputSize, netInputSize,
		poseMode, poseModel,
		alphaPose, alphaHeatmap,
		partToShow, renderThreshold,
		fpsMax, upSamplingRatio
		);
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
