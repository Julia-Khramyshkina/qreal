#pragma once

#include <simpleGenerators/binding.h>

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class EncoderPortConverter : public simple::Binding::ConverterInterface
{
public:
	QString convert(QString const &data) const override;
};

}
}
}
}
