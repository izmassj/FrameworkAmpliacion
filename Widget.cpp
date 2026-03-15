#include "Widget.h"
#include "Scene.h"
#include "GraphicsInterface.h"

Widget::Widget(Scene* myScene):Actor(myScene)
{
}

void Widget::Update(float deltaTime)
{
}

void Widget::Render()
{
	myScene->GI->PrintText("Muevete para ver la animación.", 10, 10, { 255, 255, 255 }, EFONT_SIZE::MEDIUM);
}
