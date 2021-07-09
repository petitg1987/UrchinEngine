#include "WaterReaderWriter.h"

namespace urchin {
    Water* WaterReaderWriter::loadFrom(const XmlChunk* waterChunk, const DataParser& dataParser) const {
        auto* water = new Water();
        loadGeneralPropertiesOn(water, waterChunk, dataParser);
        loadWaterSurfaceProperties(water, waterChunk, dataParser);
        loadUnderWaterProperties(water, waterChunk, dataParser);

        return water;
    }

    void WaterReaderWriter::writeOn(XmlChunk* waterChunk, const Water* water, XmlWriter& xmlWriter) const {
        writeGeneralPropertiesOn(waterChunk, water, xmlWriter);
        writeWaterSurfacePropertiesOn(waterChunk, water, xmlWriter);
        writeUnderWaterPropertiesOn(waterChunk, water, xmlWriter);
    }

    void WaterReaderWriter::loadGeneralPropertiesOn(Water* water, const XmlChunk* waterChunk, const DataParser& dataParser) {
        auto centerPositionChunk = dataParser.getUniqueChunk(true, CENTER_POSITION_TAG, DataAttribute(), waterChunk);
        water->setCenterPosition(centerPositionChunk->getPoint3Value());

        auto xSizeChunk = dataParser.getUniqueChunk(true, X_SIZE_TAG, DataAttribute(), waterChunk);
        water->setXSize(xSizeChunk->getFloatValue());

        auto zSizeChunk = dataParser.getUniqueChunk(true, Z_SIZE_TAG, DataAttribute(), waterChunk);
        water->setZSize(zSizeChunk->getFloatValue());
    }

    void WaterReaderWriter::writeGeneralPropertiesOn(XmlChunk* waterChunk, const Water* water, XmlWriter& xmlWriter) {
        auto centerPositionChunk = xmlWriter.createChunk(CENTER_POSITION_TAG, DataAttribute(), waterChunk);
        centerPositionChunk->setPoint3Value(water->getCenterPosition());

        auto xSizeChunk = xmlWriter.createChunk(X_SIZE_TAG, DataAttribute(), waterChunk);
        xSizeChunk->setFloatValue(water->getXSize());

        auto zSizeChunk = xmlWriter.createChunk(Z_SIZE_TAG, DataAttribute(), waterChunk);
        zSizeChunk->setFloatValue(water->getZSize());
    }

    void WaterReaderWriter::loadWaterSurfaceProperties(Water* water, const XmlChunk* waterChunk, const DataParser& dataParser) {
        auto waterColorChunk = dataParser.getUniqueChunk(true, WATER_COLOR_TAG, DataAttribute(), waterChunk);
        water->setWaterColor(waterColorChunk->getVector3Value());

        auto normalFilenameChunk = dataParser.getUniqueChunk(true, NORMAL_FILENAME_TAG, DataAttribute(), waterChunk);
        water->setNormalTexture(normalFilenameChunk->getStringValue());

        auto dudvMapFilenameChunk = dataParser.getUniqueChunk(true, DUDV_MAP_FILENAME_TAG, DataAttribute(), waterChunk);
        water->setDudvMap(dudvMapFilenameChunk->getStringValue());

        auto waveSpeedChunk = dataParser.getUniqueChunk(true, WAVE_SPEED_TAG, DataAttribute(), waterChunk);
        water->setWaveSpeed(waveSpeedChunk->getFloatValue());

        auto waveStrengthChunk = dataParser.getUniqueChunk(true, WAVE_STRENGTH_TAG, DataAttribute(), waterChunk);
        water->setWaveStrength(waveStrengthChunk->getFloatValue());

        auto sRepeatChunk = dataParser.getUniqueChunk(true, S_REPEAT_TAG, DataAttribute(), waterChunk);
        water->setSRepeat(sRepeatChunk->getFloatValue());

        auto tRepeatChunk = dataParser.getUniqueChunk(true, T_REPEAT_TAG, DataAttribute(), waterChunk);
        water->setTRepeat(tRepeatChunk->getFloatValue());
    }

    void WaterReaderWriter::writeWaterSurfacePropertiesOn(XmlChunk* waterChunk, const Water* water, XmlWriter& xmlWriter) {
        auto waterColorChunk = xmlWriter.createChunk(WATER_COLOR_TAG, DataAttribute(), waterChunk);
        waterColorChunk->setVector3Value(water->getWaterColor());

        auto normalFilenameChunk = xmlWriter.createChunk(NORMAL_FILENAME_TAG, DataAttribute(), waterChunk);
        normalFilenameChunk->setStringValue(water->getNormalFilename());

        auto dudvMapFilenameChunk = xmlWriter.createChunk(DUDV_MAP_FILENAME_TAG, DataAttribute(), waterChunk);
        dudvMapFilenameChunk->setStringValue(water->getDudvFilename());

        auto waveSpeedChunk = xmlWriter.createChunk(WAVE_SPEED_TAG, DataAttribute(), waterChunk);
        waveSpeedChunk->setFloatValue(water->getWaveSpeed());

        auto waveStrengthChunk = xmlWriter.createChunk(WAVE_STRENGTH_TAG, DataAttribute(), waterChunk);
        waveStrengthChunk->setFloatValue(water->getWaveStrength());

        auto sRepeatChunk = xmlWriter.createChunk(S_REPEAT_TAG, DataAttribute(), waterChunk);
        sRepeatChunk->setFloatValue(water->getSRepeat());

        auto tRepeatChunk = xmlWriter.createChunk(T_REPEAT_TAG, DataAttribute(), waterChunk);
        tRepeatChunk->setFloatValue(water->getTRepeat());
    }

    void WaterReaderWriter::loadUnderWaterProperties(Water* water, const XmlChunk* waterChunk, const DataParser& dataParser) {
        auto densityChunk = dataParser.getUniqueChunk(true, DENSITY_TAG, DataAttribute(), waterChunk);
        water->setDensity(densityChunk->getFloatValue());

        auto gradientChunk = dataParser.getUniqueChunk(true, GRADIENT_TAG, DataAttribute(), waterChunk);
        water->setGradient(gradientChunk->getFloatValue());
    }

    void WaterReaderWriter::writeUnderWaterPropertiesOn(XmlChunk* waterChunk, const Water* water, XmlWriter& xmlWriter) {
        auto densityChunk = xmlWriter.createChunk(DENSITY_TAG, DataAttribute(), waterChunk);
        densityChunk->setFloatValue(water->getDensity());

        auto gradientChunk = xmlWriter.createChunk(GRADIENT_TAG, DataAttribute(), waterChunk);
        gradientChunk->setFloatValue(water->getGradient());
    }

}
