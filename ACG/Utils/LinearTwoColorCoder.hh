#ifndef LINEARTWOCOLORCODER_HH
#define LINEARTWOCOLORCODER_HH

#include "IColorCoder.hh"

namespace ACG {

class LinearTwoColorCoder : public IColorCoder {
public:
    LinearTwoColorCoder(ACG::Vec4f minColor, ACG::Vec4f maxColor)
        : minColor_(minColor),
          maxColor_(maxColor)
    {}

    ACG::Vec4uc color4(float _v) const override;
    ACG::Vec4f color_float4(float _v) const override;


    float min() const override { return 0.0; }
    float max() const override { return 1.0; }

private:
    const ACG::Vec4f minColor_;
    const ACG::Vec4f maxColor_;
};

} // namespace ACG

#endif // LINEARTWOCOLORCODER_HH
