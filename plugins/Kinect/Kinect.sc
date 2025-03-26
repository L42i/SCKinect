Kinect : UGen {
	classvar <openDevices, <deviceCount, <packetPipelines, <jointNames, <jointCoordinates;
	*initClass {
		openDevices = Dictionary.new;
		packetPipelines = Dictionary.newFrom([
			"Dump", 0, "CPU", 1,
			"OpenGL", 2
		]);
		jointNames = Dictionary.newFrom([
			"Nose", 0, "Neck", 1, "RShoulder", 2,
			"RElbow", 3, "RWrist", 4, "LShoulder", 5,
			"LElbow", 6, "LWrist", 7, "MidHip", 8,
			"RHip", 9, "RKnee", 10, "RAnkle", 11,
			"LHip", 12, "LKnee", 13, "LAnkle", 14,
			"REye", 15, "LEye", 16, "REar", 17,
			"LEar", 18, "LBigToe", 19, "LSmallToe", 20,
			"LHeel", 21, "RBigToe", 22, "RSmallToe", 23,
			"RHeel", 24
		]);
		jointCoordinates = Dictionary.newFrom(["X", 0, "Y", 1]);
		deviceCount = 0;
	}
	*kr { |minval=0, maxval=1, joint_name="Nose", joint_coordinate="X"|
		^this.multiNew('control', minval, maxval, jointNames.at(joint_name), jointCoordinates.at(joint_coordinate));
	}
	*setPipeline { |pipeline = "CPU"|
		Server.default.sendMsg(\cmd, \setPipeline, packetPipelines.at(pipeline));
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
	*closeDevice { |serial|
		deviceCount = deviceCount - 1;
		openDevices.removeAt(serial);
		Server.default.sendMsg(\cmd, \closeDevice, serial);
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
	renderPose= -1,
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
	*showDisplay {
		Server.default.sendMsg(\cmd, \showDisplay);
	}
	*hideDisplay {
		Server.default.sendMsg(\cmd, \hideDisplay);
	}
	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
