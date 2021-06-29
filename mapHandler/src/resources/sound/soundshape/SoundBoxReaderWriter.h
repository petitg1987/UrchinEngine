#pragma once

#include <memory>

#include <UrchinCommon.h>
#include <UrchinSoundEngine.h>
#include "SoundShapeReaderWriter.h"

namespace urchin {

    class SoundBoxReaderWriter : public SoundShapeReaderWriter {
        //XML tags
        static constexpr char POSITION_TAG[] = "position";
        static constexpr char HALF_SIZES_TAG[] = "halfSizes";
        static constexpr char ORIENTATION_TAG[] = "orientation";
        static constexpr char MARGIN_TAG[] = "margin";
        static constexpr char AXIS_TAG[] = "axis";
        static constexpr char ANGLE_TAG[] = "angle";

        public:
            ~SoundBoxReaderWriter() override = default;

            SoundShape* loadFrom(const std::shared_ptr<XmlChunk>&, const XmlParser&) const override;
            void writeOn(const std::shared_ptr<XmlChunk>&, const SoundShape*, XmlWriter&) const override;
    };

}
