#include "TerrainSplitService.h"

namespace urchin
{
    TerrainSplitService::TerrainSplitService(float terrainMaxSize) :
            terrainMaxSize(terrainMaxSize)
    {

    }

    std::vector<TerrainSplit> TerrainSplitService::splitTerrain(const std::shared_ptr<AITerrain> &aiTerrain) const
    {
        std::vector<TerrainSplit> terrainSplits;

        Point3<float> terrainPosition = aiTerrain->getTransform().getPosition();
        Point3<float> farLeftVertex = terrainPosition + aiTerrain->getLocalVertices()[0];
        Point3<float> nearRightVertex = terrainPosition + aiTerrain->getLocalVertices()[(aiTerrain->getXLength() * aiTerrain->getZLength()) - 1];

        float terrainXSize = nearRightVertex.X - farLeftVertex.X;
        unsigned int xSamples = static_cast<int>(std::ceil(terrainXSize/terrainMaxSize));
        float terrainZSize = nearRightVertex.Z - farLeftVertex.Z;
        unsigned int zSamples = static_cast<int>(std::ceil(terrainZSize/terrainMaxSize));

        unsigned int zShit = 0;
        for(unsigned int zSample=0; zSample < zSamples; ++zSample)
        {
            unsigned int zSize = std::lround((float)((zSample + 1) * aiTerrain->getZLength()) / (float)zSamples) - zShit;
            unsigned int xShit = 0;

            for(unsigned int xSample=0; xSample < xSamples; ++xSample)
            {
                unsigned int xSize = std::lround((float)((xSample + 1) * aiTerrain->getXLength()) / (float)xSamples) - xShit;

                TerrainSplit terrainSplit = TerrainSplit();
                terrainSplit.name = aiTerrain->getName() + "-" + std::to_string((zSample * xSamples) + xSample);
                terrainSplit.position = aiTerrain->getTransform().getPosition();
                terrainSplit.localVertices.reserve(xSize * zSize);
                terrainSplit.xLength = xSize;
                terrainSplit.zLength = zSize;
                for(unsigned int z=0; z < zSize; ++z)
                {
                    for(unsigned int x=0; x < xSize; ++x)
                    {
                        unsigned int verticesPos = ((zShit + z) * aiTerrain->getXLength()) + xShit + x;
                        terrainSplit.localVertices.emplace_back(aiTerrain->getLocalVertices()[verticesPos]);
                    }
                }
                terrainSplits.push_back(terrainSplit);

                xShit += xSize - 1;
            }

            zShit += zSize - 1;
        }

        return terrainSplits;
    }
}