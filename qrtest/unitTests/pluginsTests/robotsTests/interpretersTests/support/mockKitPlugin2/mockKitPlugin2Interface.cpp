#include "mockKitPlugin2Interface.h"

using namespace qrTest::mockKitPlugin2;

QString MockKitPlugin2Interface::kitId() const
{
	return "MockKitPlugin2";
}

QString MockKitPlugin2Interface::friendlyKitName() const
{
	return "Mock Kit Plugin 2";
}

QList<interpreterBase::robotModel::RobotModelInterface *> MockKitPlugin2Interface::robotModels()
{
	return QList<interpreterBase::robotModel::RobotModelInterface *>() << &mRobotModel;
}

interpreterBase::AdditionalPreferences *MockKitPlugin2Interface::settingsWidget()
{
	return nullptr;
}

qReal::IdList MockKitPlugin2Interface::unsupportedBlocks() const
{
	return {};
}