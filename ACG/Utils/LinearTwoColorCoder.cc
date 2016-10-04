#include "LinearTwoColorCoder.hh"

namespace ACG {

ACG::Vec4uc LinearTwoColorCoder::color4(float _v) const
{
    ACG::Vec4f c = color_float4(_v);
    return ACG::Vec4uc(
            static_cast<unsigned char>(c[0] * 255 + 0.5),
            static_cast<unsigned char>(c[1] * 255 + 0.5),
            static_cast<unsigned char>(c[2] * 255 + 0.5),
            static_cast<unsigned char>(c[3] * 255 + 0.5));
}

ACG::Vec4f LinearTwoColorCoder::color_float4(float _v) const
{
    return (1 - _v) * minColor_ + _v * maxColor_;
}

} // namespace ACG
