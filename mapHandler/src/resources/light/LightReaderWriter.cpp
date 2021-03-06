#include "LightReaderWriter.h"

namespace urchin {

    Light* LightReaderWriter::loadFrom(const DataChunk* lightChunk, const DataParser& dataParser) {
        Light* light = buildLightFrom(lightChunk, dataParser);

        loadPropertiesFrom(light, lightChunk, dataParser);
        loadFlagsFrom(light, lightChunk, dataParser);

        return light;
    }

    void LightReaderWriter::writeOn(DataChunk* lightChunk, const Light* light, DataWriter& dataWriter) {
        buildChunkFrom(lightChunk, light, dataWriter);

        writePropertiesOn(lightChunk, light, dataWriter);
        writeFlagsOn(lightChunk, light, dataWriter);
    }

    Light* LightReaderWriter::buildLightFrom(const DataChunk* lightChunk, const DataParser& dataParser) {
        std::string lightType = lightChunk->getAttributeValue(TYPE_ATTR);
        if (lightType == OMNIDIRECTIONAL_VALUE) {
            auto positionChunk = dataParser.getUniqueChunk(true, POSITION_TAG, DataAttribute(), lightChunk);
            auto* omnidirectional = new OmnidirectionalLight(positionChunk->getPoint3Value());

            auto exponentialAttenuationChunk = dataParser.getUniqueChunk(true, EXPONENTIAL_ATTENUATION_TAG, DataAttribute(), lightChunk);
            omnidirectional->setAttenuation(exponentialAttenuationChunk->getFloatValue());

            return omnidirectional;
        } else if (lightType == SUN_VALUE) {
            auto directionChunk = dataParser.getUniqueChunk(true, DIRECTION_TAG, DataAttribute(), lightChunk);

            return new SunLight(directionChunk->getVector3Value());
        }

        throw std::invalid_argument("Unknown light type read from map: " + lightType);
    }

    void LightReaderWriter::buildChunkFrom(DataChunk* lightChunk, const Light* light, DataWriter& dataWriter) {
        if (light->getLightType() == Light::OMNIDIRECTIONAL) {
            const auto* omnidirectionalLight = dynamic_cast<const OmnidirectionalLight*>(light);
            lightChunk->addAttribute(DataAttribute(TYPE_ATTR, OMNIDIRECTIONAL_VALUE));

            auto positionChunk = dataWriter.createChunk(POSITION_TAG, DataAttribute(), lightChunk);
            positionChunk->setPoint3Value(omnidirectionalLight->getPosition());

            auto exponentialAttenuationChunk = dataWriter.createChunk(EXPONENTIAL_ATTENUATION_TAG, DataAttribute(), lightChunk);
            exponentialAttenuationChunk->setFloatValue(omnidirectionalLight->getExponentialAttenuation());
        } else if (light->getLightType() == Light::SUN) {
            const auto* sunLight = dynamic_cast<const SunLight*>(light);
            lightChunk->addAttribute(DataAttribute(TYPE_ATTR, SUN_VALUE));

            auto directionChunk = dataWriter.createChunk(DIRECTION_TAG, DataAttribute(), lightChunk);
            directionChunk->setVector3Value(sunLight->getDirections()[0]);
        } else {
            throw std::invalid_argument("Unknown light type to write in map: " + std::to_string(light->getLightType()));
        }
    }

    void LightReaderWriter::loadPropertiesFrom(Light* light, const DataChunk* lightChunk, const DataParser& dataParser) {
        auto ambientColorChunk = dataParser.getUniqueChunk(true, AMBIENT_COLOR_TAG, DataAttribute(), lightChunk);
        light->setAmbientColor(ambientColorChunk->getPoint3Value());
    }

    void LightReaderWriter::writePropertiesOn(const DataChunk* lightChunk, const Light* light, DataWriter& dataWriter) {
        auto ambientColorChunk = dataWriter.createChunk(AMBIENT_COLOR_TAG, DataAttribute(), lightChunk);
        ambientColorChunk->setPoint3Value(light->getAmbientColor());
    }

    void LightReaderWriter::loadFlagsFrom(Light* light, const DataChunk* lightChunk, const DataParser& dataParser) {
        auto produceShadowChunk = dataParser.getUniqueChunk(true, PRODUCE_SHADOW_TAG, DataAttribute(), lightChunk);
        light->setProduceShadow(produceShadowChunk->getBoolValue());
    }

    void LightReaderWriter::writeFlagsOn(const DataChunk* lightChunk, const Light* light, DataWriter& dataWriter) {
        auto produceShadowChunk = dataWriter.createChunk(PRODUCE_SHADOW_TAG, DataAttribute(), lightChunk);
        produceShadowChunk->setBoolValue(light->isProduceShadow());
    }

}
