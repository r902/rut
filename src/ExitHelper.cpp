#include <signal.h>
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <unistd.h>
#include "ExitHelper.h"
#ifdef QT_GUI_LIB
#include <QAction>
#endif

#ifdef QT_WIDGETS_LIB
#include <QMainWindow>
#endif

// Обработчик сигнала ОС
void signalhandler(int sig) {
	printf("SIG = %i\n", sig);
	int ExitHelperCode = 0;
#ifdef Q_OS_LINUX
	if (sig == SIGINT || sig == SIGTERM) ExitHelperCode = EXITHELPER_NORMALEXITCODE;
	else if (sig == SIGUSR1) ExitHelperCode = EXITHELPER_UPDATEEXITCODE;
#endif
	qApp->exit(ExitHelperCode);
}

ExitHelper::ExitHelper(QObject *parent, QCoreApplication *app) : QObject(parent), app_(app)
{
#ifdef Q_OS_LINUX
	signal(SIGINT,  signalhandler); // Выход по требованию скрипта (ОС) Ctrl+c
	signal(SIGTERM, signalhandler); // Выход по требованию ОС (крест или завершение программфы при logout)
	signal(SIGUSR1, signalhandler); // Выход по требованию скрипта для обновления
#endif

#ifdef QT_WIDGETS_LIB
	auto *mainWindow = qobject_cast<QMainWindow *>(parent);
	if (mainWindow) {
		auto* aExit = new QAction(this); aExit->setShortcut(QKeySequence(QStringLiteral("Alt+X")));
		connect(aExit, &QAction::triggered, this,  [mainWindow] { mainWindow->close(); }); mainWindow->addAction(aExit);
	}
#endif

}

ExitHelper::~ExitHelper()
{
}

void ExitHelper::killClones(int timeToKill)
{
	if (app_) killClones(app_->arguments(), timeToKill);
}

void ExitHelper::killClones(int argc, char *argv[], int timeToKill)
{
	QCoreApplication a(argc, argv);
	killClones(a.arguments(), timeToKill);
}

void ExitHelper::killClones(const QStringList &args, int timeToKill)
{
	std::string prgName = args[0].toStdString();
	std::string cmd = "kill $(pgrep -f " + prgName + " | grep -v " + std::to_string(getpid()) + ")";
	(void)!std::system(cmd.c_str());
	usleep(timeToKill);
}

