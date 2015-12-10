
namespace align{

template< class MeshT >
void moveToMean(MeshT& _mesh);

template< class MeshT >
void rotate(MeshT& _mesh);

template< class MeshT >
void moveCenterOfBBToOrigin(MeshT& _mesh);

template< class MeshT >
void scaleToUnitCubeNonUniform(MeshT& _mesh);

template< class MeshT >
void scaleToUnitCubeUniform(MeshT& _mesh);

} // namespace align


#if defined(INCLUDE_TEMPLATES) && !defined(ALIGNT_CC)
#define MALIGNT_TEMPLATES
#include "AlignT.cc"
#endif


