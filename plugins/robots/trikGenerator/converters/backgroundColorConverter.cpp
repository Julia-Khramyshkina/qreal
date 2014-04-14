#include "backgroundColorConverter.h"

using namespace qReal::robots::generators::converters;

BackgroundColorConverter::BackgroundColorConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping(QString::fromUtf8("White"), "backgroundColors/white.t");
	addMapping(QString::fromUtf8("Black"), "backgroundColors/black.t");
	addMapping(QString::fromUtf8("Red"), "backgroundColors/red.t");
	addMapping(QString::fromUtf8("Dark Red"), "backgroundColors/darkRed.t");
	addMapping(QString::fromUtf8("Green"), "backgroundColors/green.t");
	addMapping(QString::fromUtf8("Dark Green"), "backgroundColors/darkGreen.t");
	addMapping(QString::fromUtf8("Blue"), "backgroundColors/blue.t");
	addMapping(QString::fromUtf8("Dark Blue"), "backgroundColors/darkBlue.t");
	addMapping(QString::fromUtf8("Cyan"), "backgroundColors/cyan.t");
	addMapping(QString::fromUtf8("Dark Cyan"), "backgroundColors/darkCyan.t");
	addMapping(QString::fromUtf8("Magenta"), "backgroundColors/magenta.t");
	addMapping(QString::fromUtf8("Dark Magenta"), "backgroundColors/darkMagenta.t");
	addMapping(QString::fromUtf8("Yellow"), "backgroundColors/yellow.t");
	addMapping(QString::fromUtf8("Dark Yellow"), "backgroundColors/darkYellow.t");
	addMapping(QString::fromUtf8("Gray"), "backgroundColors/gray.t");
	addMapping(QString::fromUtf8("Dark Gray"), "backgroundColors/darkGray.t");
	addMapping(QString::fromUtf8("Light Gray"), "backgroundColors/lightGray.t");
}
