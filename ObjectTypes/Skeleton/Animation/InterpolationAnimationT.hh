#ifndef INTERPOLATIONANIMATIONT_HH
#define INTERPOLATIONANIMATIONT_HH

#include <vector>
#include <map>

#include "../PoseT.hh"
#include "InterpolationT.hh"
#include "InterpolationMatrixManipulatorT.hh"

template<typename PointT>
class InterpolationAnimationT : public AnimationT<PointT>
{ 
  template<typename>
  friend class SkeletonT;

  public:
    typedef PointT                                          Point;
    typedef typename Point::value_type                      Scalar;
    typedef PoseT<PointT>                                   Pose;
    typedef InterpolationMatrixManipulatorT<Scalar>         MatrixManipulator;
    typedef InterpolationT<Scalar>                          Interpolator;
    typedef std::vector<Scalar>                             TargetType;
  
  public:
    InterpolationAnimationT(const InterpolationAnimationT<PointT> &_other);
    InterpolationAnimationT(Skeleton* _skeleton, MatrixManipulator *_matrixManipulator);
    virtual ~InterpolationAnimationT();
    
    virtual AnimationT<PointT>* copy();
    
    virtual void updateFromGlobal(unsigned int _index);
    
    virtual bool getMinInput(Scalar& _result);
    virtual bool getMaxInput(Scalar& _result);

  protected:
        SkeletonHierarchyI *hierarchy_;                 ///< Stores a pointer to the skeleton hierarchy, is passed to new poses as they are created
        Pose *reference_;                              ///< Stores the reference pose attached to the skeleton, the poses use it for some precalculations

  
        std::vector<int> influencedJoints_;
        std::vector< Interpolator* > interpolators_;
        //Hier muss es einen Mapper geben, der weiß, wie er auf die Matrix aus der Pose die Werte, die der Interpolator
        //erzeugt, anwendet.
        std::map < Interpolator*, std::vector < TargetType > > precalculations_;
        MatrixManipulator* matrixManipulator_;
        unsigned int frames_;
        
        std::map < unsigned int, Pose* > interpolatedPoses_;
        
        unsigned int calcAbsoluteMaxForInterpolator(uint _index);
  
  public:
        static const int FPS = 60;
  
  public:
        /**
         * @name Frame access
         * There is one pose per frame.
         */
        //@{
        virtual Pose *getPose(unsigned int _iFrame);
        virtual Pose *getPose(unsigned int _iFrame, Pose* _reference);
        inline unsigned int getFrameCount();
        //@}

        /**
         * @name Synchronization
         * Use these methods to keep the poses in sync with the number (and indices) of the joints.
         */
        //@{
        virtual void insert_at(unsigned int _index);
        virtual void remove_at(unsigned int _index);
        //@}
        
        /**
         * @name Interpolators access
         */
        //@{
        // We use only interpolators with time as input.
        void          addInterpolator(InterpolationT<double> *_interpolator);
        Interpolator* interpolator(unsigned int _index);
        unsigned int  interpolatorCount();
        //@}
        
        /**
         * @name InfluencedJoints access
         */
        //@{
        void addInfluencedJoint(int _joint) {
          influencedJoints_.push_back(_joint);
        }
        
        bool isInfluenced(int _joint);
        
        std::vector<int>& influencedJoints();
        
        //@}
        
        /**
         * @name MatrixManipulator access
         */
        //@{
        MatrixManipulator* matrixManipulator() {
          return matrixManipulator_;
        }
        //@}
        
        
        
        Pose* getReference() {
          return getPose(0);
        }
        
        virtual void clearPoseCache() {
          if (interpolatedPoses_.size() == 1 && interpolatedPoses_.find(0) != interpolatedPoses_.end())
            return;
          
          if (interpolatedPoses_.find(0) != interpolatedPoses_.end()) {
//             Pose* frame0 = (interpolatedPoses_.find(0)->second);

      if (interpolatedPoses_.size() > 1) {
        typename std::map< unsigned int, Pose* >::iterator ip_it = interpolatedPoses_.begin();
        ++ip_it;
        for ( ; ip_it != interpolatedPoses_.end(); ++ip_it) {
    delete ip_it->second;
    interpolatedPoses_.erase(ip_it);
        }
      }
      
//             interpolatedPoses_.insert(std::pair<unsigned int, Pose>(0, frame0));
          } else {
            interpolatedPoses_.clear();
          }
        }
};

//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(INTERPOLATIONANIMATIONT_C)
#define INTERPOLATIONANIMATIONT_TEMPLATES
#include "InterpolationAnimationT.cc"
#endif
//=============================================================================

#endif //INTERPOLATIONANIMATIONT_HH