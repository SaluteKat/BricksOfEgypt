#ifndef __MENU_SEBCE_H__
#define __MENU_SEBCE_H__

#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
using namespace std;
USING_NS_CC;

class MenuScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// 4 selector callback functions
	void startMenuCallback(cocos2d::Ref* pSender);
	void opCallback(cocos2d::Ref * pSender);
	void helpCallback(cocos2d::Ref * pSender);
	void exitCallback(cocos2d::Ref * pSender);
	void backCallback(cocos2d::Ref * pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(MenuScene);

private:
	cocos2d::ui::ScrollView* scrollView;
	void addText();
	Size visibleSize;
	Vec2 origin;
};

#endif // __MENU_SEBCE_H__
