#include "QtGreedySnake.h"
#include "BeginPage.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGreedySnake w;
	w.show();

	//BeginPage m;
	//m.show();
	return a.exec();
}
