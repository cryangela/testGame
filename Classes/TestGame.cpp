#include <algorithm>
#include "TestGame.h"



using std::random_shuffle;
USING_NS_CC;
TestGame::TestGame()
{
	// 屏幕可视化长宽
	winWith = Director::getInstance()->getWinSize().width;
	winHeight = Director::getInstance()->getWinSize().height;

}
TestGame::~TestGame()
{}

bool TestGame::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::create());
		this->initPic();

		auto lisener = EventListenerTouchOneByOne::create();
		lisener->onTouchBegan = CC_CALLBACK_2(TestGame::onTouchBegan, this);
		lisener->onTouchMoved = CC_CALLBACK_2(TestGame::onTouchMoved, this);
		lisener->onTouchEnded = CC_CALLBACK_2(TestGame::onTouchEnded, this);
		//lisener->onTouchCancelled = CC_CALLBACK_2(MainLayer::onTouchCancelled, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(lisener, this);

		bRet = true;
	} while (0);
	return bRet;
}

bool TestGame::onTouchBegan(Touch * t, Event * e)
{
	auto location = this->convertToNodeSpace(t->getLocation());
    for (unsigned int i = 0; i < succlistSp.size(); i++)
    {
        auto sp = succlistSp.at(i);
        if (sp->getBoundingBox().containsPoint(location))
        {
            selectNode = sp;
            beganLocatin = sp->getPosition();
            succmove = true;
            return true;
        }
    }
    succmove = false;
	for (unsigned int i = 0; i < listSp.size(); i++)
	{
		auto sp = listSp.at(i);
        if (sp->getBoundingBox().containsPoint(location)&&sp->isVisible())
		{
			selectIndex = i;
            selectNode = sp;
			beganLocatin = sp->getPosition();
			sp->setZOrder(10);

			break;
		}
	}
    
	return true;
}

void TestGame::onTouchMoved(Touch * t, Event * e)
{
    auto contentSize = selectNode->getContentSize();
	auto loc = t->getLocation();
    loc.x -=contentSize.width/2;
    loc.y -=contentSize.height/2;
	if (loc.x - contentSize.width / 2 < 0){
		loc.x = 0;
	}
	if (loc.x + contentSize.width > winWith){
		loc.x = winWith - contentSize.width;
	}
//	if (loc.y - contentSize.height < 0){
//		loc.y = contentSize.height;
//	}
	if (loc.y + contentSize.height > winHeight){
		loc.y = winHeight;
	}
    
    selectNode->setPosition(loc);
}

void TestGame::onTouchEnded(Touch * t, Event * e)
{
    if (succmove){
        selectNode->setPosition(beganLocatin);
        return;
    }
    
	bool hasR = false;
	auto location = this->convertToNodeSpace(t->getLocation());
	for (unsigned int i = 0; i < listSp.size(); i++)
	{
		auto sp = listSp.at(i);
		if (sp->getBoundingBox().containsPoint(location))
		{
			if (i == selectIndex){
				continue;
			}
			hasR = true;
			auto tempVec2 = sp->getPosition();
			sp->setPosition(beganLocatin);
			listSp.at(selectIndex)->setPosition(tempVec2);
			sp->setZOrder(0);
			listSp.at(selectIndex)->setZOrder(0);
			listSp.swap(i, selectIndex);
			break;
		}
	}
	int success = 0;

	for (unsigned int i = 0; i < listSp.size(); i++)
	{
		if (listSp.at(i)->getTag() == i){
			listSp.at(i)->setColor(Color3B(255, 255, 255));
			success++;
		}
		else{
			listSp.at(i)->setColor(Color3B(122, 122, 122));
		}
	}

    
	if (!hasR){
		listSp.at(selectIndex)->setPosition(beganLocatin);
	}
    else{
        checkSptite();
    }

}

void TestGame::initPic()
{
	auto texture = TextureCache::getInstance()->addImage("pingtu.png");
    auto size = texture->getContentSize();

    textureSize =size;
    auto lw = size.width / 3;
    auto lh = size.height / 3;
	listSp.reserve(9);
	int tag = 0;
	
    position.reserve(9);


	for (int w = 0; w < 3; w++){
		for (int h = 0; h <3; h++)
		{
			auto sp = Sprite::createWithTexture(texture, Rect((0 + h)*lw, (0 + w)*lh, lw, lh));
            sp:setAnchorPoint(Vec2(0, 0));
            auto drawNode = DrawNode::create();
            Size imgSize = sp->getContentSize();
            Vec2 vertices[] = {
                    Vec2(0, -lh),
                    Vec2(0, lh),
                    Vec2(lw, lh),
                    Vec2(lw, 0),
                };
            drawNode->setAnchorPoint(Vec2(0, 0));
            drawNode->drawPolygon(vertices,4, Color4F(1, 1, 1, 0), 0, Color4F::WHITE);
//            drawNode->drawPoly(vertices,8, true,Color4F(1, 1, 1, 0));
            auto stencil = ClippingNode::create();
            stencil->setCascadeOpacityEnabled(true);
            stencil->setOpacity(100);
            sp->setAnchorPoint(Vec2(0, 0));
            stencil->setAnchorPoint(Vec2(0, 0));
            stencil->setStencil(drawNode);
            stencil->addChild(sp);
            stencil->setContentSize(Size(lw,lh));
			auto s = Vec2(h*lw, (2-w)* lh);
            position.push_back(s);
            stencil->setTag(tag);
			listSp.pushBack(stencil);
			++tag;
		}
	}
	srand(static_cast<unsigned int>(time(0))); 
	random_shuffle(listSp.begin(), listSp.end());
	for (int i = 0; i < 9; i++)
	{
		auto s = position.at(i);
		listSp.at(i)->setPosition(s);
		this->addChild(listSp.at(i));

	}
    
}
bool TestGame::checkSuccess(int tag)
{
    
    for (unsigned int i = 0; i < _success.size(); i++)
    {
        if (_success.at(i) == tag){
            return true;
        }
    }
    return false;
}
void TestGame::checkSptite()
{
    for (unsigned int i = 0; i < succlistSp.size(); i++)
    {
        succlistSp.at(i)->removeFromParent();
    }
    succlistSp.clear();
    auto texture = TextureCache::getInstance()->getTextureForKey("pingtu.png");
    auto lw = textureSize.width / 3;
    auto lh = textureSize.height / 3;
    
    _success.clear();
    
    for (unsigned int i = 0; i < listSp.size(); i++)
    {
        listSp.at(i)->setVisible(true);
        if (listSp.at(i)->getTag() == i){
            _success.push_back(i);
        }
    }
    
    if (_success.size() > 1){
        int h = 0;
        for (unsigned int j = 0; j < 3; j++){
            std::vector<cocos2d::Vec2> pos;
            Size  nodesize = Size(0,0);
            Vec2 bpos;
            bool ishave = false;
            float  minx = 10000;
            float  maxx = 0;
            float  miny = 10000;
            float  maxy = 0;
            for (unsigned int i = h; i < (_success.size()-1); i++)
            {
                if (_success.at(i)/3 !=_success.at(i+1)/3) {
                    h = i;
                    break;
                }
                int tag1 = _success.at(i);
                int tag2 = _success.at(i+1);
                if (_success.at(i) ==_success.at(i+1)-1&&_success.at(i)/3 ==_success.at(i+1)/3){
                    
                    
                    if (lw*(tag1%3) < minx)
                        minx = lw*(tag1%3);
                    if (lw*(tag2%3+1) > maxx)
                        maxx = lw*(tag2%3+1);
                    if ((tag1/3)* lh < miny)
                        miny =(tag1/3)* lh;
                    if ((tag2/3)* lh > maxy)
                        maxy =(tag2/3)* lh* lh;
                    nodesize.width = maxx-minx;
                    nodesize.height = (2-tag1/3)* lh;
                    bpos =Vec2(minx, (2-tag1/3)* lh);
                    ishave = true;
                    for (unsigned int k = 0; k < listSp.size(); k++)
                    {
                        if (listSp.at(k)->getTag() == _success.at(i)||listSp.at(k)->getTag() == _success.at(i+1)){
                            listSp.at(k)->setVisible(false);
                        }
                    }
                }
            }
            if (!ishave) break;
            Vec2 vertices[4] = {
                    Vec2(0, -nodesize.height),
                    Vec2(0, nodesize.height),
                    Vec2(nodesize.width, nodesize.height),
                    Vec2(nodesize.width, 0),
                };
            
   
            
            auto sp = Sprite::createWithTexture(texture, Rect(minx, miny, nodesize.width, lh));
            sp:setAnchorPoint(Vec2(0, 0));
            auto drawNode = DrawNode::create();
            Size imgSize = sp->getContentSize();
            drawNode->drawPolygon(vertices,4, Color4F(1, 1, 1, 1), 5, Color4F::WHITE);
            
            auto stencil = ClippingNode::create();
            drawNode->setAnchorPoint(Vec2(0, 0));
            sp->setAnchorPoint(Vec2(0, 0));
            stencil->setAnchorPoint(Vec2(0, 0));

            stencil->setContentSize(nodesize);
            stencil->setStencil(drawNode);
            stencil->addChild(sp,999);
            stencil->setPosition(bpos);
            this->addChild(stencil);
            succlistSp.pushBack(stencil);
        }
        
    }

}




