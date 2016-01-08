
namespace align{

/** \brief Move Mesh into center of gravity
 *
 * @param _mesh Mesh to modify
 */
template< class MeshT >
void moveToCOG(MeshT& _mesh);

/** \brief Rotate a mesh such that its largest PCA axis is aligned to x and second largest aligned to y
 *
 * @param _mesh Mesh to modify
 */
template< class MeshT >
void rotate(MeshT& _mesh);

/** \brief Move mesh such that its bounding box center is in the origin
 *
 * @param _mesh Mesh to modify
 */
template< class MeshT >
void moveCenterOfBBToOrigin(MeshT& _mesh);

} // namespace align


#if defined(INCLUDE_TEMPLATES) && !defined(ALIGNT_CC)
#define MALIGNT_TEMPLATES
#include "AlignT.cc"
#endif


