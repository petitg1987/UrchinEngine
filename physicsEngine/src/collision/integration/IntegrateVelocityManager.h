#pragma once

#include <UrchinCommon.h>
#include <vector>

#include <body/BodyManager.h>
#include <collision/OverlappingPair.h>

namespace urchin {

    /**
    * Manager allowing to perform integration on bodies velocity
    */
    class IntegrateVelocityManager {
        public:
            explicit IntegrateVelocityManager(const BodyManager&);

            void integrateVelocity(float, const std::vector<std::unique_ptr<OverlappingPair>>&, const Vector3<float>&);

        private:
            void applyGravityForce(const Vector3<float>&, float);
            void applyRollingFrictionResistanceForce(float , const std::vector<std::unique_ptr<OverlappingPair>>&);

            const BodyManager& bodyManager;
    };

}
