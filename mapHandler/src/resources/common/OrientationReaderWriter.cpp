#include "OrientationReaderWriter.h"

namespace urchin {

    Quaternion<float> OrientationReaderWriter::loadOrientation(const XmlChunk* parentChunk, const DataParser& dataParser) {
        auto orientationChunk = dataParser.getUniqueChunk(true, ORIENTATION_TAG, DataAttribute(), parentChunk);

        auto orientationAxisChunk = dataParser.getUniqueChunk(true, AXIS_TAG, DataAttribute(), orientationChunk.get());
        auto orientationAngleChunk = dataParser.getUniqueChunk(true, ANGLE_TAG, DataAttribute(), orientationChunk.get());

        return {orientationAxisChunk->getVector3Value(), orientationAngleChunk->getFloatValue()};
    }

    void OrientationReaderWriter::writeOrientation(const XmlChunk* parentChunk, const Quaternion<float>& orientation, XmlWriter& xmlWriter) {
        auto orientationChunk = xmlWriter.createChunk(ORIENTATION_TAG, DataAttribute(), parentChunk);

        auto orientationAxisChunk = xmlWriter.createChunk(AXIS_TAG, DataAttribute(), orientationChunk.get());
        auto orientationAngleChunk = xmlWriter.createChunk(ANGLE_TAG, DataAttribute(), orientationChunk.get());

        Vector3<float> orientationAxis;
        float orientationAngle;
        orientation.toAxisAngle(orientationAxis, orientationAngle);

        orientationAxisChunk->setVector3Value(orientationAxis);
        orientationAngleChunk->setFloatValue(orientationAngle);
    }

}
