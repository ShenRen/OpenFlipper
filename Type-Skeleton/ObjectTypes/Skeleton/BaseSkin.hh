/*===========================================================================*\
*                                                                            *
*                              OpenFlipper                                   *
 *           Copyright (c) 2001-2015, RWTH-Aachen University                 *
 *           Department of Computer Graphics and Multimedia                  *
 *                          All rights reserved.                             *
 *                            www.openflipper.org                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * This file is part of OpenFlipper.                                         *
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Redistribution and use in source and binary forms, with or without        *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 * 1. Redistributions of source code must retain the above copyright notice, *
 *    this list of conditions and the following disclaimer.                  *
 *                                                                           *
 * 2. Redistributions in binary form must reproduce the above copyright      *
 *    notice, this list of conditions and the following disclaimer in the    *
 *    documentation and/or other materials provided with the distribution.   *
 *                                                                           *
 * 3. Neither the name of the copyright holder nor the names of its          *
 *    contributors may be used to endorse or promote products derived from   *
 *    this software without specific prior written permission.               *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED *
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        *
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      *
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              *
*                                                                            *
\*===========================================================================*/

/*===========================================================================*\
*                                                                            *
*   $Revision$                                                       *
*   $LastChangedBy$                                                *
*   $Date$                     *
*                                                                            *
\*===========================================================================*/

#ifndef BASESKIN_HH
#define BASESKIN_HH

#include <map>

#include "OpenFlipper/common/perObjectData.hh"

#include <ObjectTypes/Skeleton/SkeletonT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

#define OBJECTDATA_SKIN   "Skin Object-Data"
#define SKIN_WEIGHTS_PROP "skin-weights"
#define DEFAULTPOSE_PROP  "Default pose"

/**
 * @brief Abstract base class for the skin template, wrapping all template versions of the skin
 */
class BaseSkin : public PerObjectData
{

public:

  /**
   * @name Properties
   * These structures store mesh properties used to make the mesh a skin.
   */
  //@{
  /**
   * @brief Holds the skins default pose
   *
   * The default pose of the mesh is stored in a vertex property. From this pose all deformed poses are
   * derived. An additional point and normal are stored per vertex.
   */
  struct DefaultPose
  {
    DefaultPose() {};
    DefaultPose(OpenMesh::Vec3d point, OpenMesh::Vec3d normal) : point(point), normal(normal) {};

    OpenMesh::Vec3d point;  ///< The points position in the default pose
    OpenMesh::Vec3d normal; ///< The points normal in the default pose
  };
  
  /**
   * @brief Stores the joint weights per vertex
   *
   * In general every joint can influence every vertex. In practice only a small number of joints manipulate
   * a vertex. Therefore they are stored in a map using the joint identifier as key. The weights for one
   * vertex are supposed to sum to one.
   */
    typedef std::map<unsigned int, double> SkinWeights;
  //@}
  
  /**
   * @brief Possible deformation methods
   *
   * LBS = Linear Blend Skinning
   * DBS = Dual Quaternion Blend Skinning
   *
   */
  enum Method
  {
    M_LBS = 0,
    M_DBS = 1
  };

public:
  BaseSkin(int _skeletonId) {
    skeleton_ = _skeletonId;
  };

  ~BaseSkin() {};

public:
  /**
   * @name Skinning
   * These methods are related to using a mesh as skin with this skeleton.
   */
  //@{
  virtual void attachSkin() = 0;
  virtual void deformSkin() = 0;
  virtual void deformSkin(const AnimationHandle &_hAni, Method _method = M_LBS) = 0;
  virtual void releaseSkin() = 0;
  //@}

  /**
   * @name Information about the attached skeleton
   */
  //@{
  int skeletonId() {return skeleton_;};

  //@}

private:
  /// Holds the associated skeleton
  int skeleton_;
};


// ----------------------------------------------------------------------------
// support persistence for struct Weights
// ----------------------------------------------------------------------------

namespace OpenMesh {
  namespace IO {

    template <> struct binary<BaseSkin::SkinWeights>
    {
      typedef BaseSkin::SkinWeights value_type;

      static const bool is_streamable = true;

      // return binary size of the value

      static size_t size_of(void) 
      { 
        return UnknownSize;
      }

      static size_t size_of(const value_type& _v)
      { 
        if (_v.empty())
          return sizeof(unsigned int);
        
        value_type::const_iterator it = _v.begin();
        unsigned int   N     = static_cast<unsigned int>(_v.size());
        size_t         bytes = IO::size_of(N);

        for(;it!=_v.end(); ++it)
        {
          bytes += IO::size_of( it->first );
          bytes += IO::size_of( it->second );
        }
        return bytes;
      }

      static size_t store(std::ostream& _os, const value_type& _v, bool _swap=false)
      { 
        value_type::const_iterator it = _v.begin();
        unsigned int   N     = static_cast<unsigned int>(_v.size());
        
        size_t bytes;
        bytes  = IO::store( _os, N, _swap );

        for(;it!=_v.end(); ++it)
        {
          bytes += IO::store( _os, it->first,  _swap );
          bytes += IO::store( _os, it->second, _swap );
        }

        return _os.good() ? bytes : 0;
      }

      static size_t restore( std::istream& _is, value_type& _v, bool _swap=false)
      { 
        unsigned int   N     = static_cast<unsigned int>(_v.size());

        size_t bytes;
        bytes  = IO::restore( _is, N, _swap );

        for(unsigned int i=0; i < N; i++)
        {
          unsigned int first;
          double       second;

          bytes += IO::restore( _is, first,  _swap );
          bytes += IO::restore( _is, second, _swap );

          _v[first] = second;
        }

        return _is.good() ? bytes : 0;
      }
    };
  }
}


#endif //BASESKIN_HH
