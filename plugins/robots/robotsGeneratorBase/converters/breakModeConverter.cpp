#include "breakModeConverter.h"

using namespace qReal::robots::generators::converters;

BreakModeConverter::BreakModeConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping("", "engines/breakMode/break.t");
	addMapping(QString::fromUtf8("break"), "engines/breakMode/break.t");
	addMapping(QString::fromUtf8("float"), "engines/breakMode/float.t");
}
