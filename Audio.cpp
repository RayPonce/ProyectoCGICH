#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <vector>
#include <string>

class AudioManager {
public:
    AudioManager() {
        alutInit(0, nullptr);
    }

    ~AudioManager() {
        alutExit();
    }

    // Cargar un sonido y devolver su ID
    ALuint LoadSound(const std::string& filename) {
        ALuint buffer = alutCreateBufferFromFile(filename.c_str());
        return buffer;
    }

    // Reproducir un sonido en loop (soundtrack)
    void PlaySoundtrack(const std::string& filename) {
        ALuint buffer = LoadSound(filename);
        ALuint source;
        alGenSources(1, &source);
        alSourcei(source, AL_LOOPING, AL_TRUE);
        alSourcei(source, AL_BUFFER, buffer);
        alSourcePlay(source);
        soundtrackSource = source; // Guardar la fuente para poder detenerla más tarde
    }

    // Reproducir audio espacial
    void PlaySpatialSound(const std::string& filename, const glm::vec3& position) {
        ALuint buffer = LoadSound(filename);
        ALuint source;
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffer);
        alSource3f(source, AL_POSITION, position.x, position.y, position.z);
        alSourcePlay(source);
    }

    // Reproducir efecto de sonido
    void PlayEffectSound(const std::string& filename) {
        ALuint buffer = LoadSound(filename);
        ALuint source;
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffer);
        alSourcePlay(source);
    }

    // Detener el soundtrack
    void StopSoundtrack() {
        alSourceStop(soundtrackSource);
    }

private:
    ALuint soundtrackSource; // Fuente para el soundtrack
};