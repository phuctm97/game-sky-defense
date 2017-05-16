#include "MainScene.h"
USING_NS_CC;

Scene* MainScene::createScene()
{
	Scene* scene = Scene::create();
	Layer* layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}
MainScene::~MainScene() 
{
	CC_SAFE_RELEASE(actHealthSwing);
	CC_SAFE_RELEASE(actBombGrow);
	CC_SAFE_RELEASE(actObjRotate);
	CC_SAFE_RELEASE(actShockWave);
	CC_SAFE_RELEASE(actHitGround);
	CC_SAFE_RELEASE(actExplosion);
	CC_SAFE_RELEASE(actUfo);
	CC_SAFE_RELEASE(actRay);

	poolMeteors.clear();
	poolHealths.clear();
	poolClouds.clear();
	fallingObjs.clear();
}



bool MainScene::init()
{
	if (!Layer::init())return false;

	doInitProperties();
	doInitGraphics();
	doInitEvents();
	doInitAudios();

	return true;
}
void MainScene::doInitProperties() 
{
	SPRITE_FRAME_CACHE->addSpriteFramesWithFile("sprite_sheet.plist");

	indexOfMeteor = 0; indexOfHealth = 0;

	playing = false;
}
void MainScene::doInitGraphics() 
{
	Sprite* tmp; int i;

	// ===========================================
	// Background
	// ----------
	tmp = Sprite::create("bg.png");
	tmp->setPosition(SIZE_VISIBLE*0.5f);
	this->addChild(tmp, Z_ORDERS::BG0);
	// ===========================================

	// ===========================================
	// Cities
	// ------
	for(i=0; i<2; i++) {
		tmp = Sprite::createWithSpriteFrameName("city_dark.png");
		tmp->setAnchorPoint(Vec2(0.5f, 0));
		tmp->setPosition(Vec2(SIZE_VISIBLE.width*(0.25f + i*0.5f), 0));
		this->addChild(tmp, Z_ORDERS::BG1);

		tmp = Sprite::createWithSpriteFrameName("city_light.png");
		tmp->setAnchorPoint(Vec2(0.5f, 0));
		tmp->setPosition(Vec2(SIZE_VISIBLE.width*(0.25f + i*0.5f), SIZE_VISIBLE.height*0.1f));
		this->addChild(tmp, Z_ORDERS::BG0);
	}
	// ===========================================

	// ===========================================
	// Trees
	// -----
	for(i=0; i<3; i++) {
		tmp = Sprite::createWithSpriteFrameName("trees.png");
		tmp->setAnchorPoint(Vec2(0.5f, 0));
		tmp->setPosition(Vec2(SIZE_VISIBLE.width*(0.2f + i*0.3f), 0));
		this->addChild(tmp, Z_ORDERS::BG1);
	}
	// ===========================================

	// ===========================================
	// Clouds
	// ------
	poolClouds.reserve(4);
	for(i=0; i<4; i++) {
		tmp = Sprite::createWithSpriteFrameName("cloud.png");
		tmp->setPosition(Vec2(
			SIZE_VISIBLE.width*( 0.1f + i*0.3f ),
			SIZE_VISIBLE.height*( i%2==0 ? 0.4f : 0.5f )));
		this->addChild(tmp, Z_ORDERS::BG1);
		poolClouds.pushBack(tmp);
	}
	// ===========================================

	// ===========================================
	// Labels
	// ------
	lblScore = Label::createWithBMFont("font.fnt", "0");
	lblScore->setAnchorPoint(Vec2(1, 0.5f));
	lblScore->setPosition(Vec2(SIZE_VISIBLE.width*0.8f, SIZE_VISIBLE.height*0.94f));
	this->addChild(lblScore, Z_ORDERS::UI0); 

	lblEnergy = Label::createWithBMFont("font.fnt", "100%");
	lblEnergy->setAnchorPoint(Vec2(0, 0.5f));
	lblEnergy->setPosition(Vec2(SIZE_VISIBLE.width*0.2f, SIZE_VISIBLE.height*0.94f));
	this->addChild(lblEnergy, Z_ORDERS::UI0);

	lblCombo = Label::createWithTTF("combo", "font.ttf", 30);
	lblCombo->setTextColor(Color4B::WHITE);
	lblCombo->enableOutline(Color4B::BLACK, 3);
	lblCombo->setVisible(false);
	this->addChild(lblCombo, Z_ORDERS::UI1);

	tmp = Sprite::createWithSpriteFrameName("health_icon.png");
	tmp->setAnchorPoint(Vec2(1, 0.5f));
	tmp->setPosition(lblEnergy->getPosition() - Vec2(10, 0));
	this->addChild(tmp, Z_ORDERS::UI0);
	// ===========================================

	// ===========================================
	// Panel Intro & Game-over
	// -----------------------
	pnlIntro = Sprite::createWithSpriteFrameName("logo.png");
	pnlIntro->setPosition(Vec2(SIZE_VISIBLE.width*0.5f, SIZE_VISIBLE.height*0.6f));
	this->addChild(pnlIntro, Z_ORDERS::UI1);

	pnlGameOver = Sprite::createWithSpriteFrameName("gameover.png");
	pnlGameOver->setPosition(Vec2(SIZE_VISIBLE.width*0.5f, SIZE_VISIBLE.height*0.65f));
	pnlGameOver->setVisible(false);
	this->addChild(pnlGameOver, Z_ORDERS::UI1);
	// ===========================================

	// ===========================================
	// Bomb & Shockwave
	// ----------------
	bomb = Sprite::createWithSpriteFrameName("bomb.png");
	bomb->getTexture()->generateMipmap();
	bomb->setPosition(SIZE_VISIBLE*0.5f);
	bomb->setVisible(false);
	this->addChild(bomb, Z_ORDERS::MAIN1);

	Size sizeOfBomb = bomb->getContentSize();

	sparkle = Sprite::createWithSpriteFrameName("sparkle.png");
	sparkle->setPosition(Vec2(sizeOfBomb.width*0.72f, sizeOfBomb.height*0.72f));
	bomb->addChild(sparkle);

	halo = Sprite::createWithSpriteFrameName("halo.png");
	halo->setPosition(Vec2(sizeOfBomb.width*0.4f, sizeOfBomb.height*0.4f));
	bomb->addChild(halo);

	shockWave = Sprite::createWithSpriteFrameName("shockwave.png");
	shockWave->getTexture()->generateMipmap();
	shockWave->setVisible(false);
	this->addChild(shockWave, Z_ORDERS::MAIN1);
	// ===========================================

	// ===========================================
	// Pool of Meteors & Healths
	// -------------------------
	poolMeteors.reserve(50);
	for(i=0; i<50; i++) {
		tmp = Sprite::createWithSpriteFrameName("meteor.png");
		tmp->setVisible(false);
		this->addChild(tmp, Z_ORDERS::MAIN0, OBJ_TAG::METEOR);
		poolMeteors.pushBack(tmp);
	}

	poolHealths.reserve(20);
	for(i=0; i<20; i++) {
		tmp = Sprite::createWithSpriteFrameName("health.png");
		tmp->setVisible(false);
		tmp->setAnchorPoint(Vec2(0.5f, 0.8f));
		this->addChild(tmp, Z_ORDERS::MAIN0, OBJ_TAG::HEALTH);
		poolHealths.pushBack(tmp);
	}
	// ===========================================
	
	// ===========================================
	// UFO & Ray
	// ---------
	ufo = Sprite::createWithSpriteFrameName("ufo_1.png");
	ufo->setVisible(false);
	this->addChild(ufo, Z_ORDERS::MAIN0);

	ray = Sprite::createWithSpriteFrameName("ray.png");
	ray->setAnchorPoint(Vec2(0.5f, 1));
	ray->setPosition(Vec2(
		ufo->getContentSize().width * 0.52f,
		ufo->getContentSize().height * 0.5f));
	ray->setVisible(false);
	ufo->addChild(ray);
	// ===========================================
	
	// ===========================================
	// Actions
	// -------
	actHealthSwing = RepeatForever::create((ActionInterval*) Sequence::create(
		EaseInOut::create(RotateTo::create(1.2f, 10), 2),
		EaseInOut::create(RotateTo::create(1.2f, -10), 2),
		nullptr));
	actHealthSwing->retain();

	actShockWave = Sequence::create(
		FadeOut::create(1.0f),
		CallFunc::create(CC_CALLBACK_0(MainScene::onShockWaveDone, this)),
		nullptr);
	actShockWave->retain();

	actBombGrow = ScaleTo::create(6.0f, 1.0f);
	actBombGrow->retain();

	actObjRotate = RepeatForever::create(RotateBy::create(0.5f, -90));
	actObjRotate->retain();

	Animation* ani;
	ani = Animation::create();
	for(i=1; i<=10; i++) {
		std::string name = StringUtils::format("boom%i.png", i);
		ani->addSpriteFrame(SPRITE_FRAME_CACHE->getSpriteFrameByName(name.c_str()));
	}
	ani->setDelayPerUnit(1/10.0f);
	ani->setRestoreOriginalFrame(true);
	actHitGround = Sequence::create(
		MoveBy::create(0, Vec2(0, SIZE_VISIBLE.height*0.12f)),
		Animate::create(ani),
		CallFuncN::create(CC_CALLBACK_1(MainScene::onActionDone, this)),
		nullptr);
	actHitGround->retain();

	ani = Animation::create();
	for(i=1; i<=7; i++) {
		std::string name = StringUtils::format("explosion_small%i.png", i);
		ani->addSpriteFrame(SPRITE_FRAME_CACHE->getSpriteFrameByName(name));
	}
	ani->setDelayPerUnit(1/7.0f);
	ani->setRestoreOriginalFrame(true);
	actExplosion = Sequence::create(
		Animate::create(ani),
		CallFuncN::create(CC_CALLBACK_1(MainScene::onActionDone, this)),
		nullptr);
	actExplosion->retain();

	ani = Animation::create();
	for(i=1; i<=4; i++) {
		std::string name = StringUtils::format("ufo_%i.png", i);
		ani->addSpriteFrame(SPRITE_FRAME_CACHE->getSpriteFrameByName(name));
	}
	ani->setDelayPerUnit(1/4.0f);
	ani->setLoops(-1);
	actUfo = Animate::create(ani);
	actUfo->retain();

	actRay = RepeatForever::create(Blink::create(4, 6));
	actRay->retain();
	// ===========================================
}
void MainScene::doInitEvents() 
{
	EventListenerKeyboard* listenerKeyboard = EventListenerKeyboard::create();
	listenerKeyboard->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
	EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listenerKeyboard, this);

	EventListenerTouchOneByOne* listenerTouch = EventListenerTouchOneByOne::create();
	listenerTouch->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority(listenerTouch, this);

	this->scheduleUpdate();
}
void MainScene::doInitAudios() 
{
}


bool MainScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* e)
{
	if(!playing) {
		if(pnlIntro->isVisible()) {
			pnlIntro->setVisible(false);
			AUDIO_ENGINE::stopAll();
			AUDIO_PLAY_2("background.mp3", true);
		}
		else if(pnlGameOver->isVisible()) {
			pnlGameOver->setVisible(false);
			AUDIO_ENGINE::stopAll();
			AUDIO_PLAY_2("background.mp3", true);
		}

		playing = true;
		doResetGame();

		return true;
	}

	if(bomb->isVisible()) {
		bomb->stopAllActions();
		sparkle->stopAllActions();
		halo->stopAllActions();

		if(bomb->getScale() > 0.3f) {
			shockWave->setScale(0.1f);
			shockWave->setVisible(true);
			shockWave->setOpacity(255);
			shockWave->setPosition(bomb->getPosition());
			shockWave->runAction(ScaleTo::create(0.5f, bomb->getScale()*2));
			shockWave->runAction(actShockWave->clone());

			AUDIO_PLAY("bombRelease.mp3");
		}
		else {
			AUDIO_PLAY("bombFail.mp3");
		}

		bomb->setVisible(false);
		comboHits = 0;
	}
	else {
		bomb->setVisible(true);
		bomb->setPosition(touch->getLocation());
		bomb->setScale(0.1f);
		bomb->setOpacity(50);
		bomb->runAction(actBombGrow->clone());

		sparkle->runAction(actObjRotate->clone());
		halo->runAction(actObjRotate->clone());
	}

	return true;
}
void MainScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e) 
{
	if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		DIRECTOR->end();
	}
}


void MainScene::onActionDone(cocos2d::Node* sender)
{
	sender->setVisible(false);
}
void MainScene::onShockWaveDone()
{
	shockWave->setVisible(false);
	if(comboHits > 1) {
		lblCombo->stopAllActions();

		lblCombo->setString(StringUtils::format("combo +%i", comboHits));
		lblCombo->setVisible(true);
		lblCombo->setPosition(shockWave->getPosition()+Vec2(0,20));
		lblCombo->setScale(0.5f);
		lblCombo->runAction(FadeIn::create(0.1f));
		lblCombo->runAction(Sequence::create(
			EaseInOut::create(ScaleTo::create(0.5f, 1.5f), 2),
			FadeOut::create(0.1f),
			CallFuncN::create(CC_CALLBACK_1(MainScene::onActionDone, this)),
			nullptr));
	}
}
void MainScene::onFallingDone(cocos2d::Node* sender) 
{
	fallingObjs.eraseObject((Sprite*)sender);

	sender->stopAllActions();
	sender->setRotation(0);

	if(sender->getTag() == OBJ_TAG::METEOR) {
		energy -= 15;
		sender->runAction(actHitGround->clone());

		AUDIO_PLAY("boom.mp3");
	}
	else {
		sender->setVisible(false);
		if(energy>=100) {
			score += 25;
			lblScore->setString(StringUtils::toString<int>(score));
		}
		else {
			energy = MIN(100, energy+10);
		}

		AUDIO_PLAY("health.mp3");
	}
	lblEnergy->setString(StringUtils::toString<int>(int(energy)));

	if(energy<=0) {
		energy = 0;
		doStopGame();

		AUDIO_ENGINE::stopAll();
		AUDIO_PLAY("fire_truck.mp3");
	}
}

void MainScene::update(float dt)
{
	if(!playing)return;

	if(shockWave->isVisible()) {
		Sprite* fallingObj;
		int numberOfFallingObjs = fallingObjs.size();
		for(int i=numberOfFallingObjs-1; i>=0; i--) {
			fallingObj = fallingObjs.at(i);
			if((shockWave->getPosition()-fallingObj->getPosition()).getLengthSq()
				<= pow(shockWave->getContentSize().width*0.5f, 2)) {
				fallingObj->stopAllActions();
				fallingObj->runAction(actExplosion->clone());
				if(fallingObj->getTag() == OBJ_TAG::METEOR) {
					comboHits++;
					score += comboHits*15;
					lblScore->setString(StringUtils::toString<int>(score));	
				}

				fallingObjs.erase(i);
				AUDIO_PLAY("boom.mp3");
			}
		}
	}

	if(ufo->isVisible() && ray->isVisible()) {
		if(ufo->getPositionX() >= SIZE_VISIBLE.width*0.1f
			&& ufo->getPositionX() <= SIZE_VISIBLE.width*0.9f) {
			energy -= 0.1f;
			if(energy<=0) {
				energy = 0;
				doStopGame(); 
				AUDIO_ENGINE::stopAll();
				AUDIO_PLAY("fire_truck.mp3");
			}
			lblEnergy->setString(StringUtils::toString<int>(int(energy)));
		}
	}

	if(bomb->isVisible() && bomb->getScale()>0.3f && bomb->getOpacity()<255) {
		bomb->setOpacity(255);
	}
	if(ufo->isVisible() && (!killedUfo)) {
		if((shockWave->getPosition()-ufo->getPosition()).getLengthSq()
				<= pow(shockWave->getContentSize().width*0.5f, 2)) {
			killedUfo = true;
			ufo->stopAllActions();
			ray->stopAllActions();
			ray->setVisible(false);

			ufo->runAction(actExplosion->clone());
			AUDIO_PLAY("boom.mp3");

			comboHits++;
			score += comboHits * 20;
			lblScore->setString(StringUtils::toString<int>(score));
		}
	}

	for(Sprite* cloud: poolClouds) {
		cloud->setPositionX(cloud->getPositionX() + dt*20);
		if(cloud->getPositionX() > SIZE_VISIBLE.width + cloud->getContentSize().width*0.5f)
			cloud->setPositionX(-cloud->getContentSize().width*0.5f);
	}

	timerOfMeteor -= dt;
	if(timerOfMeteor<=0) {
		doGenerateNewMeteor();
		timerOfMeteor = intervalOfMeteor;
	}

	timerOfHealth -= dt;
	if(timerOfHealth<=0) {
		doGenerateNewHealth();
		timerOfHealth = intervalOfHealth;
	}

	timerOfUfo -= dt;
	if(timerOfUfo<=0) {
		doGenerateNewUfo();
		timerOfUfo = intervalOfUfo;
	}

	timerOfDifficulty -= dt;
	if(timerOfDifficulty<=0) {
		doIncreaseDifficulty();
		timerOfDifficulty = intervalOfDifficulty;
	}
}


void MainScene::doResetGame() 
{
	score = 0; energy = 100;
	comboHits = 0;

	intervalOfMeteor = 3.0f;
	timerOfMeteor = intervalOfMeteor;
	speedOfMeteor = 9;

	intervalOfHealth = 17;
	timerOfHealth = intervalOfHealth;
	speedOfHealth = 15;

	intervalOfUfo = 15;
	timerOfUfo = intervalOfUfo;
	speedOfUfo = 30;
	killedUfo = true;

	intervalOfDifficulty = 60;
	timerOfDifficulty = timerOfDifficulty;

	lblScore->setString(StringUtils::toString<int>(score));
	lblEnergy->setString(StringUtils::toString<int>(int(energy)));
}
void MainScene::doStopGame() 
{
	if(bomb->isVisible()) {
		bomb->stopAllActions();
		halo->stopAllActions();
		sparkle->stopAllActions();

		bomb->setVisible(false);
	}
	if(shockWave->isVisible()) {
		shockWave->stopAllActions();
		shockWave->setVisible(false);
	}

	Sprite* fallingObj;
	int numberOfFallingObjs = fallingObjs.size();
	for(int i=numberOfFallingObjs-1; i>=0; i--) {
		fallingObj = fallingObjs.at(i);
		fallingObj->stopAllActions();
		fallingObj->setVisible(false);

		fallingObjs.erase(i);
		fallingObj->retain();
	}

	ufo->stopAllActions();
	ufo->setVisible(false);
	ray->stopAllActions();
	ray->setVisible(false);

	playing = false; killedUfo = true;
	pnlGameOver->setVisible(true);
}

void MainScene::doGenerateNewMeteor()
{
	if(fallingObjs.size()>30)return;

	Sprite* meteor = poolMeteors.at(indexOfMeteor);
	indexOfMeteor = (indexOfMeteor==poolMeteors.size()-1) ? 0 : indexOfMeteor+1;

	meteor->stopAllActions();
	meteor->setVisible(true);
	meteor->setPosition(Vec2(
		SIZE_VISIBLE.width * random<float>(0.1f, 0.9f),
		SIZE_VISIBLE.height + meteor->getContentSize().height*0.5f));

	meteor->runAction(actObjRotate->clone());
	meteor->runAction(Sequence::create(
		MoveTo::create(speedOfMeteor, Vec2(SIZE_VISIBLE.width * random<float>(0.1f, 0.9f), SIZE_VISIBLE.height*0.15f)),
		CallFuncN::create(CC_CALLBACK_1(MainScene::onFallingDone, this)),
		nullptr));

	fallingObjs.pushBack(meteor);
}
void MainScene::doGenerateNewHealth()
{
	if(fallingObjs.size()>30)return;

	Sprite* health = poolHealths.at(indexOfHealth);
	indexOfHealth = (indexOfHealth==poolHealths.size()-1) ? 0 : indexOfHealth+1;

	health->stopAllActions();
	health->setVisible(true);
	health->setPosition(Vec2(
		SIZE_VISIBLE.width * random<float>(0.1f, 0.9f),
		SIZE_VISIBLE.height + health->getContentSize().height*0.5f));

	health->runAction(actHealthSwing->clone());
	health->runAction(Sequence::create(
		MoveTo::create(speedOfHealth, Vec2(SIZE_VISIBLE.width * random<float>(0.1f, 0.9f), SIZE_VISIBLE.height*0.15f)),
		CallFuncN::create(CC_CALLBACK_1(MainScene::onFallingDone, this)),
		nullptr));

	fallingObjs.pushBack(health);
}
void MainScene::doGenerateNewUfo() 
{
	if(ufo->isVisible())return;
	ufo->stopAllActions();
	ray->stopAllActions();

	int onLeft = random<int>(0,1);

	ufo->setPosition(Vec2(
		onLeft ? -ufo->getContentSize().width*0.5f : SIZE_VISIBLE.width + ufo->getContentSize().width*0.5f,
		SIZE_VISIBLE.height * random<float>(0.3f, 0.7f)
		));
	ufo->setVisible(true);
	ufo->runAction(actUfo->clone());

	ufo->runAction(Sequence::create(
		MoveBy::create(speedOfUfo, Vec2((SIZE_VISIBLE.width + ufo->getContentSize().width)*(onLeft ? 1 : -1), 0)),
		CallFuncN::create(CC_CALLBACK_1(MainScene::onActionDone, this)),
		nullptr));

	ray->setVisible(false);
	ray->runAction(actRay->clone());

	killedUfo = false;
	AUDIO_PLAY("pew.mp3");
}
void MainScene::doIncreaseDifficulty() 
{
	intervalOfMeteor = MAX(0.25f, intervalOfMeteor-0.2f);
	speedOfMeteor = MAX(4, speedOfMeteor-1);
	speedOfHealth = MAX(8, speedOfHealth-1);
}
