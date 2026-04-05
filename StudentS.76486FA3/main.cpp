#include "LoginWidget.h"
#include<QApplication>


int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/resourses/NJUST.png"));
	LoginWidget w;
	w.show();
	return app.exec();
}
