#ifndef ACG_COLORCONVERSION_HH
#define ACG_COLORCONVERSION_HH

#include <QColor>
#include <ACG/Math/VectorT.hh>

namespace ACG {

inline Vec4f to_Vec4f(const QColor _color){
  return Vec4f(_color.redF(),
               _color.greenF(),
               _color.blueF(),
               _color.alphaF());
}

} // namespace ACG

#endif // COLORCONVERSION_HH
