
#ifndef BEGINPAGE_H
#define BEGINPAGE_H

#include <QWidget>
#include <QAbstractButton> 
#include <QPushButton>
#include <QPainter>

class BeginPage : public QWidget
{
	Q_OBJECT

public:
	BeginPage(QWidget *parent = Q_NULLPTR);
	void setUi();
	~BeginPage();

private:
	QAbstractButton* _BeginButton;
	QAbstractButton* _OptionButton;
	QAbstractButton* _QuitButton;

	QPainter*  _Painter;

private:
	void paintEvent(QPaintEvent*);
};

#endif
