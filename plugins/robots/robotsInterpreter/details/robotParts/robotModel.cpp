#include "robotModel.h"
#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations;

RobotModel::RobotModel()
	: mRobotImpl(new NullRobotModelImplementation)
	, mBrick(&mRobotImpl->brick())
	, mMotorA(0, &mRobotImpl->motorA())
	, mMotorB(1, &mRobotImpl->motorA())
	, mMotorC(2, &mRobotImpl->motorA())
	, mEncoderA(&mRobotImpl->encoderA(), outputPort::port1)
	, mEncoderB(&mRobotImpl->encoderB(), outputPort::port2)
	, mEncoderC(&mRobotImpl->encoderC(), outputPort::port3)
{
	mSensors.resize(4);
	connect(mRobotImpl, SIGNAL(sensorsConfigured()), this, SLOT(sensorsConfiguredSlot()));
	connect(mRobotImpl, SIGNAL(connected()), this, SLOT(connectedSlot()));
}

RobotModel::~RobotModel()
{
	delete mRobotImpl;
	for (int i = 0; i < 4; ++i)
		delete mSensors[i];
}

robotParts::Brick &RobotModel::brick()
{
	return mBrick;
}

robotParts::TouchSensor *RobotModel::touchSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<robotParts::TouchSensor *>(mSensors[port]);
}

robotParts::SonarSensor *RobotModel::sonarSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<robotParts::SonarSensor *>(mSensors[port]);
}

robotParts::ColorSensor *RobotModel::colorSensor(inputPort::InputPortEnum const &port) const
{
	return dynamic_cast<robotParts::ColorSensor *>(mSensors[port]);
}


robotParts::Sensor *RobotModel::sensor(const inputPort::InputPortEnum &port) const
{
	return mSensors[port];
}

void RobotModel::configureSensors(sensorType::SensorTypeEnum const &port1
		, sensorType::SensorTypeEnum const &port2
		, sensorType::SensorTypeEnum const &port3
		, sensorType::SensorTypeEnum const &port4)
{
	configureSensor(port1, inputPort::port1);
	configureSensor(port2, inputPort::port2);
	configureSensor(port3, inputPort::port3);
	configureSensor(port4, inputPort::port4);
}

void RobotModel::configureSensor(sensorType::SensorTypeEnum const &sensorType
		, inputPort::InputPortEnum const &port)
{
	mRobotImpl->configureSensor(sensorType, port);
}

void RobotModel::sensorsConfiguredSlot()
{
	for (int i = 0; i < 4; ++i) {
		delete mSensors[i];  // Since it deletes a sensor that is exposed to blocks, this method can not be called when diagram is interpreted. Blocks shall be recreated after calling this one.
		mSensors[i] = NULL;
	}
	for (int i = 0; i < 4; ++i) {
		inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(i);
		sensorImplementations::AbstractSensorImplementation const * const sensorImpl = mRobotImpl->sensor(port);
		if (sensorImpl == NULL)
			continue;

		sensorType::SensorTypeEnum const sensorType = mRobotImpl->sensor(port)->type();

		switch (sensorType) {
		case sensorType::unused:
			break;
		case sensorType::touchBoolean:
			mSensors[port] = new robotParts::TouchSensor(mRobotImpl->sensor(port), port);
			break;
		case sensorType::touchRaw:
			break;
		case sensorType::sonar:
			mSensors[port] = new robotParts::SonarSensor(mRobotImpl->sensor(port), port);
			break;
		case sensorType::colorFull:
		case sensorType::colorRed:
		case sensorType::colorGreen:
		case sensorType::colorBlue:
		case sensorType::colorNone:
			mSensors[port] = new robotParts::ColorSensor(mRobotImpl->sensor(port), port);
			break;
		default:
			// TODO: Throw an exception
			break;
		}
	}
	emit sensorsConfigured();
}

bool RobotModel::needsConnection() const
{
	return mRobotImpl->needsConnection();
}

void RobotModel::startInterpretation()
{
	return mRobotImpl->startInterpretation();
}

void RobotModel::connectedSlot(bool success)
{
	emit connected(success);
}

void RobotModel::init()
{
	mRobotImpl->init();
}

void RobotModel::stopRobot()
{
	mRobotImpl->stopRobot();
}

robotParts::Motor &RobotModel::motorA()
{
	return mMotorA;
}

robotParts::Motor &RobotModel::motorB()
{
	return mMotorB;
}

robotParts::Motor &RobotModel::motorC()
{
	return mMotorC;
}

robotParts::EncoderSensor &RobotModel::encoderA()
{
	return mEncoderA;
}

robotParts::EncoderSensor &RobotModel::encoderB()
{
	return mEncoderB;
}

robotParts::EncoderSensor &RobotModel::encoderC()
{
	return mEncoderC;
}

void RobotModel::setRobotImplementation(robotImplementations::AbstractRobotModelImplementation *robotImpl)
{
	disconnect(mRobotImpl, SIGNAL(sensorsConfigured()), this, SLOT(sensorsConfiguredSlot()));
	disconnect(mRobotImpl, SIGNAL(connected()), this, SLOT(connectedSlot()));
	mRobotImpl = robotImpl;
	connect(mRobotImpl, SIGNAL(sensorsConfigured()), this, SLOT(sensorsConfiguredSlot()));
	connect(mRobotImpl, SIGNAL(connected()), this, SLOT(connectedSlot()));

	mMotorA.setImplementation(&mRobotImpl->motorA());
	mMotorB.setImplementation(&mRobotImpl->motorB());
	mMotorC.setImplementation(&mRobotImpl->motorC());
	mEncoderA.setImplementation(&mRobotImpl->encoderA());
	mEncoderB.setImplementation(&mRobotImpl->encoderB());
	mEncoderC.setImplementation(&mRobotImpl->encoderC());

	mBrick.setImplementation(&mRobotImpl->brick());

	for (int i = 0; i < 4; ++i) {
		if (mSensors[i] != NULL) {
			if (mRobotImpl->sensor(static_cast<inputPort::InputPortEnum>(i)))
				mSensors[i]->setImplementation(mRobotImpl->sensor(static_cast<inputPort::InputPortEnum>(i)));
			else {
				delete mSensors[i];
				mSensors[i] = NULL;
			}
		}
	}
}
