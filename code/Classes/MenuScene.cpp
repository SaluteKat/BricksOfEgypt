#include "MenuScene.h"
#include "HelloWorldScene.h"
USING_NS_CC;

Scene* MenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("startMenu.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
	addChild(bg, 0);

	//add the menuItems and menu
	auto start = MenuItemImage::create("start.png", "start.png", CC_CALLBACK_1(MenuScene::startMenuCallback, this));
	start->setPosition(Vec2(visibleSize.width / 3 + origin.x + 14, visibleSize.height / 3 + origin.y + 6));
	auto help = MenuItemImage::create("help.png", "help.png", CC_CALLBACK_1(MenuScene::helpCallback, this));
	help->setPosition(Vec2(visibleSize.width / 1.5 + origin.x - 15, visibleSize.height / 3 + origin.y + 6));
	auto options = MenuItemImage::create("options.png", "options.png", CC_CALLBACK_1(MenuScene::opCallback, this));
	options->setPosition(Vec2(visibleSize.width / 3 + origin.x + 15, visibleSize.height / 5 + origin.y + 10));
	auto exit = MenuItemImage::create("exit.png", "exit.png", CC_CALLBACK_1(MenuScene::exitCallback, this));
	exit->setPosition(Vec2(visibleSize.width / 1.5 + origin.x - 17, visibleSize.height / 5 + origin.y + 10));
	
	auto menu = Menu::create(start, help, options, exit, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu, 1);

	return true;
}

bool HelloWorld::endless = false; //init the static var

void MenuScene::startMenuCallback(cocos2d::Ref * pSender) {
	auto s = HelloWorld::createScene();
	Director::getInstance()->replaceScene(s);
}
void MenuScene::opCallback(cocos2d::Ref * pSender) {
	HelloWorld::endless = true;
	auto s = HelloWorld::createScene();
	Director::getInstance()->replaceScene(s);
}
void MenuScene::helpCallback(cocos2d::Ref * pSender) {
	scrollView = ui::ScrollView::create();
	scrollView->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	scrollView->setBackGroundColor(Color3B(139, 69, 19));
	scrollView->setContentSize(visibleSize);
	scrollView->setInnerContainerSize(Size(visibleSize.width, visibleSize.height * 2));
	scrollView->setBounceEnabled(true);
	scrollView->setAnchorPoint(Vec2(0.5, 0.5));
	scrollView->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	addText();
	addChild(scrollView, 2);
}
void MenuScene::exitCallback(cocos2d::Ref * pSender) {
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

//called by helpCallback()
void MenuScene::addText() {
	auto text = ui::Text::create("In old Egyptian legend", "fonts/arial.ttf", 30);
	text->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*2 - 40));
	scrollView->addChild(text);
	auto text1 = ui::Text::create("there's the cure which can make people evergreen", "fonts/arial.ttf", 30);
	text1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*2 - 2*40));
	scrollView->addChild(text1);
	auto text2 = ui::Text::create("buried deeply in the most dangerous pyramid", "fonts/arial.ttf", 30);
	text2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*2 - 3*40));
	scrollView->addChild(text2);
	auto text3 = ui::Text::create("Once you get there", "fonts/arial.ttf", 30);
	text3->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 - 4 * 40));
	scrollView->addChild(text3);
	auto text4 = ui::Text::create("you'd better keep your ball away from the ground", "fonts/arial.ttf", 30);
	text4->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 - 5 * 40));
	scrollView->addChild(text4);
	auto text5 = ui::Text::create("by moving your board to catch it", "fonts/arial.ttf", 30);
	text5->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 - 6 * 40));
	scrollView->addChild(text5);
	auto text6 = ui::Text::create("Or Anubis, the angel of death will draw you to the hell", "fonts/arial.ttf", 30);
	text6->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 - 7 * 40));
	scrollView->addChild(text6);
	auto text7 = ui::Text::create("It's better to focus on the ball", "fonts/arial.ttf", 30);
	text7->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 - 8 * 40));
	scrollView->addChild(text7);
	auto text8 = ui::Text::create("to judge and predict its track", "fonts/arial.ttf", 30);
	text8->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 - 9 * 40));
	scrollView->addChild(text8);
	auto text9 = ui::Text::create("The good news is that", "fonts/arial.ttf", 30);
	text9->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 - 10 * 40));
	scrollView->addChild(text9);
	auto text10 = ui::Text::create("you can have a practice", "fonts/arial.ttf", 30);
	text10->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 - 11 * 40));
	scrollView->addChild(text10);
	auto text11 = ui::Text::create("by choosing \"options\" in a endless mode", "fonts/arial.ttf", 30);
	text11->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 - 12 * 40));
	scrollView->addChild(text11);
	auto text12 = ui::Text::create("Take it slow", "fonts/arial.ttf", 30);
	text12->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 - 13 * 40));
	scrollView->addChild(text12);
	auto text13 = ui::Text::create("And take care", "fonts/arial.ttf", 30);
	text13->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 - 14 * 40));
	scrollView->addChild(text13);
	auto text14 = ui::Text::create("\nGood luck!", "fonts/arial.ttf", 30);
	text14->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 - 15.5 * 40));
	scrollView->addChild(text14);

	auto back = MenuItemFont::create("Back", CC_CALLBACK_1(MenuScene::backCallback, this));
	back->setFontName("fonts/arial.ttf");
	back->setColor(Color3B(0, 0, 0));
	back->setFontSize(50);
	back->setPosition(Vec2(origin.x + visibleSize.width / 2 + 150, origin.y + visibleSize.height * 2 - 18 * 40));
	
	auto menu = Menu::create(back, NULL);
	menu->setPosition(Vec2::ZERO);
	scrollView->addChild(menu);
}
//called by addText()
void MenuScene::backCallback(cocos2d::Ref * pSender) {
	scrollView->removeFromParent();
}