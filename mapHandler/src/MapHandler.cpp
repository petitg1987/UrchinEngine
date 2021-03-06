#include <stdexcept>

#include "MapHandler.h"
#include <load/NullLoadCallback.h>

namespace urchin {

    MapHandler::MapHandler(Renderer3d* renderer3d, PhysicsWorld* physicsWorld, SoundManager* soundManager, AIManager* aiManager) :
            map(nullptr) {
        SignalHandler::instance()->initialize();

        if (!renderer3d) {
            throw std::invalid_argument("Renderer 3d cannot be null in map handler");
        }

        if (!soundManager) {
            throw std::invalid_argument("Sound manager cannot be null in map handler");
        }

        if (!aiManager) {
            throw std::invalid_argument("AI manager cannot be null in map handler");
        }

        map = new Map(renderer3d, physicsWorld, soundManager, aiManager);
    }

    MapHandler::~MapHandler() {
        delete map;
    }

    void MapHandler::loadMapFromFile(const std::string& filename, LoadCallback& loadCallback) {
        Logger::instance()->logInfo("Load map: " + filename);
        DataParser dataParser(filename);

        relativeWorkingDirectory = dataParser.getRootChunk()->getAttributeValue(WORKING_DIR_ATTR);
        map->loadFrom(dataParser.getRootChunk().get(), dataParser, loadCallback);
    }

    void MapHandler::writeMapOnFile(const std::string& filename) const {
        DataWriter dataWriter(filename);

        auto rootChunk = dataWriter.createChunk(SCENE_TAG, DataAttribute(WORKING_DIR_ATTR, relativeWorkingDirectory));
        map->writeOn(rootChunk.get(), dataWriter);

        dataWriter.saveInFile();
    }

    /**
     * @param filename Absolute path to file containing map information
     * @return Working directory relative to the map file
     */
    std::string MapHandler::getRelativeWorkingDirectory(const std::string& filename) {
        DataParser dataParser(filename, "");
        return dataParser.getRootChunk()->getAttributeValue(WORKING_DIR_ATTR);
    }

    /**
     * @return Working directory relative to the map file
     */
    std::string MapHandler::getRelativeWorkingDirectory() const {
        return relativeWorkingDirectory;
    }

    /**
     * @param relativeWorkingDirectory Relative working directory to the map file
     */
    void MapHandler::setRelativeWorkingDirectory(const std::string& relativeWorkingDirectory) {
        this->relativeWorkingDirectory = relativeWorkingDirectory;
    }

    void MapHandler::refreshMap() {
        map->refreshMap();
    }

    Map* MapHandler::getMap() const {
        return map;
    }

    void MapHandler::pause() {
        map->pause();
    }

    void MapHandler::unpause() {
        map->unpause();
    }

}
