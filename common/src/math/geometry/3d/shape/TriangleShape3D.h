#pragma once

#include <memory>

#include <math/geometry/3d/shape/ConvexShape3D.h>
#include <math/algebra/point/Point3.h>

namespace urchin {

    template<class T> class TriangleShape3D : public ConvexShape3D<T> {
        public:
            explicit TriangleShape3D(const Point3<T>*);
            TriangleShape3D(const Point3<T>&, const Point3<T>&, const Point3<T>&);

            const Point3<T>* getPoints() const;

            std::unique_ptr<ConvexShape3D<T>> clone() const override;
            std::unique_ptr<ConvexObject3D<T>> toConvexObject(const Transform<T>&) const override;

        private:
            Point3<T> points[3];
    };

}
