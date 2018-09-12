#include "QtGreedySnake.h"
#define Map_Width 120;
#define Map_Height 100;
#define Rect_Length 20;
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

QtGreedySnake::QtGreedySnake(QWidget *parent)
	: QMainWindow(parent)
{
	this->setWindowTitle(tr("GreedySnake"));//设置窗口名称
	this->setGeometry(600, 200, 35 * 20 + 300, 30 * 20);

	//初始化贪吃蛇蛇头坐标容器，用容器InitSnakeCor的第一个值表示x坐标，第二个值表示y坐标
	QVector<int> InitSnakeCor;
	int initSnakeX = 15;
	int initSnakeY = 10;
	InitSnakeCor.push_back(initSnakeX);
	InitSnakeCor.push_back(initSnakeY);

	//初始化贪吃蛇链表
	_Snake = new QList<QVector<int>>;
	_Snake->push_back(InitSnakeCor);
	for (int i = InitSnakeCor.at(0) + 1; i <= InitSnakeCor.at(0) + 5; i++)
	{
		QVector<int> snakeBodyCor;
		snakeBodyCor.push_back(i);
		snakeBodyCor.push_back(InitSnakeCor.at(1));
		_Snake->push_back(snakeBodyCor);
	}

	//初始化食物容器指针
	_Food = new QVector<int>;
	int food_x = qrand() % 35;
	int food_y = qrand() % 35;
	_Food->push_back(food_x);
	_Food->push_back(food_y);

	//蛇头运动方向初始化
	_Direct = LEFT;

	//计时器初始化
	_Timer = new QTimer(this);
	connect(_Timer, SIGNAL(timeout()), this, SLOT(MyUpdate()));
	_Timer->start(400); 

	//QPiainter初始化
	_MapPainter = new QPainter;

	//当前时间指针初始化
	_CurTime = new QTime;

	//用于决定是否更新图形界面的开关变量
	_IfUpdate = true;

	//初始化声音播放指针并播放声音
	_BackMusic = new QMediaPlayer;
	playsound();

	//初始化得分
	_Score = 0;
}

//计时器更新函数
void QtGreedySnake::MyUpdate()//自定义更新槽函数，由定时器信号触发
{
	if (_IfUpdate)
	{
		update();
		snakeAutoMove();
		collDetection();
		eatFood();
	}

}

void QtGreedySnake::paintEvent(QPaintEvent*)//绘图函数，负责地图绘制，贪吃蛇绘制，食物绘制
{

	_MapPainter->begin(this);//绘制地图
	_MapPainter->setPen(QPen(Qt::black, 4, Qt::SolidLine));

	for (int i = 0; i <= 35; i++)
	{
		for (int j = 0; j <= 30; j++)
		{
			_MapPainter->drawRect(QRect(i * 20, j * 20, 20, 20));
			_MapPainter->fillRect(QRect(i * 20, j * 20, 20, 20), Qt::darkCyan);
		}
	}

	//绘制蛇的图形
	_MapPainter->drawRect(QRect(_Snake->at(0).at(0) * 20, _Snake->at(0).at(1) * 20, 20, 20));
	_MapPainter->fillRect(QRect(_Snake->at(0).at(0) * 20, _Snake->at(0).at(1) * 20, 20, 20), Qt::darkRed);//蛇头正方形绘制
		for (int i = 1; i < _Snake->size(); i++)
		{
			_MapPainter->drawRect(QRect(_Snake->at(i).at(0) * 20, _Snake->at(i).at(1) * 20, 20, 20));
			_MapPainter->fillRect(QRect(_Snake->at(i).at(0) * 20, _Snake->at(i).at(1) * 20, 20, 20), Qt::darkYellow);//蛇身多个正方形绘制
		}
    //绘制食物
	creatFood(_MapPainter);

	//绘制得分
	_MapPainter->drawText(QRect(800,0,40,40), QString("%1%2").arg("score:").arg(_Score));
	_MapPainter->end();
}

void QtGreedySnake::keyPressEvent(QKeyEvent* key)//由键盘改变贪吃蛇运动的方向
{
	switch (key->key())
	{
	case Qt::Key_Up: 
		if (_Direct != DOWN)
			_Direct = UP;
		break;
	case Qt::Key_Down: 
		if (_Direct != UP)
			_Direct = DOWN; 
		break;
	case Qt::Key_Right: 
		if (_Direct != LEFT)
			_Direct = RIGHT; 
		break;
	case Qt::Key_Left: 
		if (_Direct != RIGHT)
			_Direct = LEFT; 
		break;

	default:
		break;
	}
}

void QtGreedySnake::snakeAutoMove()//蛇随定时器触发间隔自动沿着控制方向移动
{

	for (int i = _Snake->size() - 1; i > 0; i--)//移动时把蛇的前一个节点坐标赋给后一个节点坐标，来实现移动
	{
		((*_Snake)[i])[0] = ((*_Snake)[i - 1])[0];
		((*_Snake)[i])[1] = ((*_Snake)[i - 1])[1];
	}

	switch (_Direct)
	{
	case UP: ((*_Snake)[0])[1] -= 1; break;//蛇头节点随方向改变坐标值
	case DOWN: ((*_Snake)[0])[1] += 1; break;
	case RIGHT: ((*_Snake)[0])[0] += 1; break;
	case LEFT: ((*_Snake)[0])[0] -= 1; break;

	default:
		break;
	}

}

void QtGreedySnake::collDetection()//碰撞检测函数，将判断结果保存到_IfUpdate中
{
	int _x = _Snake->at(0).at(0);
	int _y = _Snake->at(0).at(1);
	if (_x < 0 || _x > 35 || _y < 0 || _y > 30)
	{
		_IfUpdate = false;
	}
	for (int i = 1; i < _Snake->size(); i++)
	{
		if (_x == _Snake->at(i).at(0) && _y == _Snake->at(i).at(1))
		{
			_IfUpdate = false;
		}
	}
}

void QtGreedySnake::creatFood(QPainter*)//用随机数发衡器创造食物坐标，供绘图调用
{

	if (_Food->at(0) == _Snake->at(0).at(0) && _Food->at(1) == _Snake->at(0).at(1))
	{
		_Food->clear();
		*_CurTime = QTime::currentTime();
		qsrand(_CurTime->second());

		_Food->push_back(qrand() % 35);
		_Food->push_back(qrand() % 30);
		_Score = _Score + 1;
	}
	_MapPainter->drawRect(QRect(_Food->at(0) * 20, _Food->at(1) * 20, 20, 20));
	_MapPainter->fillRect(QRect(_Food->at(0) * 20, _Food->at(1) * 20, 20, 20), Qt::blue);
}

void QtGreedySnake::eatFood()
{
	if (_Food->at(0) == _Snake->at(0).at(0) && _Food->at(1) == _Snake->at(0).at(1))
	{
		QVector<int> newNode;
		newNode.append(_Food->at(0));
		newNode.append(_Food->at(1));
		_Snake->append(newNode);
	}
}

void QtGreedySnake::playsound()
{
	_BackMusic->setMedia(QUrl::fromLocalFile("Summer.mp3"));
	_BackMusic->setVolume(10);
	_BackMusic->play();

}