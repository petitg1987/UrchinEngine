#pragma once

#include <memory>
#include <UrchinCommon.h>

#include <shape/CollisionShape3D.h>
#include <object/CollisionConvexObject3D.h>
#include <utils/math/PhysicsTransform.h>

namespace urchin {

    class CollisionCylinderShape : public CollisionShape3D {
        public:
            CollisionCylinderShape(float, float, CylinderShape<float>::CylinderOrientation);
            CollisionCylinderShape(CollisionCylinderShape&&) noexcept;
            CollisionCylinderShape(const CollisionCylinderShape&) = delete;
            ~CollisionCylinderShape() override = default;

            CollisionShape3D::ShapeType getShapeType() const override;
            const ConvexShape3D<float>& getSingleShape() const override;
            float getRadius() const;
            float getHeight() const;
            CylinderShape<float>::CylinderOrientation getCylinderOrientation() const;

            std::unique_ptr<CollisionShape3D> scale(float) const override;

            AABBox<float> toAABBox(const PhysicsTransform&) const override;
            std::unique_ptr<CollisionConvexObject3D, ObjectDeleter> toConvexObject(const PhysicsTransform&) const override;

            Vector3<float> computeLocalInertia(float) const override;
            float getMaxDistanceToCenter() const override;
            float getMinDistanceToCenter() const override;

            std::unique_ptr<CollisionShape3D> clone() const override;

        private:
            void computeSafeMargin();

            std::unique_ptr<CylinderShape<float>> cylinderShape; //shape including margin
    };

}
