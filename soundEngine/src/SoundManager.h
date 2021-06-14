#pragma once

#include <vector>
#include <thread>
#include <UrchinCommon.h>

#include <AudioController.h>
#include <device/DeviceManager.h>
#include <sound/Sound.h>
#include <trigger/SoundTrigger.h>
#include <player/stream/StreamUpdateWorker.h>

namespace urchin {

    class SoundManager {
        public:
            SoundManager();
            ~SoundManager();

            void addSound(Sound*, SoundTrigger*);
            void removeSound(const Sound*);
            void changeSoundTrigger(const Sound*, SoundTrigger*);
            void setupSoundsVolume(Sound::SoundType, float); //TODO use SoundCategory instead of SoundType

            void setMasterVolume(float);
            float getMasterVolume() const;

            void pause();
            void unpause();

            void checkNoExceptionRaised();
            void process(const Point3<float>&);
            void process();

        private:
            void deleteAudioController(AudioController*) const;

            std::vector<AudioController*> audioControllers;

            //stream chunk updater thread
            StreamUpdateWorker* streamUpdateWorker;
            std::thread *streamUpdateWorkerThread;
    };

}
