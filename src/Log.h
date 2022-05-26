#pragma once

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QStringBuilder>

// Простой логгер
// Возможна работа с цветом: https://habr.com/ru/post/119436/  https://misc.flogisoft.com/bash/tip_colors_and_formatting
// В конце вывода убирается цвет на "по умолчанию"

Q_GLOBAL_STATIC_WITH_ARGS(const QString, cLogDateTimeFormat, (QLatin1String("hh:mm:ss.zzz")));
Q_GLOBAL_STATIC_WITH_ARGS(const QString, cLogModuleOn, (QLatin1String("\033[036m[")));
Q_GLOBAL_STATIC_WITH_ARGS(const QString, cLogModuleOff, (QLatin1String("]\033[0m")));
Q_GLOBAL_STATIC_WITH_ARGS(const QString, cLogColorOff, (QLatin1String("\033[0m")));
#define LOG_DEST qDebug().noquote()
#define LOG_DT_STR QTime::currentTime().toString(*cLogDateTimeFormat)

// Обман компилятора для доступа к QDebug::Stream
// Исходя из того, что поле-указатель на структуру Stream в начале класса QDebug
struct Log_QDebug_Stream { QTextStream ts; QString buffer; };

// Вывод в stderr одной переменной
template <typename T> void LOG_ONE(T t, QDebug& D) { D << t; }

// Вывод в лог перечня переменных
// Возврат - результирующая строка
template <typename... Args> QString LOG_BASE(Args&&... args) {
	QDebug D = LOG_DEST;
	D << LOG_DT_STR;
	((void) LOG_ONE(std::forward<Args>(args), D), ...); // https://stackoverflow.com/questions/25680461/variadic-template-pack-expansion
	D << *cLogColorOff; // Убрать цвет
	Log_QDebug_Stream** pp = reinterpret_cast<Log_QDebug_Stream**>(&D); // make gcc happy
	return (*pp)->buffer;
}

// Заглушка имени модуля. Класс, вызывающий LOG, "переопределяет" ее, напремер как metaObject()->className()
QString logModuleName();

// Основной макрос вывода в лог
#define LOG(mess, args...) LOG_BASE(logModuleName().isEmpty() ? QString() : *cLogModuleOn % logModuleName() % *cLogModuleOff, mess, ## args)
// Вывод в лог с явным указанным имени модуля
#define LOGM(module, mess, args...) LOG_BASE(*cLogModuleOn % QString(module) % *cLogModuleOff, mess, ## args)
// Вывод с префиксом сигнатуры функции
#define LOG_PF(mess, args...) LOG(__PRETTY_FUNCTION__, mess, ## args)
// Вывод только префикса сигнатуры функции
#define LOGPF LOG(__PRETTY_FUNCTION__)
// Вывод с префиксом сигнатуры функции и номера строки
#define LOGPFL LOG(__PRETTY_FUNCTION__, QString::number(__LINE__))
