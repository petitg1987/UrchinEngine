#pragma once

#include <memory>

#include <body/model/AbstractBody.h>
#include <object/TemporalObject.h>
#include <collision/narrowphase/algorithm/continuous/result/ContinuousCollisionResult.h>
#include <collision/narrowphase/algorithm/util/AlgorithmResultDeleter.h>

namespace urchin {

    /**
    * Implementation of GJK Ray Cast algorithm (see http://www.dtecta.com/papers/unpublished04raycast.pdf)
    */
    template<class T, class U> class GJKContinuousCollisionAlgorithm {
        public:
            GJKContinuousCollisionAlgorithm();

            std::unique_ptr<ContinuousCollisionResult<U>, AlgorithmResultDeleter> calculateTimeOfImpact(const TemporalObject&, const TemporalObject&, AbstractBody*) const;

        private:
            Point3<T> getWorldSupportPoint(const TemporalObject&, const Vector3<T>&, const PhysicsTransform&) const;
            Point3<float> interpolate(const Point3<float>&, const Point3<float>&, T) const;

            void logInputData(const TemporalObject&, const TemporalObject&, const std::string&, Logger::CriticalityLevel) const;

            const T squareEpsilon;
            const unsigned int maxIteration;
            const float terminationTolerance;
    };

}
