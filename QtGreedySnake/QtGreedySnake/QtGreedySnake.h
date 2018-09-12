#ifndef QTGREEDYSNAKE_H
#define QTGREEDYSNAKE_H

#include <QtWidgets/QMainWindow>
#include <QPainter>
#include <QList>
#include <QVector>
#include <QKeyEvent> 
#include <QTimer>
#include <QTime>

#include <QMediaPlayer>
#include <QtGlobal>


class QtGreedySnake : public QMainWindow
{
	Q_OBJECT

public:
	QtGreedySnake(QWidget *parent = Q_NULLPTR);
	void snakeAutoMove();
	void collDetection();
	void creatFood(QPainter*);
	void eatFood();
	void playsound();

private slots:
	void MyUpdate();

private:

	QPainter*  _MapPainter;
	QList<QVector<int>>* _Snake;
	QVector<int>* _Food;
	QTimer* _Timer;
	QTime* _CurTime;
	QMediaPlayer* _BackMusic;

	int _Direct;
	bool _IfUpdate;
	int _Score;
	
protected:
	void paintEvent(QPaintEvent*);
	void keyPressEvent(QKeyEvent* key);
};

#endif // !QTGREEDYSNAKE_H
