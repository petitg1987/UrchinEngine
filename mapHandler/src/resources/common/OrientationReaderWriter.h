#pragma once

#include <memory>
#include <string>

#include <UrchinCommon.h>

namespace urchin {

    class OrientationReaderWriter {
        public:
            static Quaternion<float> loadOrientation(const XmlChunk*, const DataParser&);
            static void writeOrientation(const XmlChunk*, const Quaternion<float>&, XmlWriter&);

        private:
            static constexpr char ORIENTATION_TAG[] = "orientation";
            static constexpr char AXIS_TAG[] = "axis";
            static constexpr char ANGLE_TAG[] = "angle";
    };

}
