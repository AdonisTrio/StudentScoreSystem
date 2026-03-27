#include "MainWindow.h"
#include "InitialWidget.h"
#include<QApplication>


int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/resourses/NJUST.png"));
	InitialWidget w;
	w.show();
	return app.exec();
}
