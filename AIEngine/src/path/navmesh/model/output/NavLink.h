#ifndef URCHINENGINE_NAVLINK_H
#define URCHINENGINE_NAVLINK_H

#include <memory>

namespace urchin
{

    class NavPolygon;
    class NavTriangle;

    /**
     * Represent a link between two triangles.
     * Source triangle is the one which contains the NavLink class and the target polygon/triangle is defined in NavLink class.
     */
    class NavLink
    {
        public:
            enum LinkType
            {
                DIRECT,
                JUMP
            };

            NavLink(LinkType, unsigned int, const std::shared_ptr<NavPolygon> &, const std::shared_ptr<NavTriangle> &);

            LinkType getLinkType() const;

            unsigned int getSourceEdgeIndex() const;

            std::shared_ptr<NavPolygon> getTargetPolygon() const;
            std::shared_ptr<NavTriangle> getTargetTriangle() const;

        private:
            LinkType linkType;

            unsigned int sourceEdgeIndex;

            std::weak_ptr<NavPolygon> targetPolygon;
            std::weak_ptr<NavTriangle> targetTriangle; //use weak_ptr to avoid cyclic references between triangles //TODO review comment & weak_ptr ?

            //TODO add cost multiplicand depending of link type ?
    };

}

#endif
