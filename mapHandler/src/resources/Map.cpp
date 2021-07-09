#include <stdexcept>
#include <memory>

#include "Map.h"

namespace urchin {

    Map::Map(Renderer3d* renderer3d, PhysicsWorld* physicsWorld, SoundManager* soundManager, AIManager* aiManager) :
            renderer3d(renderer3d),
            physicsWorld(physicsWorld),
            soundManager(soundManager),
            aiManager(aiManager),
            sceneSky(new SceneSky(renderer3d)),
            sceneAI(new SceneAI(aiManager)) {

    }

    Map::~Map() {
        delete sceneAI;
        delete sceneSky;

        for (SceneObject* sceneObject : sceneObjects) {
            delete sceneObject;
        }

        for (SceneLight* sceneLight : sceneLights) {
            delete sceneLight;
        }

        for (SceneTerrain* sceneTerrain : sceneTerrains) {
            delete sceneTerrain;
        }

        for (SceneWater* sceneWater : sceneWaters) {
            delete sceneWater;
        }

        for (SceneSound* sceneSound : sceneSounds) {
            delete sceneSound;
        }
    }

    void Map::loadFrom(const XmlChunk* rootChunk, const DataParser& dataParser, LoadCallback& loadCallback) {
        if (renderer3d && !renderer3d->isPaused()) { //to avoid move camera before being able to see the map
            throw std::runtime_error("Renderer 3d should be paused while loading map.");
        }

        if (physicsWorld && !physicsWorld->isPaused()) { //to avoid miss of collision between objects just loaded and on objects not loaded yet
            throw std::runtime_error("Physics world should be paused while loading map.");
        }

        if (aiManager && !aiManager->isPaused()) { //to avoid compute path based on a world with missing objects
            throw std::runtime_error("AI manager should be paused while loading map.");
        }

        loadSceneObjectsFrom(rootChunk, dataParser);
        loadCallback.execute(LoadCallback::OBJECTS);

        loadSceneLightsFrom(rootChunk, dataParser);
        loadCallback.execute(LoadCallback::LIGHTS);

        loadSceneTerrainFrom(rootChunk, dataParser);
        loadSceneWaterFrom(rootChunk, dataParser);
        loadSceneSkyFrom(rootChunk, dataParser);
        loadCallback.execute(LoadCallback::LANDSCAPE);

        loadSceneSoundsFrom(rootChunk, dataParser);
        loadCallback.execute(LoadCallback::SOUNDS);

        loadSceneAIFrom(rootChunk, dataParser);
        loadCallback.execute(LoadCallback::AI);
    }

    void Map::loadSceneObjectsFrom(const XmlChunk* chunk, const DataParser& dataParser) {
        auto objectsListChunk = dataParser.getUniqueChunk(true, OBJECTS_TAG, DataAttribute(), chunk);
        auto objectsChunk = dataParser.getChunks(OBJECT_TAG, DataAttribute(), objectsListChunk.get());

        for (const auto& objectChunk : objectsChunk) {
            auto* sceneObject = new SceneObject();
            sceneObject->loadFrom(objectChunk.get(), dataParser);

            addSceneObject(sceneObject);
        }
    }

    void Map::loadSceneLightsFrom(const XmlChunk* chunk, const DataParser& dataParser) {
        auto lightsListChunk = dataParser.getUniqueChunk(true, LIGHTS_TAG, DataAttribute(), chunk);
        auto lightsChunk = dataParser.getChunks(LIGHT_TAG, DataAttribute(), lightsListChunk.get());

        for (const auto& lightChunk : lightsChunk) {
            auto* sceneLight = new SceneLight();
            sceneLight->loadFrom(lightChunk.get(), dataParser);

            addSceneLight(sceneLight);
        }
    }

    void Map::loadSceneTerrainFrom(const XmlChunk* chunk, const DataParser& dataParser) {
        auto terrainsListChunk = dataParser.getUniqueChunk(true, TERRAINS_TAG, DataAttribute(), chunk);
        auto terrainsChunk = dataParser.getChunks(TERRAIN_TAG, DataAttribute(), terrainsListChunk.get());

        for (const auto& terrainChunk : terrainsChunk) {
            auto* sceneTerrain = new SceneTerrain();
            sceneTerrain->loadFrom(terrainChunk.get(), dataParser);

            addSceneTerrain(sceneTerrain);
        }
    }

    void Map::loadSceneWaterFrom(const XmlChunk* chunk, const DataParser& dataParser) {
        auto watersListChunk = dataParser.getUniqueChunk(true, WATERS_TAG, DataAttribute(), chunk);
        auto watersChunk = dataParser.getChunks(WATER_TAG, DataAttribute(), watersListChunk.get());

        for (const auto& waterChunk : watersChunk) {
            auto* sceneWater = new SceneWater();
            sceneWater->loadFrom(waterChunk.get(), dataParser);

            addSceneWater(sceneWater);
        }
    }

    void Map::loadSceneSkyFrom(const XmlChunk* chunk, const DataParser& dataParser) {
        auto skyChunk = dataParser.getUniqueChunk(true, SKY_TAG, DataAttribute(), chunk);

        sceneSky->loadFrom(skyChunk.get(), dataParser);
    }

    void Map::loadSceneSoundsFrom(const XmlChunk* chunk, const DataParser& dataParser) {
        auto soundElementsListChunk = dataParser.getUniqueChunk(true, SOUND_ELEMENTS_TAG, DataAttribute(), chunk);
        auto soundElementsChunk = dataParser.getChunks(SOUND_ELEMENT_TAG, DataAttribute(), soundElementsListChunk.get());

        for (const auto& soundElementChunk : soundElementsChunk) {
            auto* sceneSound = new SceneSound();
            sceneSound->loadFrom(soundElementChunk.get(), dataParser);

            addSceneSound(sceneSound);
        }
    }

    void Map::loadSceneAIFrom(const XmlChunk* chunk, const DataParser& dataParser) {
        auto aiElementsListChunk = dataParser.getUniqueChunk(true, AI_ELEMENTS_TAG, DataAttribute(), chunk);

        sceneAI->loadFrom(aiElementsListChunk.get(), dataParser);
    }

    void Map::writeOn(XmlChunk* chunk, XmlWriter& xmlWriter) const {
        writeSceneObjectsOn(chunk, xmlWriter);
        writeSceneLightsOn(chunk, xmlWriter);
        writeSceneTerrainsOn(chunk, xmlWriter);
        writeSceneWatersOn(chunk, xmlWriter);
        writeSceneSkyOn(chunk, xmlWriter);
        writeSceneSoundsOn(chunk, xmlWriter);
        writeSceneAIOn(chunk, xmlWriter);
    }

    void Map::writeSceneObjectsOn(XmlChunk* chunk, XmlWriter& xmlWriter) const {
        auto objectsListChunk = xmlWriter.createChunk(OBJECTS_TAG, DataAttribute(), chunk);

        for (auto sceneObject : sceneObjects) {
            auto objectsChunk = xmlWriter.createChunk(OBJECT_TAG, DataAttribute(), objectsListChunk.get());
            sceneObject->writeOn(objectsChunk.get(), xmlWriter);
        }
    }

    void Map::writeSceneLightsOn(XmlChunk* chunk, XmlWriter& xmlWriter) const {
        auto lightsListChunk = xmlWriter.createChunk(LIGHTS_TAG, DataAttribute(), chunk);

        for (auto sceneLight : sceneLights) {
            auto lightsChunk = xmlWriter.createChunk(LIGHT_TAG, DataAttribute(), lightsListChunk.get());
            sceneLight->writeOn(lightsChunk.get(), xmlWriter);
        }
    }

    void Map::writeSceneTerrainsOn(XmlChunk* chunk, XmlWriter& xmlWriter) const {
        auto terrainsListChunk = xmlWriter.createChunk(TERRAINS_TAG, DataAttribute(), chunk);

        for (auto sceneTerrain : sceneTerrains) {
            auto terrainsChunk = xmlWriter.createChunk(TERRAIN_TAG, DataAttribute(), terrainsListChunk.get());
            sceneTerrain->writeOn(terrainsChunk.get(), xmlWriter);
        }
    }

    void Map::writeSceneWatersOn(XmlChunk* chunk, XmlWriter& xmlWriter) const {
        auto watersListChunk = xmlWriter.createChunk(WATERS_TAG, DataAttribute(), chunk);

        for (auto sceneWater : sceneWaters) {
            auto watersChunk = xmlWriter.createChunk(WATER_TAG, DataAttribute(), watersListChunk.get());
            sceneWater->writeOn(watersChunk.get(), xmlWriter);
        }
    }

    void Map::writeSceneSkyOn(XmlChunk* chunk, XmlWriter& xmlWriter) const {
        auto skyChunk = xmlWriter.createChunk(SKY_TAG, DataAttribute(), chunk);

        sceneSky->writeOn(skyChunk.get(), xmlWriter);
    }

    void Map::writeSceneSoundsOn(XmlChunk* chunk, XmlWriter& xmlWriter) const {
        auto soundElementsListChunk = xmlWriter.createChunk(SOUND_ELEMENTS_TAG, DataAttribute(), chunk);

        for (auto sceneSound : sceneSounds) {
            auto soundElementsChunk = xmlWriter.createChunk(SOUND_ELEMENT_TAG, DataAttribute(), soundElementsListChunk.get());
            sceneSound->writeOn(soundElementsChunk.get(), xmlWriter);
        }
    }

    void Map::writeSceneAIOn(XmlChunk* chunk, XmlWriter& xmlWriter) const {
        auto aiElementsListChunk = xmlWriter.createChunk(AI_ELEMENTS_TAG, DataAttribute(), chunk);

        sceneAI->writeOn(aiElementsListChunk.get(), xmlWriter);
    }

    const std::list<SceneObject*>& Map::getSceneObjects() const {
        return sceneObjects;
    }

    SceneObject* Map::getSceneObject(const std::string& name) const {
        for (auto sceneObject : sceneObjects) {
            if (sceneObject->getName() == name) {
                return sceneObject;
            }
        }

        throw std::invalid_argument("Impossible to find a scene object having name: " + name);
    }

    void Map::addSceneObject(SceneObject* sceneObject) {
        sceneObject->setObjectManagers(renderer3d, physicsWorld, aiManager);
        sceneObjects.push_back(sceneObject);
    }

    void Map::removeSceneObject(SceneObject* sceneObject) {
        sceneObjects.remove(sceneObject);
        delete sceneObject;
    }

    const std::list<SceneLight*>& Map::getSceneLights() const {
        return sceneLights;
    }

    SceneLight* Map::getSceneLight(const std::string& name) const {
        for (auto sceneLight : sceneLights) {
            if (sceneLight->getName() == name) {
                return sceneLight;
            }
        }

        throw std::invalid_argument("Impossible to find a scene light having name: " + name);
    }

    void Map::addSceneLight(SceneLight* sceneLight) {
        sceneLight->setLightManager(renderer3d->getLightManager());
        sceneLights.push_back(sceneLight);
    }

    void Map::removeSceneLight(SceneLight* sceneLight) {
        sceneLights.remove(sceneLight);
        delete sceneLight;
    }

    const std::list<SceneTerrain*>& Map::getSceneTerrains() const {
        return sceneTerrains;
    }

    SceneTerrain* Map::getSceneTerrain(const std::string& name) const {
        for (auto sceneTerrain : sceneTerrains) {
            if (sceneTerrain->getName() == name) {
                return sceneTerrain;
            }
        }

        throw std::invalid_argument("Impossible to find a scene terrain having name: " + name);
    }

    void Map::addSceneTerrain(SceneTerrain* sceneTerrain) {
        sceneTerrain->setTerrainManagers(renderer3d, physicsWorld, aiManager);
        sceneTerrains.push_back(sceneTerrain);
    }

    void Map::removeSceneTerrain(SceneTerrain* sceneTerrain) {
        sceneTerrains.remove(sceneTerrain);
        delete sceneTerrain;
    }

    const std::list<SceneWater*>& Map::getSceneWaters() const {
        return sceneWaters;
    }

    SceneWater* Map::getSceneWater(const std::string& name) const {
        for (auto sceneWater : sceneWaters) {
            if (sceneWater->getName() == name) {
                return sceneWater;
            }
        }

        throw std::invalid_argument("Impossible to find a scene water having name: " + name);
    }

    void Map::addSceneWater(SceneWater* sceneWater) {
        sceneWater->setWaterManagers(renderer3d);
        sceneWaters.push_back(sceneWater);
    }

    void Map::removeSceneWater(SceneWater* sceneWater) {
        sceneWaters.remove(sceneWater);
        delete sceneWater;
    }

    const SceneSky* Map::getSceneSky() const {
        return sceneSky;
    }

    void Map::updateSceneSky(std::unique_ptr<Skybox> skybox) {
        sceneSky->changeSkybox(std::move(skybox));
    }

    const std::list<SceneSound*>& Map::getSceneSounds() const {
        return sceneSounds;
    }

    SceneSound* Map::getSceneSound(const std::string& name) const {
        for (auto sceneSound : sceneSounds) {
            if (sceneSound->getName() == name) {
                return sceneSound;
            }
        }

        throw std::invalid_argument("Impossible to find a scene sound having name: " + name);
    }

    void Map::addSceneSound(SceneSound* sceneSound) {
        sceneSound->setSoundManager(soundManager);
        sceneSounds.push_back(sceneSound);
    }

    void Map::removeSceneSound(SceneSound* sceneSound) {
        sceneSounds.remove(sceneSound);
        delete sceneSound;
    }

    const SceneAI* Map::getSceneAI() const {
        return sceneAI;
    }

    void Map::updateSceneAI(const std::shared_ptr<NavMeshAgent>& navMeshAgent) {
        sceneAI->changeNavMeshAgent(navMeshAgent);
    }

    void Map::pause() {
        if (renderer3d) {
            renderer3d->pause();
        }

        if (physicsWorld) {
            physicsWorld->pause();
        }

        if (aiManager) {
            aiManager->pause();
        }

        if (soundManager) {
            soundManager->pause();
        }
    }

    void Map::unpause() {
        if (renderer3d) {
            renderer3d->unpause();
        }

        if (physicsWorld) {
            physicsWorld->unpause();
        }

        if (aiManager) {
            aiManager->unpause();
        }

        if (soundManager) {
            soundManager->unpause();
        }
    }

    void Map::refreshMap() {
        physicsWorld->checkNoExceptionRaised();
        aiManager->checkNoExceptionRaised();
        soundManager->checkNoExceptionRaised();

        refreshEntities();
        refreshSound();
    }

    void Map::refreshEntities() {
        for (SceneObject* sceneObject : sceneObjects) {
            sceneObject->refresh();
        }

        for (SceneTerrain* sceneTerrain : sceneTerrains) {
            sceneTerrain->refresh();
        }
    }

    void Map::refreshSound() {
        //update sound event
        if (renderer3d && renderer3d->getCamera()) {
            soundManager->process(renderer3d->getCamera()->getPosition());
        } else {
            soundManager->process();
        }
    }
}

