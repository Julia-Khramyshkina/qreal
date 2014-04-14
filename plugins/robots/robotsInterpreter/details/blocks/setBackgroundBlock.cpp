#include "setBackgroundBlock.h"

using namespace qReal::interpreters::robots::details::blocks;

SetBackgroundBlock::SetBackgroundBlock(robotParts::Display &display)
	: mDisplay(display)
{
}

void SetBackgroundBlock::run()
{
	mDisplay.setBackground(propertyToColor(stringProperty("Color")));
	emit done(mNextBlock);
}

QColor SetBackgroundBlock::propertyToColor(QString const &property) const
{
	if (property == QString::fromUtf8("White")) {
		return QColor(Qt::white);
	} else if (property == QString::fromUtf8("Black")) {
		return QColor(Qt::black);
	} else if (property == QString::fromUtf8("Red")) {
		return QColor(Qt::red);
	} else if (property == QString::fromUtf8("Dark Red")) {
		return QColor(Qt::darkRed);
	} else if (property == QString::fromUtf8("Green")) {
		return QColor(Qt::green);
	} else if (property == QString::fromUtf8("Dark Green")) {
		return QColor(Qt::darkGreen);
	} else if (property == QString::fromUtf8("Blue")) {
		return QColor(Qt::blue);
	} else if (property == QString::fromUtf8("Dark Blue")) {
		return QColor(Qt::darkBlue);
	} else if (property == QString::fromUtf8("Cyan")) {
		return QColor(Qt::cyan);
	} else if (property == QString::fromUtf8("Dark Cyan")) {
		return QColor(Qt::darkCyan);
	} else if (property == QString::fromUtf8("Magenta")) {
		return QColor(Qt::magenta);
	} else if (property == QString::fromUtf8("Dark Magenta")) {
		return QColor(Qt::darkMagenta);
	} else if (property == QString::fromUtf8("Yellow")) {
		return QColor(Qt::yellow);
	} else if (property == QString::fromUtf8("Dark Yellow")) {
		return QColor(Qt::darkYellow);
	} else if (property == QString::fromUtf8("Gray")) {
		return QColor(Qt::gray);
	} else if (property == QString::fromUtf8("Dark Gray")) {
		return QColor(Qt::darkGray);
	} else if (property == QString::fromUtf8("Light Gray")) {
		return QColor(Qt::lightGray);
	}

	return QColor(Qt::transparent);
}
