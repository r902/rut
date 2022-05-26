#pragma once

#include <QObject>

#define EXITHELPER_NORMALEXITCODE 200
#define EXITHELPER_UPDATEEXITCODE 201
#define EXITHELPER_TIMETOKILL_US 100000

class QCoreApplication;
/*
 * Вспомогательный класс выхода из программы
 * Вешается на сигналы ОС SIGINT для выхода, SIGUSR1 для обновления, SIGUSR2 для переодического выхода
 * Если есть GUI, то на Alt+X - обработчик выхода
 * При старте программы - прибивает остальные копии себя (опция)
 * В программе необходимо создавать только один экземпляр класса
 */
class ExitHelper : public QObject
{
	Q_OBJECT
public:
	explicit ExitHelper(QObject *parent = nullptr, QCoreApplication* app = nullptr);
	~ExitHelper() override;

	// timeToKill - Время на ожидание прибития
	void killClones(int timeToKill = EXITHELPER_TIMETOKILL_US);
	static void killClones(int argc, char *argv[], int timeToKill = EXITHELPER_TIMETOKILL_US);
	static void killClones(const QStringList& args, int timeToKill = EXITHELPER_TIMETOKILL_US);

private:
	QCoreApplication* app_ = nullptr;
};

extern ExitHelper* exitHelper;

