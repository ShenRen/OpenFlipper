#ifndef INTERPOLATIONMATRIXMANIPULATORT_HH
#define INTERPOLATIONMATRIXMANIPULATORT_HH

#include <ACG/Math/GLMatrixT.hh>
#include <vector>


/** \brief Knows how to apply the values generated by an interpolator to a matrix.
* When playing back an InterpolationAnimation, an InterpolationT interpolates between two points. This can be done linearly or via a bezier curve or whatever.
* The values coming out of this interpolation are just a vector of numbers that now have to be applied to a transformation in the pose.
* How this has to be done depends on the type of animation. For example, the interpolator might interpolate between two angles (i.e. output a single number).
* This would have to be applied to the matrix using a rotation function. The same is imaginable with a translation (3 values) and so on.
*
*/
template<typename Scalar>
class InterpolationMatrixManipulatorT
{
  public:    
    typedef ACG::GLMatrixT<Scalar> GLMatrixT;
    
    /**
    * This applies the given values to the given matrix in the fashion that is suitable.
    */
    virtual void doManipulation(GLMatrixT& _matrix, std::vector<Scalar> _value) = 0;
  protected:
    
  private:
};

#endif //INTERPOLATIONMATRIXMANIPULATORT_HH