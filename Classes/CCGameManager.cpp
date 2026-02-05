#include "CCGameManager.h"
#include "cocos-ext.h"
#include <fstream>
#include <sstream>

USING_NS_CC;

CCGameManager* CCGameManager::s_instance = NULL;

CCGameManager* CCGameManager::sharedManager()
{
	if (!s_instance) {
		s_instance = new CCGameManager();
		s_instance->loadData();
	}
	return s_instance;
}

CCGameManager::CCGameManager()
    : m_currentScore(0)
    , m_bestScore(0)
    , m_totalGames(0)
    , m_totalBoxes(0)
    , m_musicVolume(1.0f)
    , m_effectsVolume(1.0f)
    , m_isMuted(false)
{
    CCAudioManager::sharedManager()->preloadSoundEffect("560576__theplax__explosion-3.wav");
    CCAudioManager::sharedManager()->preloadSoundEffect("levelEnding.ogg");
    CCAudioManager::sharedManager()->preloadSoundEffect("zapsplat_multimedia_ui_positive_tone_plucked_warm_positive_success_97094.mp3");
}

CCGameManager::~CCGameManager()
{
    saveData();
}

void CCGameManager::resetCurrentScore()
{
    m_currentScore = 0;
}

void CCGameManager::setCurrentScore(int score)
{
    m_currentScore = score;

    if (m_currentScore > m_bestScore) {
        m_bestScore = m_currentScore;
    }
}

void CCGameManager::addScore(int points)
{
    setCurrentScore(m_currentScore + points);
}

bool CCGameManager::isNewBestScore() const
{
    return m_currentScore >= m_bestScore && m_currentScore > 0;
}

void CCGameManager::incrementGamesPlayed()
{
    m_totalGames++;
    saveData();
}

void CCGameManager::incrementBoxesDestroyed()
{
    m_totalBoxes++;
}

void CCGameManager::setMusicVolume(float volume)
{
    m_musicVolume = volume;
    CCAudioManager::sharedManager()->setMusicVolume(volume);
}

void CCGameManager::setEffectsVolume(float volume)
{
    m_effectsVolume = volume;
    CCAudioManager::sharedManager()->setEffectsVolume(volume);
}

void CCGameManager::setMuted(bool muted)
{
    m_isMuted = muted;
    CCAudioManager::sharedManager()->setMuted(muted);
}

void CCGameManager::playSoundEffect(const std::string& file, bool loop)
{
    CCAudioManager::sharedManager()->playSoundEffect(file, loop);
}

void CCGameManager::playBackgroundMusic(const std::string& file, bool loop)
{
    CCAudioManager::sharedManager()->playBgMusic(file, loop);
}

void CCGameManager::stopBackgroundMusic()
{
    CCAudioManager::sharedManager()->stopBgMusic();
}

void CCGameManager::pauseBackgroundMusic()
{
    CCAudioManager::sharedManager()->pauseBgMusic();
}

void CCGameManager::resumeBackgroundMusic()
{
    CCAudioManager::sharedManager()->resumeBgMusic();
}

void CCGameManager::saveData()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();

    userDefault->setIntegerForKey("bestScore", m_bestScore);
    userDefault->setIntegerForKey("totalGames", m_totalGames);
    userDefault->setIntegerForKey("totalBoxes", m_totalBoxes);
    userDefault->setFloatForKey("musicVolume", m_musicVolume);
    userDefault->setFloatForKey("effectsVolume", m_effectsVolume);
    userDefault->setBoolForKey("isMuted", m_isMuted);

    userDefault->flush();

    CCLOG("Game data saved: Best=%d, MusicVol=%.1f, EffectsVol=%.1f",
        m_bestScore, m_musicVolume, m_effectsVolume);
}

void CCGameManager::loadData()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();

    m_bestScore = userDefault->getIntegerForKey("bestScore", 0);
    m_totalGames = userDefault->getIntegerForKey("totalGames", 0);
    m_totalBoxes = userDefault->getIntegerForKey("totalBoxes", 0);
    m_musicVolume = userDefault->getFloatForKey("musicVolume", 1.0f);
    m_effectsVolume = userDefault->getFloatForKey("effectsVolume", 1.0f);
    m_isMuted = userDefault->getBoolForKey("isMuted", false);

    CCAudioManager::sharedManager()->setMusicVolume(m_musicVolume);
    CCAudioManager::sharedManager()->setEffectsVolume(m_effectsVolume);
    CCAudioManager::sharedManager()->setMuted(m_isMuted);

    CCLOG("Game data loaded: Best=%d, Games=%d, Boxes=%d, MusicVol=%.1f, EffectsVol=%.1f, Muted=%d",
        m_bestScore, m_totalGames, m_totalBoxes, m_musicVolume, m_effectsVolume, m_isMuted);
}