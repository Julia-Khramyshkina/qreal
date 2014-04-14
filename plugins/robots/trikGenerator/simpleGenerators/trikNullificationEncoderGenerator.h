#pragma once

#include <simpleGenerators/bindingGenerator.h>

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'NullifyEncoder' block for TRIK
class TrikNullificationEncoderGenerator: public BindingGenerator
{
public:
	TrikNullificationEncoderGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);
};

}
}
}
}
