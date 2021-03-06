#pragma once

#include <vector>
#include <map>
#include <UrchinCommon.h>

#include <collision/constraintsolver/ConstraintSolving.h>
#include <collision/constraintsolver/solvingdata/CommonSolvingData.h>
#include <collision/constraintsolver/solvingdata/ImpulseSolvingData.h>
#include <body/BodyManager.h>
#include <collision/ManifoldResult.h>
#include <utils/pool/FixedSizePool.h>
#include <body/model/RigidBody.h>

namespace urchin {

    class ConstraintSolverManager {
        public:
            ConstraintSolverManager();
            ~ConstraintSolverManager();

            void solveConstraints(float, std::vector<ManifoldResult>&);

        private:
            void setupConstraints(std::vector<ManifoldResult>&, float);
            void solveConstraints();

            CommonSolvingData fillCommonSolvingData(const ManifoldResult&, const ManifoldContactPoint&);
            ImpulseSolvingData fillImpulseSolvingData(const CommonSolvingData&, float) const;

            void solveNormalConstraint(ConstraintSolving*);
            void solveTangentConstraint(ConstraintSolving*);

            void applyImpulse(RigidBody*, RigidBody*, const CommonSolvingData&, const Vector3<float>&) const;
            Vector3<float> computeRelativeVelocity(const CommonSolvingData&) const;
            Vector3<float> computeTangent(const CommonSolvingData&, const Vector3<float>&) const;

            void logCommonData(const std::string&, const CommonSolvingData&) const;

            std::vector<ConstraintSolving*> constraintsSolving;
            std::unique_ptr<FixedSizePool<ConstraintSolving>> constraintSolvingPool;

            const unsigned int constraintSolverIteration;
            const float biasFactor;
            const bool useWarmStarting;
            const float restitutionVelocityThreshold;
    };

}
