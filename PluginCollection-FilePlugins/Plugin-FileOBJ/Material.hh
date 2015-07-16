/*===========================================================================*\
 *                                                                           *
 *                             OpenFlipper                                   *
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
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
*                                                                            *
*   $Revision$                                                       *
*   $LastChangedBy$                                                *
*   $Date$                     *
*                                                                            *
\*===========================================================================*/

#ifndef MATERIAL_HH_INCLUDED
#define MATERIAL_HH_INCLUDED

#ifndef DOXY_IGNORE_THIS

#include <iomanip>
#include <map>
#include <string>

/** \brief Material
 *
 * This class provides means to store materials
 * like colors, texture information (texture indices, filenames, etc.).
 */
class Material
{
  public:

    Material() :
          localNum_(0),
          Kd_is_set_(false),
          Ka_is_set_(false),
          Ks_is_set_(false),
          Tr_(0.0),
          Tr_is_set_(false),
          map_Kd_(""),
          index_Kd_(0),
          map_Kd_is_set_(false)
    {
    }


    
    void cleanup() {
        localNum_      = 0;
        Kd_is_set_     = false;
        Ka_is_set_     = false;
        Ks_is_set_     = false;
        Tr_            = 0.0;
        Tr_is_set_     = false;
        map_Kd_        = "";
        index_Kd_      = 0;
        map_Kd_is_set_ = false;
    }

    bool is_valid(void) const
    { return Kd_is_set_ || Ka_is_set_ || Ks_is_set_ || Tr_is_set_; }

    /// Does material have a diffuse color?
    bool has_Kd(void)     { return Kd_is_set_;     }
    
    /// Does material have an ambient color?
    bool has_Ka(void)     { return Ka_is_set_;     }
    
    /// Does material have a specular color?
    bool has_Ks(void)     { return Ks_is_set_;     }
    
    /// Does material have transparency?
    bool has_Tr(void)     { return Tr_is_set_;     }
    
    /// Does material have a texture?
    bool has_Texture() const { return map_Kd_is_set_; }
    
    /// Compare operator - Compares all class members
    bool operator== (Material& _mat) {
        if( _mat.Kd_ != Kd_ ||
            _mat.Ka_ != Ka_ ||
            _mat.Ks_ != Ks_ ||
            _mat.Tr_ != Tr_ ||
            _mat.map_Kd_   != map_Kd_ ||
            _mat.index_Kd_ != index_Kd_ )
            return false;
        return true;
    }
    
    /// Not equal operator
    bool operator!= (Material& _mat) { return !(*this == _mat); }

    /// Returns the material number (externally set)
    int material_number() const { return localNum_; }
    void material_number(unsigned int _n) { localNum_ = _n; }

    /// Set diffuse color
    void set_Kd( float r, float g, float b )
    { Kd_=OpenMesh::Vec3f(r,g,b); Kd_is_set_=true; }

    /// Set ambient color
    void set_Ka( float r, float g, float b )
    { Ka_=OpenMesh::Vec3f(r,g,b); Ka_is_set_=true; }

    /// Set specular color
    void set_Ks( float r, float g, float b )
    { Ks_=OpenMesh::Vec3f(r,g,b); Ks_is_set_=true; }

    /// Set transparency value (between 0 and 1)
    void set_Tr( float t )
    { Tr_=t;            Tr_is_set_=true; }

    /// Set texture file and index
    void set_map_Kd( std::string _name, int _index_Kd )
    { map_Kd_ = _name, index_Kd_ = _index_Kd; map_Kd_is_set_ = true; };

    /// Get diffuse color
    const OpenMesh::Vec3f& Kd( void ) const { return Kd_; }
    
    /// Get ambient color
    const OpenMesh::Vec3f& Ka( void ) const { return Ka_; }
    
    /// Get specular color
    const OpenMesh::Vec3f& Ks( void ) const { return Ks_; }
    
    /// Get transparency value
    float Tr( void ) const { return Tr_; }
    
    /// Get texture file
    const std::string& map_Kd( void ) { return map_Kd_ ; }
    
    /// Get texture index
    const int& map_Kd_index( void ) { return index_Kd_ ; }
    
  private:
    
    unsigned int localNum_;
    OpenMesh::Vec3f Kd_;                          bool Kd_is_set_; // diffuse
    OpenMesh::Vec3f Ka_;                          bool Ka_is_set_; // ambient
    OpenMesh::Vec3f Ks_;                          bool Ks_is_set_; // specular
    float Tr_;                                    bool Tr_is_set_; // transperency

    std::string map_Kd_; int index_Kd_; bool map_Kd_is_set_; // Texture
};

inline std::ostream&
operator<< (std::ostream& _os, const Material& _mat) {
    _os << "mat" << std::setfill('0') << std::setw(3) << _mat.material_number();
    
    return _os;
}

typedef std::map<std::string, Material> MaterialList;

#endif

#endif // MATERIAL_HH_INCLUDED
