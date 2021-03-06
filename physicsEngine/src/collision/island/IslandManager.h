#pragma once

#include <vector>

#include <collision/island/IslandContainer.h>
#include <collision/ManifoldResult.h>
#include <body/BodyManager.h>
#include <body/model/RigidBody.h>

namespace urchin {

    class IslandManager {
        public:
            explicit IslandManager(const BodyManager&);

            void refreshBodyActiveState(const std::vector<ManifoldResult>&);

        private:
            void buildIslands(const std::vector<ManifoldResult>&);
            unsigned int computeNumberElements(const std::vector<IslandElementLink>&, unsigned int) const;
            bool isBodyMoving(const RigidBody*) const;

            void printIslands(const std::vector<IslandElementLink>&);

            const BodyManager& bodyManager;
            std::vector<IslandElement*> islandElements;
            IslandContainer islandContainer;

            const float squaredLinearSleepingThreshold;
            const float squaredAngularSleepingThreshold;
    };

}
