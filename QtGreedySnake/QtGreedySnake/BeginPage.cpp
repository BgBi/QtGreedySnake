#include "BeginPage.h"

BeginPage::BeginPage(QWidget *parent)
	: QWidget(parent)
{
	this->setWindowTitle(tr("GreedySnake"));//���ô�������
	this->setGeometry(300, 200, 35 * 20 + 300, 30 * 20);

	_BeginButton = new QPushButton("��ʼ");
	_OptionButton = new QPushButton("ѡ��");
	_QuitButton = new QPushButton("�˳�");

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