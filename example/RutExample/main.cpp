#include "mainwindow.h"
#include "ExitHelper.h"
#include "Sets.h"
#include "Log.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LOG("app start");
	RutSets::init("rut.example.com", a.applicationName(), &a, argc, argv);
	MainWindow w;
	ExitHelper EH(&w, &a);
	EH.killClones();
	w.show();
	return a.exec();
}
