#include "colorConverter.h"

using namespace qReal::robots::generators::converters;

ColorConverter::ColorConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping(QString::fromUtf8("Red"), "colors/red.t");
	addMapping(QString::fromUtf8("Green"), "colors/green.t");
	addMapping(QString::fromUtf8("Blue"), "colors/blue.t");
	addMapping(QString::fromUtf8("Black"), "colors/black.t");
	addMapping(QString::fromUtf8("Yellow"), "colors/yellow.t");
	addMapping(QString::fromUtf8("White"), "colors/white.t");
}
