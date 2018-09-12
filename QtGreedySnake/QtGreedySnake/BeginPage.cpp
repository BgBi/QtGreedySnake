#include "BeginPage.h"

BeginPage::BeginPage(QWidget *parent)
	: QWidget(parent)
{
	this->setWindowTitle(tr("GreedySnake"));//设置窗口名称
	this->setGeometry(300, 200, 35 * 20 + 300, 30 * 20);

	_BeginButton = new QPushButton("开始");
	_OptionButton = new QPushButton("选项");
	_QuitButton = new QPushButton("退出");

	_Painter = new QPainter(this);

	//setUi();
}

BeginPage::~BeginPage()
{
}

void BeginPage::setUi()
{
	_BeginButton;
	_OptionButton;
	_QuitButton;

}

void BeginPage::paintEvent(QPaintEvent*)
{
	_Painter->drawPixmap(0, 0, this->width(), this->height(), QPixmap("GreedySnake.png"));
}