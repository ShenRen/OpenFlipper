
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

/** \brief Scale mesh such that it has length 1 on each separate axis
 *
 * @param _mesh Mesh to modify
 */
template< class MeshT >
void scaleToUnitCubeNonUniform(MeshT& _mesh);

/** \brief Scale mesh such that it has length 1 on the largest axis (keeps ratios of axis)
 *
 * @param _mesh Mesh to modify
 */
template< class MeshT >
void scaleToUnitCubeUniform(MeshT& _mesh);

} // namespace align


#if defined(INCLUDE_TEMPLATES) && !defined(ALIGNT_CC)
#define MALIGNT_TEMPLATES
#include "AlignT.cc"
#endif


