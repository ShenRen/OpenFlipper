#ifndef BASESKIN_HH
#define BASESKIN_HH

#include <ObjectTypes/Skeleton/SkeletonT.hh>

/**
 * @brief Abstract base class for the skin template, wrapping all template versions of the skin
 */
class BaseSkin
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
    typedef map<unsigned int, double> SkinWeights;
    SkinWeights weights;
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
  BaseSkin() {};
  ~BaseSkin() {};

public:
  /**
   * @name Skinning
   * These methods are related to using a mesh as skin with this skeleton.
   */
  //@{
  virtual void AttachSkin() = 0;
  virtual void DeformSkin(const AnimationHandle &_hAni, Method _method = M_LBS) = 0;
  virtual void ReleaseSkin() = 0;
  //@}
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
        unsigned int   N     = _v.size();
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
        unsigned int   N     = _v.size();
        
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
        unsigned int   N     = _v.size();

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
