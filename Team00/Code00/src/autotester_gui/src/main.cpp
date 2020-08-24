#include <QtWidgets>

//#include "codeeditor.h"
#include "mainwindow.h"
int main(int argv, char **args)
{
    QApplication app(argv, args);

	//QApplication a(argc, argv);
	MainWindow w;
	w.show();
    return app.exec();
}
