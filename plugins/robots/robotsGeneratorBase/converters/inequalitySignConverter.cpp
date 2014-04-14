#include "inequalitySignConverter.h"

using namespace qReal::robots::generators::converters;

InequalitySignConverter::InequalitySignConverter(QString const &pathToTemplates)
	: EnumConverterBase(pathToTemplates)
{
	addMapping(QString::fromUtf8("less"), "signs/less.t");
	addMapping(QString::fromUtf8("greater"), "signs/greater.t");
	addMapping(QString::fromUtf8("not less"), "signs/notLess.t");
	addMapping(QString::fromUtf8("not greater"), "signs/notGreater.t");
	addMapping(QString::fromUtf8("equals"), "signs/equals.t");
}
