#ifndef ACG_ICOLORCODER_HH
#define ACG_ICOLORCODER_HH

#include <ACG/Math/VectorT.hh>
#include <ACG/Config/ACGDefines.hh>
#include <QColor>

namespace ACG {

class IColorCoder {
public:
    virtual ~IColorCoder() = default;

    virtual ACG::Vec4uc color4(float _v) const = 0;
    virtual ACG::Vec4f color_float4(float _v) const = 0;

    /// min scalar value
    virtual float min() const = 0;

    /// max scalar value
    virtual float max() const = 0;

    inline ACG::Vec3uc color(float _v) const {
        ACG::Vec4uc c = color4(_v);
        return ACG::Vec3uc(c[0], c[1], c[2]);
    }

    inline ACG::Vec3f color_float(float _v) const
    {
        ACG::Vec4f c = color_float4(_v);
        return ACG::Vec3f(c[0], c[1], c[2]);
    }

    inline QColor color_qcolor(float _v) const
    {
        ACG::Vec4uc c = color4(_v);
        return QColor(c[0], c[1], c[2], c[3]);
    }

    // Make the color coder usable as a function operator.
    inline ACG::Vec4f operator() (float _v) const {
        return color_float4(_v);
    }
};

} // namespace ACG

#endif // ICOLORCODER_HH
