#pragma once

#include <UrchinCommon.h>

#include <graphic/render/target/RenderTarget.h>
#include <scene/renderer3d/camera/Camera.h>
#include <scene/renderer3d/landscape/terrain/Terrain.h>

namespace urchin {

    class TerrainManager {
        public:
            struct Config {
                float grassDisplayDistance = 100;
            };

            explicit TerrainManager(RenderTarget&);

            void onCameraProjectionUpdate(const Camera*);

            void addTerrain(Terrain*);
            void removeTerrain(Terrain*);

            void updateConfig(const Config&);
            const Config& getConfig() const;

            void prepareRendering(const Camera*, float) const;

        private:
            void updateAllTerrainConfig();
            void updateTerrainConfig(Terrain*) const;

            RenderTarget& renderTarget;

            std::vector<Terrain*> terrains;

            Matrix4<float> projectionMatrix;

            Config config;
    };

}
