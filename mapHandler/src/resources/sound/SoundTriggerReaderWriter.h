#pragma once

#include <memory>
#include <string>

#include <UrchinCommon.h>
#include <UrchinSoundEngine.h>

namespace urchin {

    class SoundTriggerReaderWriter {
        public:
            static SoundTrigger* loadFrom(const std::shared_ptr<XmlChunk>&, const XmlParser&);
            static void writeOn(const std::shared_ptr<XmlChunk>&, const SoundTrigger*, XmlWriter&);

        private:
            static SoundTrigger* buildSoundTriggerFrom(const std::shared_ptr<XmlChunk>&, const XmlParser&);
            static void buildChunkFrom(const std::shared_ptr<XmlChunk>&, const SoundTrigger*, XmlWriter&);

            static SoundTrigger::PlayBehavior loadPlayBehaviorFrom(const std::shared_ptr<XmlChunk>&, const XmlParser&);
            static void writePlayBehaviorFrom(const std::shared_ptr<XmlChunk>&, SoundTrigger::PlayBehavior, XmlWriter&);

            static constexpr char TYPE_ATTR[] = "type";
            static constexpr char MANUAL_VALUE[] = "MANUAL";
            static constexpr char SHAPE_VALUE[] = "SHAPE";
            static constexpr char PLAY_ONCE_VALUE[] = "PLAY_ONCE";
            static constexpr char PLAY_LOOP_VALUE[] = "PLAY_LOOP";
            static constexpr char PLAY_BEHAVIOR_TAG[] = "playBehavior";
            static constexpr char SOUND_SHAPE_TAG[] = "soundShape";
    };

}
