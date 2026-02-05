#pragma once

#include "cocos2d.h"
#include <string>
#include "CCAudioManager.h"

class CCGameManager
{
public:
	static CCGameManager* sharedManager();

	int getCurrentScore() const { return m_currentScore; }
	int getBestScore() const { return m_bestScore; }

	void setCurrentScore(int score);
	void addScore(int points);
	void resetCurrentScore();

	bool isNewBestScore() const;

	void saveData();
	void loadData();

	int getTotalGamesPlayed() const { return m_totalGames; }
	int getTotalBoxesDestroyed() const { return m_totalBoxes; }
	

	void incrementGamesPlayed();
	void incrementBoxesDestroyed();

	void setMusicVolume(float volume);
	void setEffectsVolume(float volume);
	void setMuted(bool muted);
	float getMusicVolume() const { return m_musicVolume; }
	float getEffectsVolume() const { return m_effectsVolume; }
	bool isMuted() const { return m_isMuted; }

	void playSoundEffect(const std::string& file, bool loop = false);
	void playBackgroundMusic(const std::string& file, bool loop = true);
	void stopBackgroundMusic();
	void pauseBackgroundMusic();
	void resumeBackgroundMusic();

private:
	CCGameManager();
	~CCGameManager();

	static CCGameManager* s_instance;

	int m_currentScore;
	int m_bestScore;
	int m_totalGames;
	int m_totalBoxes;

	float m_musicVolume;
	float m_effectsVolume;
	bool m_isMuted;

	const std::string SAVE_FILE = "CCGameManager.json";
};