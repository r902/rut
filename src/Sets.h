#pragma once

#include <QSettings>

class QWidget;
class QSettings;

// Глобальный класс настроек программы
// TODO Нуждается в доработке
class RutSets: public QObject
{
	Q_OBJECT
public:

	static RutSets *instance() { return self; }

	RutSets(const QString& comp, const QString& app, QObject* parent);
	~RutSets() override;

	// Инициализация, загрузка настроек (вызвать в начале программы)
	// Указать parent = &QCoreApplication
	static void init(const QString& comp, const QString& app, QObject* parent, int argc, char *argv[]);

	// Сохраняет значение виджета в зависимости от его типа
	// У виджета должен быть parent - из него берется имя группы настроек
	// Если задан userPrefix, то он добавляется в имя группы настроек
	void saveWidget(const QWidget *w, const QString& userPrefix = QString());

	// Сохранение виджета w, дерево предков ограничено maxParent, возвращает префикс
	QString saveWidget(const QWidget *w, const QWidget *maxParent);
	void saveGroupWidgets(const QWidget *w, const QWidget *maxParent);

	// Сохранение данных виджета w, дерево предков ограничено maxParent
	void saveWidgetData(const QWidget *w, const QWidget *maxParent, const QVariant& data);

	// Загрузка и установка значения виджета
	void loadWidget(QWidget *w, const QString& userPrefix = QString());

	// Загрузка виджета w, дерево предков ограничено maxParent, возвращает префикс
	QString loadWidget(QWidget *w, const QWidget *maxParent);
	void loadGroupWidgets(QWidget *w, const QWidget *maxParent);

	// Загрузка данных виджета w, дерево предков ограничено maxParent
	QVariant loadWidgetData(QWidget *w, const QWidget *maxParent);

	// Сохранить значение value параметра key
	// Возврат - код ошибки:
	// 0 = нет ошибок
	// 1 = значение параметра вне допустимых пределов
	int set(const QString& key, const QVariant& value);

	// Получить значение для параметра key
	// Если defaultValue задано, то при остутсвии зачения по ключу, будет установлено это значение defaultValue и оно же и возвращено
	QVariant get(const QString& key, const QVariant &defaultValue = {});

	// Содержит ли командная строка указанный параметр
	bool argsContains(const QString& param);

	// Значение параметра: значение передается после имени параметра, пример: valName 123
	QString argsValue(const QString& param);

	// Сброс всех настроек (удаление)
	void clear();

signals:
	// Событие изменения настройки
	void changed(const QString& key, const QVariant& value);

private:
	QSettings *gRutSets = nullptr;
	static RutSets *self;

	void ssaveWidget(const QString& prefix, const QWidget *w);
	void lloadWidget(const QString& prefix, QWidget *w);
	QString makePrefix(const QWidget* w, const QWidget *maxParent);
	void sync();
};

#define theRutSets RutSets::instance()
#define RUT_SETS_MAIN_WINDOWS_GEO "App/Geometry"
#define RUT_SETS_MAIN_WINDOWS_STA "App/State"
#define RUT_SETS_QTABLEWIDGET_DATA_PROPERTY "RUT_SETS_QTABLEWIDGET_DATA_PROPERTY" // Сохраняемое свойство для QTableWidget, может содержать данные
#define RUT_SETS_SAVE_GROUP_WIDGETS(w) theRutSets->saveGroupWidgets(w, this);
#define RUT_SETS_LOAD_GROUP_WIDGETS(w) theRutSets->loadGroupWidgets(w, this);
#define RUT_SETS_SAVE_WIDGET(w) theRutSets->saveWidget(w, this);
#define RUT_SETS_LOAD_WIDGET(w) theRutSets->loadWidget(w, this);
#define RUT_SETS_SAVE_WIDGETP(w, p) theRutSets->saveWidget(w, p)
#define RUT_SETS_LOAD_WIDGETP(w, p) theRutSets->loadWidget(w, p)
#define RUT_SETS_GET(k) theRutSets->get(k)
#define RUT_SETS_SET(k, v) theRutSets->set(k, v)
#define RUT_SETS_APP_ARG(k) theRutSets->argsValue(k)
#define RUT_SETS_APP_CONTAINS_ARG(k) theRutSets->argsContains(k)
#define RUT_SETS_SAVE_WIDGET_DATA(w, max_parent, data) theRutSets->saveWidgetData(w, max_parent, data)
#define RUT_SETS_LOAD_WIDGET_DATA(w, max_parent) theRutSets->loadWidgetData(w, max_parent)
#define RUT_SETS_SAVE_WINDOWS_GEO RUT_SETS_SET(RUT_SETS_MAIN_WINDOWS_GEO, this->saveGeometry())
#define RUT_SETS_SAVE_WINDOWS_STA RUT_SETS_SET(RUT_SETS_MAIN_WINDOWS_STA, this->saveState())
#define RUT_SETS_GET_WINDOWS_GEO RUT_SETS_GET(RUT_SETS_MAIN_WINDOWS_GEO).toByteArray()
#define RUT_SETS_GET_WINDOWS_STA RUT_SETS_GET(RUT_SETS_MAIN_WINDOWS_STA).toByteArray()
