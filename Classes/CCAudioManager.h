#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <string>
#include <unordered_set>

class CCAudioManager
{
public:
    static CCAudioManager* sharedManager();

    void playBgMusic(const std::string& file, bool loop = true);
    void stopBgMusic();
    void pauseBgMusic();
    void resumeBgMusic();
    void setBgMusicVolume(float volume);

    void playSoundEffect(const std::string& file, bool loop = false);
    void stopSoundEffect();
    void pauseSoundEffect();
    void resumeSoundEffect();
    void setSoundEffectVolume(float volume);

    void setMuted(bool muted);
    bool isMuted() const { return m_isMuted; }
    void setMusicVolume(float volume);
    void setEffectsVolume(float volume);
    float getMusicVolume() const { return m_musicVolume; }
    float getEffectsVolume() const { return m_effectsVolume; }

    void preloadSoundEffect(const std::string& file);
    void unloadSoundEffect(const std::string& file);

    bool isBgMusicPlaying();

private:
    CCAudioManager();
    ~CCAudioManager();

    static CCAudioManager* s_instance;

    bool m_isMuted;
    float m_musicVolume;
    float m_effectsVolume;
    std::string m_currentBgMusic;

    std::unordered_set<std::string> m_preloadEffects;
};
