#include "CCAudioManager.h"

USING_NS_CC;

CCAudioManager* CCAudioManager::s_instance = NULL;

CCAudioManager* CCAudioManager::sharedManager()
{
	if (!s_instance) {
		s_instance = new CCAudioManager();
	}
	return s_instance;
}

CCAudioManager::CCAudioManager()
	: m_isMuted(false)
	, m_musicVolume(1.0f)
	, m_effectsVolume(1.0f)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("");
}

CCAudioManager::~CCAudioManager()
{
	s_instance = NULL;
}					  

void CCAudioManager::playBgMusic(const std::string& file, bool loop)
{
	if (m_isMuted) return;

	if (file != m_currentBgMusic) {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(file.c_str(), loop);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_musicVolume);
		m_currentBgMusic = file;
	}
}

void CCAudioManager::stopBgMusic()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    m_currentBgMusic.clear();
}

void CCAudioManager::pauseBgMusic()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void CCAudioManager::resumeBgMusic()
{
    if (m_isMuted) return;

    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void CCAudioManager::setBgMusicVolume(float volume)
{
    m_musicVolume = volume;
    if (!m_isMuted) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
    }
}

void CCAudioManager::playSoundEffect(const std::string& file, bool loop)
{
    if (m_isMuted) return;

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(file.c_str(), loop);
}

void CCAudioManager::stopSoundEffect()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void CCAudioManager::pauseSoundEffect()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

void CCAudioManager::resumeSoundEffect()
{
    if (m_isMuted) return;

    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

void CCAudioManager::setSoundEffectVolume(float volume)
{
    m_effectsVolume = volume;
    if (!m_isMuted) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
    }
}

void CCAudioManager::setMuted(bool muted)
{
    m_isMuted = muted;

    if (m_isMuted) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
    }
    else {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_musicVolume);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_effectsVolume);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    }
}

void CCAudioManager::setMusicVolume(float volume)
{
    setBgMusicVolume(volume);
}

void CCAudioManager::setEffectsVolume(float volume)
{
    setSoundEffectVolume(volume);
}

void CCAudioManager::preloadSoundEffect(const std::string& file)
{
    if (m_preloadEffects.find(file) == m_preloadEffects.end()) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(file.c_str());
        m_preloadEffects.insert(file);
    }
}

void CCAudioManager::unloadSoundEffect(const std::string& file)
{
    auto it = m_preloadEffects.find(file);
    if (it != m_preloadEffects.end()) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect(file.c_str());
        m_preloadEffects.erase(it);
    }
}

bool CCAudioManager::isBgMusicPlaying()
{
    return !m_currentBgMusic.empty();
}