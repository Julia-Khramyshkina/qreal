#include "trikNullificationEncoderGenerator.h"

#include <generatorCustomizer.h>
#include "trikGeneratorFactory.h"

using namespace qReal::robots::generators::simple;

TrikNullificationEncoderGenerator::TrikNullificationEncoderGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "engines/nullifyEncoder.t", QList<Binding *>()
			<< Binding::createMultiTarget("@@PORT@@", "Ports"
					, dynamic_cast<trik::TrikGeneratorFactory *>(customizer.factory())->encodersConverter())
			, parent)
{
}
