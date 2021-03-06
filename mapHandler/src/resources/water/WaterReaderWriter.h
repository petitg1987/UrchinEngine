#pragma once

#include <memory>

#include <UrchinCommon.h>
#include <Urchin3dEngine.h>

namespace urchin {

    class WaterReaderWriter {
        public:
            Water* loadFrom(const DataChunk*, const DataParser&) const;
            void writeOn(DataChunk*, const Water*, DataWriter&) const;

        private:
            static void loadGeneralPropertiesOn(Water*, const DataChunk*, const DataParser&);
            static void writeGeneralPropertiesOn(DataChunk*, const Water*, DataWriter&);

            static void loadWaterSurfaceProperties(Water*, const DataChunk*, const DataParser&);
            static void writeWaterSurfacePropertiesOn(DataChunk*, const Water*, DataWriter&);

            static void loadUnderWaterProperties(Water*, const DataChunk*, const DataParser&);
            static void writeUnderWaterPropertiesOn(DataChunk*, const Water*, DataWriter&);

            static constexpr char CENTER_POSITION_TAG[] = "centerPosition";
            static constexpr char X_SIZE_TAG[] = "xSize";
            static constexpr char Z_SIZE_TAG[] = "zSize";
            static constexpr char WATER_COLOR_TAG[] = "waterColor";
            static constexpr char NORMAL_FILENAME_TAG[] = "normalFilename";
            static constexpr char DUDV_MAP_FILENAME_TAG[] = "dudvMapFilename";
            static constexpr char WAVE_SPEED_TAG[] = "waveSpeed";
            static constexpr char WAVE_STRENGTH_TAG[] = "waveStrength";
            static constexpr char S_REPEAT_TAG[] = "sRepeat";
            static constexpr char T_REPEAT_TAG[] = "tRepeat";
            static constexpr char DENSITY_TAG[] = "density";
            static constexpr char GRADIENT_TAG[] = "gradient";
    };

}
