#ifndef _DEMO_MAINLAYER_H_
#define _DEMO_MAINLAYER_H_

#include "cocos2d.h"

class TestGame : public cocos2d::Layer
{
public:
    TestGame();
	~TestGame();
	virtual bool init();
	CREATE_FUNC(TestGame);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	//virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:

    cocos2d::Vector<cocos2d::ClippingNode * > succlistSp;
    cocos2d::Vector<cocos2d::ClippingNode * > listSp;
	CC_SYNTHESIZE(float, winWith, WinWith);
	CC_SYNTHESIZE(float, winHeight, WinHeight);
	void initPic();
	int state[3][3];
    void checkSptite();
	int selectIndex;
    cocos2d::ClippingNode *selectNode;
    std::vector<cocos2d::Vec2> position;
	cocos2d::Vec2 beganLocatin;
	cocos2d::Sprite * coverSp;
    cocos2d::Size textureSize;
    std::vector<int> _success;
    bool checkSuccess(int tag);
    bool succmove = false;
};


#endif //_DEMO_MAINLAYER_H_
