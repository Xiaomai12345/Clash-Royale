#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

class AudioManager {
public:
	//单例
	static AudioManager* getInstance();
	static void destroyInstance();
	//初始化音频
	void initAudio();

	//循环播放背景音乐
	void playBgMusic(const std::string& musicPath);

	//暂停背景音乐
	void pauseBgMusic();

	//停止背景音乐
	void stopBgMusic();

	//播放音效
	void playEffect(const std::string& effectPath);

	//设置背景音乐音量(0~1)
	void setBgVolume(float Volume);

	//设置音效音乐音量
	void setEffectVolume(float Volume);

private:
	//私有构造，禁止外部修改
	AudioManager() = default;
	~AudioManager() = default;

	//禁止拷贝
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	static AudioManager* _instance;
};



#endif // AUDIO_MANAGER_H