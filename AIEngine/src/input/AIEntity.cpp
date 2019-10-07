#include "AIEntity.h"

#include <utility>
#include "path/navmesh/polytope/Polytope.h"

namespace urchin
{
    AIEntity::AIEntity(std::string name, Transform<float> transform, bool bIsObstacleCandidate) :
            bToRebuild(true),
            name(std::move(name)),
            transform(std::move(transform)),
            bIsObstacleCandidate(bIsObstacleCandidate)
    {

    }

    void AIEntity::updateTransform(const Point3<float> &position, const Quaternion<float> &orientation)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            this->transform = Transform<float>(position, orientation, 1.0);
        }

        this->bToRebuild.store(true, std::memory_order_relaxed);
    }

    bool AIEntity::isToRebuild() const
    {
        return bToRebuild.load(std::memory_order_relaxed);
    }

    void AIEntity::markRebuilt()
    {
        this->bToRebuild.store(false, std::memory_order_relaxed);
    }

    const std::string &AIEntity::getName() const
    {
        return name;
    }

    Transform<float> AIEntity::getTransform() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return transform;
    }

    bool AIEntity::isObstacleCandidate() const
    {
        return bIsObstacleCandidate;
    }

    void AIEntity::addExpandedPolytope(const std::shared_ptr<Polytope> &polytope)
    {
        expandedPolytopes.push_back(polytope);
    }

    const std::vector<std::shared_ptr<Polytope>> &AIEntity::getExpandedPolytopes() const
    {
        return expandedPolytopes;
    }

}
