#include "waitForColorBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details::blocks;

WaitForColorBlock::WaitForColorBlock(details::RobotModel * const robotModel)
	: WaitForColorSensorBlockBase(robotModel, robots::enums::sensorType::colorFull)
{
}

void WaitForColorBlock::responseSlot(int reading)
{
	QString const targetColor = stringProperty("Color");
	QString color;
	switch (reading) {
	case 1: color = "Black";
		break;
	case 2: color = "Blue";
		break;
	case 3: color = "Green";
		break;
	case 4: color = "Yellow";
		break;
	case 5: color = "Red";
		break;
	case 6: color = "White";
		break;
	default:
		return;
	}

	if (targetColor == color) {
		stop();
	}
}
