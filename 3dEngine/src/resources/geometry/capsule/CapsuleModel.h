#ifndef URCHINENGINE_CAPSULEMODEL_H
#define URCHINENGINE_CAPSULEMODEL_H

#include "UrchinCommon.h"

#include "resources/geometry/GeometryModel.h"

namespace urchin {

    class CapsuleModel : public GeometryModel {
        public:
            CapsuleModel(Capsule<float> , int, int);

        protected:
            Matrix4<float> retrieveModelMatrix() const override;
            std::vector<Point3<float>> retrieveVertexArray() const override;

            void drawGeometry() const override;

        private:
            void fillWithCaps(std::vector<Point3<float>> &, const Quaternion<float> &) const;

            Capsule<float> capsule;
            int sides, slices;
    };

}

#endif
