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

#ifndef TEXTUREMATH_HH
#define TEXTUREMATH_HH

#include "TextureParameters.hh"

class TextureMath {
public:

  /** \ brief Initialize converter
   *
   * The values are modified in the following way:\n
   *
   * First the absolute value of the property is taken if requested (_abs)
   *
   * Then this value is clamped against the given values (_clamp,_clampMin,_clampMax)
   *
   * If the texture should be repeated, the values are translated such that the minimum is at min_val(0.0 by default)
   * and than scaled such that the maximum is at max_val. This data is than passed to the rendering (_repeat,_minRepeat,_maxRepeat)
   *
   * If the texture should not be repeated you could choose to center the data around 0.5. The negative values are than mapped to 0..0.5
   * and the positive values to 0.5..1 (_center)
   *
   * Otherwise, the values are mapped directly to 0..1. You can also disable that final scaling with (_scale)
   *
   * @param _abs           Take absolute value?
   * @param _clamp         Clamp values?
   * @param _clampMin      Minimal clamping value
   * @param _clampMax      Maximal Clamping value
   * @param _repeat        Repeat Texture?
   * @param _minRepeat     Minimal value for repeat
   * @param _maxRepeat     Maximal value for repeat
   * @param _center        Center values?
   * @param _scale         Scale values?
   * @param _minimalInput  Minimal input value given
   * @param _maximalInput  Maximal input value given
   */
  TextureMath(
      const bool _abs,
      const bool _clamp,
      const double _clampMin,
      const double _clampMax,
      const bool _repeat,
      const double _minRepeat,
      const double _maxRepeat,
      const bool _center,
      const bool _scale,
      const double _minimalInput,
      const double _maximalInput);

  /** \brief Convenience constructor
   *
   * @param _parameters texture parameters
   * @param _minimalInput
   * @param _maximalInput
   */
  TextureMath( const TexParameters& _parameters,
               const double _minimalInput,
               const double _maximalInput);

  /** \brief Modify given values based on the specified parameters given to the constructor.
   *
   *
   * @param _input Input value
   * @return       Modified value
   */
   double transform(const double _input) const;

private:
   bool   abs_;
   bool   clamp_;
   double clampMin_;
   double clampMax_;
   bool   repeat_;
   double repeatMin_;
   double repeatMax_;
   bool   center_;
   bool   scale_;

   double minInput_;
   double maxInput_;
};

#endif // TEXTUREMATH_HH
