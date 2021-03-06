#include <AudioController.h>
#include <player/stream/AudioStreamPlayer.h>

namespace urchin {

    /**
     * @param soundTrigger Trigger used to play the sound
     */
    AudioController::AudioController(std::shared_ptr<Sound> sound, std::shared_ptr<SoundTrigger> soundTrigger, StreamUpdateWorker& streamUpdateWorker) :
            sound(std::move(sound)),
            soundTrigger(std::move(soundTrigger)),
            triggerValue(SoundTrigger::STOPPED),
            audioPlayer(std::make_unique<AudioStreamPlayer>(*this->sound, streamUpdateWorker)),
            isPaused(false) {

    }

    AudioController::~AudioController() {
        audioPlayer->stop();
    }

    Sound& AudioController::getSound() const {
        return *sound;
    }

    SoundTrigger& AudioController::getSoundTrigger() const {
        return *soundTrigger;
    }

    void AudioController::changeSoundTrigger(std::shared_ptr<SoundTrigger> newSoundTrigger) {
        audioPlayer->stop();
        triggerValue = SoundTrigger::STOPPED;

        soundTrigger = std::move(newSoundTrigger);
    }

    void AudioController::pause() {
        if (audioPlayer->isPlaying()) {
            audioPlayer->pause();

            isPaused = true;
        }
    }

    void AudioController::unpause() {
        if (isPaused) {
            audioPlayer->play(); //as it's a unpause: use 'play' or 'playLoop' method doesn't make any difference

            isPaused = false;
        }
    }

    void AudioController::process(const Point3<float>& listenerPosition) {
        if (triggerValue != SoundTrigger::STOPPED && sound->isStopped()) {
            triggerValue = SoundTrigger::STOPPED;
        }

        SoundTrigger::TriggerResultValue oldTriggerValue = triggerValue;
        SoundTrigger::TriggerResultValue newTriggerValue = soundTrigger->evaluateTrigger(listenerPosition);
        if (newTriggerValue != SoundTrigger::NO_TRIGGER) {
            triggerValue = newTriggerValue;
        }

        processTriggerValue(oldTriggerValue);
    }

    void AudioController::processTriggerValue(SoundTrigger::TriggerResultValue oldTriggerValue) {
        if (triggerValue != oldTriggerValue) {
            if (triggerValue == SoundTrigger::PLAYING) {
                audioPlayer->play();
            } else if (triggerValue == SoundTrigger::PLAYING_LOOP) {
                audioPlayer->playLoop();
            } else if (triggerValue == SoundTrigger::STOPPED) {
                audioPlayer->stop();
            } else if (triggerValue == SoundTrigger::PAUSED) {
                audioPlayer->pause();
            }
        }
    }

}
