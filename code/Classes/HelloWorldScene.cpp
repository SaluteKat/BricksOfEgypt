#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;
#define database UserDefault::getInstance()

Scene* HelloWorld::createScene()
{
	auto scene = Scene::createWithPhysics();
	//debug
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	if(endless) count = 1;
	else count = 2;
	start = false;
	brick_now = 0;
	win = false;
	isMove = false;

	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;

	addSprite();//add the edge, ball, board bricks' countdown, backgrounds and bricks
	addListener();
	music();//preload music and play the bgm

	//record & read the num of hitted bricks from database
	if(!database->getIntegerForKey("num")) database->setIntegerForKey("num", 0);
	else brick_hit = database->getIntegerForKey("num");

	//check if the ball fall down to the groud
	schedule(schedule_selector(HelloWorld::dropBall), 0.05f, kRepeatForever, 0);
	//update the position of board and the bricks' countdown
	schedule(schedule_selector(HelloWorld::update), 0.04f, kRepeatForever, 0);
	//create the bricks randomly
	if(endless) schedule(schedule_selector(HelloWorld::addBricksRand), 8.0f, kRepeatForever, 0);
	//check if bricks all disappeared
	else schedule(schedule_selector(HelloWorld::checkBrick), 0.05f, kRepeatForever, 0);

	//keyboardEvent
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event * event) {};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

    return true;
}

void HelloWorld::addSprite() {
	//edge
	auto edgeNode = Node::create();
	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	body->getShape(0)->setFriction(0.0f);
	body->getShape(0)->setRestitution(1.0f);
	body->setCategoryBitmask(0x04);
	body->setCollisionBitmask(0x04);
	edgeNode->setPhysicsBody(body);
	edgeNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(edgeNode, 0);

	//board
	board = Sprite::create("board.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 86, 18)));
	board->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y+10));
	auto boardbody = PhysicsBody::createBox(board->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 0.0f));
	boardbody->setCategoryBitmask(0x01);
	boardbody->setCollisionBitmask(0x01);
	boardbody->setContactTestBitmask(0x01);
	boardbody->setGravityEnable(false);
	board->setPhysicsBody(boardbody);
	this->addChild(board, 1);

	//ball
	ball = Sprite::create("ball3.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 29, 28)));
	ball->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + board->getContentSize().height+10));
	auto ballbody = PhysicsBody::createCircle(ball->getContentSize().height / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	ballbody->setMass(10);
	ballbody->setContactTestBitmask(0x05);
	ballbody->setCategoryBitmask(0x05);
	ballbody->setGravityEnable(false);
	ball->setPhysicsBody(ballbody);
	this->addChild(ball, 1);
	ball->setTag(1);

	//bricks' countdown or countup
	if(endless) words = Label::createWithTTF("Bricks hit:", "fonts/arial.ttf", 30);
	else words = Label::createWithTTF("Bricks left:", "fonts/arial.ttf", 30);
	enemy = Label::createWithTTF("", "fonts/arial.ttf", 36);
	enemy->setPosition(Vec2(origin.x + 15, origin.y + visibleSize.height - enemy->getContentSize().height - 55));
	words->setPosition(Vec2(origin.x + 73, origin.y + visibleSize.height - enemy->getContentSize().height - 20));
	addChild(words, 1);
	addChild(enemy, 1);

	//bricks
	addBrick(count);
}
void HelloWorld::addListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}
void HelloWorld::music() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("bgm.wav");
	audio->preloadEffect("boom.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm.wav", true);
}

void HelloWorld::addBrick(int x) {
//a branch control function
//load brick's design and bg for different rounds in the branches
	if (x == 1) addBrick1(); //for endless mode
	if (x == 2) addBrick2();
	if (x == 3) addBrick3();
	if (x == 4) addBrick4();
}
void HelloWorld::addBrick1() {
	int bricks_num = 3;
	while (bricks_num--) {
		auto brickNode = createBrick();
		float x = visibleSize.width / 2;
		float y = visibleSize.height - (10 + brickNode->getContentSize().height)*(3 - bricks_num);
		brickNode->setPosition(x, y);
		addChild(brickNode, 1);
	}

	bg = Sprite::create("Egypt1.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
	this->addChild(bg, 0);
}
void HelloWorld::addBrick2() {
	int bricks_num = 5;
	brick_now = bricks_num;
	while (bricks_num--) {
		auto brickNode = createBrick();
		float x, y;
		if (bricks_num == 4 || bricks_num == 1) {
			x = visibleSize.width / 4;
		}
		else if (bricks_num == 3 || bricks_num == 0) {
			x = visibleSize.width / 4 * 3;
		}
		else
			x = visibleSize.width / 2;
		if (bricks_num == 4 || bricks_num == 3) {
			y = visibleSize.height - (10 + brickNode->getContentSize().height);
		}
		else if(bricks_num == 2){
			y = visibleSize.height - (10 + brickNode->getContentSize().height)*2;
		}
		else y = visibleSize.height - (10 + brickNode->getContentSize().height)*3;
		brickNode->setPosition(x, y);
		addChild(brickNode, 1);
	}

	bg = Sprite::create("Egypt2.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
	this->addChild(bg, 0);
}
void HelloWorld::addBrick3() {
	int bricks_num = 9;
	brick_now = bricks_num;
	while (bricks_num--) {
		auto brickNode = createBrick();
		float x = visibleSize.width / 4 * (3 - bricks_num % 3);
		float y = visibleSize.height - (10 + brickNode->getContentSize().height)*(3 - (int)(bricks_num / 3));
		brickNode->setPosition(x, y);
		addChild(brickNode, 1);
	}

	bg->removeFromParent();
	bg = Sprite::create("Egypt3.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
	this->addChild(bg, 0);
}
void HelloWorld::addBrick4() {
	int bricks_num = 13;
	brick_now = bricks_num;
	while (bricks_num--) {
		auto brickNode = createBrick();
		float x, y;
		int num1 = bricks_num % 5;
		if (num1 == 2) 	x = visibleSize.width / 4 * 1;
		else if (num1 == 4) x = visibleSize.width / 3 * 1;
		else if (num1 == 1) x = visibleSize.width / 2;
		else if (num1 == 3) x = visibleSize.width / 3 * 2;
		else x = visibleSize.width / 4 * 3;
		if(bricks_num == 12 || bricks_num == 11 || bricks_num == 10) 
			y = visibleSize.height - (10 + brickNode->getContentSize().height);
		else if (bricks_num == 9|| bricks_num == 8)
			y = visibleSize.height - (10 + brickNode->getContentSize().height)*2;
		else if(bricks_num == 7 || bricks_num == 6 || bricks_num == 5)
			y = visibleSize.height - (10 + brickNode->getContentSize().height)*3;
		else if(bricks_num == 4 || bricks_num == 3)
			y = visibleSize.height - (10 + brickNode->getContentSize().height)*4;
		else y = visibleSize.height - (10 + brickNode->getContentSize().height)*5;
		brickNode->setPosition(x, y);
		addChild(brickNode, 1);
	}

	bg->removeFromParent();
	bg = Sprite::create("Egypt4.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
	this->addChild(bg, 0);
}

void HelloWorld::addBricksRand(float dt) {
	auto brickNode = createBrick();
	//generate the coordinates randomly and keep them in the range
	float x = random(origin.x, visibleSize.width);
	x = x > origin.x + brickNode->getContentSize().width / 2 ? x : origin.x + brickNode->getContentSize().width / 2;
	x = x < origin.x + visibleSize.width - brickNode->getContentSize().width / 2 ? x :
		origin.x + visibleSize.width - brickNode->getContentSize().width / 2;
	float y = random(origin.y, visibleSize.height);
	y = y > origin.y + brickNode->getContentSize().height / 2 ? y : origin.y + brickNode->getContentSize().height / 2;
	y = y < origin.y + visibleSize.height - brickNode->getContentSize().height / 2 ? y :
		origin.y + visibleSize.height - brickNode->getContentSize().height / 2;
	brickNode->setPosition(x, y);
	addChild(brickNode, 1);
}
void HelloWorld::checkBrick(float dt) {
//by checking the present #bricks to judge if all bricks have been cleared
//if so, change the scene
	if (brick_now == 0) {
		if (count < 4) nextScene();
		else {
			win = true;
			showGameOver();
		}
	}
}
void HelloWorld::dropBall(float dt) {
	//ball's position is too low which means it fall
	auto ball_position = ball->getPosition();
	if (ball_position.y - origin.y <= 20) showGameOver();
}
void HelloWorld::update(float dt) {
	if(isMove) moveboard(movekey);
	String* s;
	if (endless) s = String::createWithFormat("%d", brick_hit);
	else s = String::createWithFormat("%d", brick_now);
	string str = s->getCString();
	enemy->setString(str);
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW: //left and right for moving
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		isMove = true;
		if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
			movekey = 'A';
		}
		else
		{
			movekey = 'D';
		}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE: //space for shooting
		if (start == false) {
			start = true;
			Vect force = Vect(1000, 2100.0f);
			ball->getPhysicsBody()->applyImpulse(force);
		}
	default:
		break;
	}
}
void HelloWorld::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		isMove = false;
	}
}

//called by addListener()
bool HelloWorld::onContactBegin(PhysicsContact & contact) {
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	int tagA = nodeA->getTag();
	int tagB = nodeB->getTag();
	if (tagA == 1 && tagB == 2 ) {
		nodeB->removeFromParent();
		if (endless) {
			brick_hit++;
			database->setIntegerForKey("num", brick_hit);
		}
		else brick_now--;
		SimpleAudioEngine::getInstance()->playEffect("boom.mp3", false);
	}
	else if (tagA == 2 && tagB == 1) {
		nodeA->removeFromParent();
		if (endless) {
			brick_hit++;
			database->setIntegerForKey("num", brick_hit);
		}
		else brick_now--;
		SimpleAudioEngine::getInstance()->playEffect("boom.mp3", false);
	}
	return true;
}
//called by checkBrick(float dt)
void HelloWorld::nextScene() {
	count++;
	start = false; 
	//get the ball and the board to the original position
	board->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 10));
	ball->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + board->getContentSize().height + 10));
	ball->getPhysicsBody()->setVelocity(Vec2(0, 0));
	addBrick(count);
}
void HelloWorld::showGameOver() {
	if (win) {
		auto win_label = Label::createWithTTF("YOU WIN!", "fonts/arial.ttf", 60);
		win_label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->addChild(win_label, 2);
	}
	else
	{
		ball->removeFromParent();
		//auto over = Sprite::create("Game_Over.jpg");
		/*auto over = Label::createWithTTF("Game Over!", "fonts/arial.ttf", 60);
		over->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->addChild(over, 2);*/
		auto label1 = Label::createWithTTF("Game Over!", "fonts/arial.ttf", 60);
		label1->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 50);
		this->addChild(label1,2);

		auto label2 = Label::createWithTTF("Replay", "fonts/arial.ttf", 40);
		auto replayBtn = MenuItemLabel::create(label2, CC_CALLBACK_1(HelloWorld::replayCallback, this));
		Menu* replay = Menu::create(replayBtn, NULL);
		replay->setPosition(visibleSize.width / 2 - 80, visibleSize.height / 2 - 40);
		this->addChild(replay, 2);

		auto label3 = Label::createWithTTF("Exit", "fonts/arial.ttf", 40);
		auto exitBtn = MenuItemLabel::create(label3, CC_CALLBACK_1(HelloWorld::exitCallback, this));
		Menu* exit = Menu::create(exitBtn, NULL);
		exit->setPosition(visibleSize.width / 2 + 90, visibleSize.height / 2 - 40);
		this->addChild(exit, 2);
	}

	unschedule(schedule_selector(HelloWorld::checkBrick));
	unschedule(schedule_selector(HelloWorld::dropBall));
	unschedule(schedule_selector(HelloWorld::addBricksRand));
}
//called by update(float dt)
void HelloWorld::moveboard(char c) {
	switch (c) {
	case 'A':
		board->runAction(MoveBy::create(0.01, Vec2(-min(board->getBoundingBox().getMinX(), 20.00f), 0)));
		if (start == false) {
			ball->runAction(MoveBy::create(0.01, Vec2(-min(board->getBoundingBox().getMinX(), 20.00f), 0)));
		}
		break;
	case 'D':
		board->runAction(MoveBy::create(0.01, Vec2(min(visibleSize.width - board->getBoundingBox().getMaxX(), 20.00f), 0)));
		if (start == false) {
			ball->runAction(MoveBy::create(0.01, Vec2(min(visibleSize.width - board->getBoundingBox().getMaxX(), 20.00f), 0)));
		}
		break;
	}
}
//called when bricks added -- there are five bricks'adding functions
Sprite* HelloWorld::createBrick() {
	auto brickbody = PhysicsBody::createBox(Size(210.0f, 45.5f), PhysicsMaterial(100.f, 1.0f, 0.0f));
	brickbody->setGravityEnable(false);
	brickbody->setCategoryBitmask(0x01);
	brickbody->setContactTestBitmask(0x01);
	auto brickNode = Sprite::create("brick_1.png");
	brickNode->setTag(2);
	brickNode->setScale(0.5f, 0.5f);
	brickNode->setPhysicsBody(brickbody);
	return brickNode;
}

void HelloWorld::replayCallback(Ref * pSender) {
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}

void HelloWorld::exitCallback(Ref * pSender) {
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}