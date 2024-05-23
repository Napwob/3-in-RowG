#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "cocos/ui/UIButton.h"
#include "cocos/ui/UISlider.h"
#include "ui/CocosGUI.h"

#include <cstdlib>
#include <vector>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

ui::Text* endgame_label;
ui::Text* score_label;
ui::Slider* slider;

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto backgroundLayer = LayerColor::create(Color4B(65, 102, 215, 255));
    this->addChild(backgroundLayer);

    srand(static_cast<unsigned>(time(nullptr)));

    HelloWorld::setGridSize(10, 16);
    HelloWorld::setGemsColorNumber(2);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    float visibleWidth = visibleSize.width;
    float visibleHeight = visibleSize.height;

    auto button = cocos2d::ui::Button::create("button.png");

    Size buttonSize = button->getContentSize();
    float buttonWidth = buttonSize.width;
    float buttonHeight = buttonSize.height;

    button->setPosition(Vec2(visibleWidth/2 + buttonWidth + 5, visibleHeight - buttonHeight /1.5));
    button->setTitleText("Начать");
    button->setTitleFontSize(24);
    button->addClickEventListener(CC_CALLBACK_1(HelloWorld::onStartClicked, this));
    this->addChild(button);

    slider = ui::Slider::create();
    slider->setMaxPercent(4);
    slider->loadBarTexture("hardbar.png");
    slider->loadSlidBallTextureNormal("roller.png");
    slider->loadProgressBarTexture("hardbar.png");
    slider->setPosition(Vec2(visibleWidth / 2 - buttonWidth, visibleHeight - buttonHeight / 1.5 - 10));
    slider->addEventListener(CC_CALLBACK_2(HelloWorld::setHardLever, this));
    this->addChild(slider);

    ui::Text* roler_label = ui::Text::create("Сложность", "Arial", 24);
    roler_label->setPosition(Vec2(visibleWidth / 2 - buttonWidth , visibleHeight - buttonHeight / 1.5 + 14));
    this->addChild(roler_label);

    score_label = ui::Text::create("0", "Arial", 24);
    score_label->setPosition(Vec2(visibleWidth / 2 + 5, visibleHeight - buttonHeight / 1.5));
    score_label->addClickEventListener(CC_CALLBACK_0(HelloWorld::updateScore, this));
    this->addChild(score_label);

    endgame_label = ui::Text::create("X", "Arial", 200);
    endgame_label->setTextColor(Color4B(255, 255, 0, 255));

    endgame_label->setPosition(Vec2(visibleWidth / 2, visibleHeight / 2));
    endgame_label->setVisible(false);
    endgame_label->setLocalZOrder(1);
    this->addChild(endgame_label);

    auto sprite = Sprite::create("background.png");
    sprite->setPosition(Vec2(visibleWidth/2, visibleHeight/2));
    this->addChild(sprite, 0);

    return true;
}

void HelloWorld::updateScore() {
    std::string newScore = std::to_string(getScore());
    score_label->setString(newScore); // Обновляем текст объекта ui::Text
}

void HelloWorld::setHardLever(Ref* sender, ui::Slider::EventType type)
{
    if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        ui::Slider *slider = dynamic_cast<cocos2d::ui::Slider*>(sender);
        int hardLevel = slider->getPercent()+2;
        HelloWorld::setGemsColorNumber(hardLevel);
    }
}

void HelloWorld::onStartClicked(Ref* sender)
{
    cocos2d::ui::Button* button = dynamic_cast<cocos2d::ui::Button*>(sender);
    static bool gemInited = false;
    if (gemInited == false)
    {
        HelloWorld::initGems(HelloWorld::getGridSizeX(), HelloWorld::getGridSizeY());
        gemInited = true;
        button->setTitleText("Закончить");
        button->setTitleFontSize(20);
        setScore(0);
        slider->setEnabled(false);
    }
    else
    {
        HelloWorld::deleteGems();
        gemInited = false;
        button->setTitleText("Начать");
        button->setTitleFontSize(24);
        slider->setEnabled(true);
    }
    endgame_label->setVisible(false);
    this->updateScore();
}

void HelloWorld::deleteGems()
{
    for (int i = 0; i < HelloWorld::getGridSizeX(); ++i)
    {
        std::vector<Gem*> gemRow;
        for (int j = 0; j < HelloWorld::getGridSizeY(); ++j)
        {
            if (gemGrid[i][j] != nullptr)
            {
                gemGrid[i][j]->removeFromParent();
                gemGrid[i][j] = nullptr;
            }
        }
        gemRow.clear();
    }
    gemGrid.clear();
}

void HelloWorld::initGems(int array_size_x, int array_size_y)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float visibleWidth = visibleSize.width;
    float visibleHeight = visibleSize.height;

    int gridSizeX = HelloWorld::getGridSizeX();
    int gridSizeY = HelloWorld::getGridSizeY();

    int middleX = visibleWidth / 2;
    int middleY = visibleHeight / 2;

    float gemWidth = 32;
    float gemHeight = 32;

    int gemGridWidth = (int)gemWidth * gridSizeX;
    int gemGridHeight = (int)gemHeight * gridSizeY;

    int offsetX = middleX - gemGridHeight/2;
    int offsetY = middleY + gemGridWidth/2;

    int maxGemsColorsNumber = getGemsColorNumber();
    for (int i = 0; i < gridSizeX; ++i)
    {
        std::vector<Gem*> rowGems;
        for (int j = 0; j < gridSizeY; ++j)
        {
            Gem* gem = nullptr;
            int gem_color = 0;
            if (rand() % 100 < 3)
            {
                if (rand() % 100 < 10)
                    gem_color = 7;
                else
                    gem_color = 6;
            }
            else
                gem_color = rand() % maxGemsColorsNumber;

            if (gem_color == 0)
                gem = Gem::create("red.png");
            if (gem_color == 1)
                gem = Gem::create("green.png");
            if (gem_color == 2)
                gem = Gem::create("blue.png");
            if (gem_color == 3)
                gem = Gem::create("yellow.png");
            if (gem_color == 4)
                gem = Gem::create("brown.png");
            if (gem_color == 5)
                gem = Gem::create("purple.png");
            if (gem_color == 6)
                gem = Gem::create("bomb.png");
            if (gem_color == 7)
                gem = Gem::create("superbomb.png");

            gem->setColor(gem_color);
            gem->setXY(i, j);

            float posX = offsetX + j * gemWidth + 16;
            float posY = offsetY - i * gemHeight - 16;

            gem->setPosition(Vec2(posX, posY));
            this->addChild(gem);

            if (gem_color == 6)
                gem->addClickEventListener(CC_CALLBACK_1(HelloWorld::onBombClicked, this, 1));
            else if (gem_color == 7)
                gem->addClickEventListener(CC_CALLBACK_1(HelloWorld::onBombClicked, this, 2));
            else
                gem->addClickEventListener(CC_CALLBACK_1(HelloWorld::onGemClicked, this));
            rowGems.push_back(gem);
        }
        gemGrid.push_back(rowGems);
    }
}

bool HelloWorld::isInRange(int value, int min_v, int max_v)
{
    if (value >= min_v && value < max_v)
        return true;
    return false;
}

int HelloWorld::neighborsCheck(int x, int y, int color)
{
    int rangeX = HelloWorld::getGridSizeX();
    int rangeY = HelloWorld::getGridSizeY();

    if (!isInRange(x, 0, rangeX) || !isInRange(y, 0, rangeY) || !gemGrid[x][y] || gemGrid[x][y]->getColor() != color)
        return 0;

    int neighbors_count = 0;
    if (isInRange(x - 1, 0, rangeX) && gemGrid[x - 1][y] && gemGrid[x - 1][y]->getColor() == color)
    {
        neighbors_count++;
    }
    if (isInRange(x + 1, 0, rangeX) && gemGrid[x + 1][y] && gemGrid[x + 1][y]->getColor() == color)
    {
        neighbors_count++;
    }
    if (isInRange(y - 1, 0, rangeY) && gemGrid[x][y - 1] && gemGrid[x][y - 1]->getColor() == color)
    {
        neighbors_count++;
    }
    if (isInRange(y + 1, 0, rangeY) && gemGrid[x][y + 1] && gemGrid[x][y + 1]->getColor() == color)
    {
        neighbors_count++;
    }

    return neighbors_count;
}

int HelloWorld::checkSameColorNeighbors(int x, int y)
{
    int rangeX = HelloWorld::getGridSizeX();
    int rangeY = HelloWorld::getGridSizeY();

    if (!isInRange(x, 0, rangeX) || !isInRange(y, 0, rangeY) || !gemGrid[x][y])
        return false;

    int color = gemGrid[x][y]->getColor();

    if (neighborsCheck(x, y, color) >= 2)
        return true;
    else
    {
        int counter = neighborsCheck(x - 1, y, color) +
                      neighborsCheck(x + 1, y, color) +
                      neighborsCheck(x, y - 1, color) +
                      neighborsCheck(x, y + 1, color);

        if (counter >= 2)
           return true;
    }

    return false;
}

void HelloWorld::removeSameColorNeighbors(int x, int y, int color, bool removeNeighbors)
{
    int rangeX = HelloWorld::getGridSizeX();
    int rangeY = HelloWorld::getGridSizeY();

    if (!isInRange(x, 0, rangeX) || !isInRange(y, 0, rangeY) || !gemGrid[x][y] || gemGrid[x][y]->getColor() != color)
        return;

    gemGrid[x][y]->removeFromParent();
    gemGrid[x][y] = nullptr;
    raiseScore(1);

    removeSameColorNeighbors(x - 1, y, color, true);
    removeSameColorNeighbors(x + 1, y, color, true);
    removeSameColorNeighbors(x, y - 1, color, true);
    removeSameColorNeighbors(x, y + 1, color, true);
}

void HelloWorld::dropGemsDown()
{
    for (int col = 0; col < gemGrid[0].size(); col++)
    {
        int emptySpaces = 0;
        for (int row = gemGrid.size() - 1; row >= 0; row--)
        {
            if (gemGrid[row][col] == nullptr)
            {
                emptySpaces++;
            }
            else
            {
                Vec2 spritePosition = gemGrid[row][col]->getPosition();
                Size gemSize = gemGrid[row][col]->getContentSize();
                gemGrid[row][col]->setPosition(Vec2(spritePosition.x, spritePosition.y - gemSize.height * emptySpaces));
            }
        }

        emptySpaces = 0;
        for (int row = gemGrid.size() - 1; row >= 0; row--)
        {
            if (gemGrid[row][col] == nullptr)
            {
                emptySpaces++;
            }
            else
            {
                std::swap(gemGrid[row+emptySpaces][col],gemGrid[row][col]);
                gemGrid[row+emptySpaces][col]->setXY(row+emptySpaces, col);
            }
        }
    }
    if (endGameCheck())
        endgame_label->setVisible(true);
}

bool HelloWorld::endGameCheck()
{
    int gridSizeX = HelloWorld::getGridSizeX();
    int gridSizeY = HelloWorld::getGridSizeY();

    for (int i = 0; i < gridSizeX; ++i)
    {
        for (int j = 0; j < gridSizeY; ++j)
        {
            if (gemGrid[i][j])
                if (checkSameColorNeighbors(i, j) ||
                    gemGrid[i][j]->getColor() == 6 ||
                    gemGrid[i][j]->getColor() == 7)
                    return false;
        }
    }
    return true;
}

void HelloWorld::onGemClicked(Ref* sender)
{
    Gem* gem = dynamic_cast<Gem*>(sender);
    if (gem)
    {
        int buttonX = gem->getX();
        int buttonY = gem->getY();

        int color = gem->getColor();
        if (checkSameColorNeighbors(buttonX, buttonY))
        {
            removeSameColorNeighbors(buttonX, buttonY, color, false);
            dropGemsDown();
        }
        else
        {
            dropScore(1);
        }
    }
    this->updateScore();
}

void HelloWorld::onBombClicked(Ref* sender, int exposionRadius)
{
    Gem* gem = dynamic_cast<Gem*>(sender);
    if (gem)
    {
        int buttonX = gem->getX();
        int buttonY = gem->getY();
        removeAllNeighbors(buttonX, buttonY, exposionRadius);
        dropGemsDown();
    }

    this->updateScore();
}

void HelloWorld::removeAllNeighbors(int x, int y, int exposionRadius)
{
    int rangeX = getGridSizeX();
    int rangeY = getGridSizeY();

    gemGrid[x][y]->removeFromParent();
    gemGrid[x][y] = nullptr;
    raiseScore(1);

    for (int i = x - exposionRadius; i < x + exposionRadius + 1; i++)
    {
        for (int j = y - exposionRadius; j < y + exposionRadius + 1; j++)
        {
            if (!isInRange(i, 0, rangeX) || !isInRange(j, 0, rangeY))
                continue;

            if (gemGrid[i][j])
            {
                if (gemGrid[i][j]->getColor() == 6)
                {
                    removeAllNeighbors(i, j, 1);
                    continue;
                }

                if (gemGrid[i][j]->getColor() == 7)
                {
                    removeAllNeighbors(i, j, 2);
                    continue;
                }

                gemGrid[i][j]->removeFromParent();
                gemGrid[i][j] = nullptr;
                raiseScore(1);
            }
        }
    }
}