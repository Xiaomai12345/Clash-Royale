#include"AudioManager.h"
#include "SimpleAudioEngine.h"
AudioManager* AudioManager::_instance = nullptr;

//单例
AudioManager* AudioManager::getInstance()
{
	if (_instance == nullptr) {
		_instance = new AudioManager();
	}
	return _instance;
}

//初始化音频
void AudioManager::initAudio()
{
	//预加载主界面bgm
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/sounds/menu_03.wav");
	//预加载加载完成音效
	SimpleAudioEngine::getInstance()->preloadEffect("music/effect/scroll_loading_01.wav");

	//设置音量
	setBgVolume(1.0f);
	setEffectVolume(1.0f);

	return;
}

//播放背景音乐
void AudioManager::playBgMusic(const std::string& musicPath)
{
	auto audio = SimpleAudioEngine::getInstance();
	CCLOG("AudioManager播放BGM：%s", musicPath.c_str());
	// 兜底：播放前先预加载
	audio->preloadBackgroundMusic(musicPath.c_str());
	
	if (audio->isBackgroundMusicPlaying()) {
		audio->stopBackgroundMusic();
	}
	// 播放
	audio->playBackgroundMusic(musicPath.c_str(), true);
	// 打印播放状态，方便排查
	CCLOG("BGM播放状态：%s", audio->isBackgroundMusicPlaying() ? "正在播放" : "播放失败");
}

//暂停背景音乐
void AudioManager::pauseBgMusic()
{
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

//停止背景音乐
void AudioManager::stopBgMusic()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

//播放音效
void AudioManager::playEffect(const std::string& effectPath)
{
	SimpleAudioEngine::getInstance()->playEffect(effectPath.c_str());
}

//设置背景音乐音量(0~1)
void AudioManager::setBgVolume(float Volume)
{
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(Volume);
}

//设置音效音乐音量
void AudioManager::setEffectVolume(float Volume)
{
	SimpleAudioEngine::getInstance()->setEffectsVolume(Volume);
}