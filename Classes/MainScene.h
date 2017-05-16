#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "Include.h"

class MainScene : public cocos2d::Layer
{
	enum OBJ_TAG {
		METEOR, HEALTH
	};
private:
	cocos2d::Label *lblScore, *lblEnergy, *lblCombo;
	cocos2d::Sprite *pnlGameOver, *pnlIntro;

	cocos2d::Sprite *bomb, *sparkle, *halo;
	cocos2d::Sprite *shockWave, *ufo, *ray;

	cocos2d::Vector<cocos2d::Sprite*> poolMeteors;
	cocos2d::Vector<cocos2d::Sprite*> poolHealths;
	cocos2d::Vector<cocos2d::Sprite*> poolClouds;
	cocos2d::Vector<cocos2d::Sprite*> fallingObjs;
	int indexOfMeteor, indexOfHealth;
	void onFallingDone(cocos2d::Node* sender);

	cocos2d::Action *actHealthSwing, *actBombGrow, *actObjRotate, *actShockWave;
	cocos2d::Action *actHitGround, *actExplosion, *actUfo, *actRay;
	void onActionDone(cocos2d::Node* sender);
	void onShockWaveDone();


	bool playing;
	int score, comboHits; float energy;
	float timerOfMeteor, intervalOfMeteor, speedOfMeteor;
	float timerOfHealth, intervalOfHealth, speedOfHealth;
	float timerOfUfo, intervalOfUfo, speedOfUfo; bool killedUfo;
	float timerOfDifficulty, intervalOfDifficulty;
	
	void doResetGame();
	void doStopGame();
	void doGenerateNewMeteor();
	void doGenerateNewHealth();
	void doGenerateNewUfo();
	void doIncreaseDifficulty();

public:
	CREATE_FUNC(MainScene);
	static cocos2d::Scene* createScene();

	~MainScene();
private:
	bool init() override;

	void doInitProperties();
	void doInitGraphics();
	void doInitEvents();
	void doInitAudios();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e);
	void update(float dt) override;

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* e);
};

#endif // !_MAIN_SCENE_H_
