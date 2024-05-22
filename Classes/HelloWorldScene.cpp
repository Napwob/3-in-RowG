#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "cocos/ui/UIButton.h"
#include "ui/CocosGUI.h"
#include <cstdlib>
#include <vector>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto backgroundLayer = LayerColor::create(Color4B(65, 102, 215, 255)); // (R, G, B, A) - компоненты цвета и прозрачность
    this->addChild(backgroundLayer);

    srand(static_cast<unsigned>(time(nullptr)));

    int array_size_x = 10;
    int array_size_y = 16;

    HelloWorld::setGridSize(array_size_x, array_size_y);

    auto button = cocos2d::ui::Button::create("button.png");

    Size visibleSize = Director::getInstance()->getVisibleSize();
    float visibleWidth = visibleSize.width;
    float visibleHeight = visibleSize.height;

    Size buttonSize = button->getContentSize();
    float buttonWidth = buttonSize.width;
    float buttonHeight = buttonSize.height;

    button->setPosition(Vec2(visibleWidth/2, visibleHeight - buttonHeight /1.5));

    button->setTitleText("Начать");
    button->setTitleFontSize(24);
    button->addClickEventListener(CC_CALLBACK_1(HelloWorld::onStartClicked, this));
    this->addChild(button);

    return true;
}

void HelloWorld::onStartClicked(Ref* sender)
{
    log("%s %d", __FUNCTION__, __LINE__);
    cocos2d::ui::Button* button = dynamic_cast<cocos2d::ui::Button*>(sender);
    static bool gemInited = false;
    if (gemInited == false)
    {
        HelloWorld::initGems(HelloWorld::getGridSizeX(), HelloWorld::getGridSizeY());
        gemInited = true;
        button->setTitleText("Закончить");
        button->setTitleFontSize(20);
    }
    else
    {
        HelloWorld::deleteGems();
        gemInited = false;
        button->setTitleText("Начать");
        button->setTitleFontSize(24);
    }
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
    for (int i = 0; i < gridSizeX; ++i)
    {
        std::vector<Gem*> rowGems;
        for (int j = 0; j < gridSizeY; ++j)
        {
            Gem* gem = nullptr;
            int gem_color = rand() % 3;

            if (gem_color == 0)
                gem = Gem::create("red.png");
            if (gem_color == 1)
                gem = Gem::create("green.png");
            if (gem_color == 2)
                gem = Gem::create("blue.png");

            gem->setColor(gem_color);
            gem->setXY(i, j);

            float posX = offsetX + j * gemWidth;
            float posY = offsetY - i * gemHeight;

            gem->setPosition(Vec2(posX, posY));
            this->addChild(gem);

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
    log("%s %d", __FUNCTION__, __LINE__);

    int rangeX = HelloWorld::getGridSizeX();
    int rangeY = HelloWorld::getGridSizeY();

    if (!isInRange(x, 0, rangeX) || !isInRange(y, 0, rangeY) || !gemGrid[x][y] || gemGrid[x][y]->getColor() != color)
        return 0;
    log("color: %d", color);

    int neighbors_count = 0;
    if (isInRange(x - 1, 0, rangeX) && gemGrid[x - 1][y] && gemGrid[x - 1][y]->getColor() == color)
    {
        log("up");
        neighbors_count++;
    }
    if (isInRange(x + 1, 0, rangeX) && gemGrid[x + 1][y] && gemGrid[x + 1][y]->getColor() == color)
    {
        log("down");
        neighbors_count++;
    }
    if (isInRange(y - 1, 0, rangeY) && gemGrid[x][y - 1] && gemGrid[x][y - 1]->getColor() == color)
    {
        log("left");
        neighbors_count++;
    }
    if (isInRange(y + 1, 0, rangeY) && gemGrid[x][y + 1] && gemGrid[x][y + 1]->getColor() == color)
    {
        log("right");
        neighbors_count++;
    }

    return neighbors_count;
}

int HelloWorld::checkSameColorNeighbors(int x, int y)
{
    log("%s %d", __FUNCTION__, __LINE__);

    int rangeX = HelloWorld::getGridSizeX();
    int rangeY = HelloWorld::getGridSizeY();

    if (!isInRange(x, 0, rangeX) || !isInRange(y, 0, rangeY) || !gemGrid[x][y])
    {
        log("main check");
        return false;
    }
    log("%d",__LINE__);
    int color = gemGrid[x][y]->getColor();
    log("%d",__LINE__);
    if (neighborsCheck(x, y, color) >= 2)
    {
        log("first");
        return true;
    }
    else
    {
        int counter = neighborsCheck(x - 1, y, color) +
                      neighborsCheck(x + 1, y, color) +
                      neighborsCheck(x, y - 1, color) +
                      neighborsCheck(x, y + 1, color);
        log("second %d", counter);
        if (counter >= 2)
           return true;
    }

    log("nothing");
    return false;
}

void HelloWorld::removeSameColorNeighbors(int x, int y, int color, bool removeNeighbors)
{
    log("%s %d", __FUNCTION__, __LINE__);

    int rangeX = HelloWorld::getGridSizeX();
    int rangeY = HelloWorld::getGridSizeY();

    if (!isInRange(x, 0, rangeX) || !isInRange(y, 0, rangeY) || !gemGrid[x][y] || gemGrid[x][y]->getColor() != color)
        return;

    gemGrid[x][y]->removeFromParent();
    gemGrid[x][y] = nullptr;

    removeSameColorNeighbors(x - 1, y, color, true);
    removeSameColorNeighbors(x + 1, y, color, true);
    removeSameColorNeighbors(x, y - 1, color, true);
    removeSameColorNeighbors(x, y + 1, color, true);
}

void HelloWorld::dropGemsDown()
{
    log("%s %d", __FUNCTION__, __LINE__);
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
}

void HelloWorld::printLogGrid()
{
    log("%s %d", __FUNCTION__, __LINE__);

    int rangeX = HelloWorld::getGridSizeX();

    for (int i=0;i<rangeX;i++)
    {
        if(gemGrid[i][0] != nullptr)
            log("color %d", gemGrid[i][0]->getColor());
        else
            log("color");
    }
}

void HelloWorld::onGemClicked(Ref* sender)
{
    log("%s %d", __FUNCTION__, __LINE__);
    Gem* gem = dynamic_cast<Gem*>(sender);
    if (gem)
    {
        int buttonX = gem->getX();
        int buttonY = gem->getY();
        log("button %d %d", buttonX, buttonY);
        int color = gem->getColor();
        if (checkSameColorNeighbors(buttonX, buttonY))
        {
            removeSameColorNeighbors(buttonX, buttonY, color, false);
            dropGemsDown();
        }

        log("Pressed button at position (%d, %d)", buttonX, buttonY);
    }
    printLogGrid();
}
