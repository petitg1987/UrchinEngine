#include "NavObject.h"

#include <utility>

namespace urchin
{

    NavObject::NavObject(std::shared_ptr<Polytope> expandedPolytope) :
            expandedPolytope(std::move(expandedPolytope))
    {
        walkableSurfaces.reserve(2); //estimated memory size
        navPolygons.reserve(4); //estimated memory size
    }

    const std::shared_ptr<Polytope> &NavObject::getExpandedPolytope()
    {
        return expandedPolytope;
    }

    void NavObject::addWalkableSurface(const std::shared_ptr<PolytopeSurface> &walkableSurface)
    {
        walkableSurfaces.push_back(walkableSurface);
    }

    const std::vector<std::shared_ptr<PolytopeSurface>> &NavObject::getWalkableSurfaces() const
    {
        return walkableSurfaces;
    }

    void NavObject::addNavPolygons(const std::vector<std::shared_ptr<NavPolygon>> &navPolygonsToAdd)
    {
        navPolygons.insert(navPolygons.end(), navPolygonsToAdd.begin(), navPolygonsToAdd.end());
    }

    const std::vector<std::shared_ptr<NavPolygon>> &NavObject::getNavPolygons() const
    {
        return navPolygons;
    }

    void NavObject::removeAllNavPolygons()
    {
        navPolygons.clear();
    }

}
