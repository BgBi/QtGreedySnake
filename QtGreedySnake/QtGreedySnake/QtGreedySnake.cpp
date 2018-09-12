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
	this->setWindowTitle(tr("GreedySnake"));//���ô�������
	this->setGeometry(600, 200, 35 * 20 + 300, 30 * 20);

	//��ʼ��̰������ͷ����������������InitSnakeCor�ĵ�һ��ֵ��ʾx���꣬�ڶ���ֵ��ʾy����
	QVector<int> InitSnakeCor;
	int initSnakeX = 15;
	int initSnakeY = 10;
	InitSnakeCor.push_back(initSnakeX);
	InitSnakeCor.push_back(initSnakeY);

	//��ʼ��̰��������
	_Snake = new QList<QVector<int>>;
	_Snake->push_back(InitSnakeCor);
	for (int i = InitSnakeCor.at(0) + 1; i <= InitSnakeCor.at(0) + 5; i++)
	{
		QVector<int> snakeBodyCor;
		snakeBodyCor.push_back(i);
		snakeBodyCor.push_back(InitSnakeCor.at(1));
		_Snake->push_back(snakeBodyCor);
	}

	//��ʼ��ʳ������ָ��
	_Food = new QVector<int>;
	int food_x = qrand() % 35;
	int food_y = qrand() % 35;
	_Food->push_back(food_x);
	_Food->push_back(food_y);

	//��ͷ�˶������ʼ��
	_Direct = LEFT;

	//��ʱ����ʼ��
	_Timer = new QTimer(this);
	connect(_Timer, SIGNAL(timeout()), this, SLOT(MyUpdate()));
	_Timer->start(400); 

	//QPiainter��ʼ��
	_MapPainter = new QPainter;

	//��ǰʱ��ָ���ʼ��
	_CurTime = new QTime;

	//���ھ����Ƿ����ͼ�ν���Ŀ��ر���
	_IfUpdate = true;

	//��ʼ����������ָ�벢��������
	_BackMusic = new QMediaPlayer;
	playsound();

	//��ʼ���÷�
	_Score = 0;
}

//��ʱ�����º���
void QtGreedySnake::MyUpdate()//�Զ�����²ۺ������ɶ�ʱ���źŴ���
{
	if (_IfUpdate)
	{
		update();
		snakeAutoMove();
		collDetection();
		eatFood();
	}

}

void QtGreedySnake::paintEvent(QPaintEvent*)//��ͼ�����������ͼ���ƣ�̰���߻��ƣ�ʳ�����
{

	_MapPainter->begin(this);//���Ƶ�ͼ
	_MapPainter->setPen(QPen(Qt::black, 4, Qt::SolidLine));

	for (int i = 0; i <= 35; i++)
	{
		for (int j = 0; j <= 30; j++)
		{
			_MapPainter->drawRect(QRect(i * 20, j * 20, 20, 20));
			_MapPainter->fillRect(QRect(i * 20, j * 20, 20, 20), Qt::darkCyan);
		}
	}

	//�����ߵ�ͼ��
	_MapPainter->drawRect(QRect(_Snake->at(0).at(0) * 20, _Snake->at(0).at(1) * 20, 20, 20));
	_MapPainter->fillRect(QRect(_Snake->at(0).at(0) * 20, _Snake->at(0).at(1) * 20, 20, 20), Qt::darkRed);//��ͷ�����λ���
		for (int i = 1; i < _Snake->size(); i++)
		{
			_MapPainter->drawRect(QRect(_Snake->at(i).at(0) * 20, _Snake->at(i).at(1) * 20, 20, 20));
			_MapPainter->fillRect(QRect(_Snake->at(i).at(0) * 20, _Snake->at(i).at(1) * 20, 20, 20), Qt::darkYellow);//�����������λ���
		}
    //����ʳ��
	creatFood(_MapPainter);

	//���Ƶ÷�
	_MapPainter->drawText(QRect(800,0,40,40), QString("%1%2").arg("score:").arg(_Score));
	_MapPainter->end();
}

void QtGreedySnake::keyPressEvent(QKeyEvent* key)//�ɼ��̸ı�̰�����˶��ķ���
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

void QtGreedySnake::snakeAutoMove()//���涨ʱ����������Զ����ſ��Ʒ����ƶ�
{

	for (int i = _Snake->size() - 1; i > 0; i--)//�ƶ�ʱ���ߵ�ǰһ���ڵ����긳����һ���ڵ����꣬��ʵ���ƶ�
	{
		((*_Snake)[i])[0] = ((*_Snake)[i - 1])[0];
		((*_Snake)[i])[1] = ((*_Snake)[i - 1])[1];
	}

	switch (_Direct)
	{
	case UP: ((*_Snake)[0])[1] -= 1; break;//��ͷ�ڵ��淽��ı�����ֵ
	case DOWN: ((*_Snake)[0])[1] += 1; break;
	case RIGHT: ((*_Snake)[0])[0] += 1; break;
	case LEFT: ((*_Snake)[0])[0] -= 1; break;

	default:
		break;
	}

}

void QtGreedySnake::collDetection()//��ײ��⺯�������жϽ�����浽_IfUpdate��
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

void QtGreedySnake::creatFood(QPainter*)//�����������������ʳ�����꣬����ͼ����
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