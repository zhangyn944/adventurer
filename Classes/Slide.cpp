#include "Slide.h"
#include "DialogLayer.h"

#include <string>
using namespace std;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

Slide::Slide() : m_world(NULL)
{

}

Slide::~Slide()
{

}

Scene* Slide::createScene()
{
	Scene* scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Point(0, 0));
	auto layer = Slide::create(scene->getPhysicsWorld());
	scene->addChild(layer);

	return scene;
}

Slide* Slide::create(PhysicsWorld* world)
{
	Slide* pRet = new Slide();
	if (pRet && pRet->init(world))
	{
		return pRet;
	}
	pRet = NULL;
	return NULL;
}

bool Slide::init(PhysicsWorld* world)
{
	if (!Layer::init())
	{
		return false;
	}
	defence = false;
	difficalty = 0.2;
	moveTimes = 10;
	bleed = 100;
	timesNow = 0.0;
	timeLeft = 10;
	bomb = 0;
	shel = 0;
	//SimpleAudioEngine::getInstance()->playBackgroundMusic("game.wav", true);
	visibleSize = Director::getInstance()->getVisibleSize();

	m_world = world;
	dispatcher = Director::getInstance()->getEventDispatcher();
	/*isCut = false;
	oldPos = Point(Director::getInstance()->getVisibleSize().width / 2, 0);*/

	bleedLabel = Label::createWithTTF("Your bleed: 100", "fonts/Marker Felt.ttf", 40);
	bleedLabel->setColor(Color3B::GREEN);
	bleedLabel->setPosition(Point(15, visibleSize.height - 20));
	this->addChild(bleedLabel, 1);

	moveNumLabel = Label::createWithTTF("Left move times: 10", "fonts/Marker Felt.ttf", 40);
	moveNumLabel->setColor(Color3B::ORANGE);
	moveNumLabel->setPosition(Point((visibleSize.width - moveNumLabel->getWidth()) / 2, visibleSize.height - 20));
	this->addChild(moveNumLabel, 1);

	timeLabel = Label::createWithTTF("Left time: 10", "fonts/Marker Felt.ttf", 40);
	timeLabel->setColor(Color3B::BLUE);
	timeLabel->setPosition(Point(visibleSize.width - timeLabel->getWidth() - 100, visibleSize.height - 20));
	this->addChild(timeLabel, 1);

	bombLabel = Label::createWithTTF("Bomb left: 0", "fonts/Marker Felt.ttf", 40);
	bombLabel->setColor(Color3B::GREEN);
	bombLabel->setPosition(Point(40, 20));
	this->addChild(bombLabel, 1);

	shelLabel = Label::createWithTTF("shel left: 0", "fonts/Marker Felt.ttf", 40);
	shelLabel->setColor(Color3B::GREEN);
	shelLabel->setPosition(Point(visibleSize.width - 80, 20));
	this->addChild(shelLabel, 1);

	auto bgsprite = Sprite::create("background1.jpg");
	bgsprite->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bgsprite, 0);

	player = Sprite::create("player2.png");
	player->setAnchorPoint(Vec2(0.5, 0.5));
	player->setPhysicsBody(PhysicsBody::createCircle(35));
	player->getPhysicsBody()->setCategoryBitmask(0x00001111);
	player->getPhysicsBody()->setContactTestBitmask(0x00001111);
	player->setPosition(visibleSize / 2);
	player->getPhysicsBody()->setTag(1);
	player->getPhysicsBody()->setAngularVelocityLimit(0);
	addChild(player);

	Node* ground = Node::create();
	ground->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(0, 0), Vec2(visibleSize.width, 0)));
	ground->getPhysicsBody()->setDynamic(false);
	addChild(ground);
	Node* ground2 = Node::create();
	ground2->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(0, 0), Vec2(0, visibleSize.height)));
	ground2->getPhysicsBody()->setDynamic(false);
	addChild(ground2);
	Node* ground3 = Node::create();
	ground3->setPhysicsBody(PhysicsBody::createEdgeSegment(visibleSize, Vec2(visibleSize.width, 0)));
	ground3->getPhysicsBody()->setDynamic(false);
	addChild(ground3);
	Node* ground4 = Node::create();
	ground4->setPhysicsBody(PhysicsBody::createEdgeSegment(visibleSize, Vec2(0, visibleSize.height)));
	ground4->getPhysicsBody()->setDynamic(false);
	addChild(ground4);

	this->schedule(schedule_selector(Slide::update), 0.2);

	preloadMusic();
	playBgm();

	TouchEvent();
	//testCustomEvent();

	testKeyboardEvent();
	//testMouseEvent();

	//testAccelerationEvent();
	testOntouch();
	return true;
}

void Slide::preloadMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
	//SimpleAudioEngine::getInstance()->preloadEffect("music/shoot.mp3");
}

void Slide::playBgm()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
}

void Slide::TouchEvent()
{
	/*streak = MotionStreak::create(0.5f, 10, 30, Color3B::WHITE, "Slide/flash.png");
	this->addChild(streak, 2);*/

	auto listner = EventListenerTouchOneByOne::create();
	listner->onTouchBegan = CC_CALLBACK_2(Slide::onTouchBegan, this);//´¥Ãþ¿ªÊ¼
	listner->onTouchMoved = CC_CALLBACK_2(Slide::onTouchMoved, this);//´¥Ãþ¹ý³Ì
	listner->onTouchEnded = CC_CALLBACK_2(Slide::onTouchEnded, this);//´¥Ãþ½áÊø
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listner, this);
}

void Slide::testKeyboardEvent()
{
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = CC_CALLBACK_2(Slide::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Slide::onKeyReleased, this);

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Slide::testOntouch() {
	auto touchListener = EventListenerPhysicsContact::create();
	touchListener->onContactBegin = CC_CALLBACK_1(Slide::onConcactBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 1);
}

bool Slide::onConcactBegan(PhysicsContact& contact) {
	if (contact.getShapeA()->getBody()->getTag() % 10 == 0) {
		auto ps = ParticleSystemQuad::create("explo.plist");//Á£×ÓÐ§¹ûÊ¹ÓÃ
		ps->setPosition(contact.getShapeA()->getBody()->getPosition());
		this->addChild(ps);

		if (contact.getShapeB()->getBody()->getTag() % 10 == 0) {
			contact.getShapeB()->getBody()->removeFromWorld();
			if (contact.getShapeB()->getBody()->getNode())
				((Sprite*)(contact.getShapeB()->getBody()->getNode()))->removeFromParentAndCleanup(true);
			enemys.eraseObject(contact.getShapeB()->getBody());
		}
		if (contact.getShapeB()->getBody()->getTag() == 1) {
			bleedNew(contact.getShapeA()->getBody()->getTag());
		}
		enemys.eraseObject(contact.getShapeA()->getBody());
		contact.getShapeA()->getBody()->removeFromWorld();
		if (contact.getShapeA()->getBody()->getNode())
			((Sprite*)(contact.getShapeA()->getBody()->getNode()))->removeFromParentAndCleanup(true);
	}
	else if (contact.getShapeA()->getBody()->getTag() == 1 && contact.getShapeB()->getBody()->getTag() % 10 == 0) {
		auto ps = ParticleSystemQuad::create("explo.plist");//Á£×ÓÐ§¹ûÊ¹ÓÃ
		ps->setPosition(contact.getShapeB()->getBody()->getPosition());
		this->addChild(ps);
		bleedNew(contact.getShapeB()->getBody()->getTag());
		enemys.eraseObject(contact.getShapeB()->getBody());
		contact.getShapeB()->getBody()->removeFromWorld();
		if (contact.getShapeB()->getBody()->getNode())
			((Sprite*)(contact.getShapeB()->getBody()->getNode()))->removeFromParentAndCleanup(true);
	}
	else if (contact.getShapeA()->getBody()->getTag() % 10 == 5 && contact.getShapeB()->getBody()->getTag() == 1) {
		switch (contact.getShapeA()->getBody()->getTag())
		{
		case 5:
			shel++;
			char ttt[30];
			sprintf(ttt, "shel left: %d", shel);
			shelLabel->setString(ttt);
			break;
		case 15:
			bomb++;
			char tt[30];
			sprintf(tt, "bomb left: %d", bomb);
			bombLabel->setString(tt);
			break;
		case 25:
			moveTimes += 3;
			char t[30];
			sprintf(t, "Left move time: %d", moveTimes);
			moveNumLabel->setString(t);
			break;
		default:
			break;
		}
		contact.getShapeA()->getBody()->removeFromWorld();
		if (contact.getShapeA()->getBody()->getNode())
			((Sprite*)(contact.getShapeA()->getBody()->getNode()))->removeFromParentAndCleanup(true);
	}
	else if (contact.getShapeB()->getBody()->getTag() % 10 == 5 && contact.getShapeA()->getBody()->getTag() == 1) {
		switch (contact.getShapeA()->getBody()->getTag())
		{
		case 5:
			shel++;
			char ttt[30];
			sprintf(ttt, "shel left: %d", shel);
			shelLabel->setString(ttt);
			break;
		case 15:
			bomb++;
			char tt[30];
			sprintf(tt, "bomb left: %d", bomb);
			bombLabel->setString(tt);
			break;
		case 25:
			moveTimes += 3;
			char t[30];
			sprintf(t, "Left move time: %d", moveTimes);
			moveNumLabel->setString(t);
			break;
		default:
			break;
		}
		contact.getShapeB()->getBody()->removeFromWorld();
		if (contact.getShapeB()->getBody()->getNode())
			((Sprite*)(contact.getShapeB()->getBody()->getNode()))->removeFromParentAndCleanup(true);
	}
	return true;
}

void Slide::update(float f) {
	newEnemys(difficalty);
	//if (CCRANDOM_0_1() < 0.01 - difficalty * 0.01) addBonus();
	if (CCRANDOM_0_1() < 0.1) addBonus();
	timesNow += f;
	if (timesNow >= 1.0) {
		timeLeft--;
		timesNow = 0.0;
	}
	char t[30];
	sprintf(t, "Left time: %d", timeLeft);
	timeLabel->setString(t);
	if (timeLeft == 0) escape();
}

//void Slide::Schedule_(Ref *ref)
//{
//	schedule(schedule_selector(Slide::update), 0.5f);
//}

bool Slide::onTouchBegan(Touch *touch, Event *unused_event){
	if (moveTimes-- > 0) {
		Point location = touch->getLocation();
		if (location.x >= visibleSize.width || location.x <= 0) {
			location.x = location.x > 0 ? visibleSize.width : 0;
		}
		if (location.y >= visibleSize.height || location.y <= 0) {
			location.y = location.y > 0 ? visibleSize.height : 0;
		}
		//Õ½»úÒÆ¶¯
		player->setPosition(location);
		char t[30];
		sprintf(t, "Left move time: %d", moveTimes);
		moveNumLabel->setString(t);
		return true;
	}
}

void Slide::onTouchMoved(Touch *touch, Event *unused_event) {
	//Point location = touch->getLocation();
	//if (location.x >= visibleSize.width || location.x <= 0) {
	//	location.x = location.x > 0 ? visibleSize.width : 0;
	//}
	//if (location.y >= visibleSize.height || location.y <= 0) {
	//	location.y = location.y > 0 ? visibleSize.height : 0;
	//}
	////Õ½»úÒÆ¶¯
	//player->runAction(MoveTo::create(5, location));
}

void Slide::onTouchEnded(Touch *touch, Event *unused_event){}

void Slide::newEnemys(float f) {
	int newNum = 1;
	if (f > 0.3) newNum = 2;
	if (f > 0.7) newNum = 3;
	while (newNum--) {
		int type = 0;
		if (CCRANDOM_0_1() > 0.85) { type = 2; }
		else if (CCRANDOM_0_1() > 0.6) { type = 1; }

		Point location = Vec2(0, 0);
		switch (rand() % 4)
		{
		case 0:
			location.y = visibleSize.height;
			location.x = rand() % (int)(visibleSize.width);
			break;
		case 1:
			location.x = visibleSize.width;
			location.y = rand() % (int)(visibleSize.height);
			break;
		case 2:
			location.y = 0;
			location.x = rand() % (int)(visibleSize.width);
			break;
		case 3:
			location.x = 0;
			location.y = rand() % (int)(visibleSize.height);
			break;
		default:
			break;
		}
		addEnemy(type, location);
	}
}

void Slide::addEnemy(int type, Point p) {
	Sprite *re;
	switch (type)
	{
	case 0:
		re = Sprite::create("stone1.png");
		re->setPhysicsBody(PhysicsBody::createCircle(re->getContentSize().height / 2));
		re->getPhysicsBody()->setTag(10);
		break;
	case 1:
		re = Sprite::create("stone2.png");
		re->setPhysicsBody(PhysicsBody::createCircle(re->getContentSize().height / 2));
		re->getPhysicsBody()->setTag(20);
		break;
	case 2:
		re = Sprite::create("stone3.png");
		re->setPhysicsBody(PhysicsBody::createCircle(re->getContentSize().height / 2));
		re->getPhysicsBody()->setTag(30);
		break;
	default:
		re = Sprite::create("stone1.png");
		re->setPhysicsBody(PhysicsBody::createCircle(re->getContentSize().height / 2));
		re->getPhysicsBody()->setTag(10);
		break;
	}

	re->setAnchorPoint(Vec2(0.5, 0.5));
	re->setScale(0.5, 0.5);
	re->setPosition(p);
	re->getPhysicsBody()->setCategoryBitmask(0x00000001);
	re->getPhysicsBody()->setContactTestBitmask(0x00000001);
	if (rand() % 100 < (100 * difficalty)) {
		re->getPhysicsBody()->setVelocity((player->getPosition() - p) * difficalty);
	}
	else {
		re->getPhysicsBody()->setVelocity((Point(rand() % (int)(visibleSize.width), rand() % (int)(visibleSize.height)) - p) * difficalty);
	}
	enemys.pushBack(re->getPhysicsBody());
	addChild(re);
}

void Slide::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		player->getPhysicsBody()->setVelocity(Point(-200, player->getPhysicsBody()->getVelocity().y));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		player->getPhysicsBody()->setVelocity(Point(200, player->getPhysicsBody()->getVelocity().y));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		player->getPhysicsBody()->setVelocity(Point(player->getPhysicsBody()->getVelocity().x, 200));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		player->getPhysicsBody()->setVelocity(Point(player->getPhysicsBody()->getVelocity().x, -200));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_Q:
		if (bomb > 0) {
			for each (PhysicsBody* s in enemys)
			{
				if (s == nullptr) continue;
				auto ps = ParticleSystemQuad::create("explo.plist");//Á£×ÓÐ§¹ûÊ¹ÓÃ
				ps->setPosition(s->getPosition());
				this->addChild(ps);
				if (s != nullptr) {
					s->removeFromWorld();
					((Sprite*)s->getNode())->removeFromParentAndCleanup(true);
				}
			}
			enemys.clear();

			bomb--;
			char t[30];
			sprintf(t, "bomb left: %d", bomb);
			bombLabel->setString(t);
		}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_E:
		if (shel > 0) {
			this->unschedule(schedule_selector(Slide::nodefence));
			defence = true;
			CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("player2_shel.png");
			player->setTexture(texture);
			this->scheduleOnce(schedule_selector(Slide::nodefence), 5.0f);

			shel--;
			char t[30];
			sprintf(t, "shel left: %d", shel);
			shelLabel->setString(t);
		}
		break;
	default:
		break;
	}
}
void Slide::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(-200, 0));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(200, 0));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(0, 200));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(0, -200));
		break;
	default:
		break;
	}
}

void Slide::bleedNew(int lose) {
	if (defence) return;
	bleed -= lose;
	char t[30];
	sprintf(t, "Your bleed: %d", bleed);
	bleedLabel->setString(t);
	if (bleed <= 30) bleedLabel->setColor(Color3B::RED);
	if (bleed <= 0) gameOver();
}

void Slide::addBonus() {
	Point location = Vec2(0, 0);
	switch (rand() % 4)
	{
	case 0:
		location.y = visibleSize.height;
		location.x = rand() % (int)(visibleSize.width);
		break;
	case 1:
		location.x = visibleSize.width;
		location.y = rand() % (int)(visibleSize.height);
		break;
	case 2:
		location.y = 0;
		location.x = rand() % (int)(visibleSize.width);
		break;
	case 3:
		location.x = 0;
		location.y = rand() % (int)(visibleSize.height);
		break;
	default:
		break;
	}
	Sprite *re;
	switch (rand() % 3)
	{
	case 0:
		re = Sprite::create("defence.png");
		re->setPhysicsBody(PhysicsBody::createCircle(30));
		re->getPhysicsBody()->setTag(5);
		re->getPhysicsBody()->setCategoryBitmask(0x00000010);
		re->getPhysicsBody()->setContactTestBitmask(0x00000010);
		break;
	case 1:
		re = Sprite::create("bomb1.png");
		re->setPhysicsBody(PhysicsBody::createCircle(30));
		re->getPhysicsBody()->setTag(15);
		re->getPhysicsBody()->setCategoryBitmask(0x00000100);
		re->getPhysicsBody()->setContactTestBitmask(0x00000100);
		break;
	default:
		re = Sprite::create("defend.png");
		re->setPhysicsBody(PhysicsBody::createCircle(30));
		re->getPhysicsBody()->setTag(25);
		re->getPhysicsBody()->setCategoryBitmask(0x00001000);
		re->getPhysicsBody()->setContactTestBitmask(0x00001000);
		break;
	}
	re->setAnchorPoint(Vec2(0.5, 0.5));
	re->setPosition(location);
	re->getPhysicsBody()->setVelocity((Point(rand() % (int)(visibleSize.width), rand() % (int)(visibleSize.height)) - location) * difficalty);
	addChild(re, 2);
}

void Slide::nodefence(float f) {
	defence = false;
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("player2.png");
	player->setTexture(texture);
}

void Slide::escape() {
	defence = false;
	difficalty = 0.2;
	moveTimes = 10;
	bleed = 100;
	timesNow = 0.0;
	timeLeft = 10;
	bomb = 0;
	shel = 0;

	auto l = Label::createWithTTF("You Escape!!", "fonts/Marker Felt.ttf", 40);
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	l->setColor(Color3B::GREEN);
	l->setPosition(visibleSize / 2);
	this->addChild(l, 2);
	//Director::getInstance()->popScene();
	auto scene = Scene::create();
	auto *layer = HelloWorld::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}

void Slide::gameOver() {
	/*auto l = Label::createWithTTF("You Fail!!", "fonts/Marker Felt.ttf", 40);
	l->setColor(Color3B::RED);
	l->setPosition(visibleSize / 2);
	this->addChild(l, 2);*/

	defence = false;
	difficalty = 0.2;
	moveTimes = 10;
	bleed = 100;
	timesNow = 0.0;
	timeLeft = 10;
	bomb = 0;
	shel = 0;

	auto scene = Scene::create();
	auto *layer = DialogLayer::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}