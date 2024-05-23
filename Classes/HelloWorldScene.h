/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/UIButton.h"

class Gem : public cocos2d::ui::Button
{
public:
    static Gem* create(const std::string& normalImage)
    {
        Gem* button = new (std::nothrow) Gem;
        if (button && button->init(normalImage, "", ""))
        {
            button->autorelease();
            return button;
        }
        CC_SAFE_DELETE(button);
        return nullptr;
    }

    void setColor(int color)
    {
        color_ = color;
    }

    int getColor()
    {
        return color_;
    }

    void setXY(int x, int y)
    {
        x_ = x;
        y_ = y;
    }

    void setX(int x)
    {
        x_ = x;
    }

    int getX()
    {
        return x_;
    }

    void setY(int y)
    {
        y_ = y;
    }

    int getY()
    {
        return y_;
    }

private:
    int x_;
    int y_;
    int color_;
};

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

private:
    std::vector<std::vector<Gem*>> gemGrid;

    int grid_size_x;
    int grid_size_y;

    void setGridSize(int x, int y)
    {
        grid_size_x = x;
        grid_size_y = y;
    }
    int getGridSizeX()
    {
        return grid_size_x;
    }
    int getGridSizeY()
    {
        return grid_size_y;
    }

    int color_number;
    void setGemsColorNumber(int number)
    {
        color_number = number;
    }
    int getGemsColorNumber()
    {
        return color_number;
    }
    int score;
    void setScore(int score_number)
    {
        score = score_number;
    }
    void raiseScore(int score_number)
    {
        score+=score_number*getGemsColorNumber()-1;
    }
    void dropScore(int score_number)
    {
        score -= score_number * getGemsColorNumber() - 1;
        if (score < 0)
            score = 0;
    }
    int getScore()
    {
        return score;
    }
    void updateScore();

    void onStartClicked(Ref* sender);
    void setHardLever(Ref* sender, cocos2d::ui::Slider::EventType type);

    bool isInRange(int value, int min_v, int max_v);

    void initGems(int array_size_x, int array_size_y);
    void deleteGems();

    void onBombClicked(Ref* sender, int exposionRadius);
    void removeAllNeighbors(int x, int y, int exposionRadius);

    int neighborsCheck(int x, int y, int color);
    int checkSameColorNeighbors(int x, int y);

    void onGemClicked(Ref* sender);
    void removeSameColorNeighbors(int x, int y, int color, bool removeNeighbors);

    void dropGemsDown();

    bool endGameCheck();

    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
