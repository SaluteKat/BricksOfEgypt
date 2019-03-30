#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
	static bool endless; //whether switching to the endless mode
	void setPhysicsWorld(PhysicsWorld * world) { m_world = world; };
    virtual bool init();

	void addSprite();
	void addListener();
	void music();

	void addBrick(int);
	Sprite* HelloWorld::createBrick();
	void addBrick1();
	void addBrick2();
	void addBrick3();
	void addBrick4();
	void addBricksRand(float dt);
	void checkBrick(float dt);
	void dropBall(float dt);
	void update(float f);

	void onKeyPressed(EventKeyboard::KeyCode code, Event * event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event * event);

	bool onContactBegin(PhysicsContact & contact);
	void nextScene();
	void showGameOver();
	void moveboard(char c);

	void replayCallback(Ref * pSender);
	void exitCallback(Ref * pSender);
	// implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	PhysicsWorld* m_world;
	Sprite* ball;
	Sprite* board;
	Sprite* bg;
	Size visibleSize;
	Vec2 origin;
	Label* words;
	Label* enemy; //bricks' countdown

	bool start; //if the ball is moving
	int count; //count for the four rounds
	int brick_now; //present #bricks in scene
	int brick_hit; //count for the endless mode
	bool win;
	bool isMove;
	char movekey;
};

#endif // __HELLOWORLD_SCENE_H__
