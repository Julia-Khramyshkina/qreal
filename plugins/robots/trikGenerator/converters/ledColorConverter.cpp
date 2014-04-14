#include "ledColorConverter.h"

using namespace qReal::robots::generators::converters;

LedColorConverter::LedColorConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping(QString::fromUtf8("Red"), "ledColors/red.t");
	addMapping(QString::fromUtf8("Green"), "ledColors/green.t");
	addMapping(QString::fromUtf8("Off"), "ledColors/off.t");
}
