#pragma once

#include <Urchin3dEngine.h>
#include <UrchinPhysicsEngine.h>

namespace urchin {

    class SceneEntity {
        public:
            virtual ~SceneEntity() = default;

            void refresh();

        protected:
            virtual RigidBody* getRigidBody() const = 0;
            virtual void moveTo(const Point3<float>&, const Quaternion<float>&) = 0;
    };

}
