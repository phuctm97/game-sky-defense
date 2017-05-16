#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <cocos2d.h>
#include <AudioEngine.h>

#define SIZE_VISIBLE cocos2d::Director::getInstance()->getVisibleSize()
#define DIRECTOR cocos2d::Director::getInstance()
#define EVENT_DISPATCHER cocos2d::Director::getInstance()->getEventDispatcher()
#define SPRITE_FRAME_CACHE cocos2d::SpriteFrameCache::getInstance()

#define AUDIO_ENGINE cocos2d::experimental::AudioEngine
#define AUDIO_PRELOAD(_fileName_) cocos2d::experimental::AudioEngine::preload(_fileName_)
#define AUDIO_PLAY(_fileName_) cocos2d::experimental::AudioEngine::play2d(_fileName_)
#define AUDIO_PLAY_2(_fileName_, _loop_) cocos2d::experimental::AudioEngine::play2d(_fileName_, _loop_)
#define AUDIO_PLAY_3(_fileName_, _loop_, _volume_) cocos2d::experimental::AudioEngine::play2d(_fileName_, _loop_, _volume_)


enum Z_ORDERS 
{
	BG0, BG1, MID0, MID1, MAIN0, MAIN1, UI0, UI1
};

#endif