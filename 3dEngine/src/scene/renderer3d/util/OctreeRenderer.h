#pragma once

#include <UrchinCommon.h>

#include <resources/geometry/aabbox/AABBoxModel.h>
#include <graphic/render/target/RenderTarget.h>

namespace urchin {

    class OctreeRenderer {
        public:
            template<class T> static std::unique_ptr<AABBoxModel> createOctreeModel(const OctreeManager<T>*, RenderTarget&, const Matrix4<float>&);
    };

    #include "OctreeRenderer.inl"

}
