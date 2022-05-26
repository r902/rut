#include <QStringBuilder>
#include <QDir>
#include <QDebug>
#include <QSettings>
#ifdef QT_GUI_LIB
	#include <QHeaderView>
	#include <QWidget>
	#include <QLineEdit>
	#include <QSpinBox>
	#include <QDoubleSpinBox>
	#include <QComboBox>
	#include <QFontComboBox>
	#include <QCheckBox>
	#include <QTextEdit>
	#include <QRadioButton>
	#include <QTabWidget>
	#include <QPushButton>
	#include <QGroupBox>
	#include <QStyleFactory>
	#include <QFontDatabase>
	#include <QTreeWidget>
	#include <QHeaderView>
	#include <QDateTimeEdit>
	#include <QTableView>
	#include <QSplitter>
	#include <QApplication>
#else
	#include <QCoreApplication>
#endif

#include "Sets.h"

//Параметры командной строки (не используются из application т.к. его может уже не быть при завершении программы)
static QStringList appArgs;

RutSets *RutSets::self = nullptr;

RutSets::RutSets(const QString &comp, const QString &app, QObject* parent):QObject(parent)
{
	gRutSets = new QSettings(comp, app, parent);
	QDir d = QFileInfo(gRutSets->fileName()).dir();
	if (!d.exists(gRutSets->fileName())) d.mkdir(d.absolutePath());
}

RutSets::~RutSets()
{
	delete gRutSets;
	gRutSets = nullptr;
}

void RutSets::init(const QString &comp, const QString &app, QObject *parent, int argc, char *argv[])
{
	self = new RutSets(comp, app, parent);
	for (int i = 0; i < argc; ++i) appArgs.append(argv[i]);
}

void RutSets::clear()
{
	if (!gRutSets) return;
	gRutSets->clear();
	gRutSets->sync();
}

bool RutSets::argsContains(const QString &param)
{
	return appArgs.contains(param);
}

QString RutSets::argsValue(const QString &param)
{
	if (qApp)
	{
		int idx = qApp->arguments().indexOf(param);
		if ((idx < 0) || (qApp->arguments().count() < idx + 2)) return QString();
		return qApp->arguments()[idx + 1];
	}
	return QString();
}

void RutSets::ssaveWidget(const QString& prefix, const QWidget *w)
{
	// DLOG_PF("prefix [%1] widget [%2]", prefix, w->objectName());
	if (!gRutSets) return;

#ifdef QT_GUI_LIB
	if (w->metaObject()->className() == QLineEdit::staticMetaObject.className())
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QLineEdit *)w)->text());
	}
	else
	if (w->metaObject()->className() == QSpinBox::staticMetaObject.className())
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QSpinBox *)w)->value());
	}
	else
	if (w->metaObject()->className() == QDoubleSpinBox::staticMetaObject.className())
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QDoubleSpinBox *)w)->value());
	}
	else
	if (w->metaObject()->className() == QComboBox::staticMetaObject.className())
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QComboBox *)w)->currentIndex());
	}
	else
	if (w->metaObject()->className() == QFontComboBox::staticMetaObject.className())
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QFontComboBox *)w)->currentIndex());
	}
	else
	if (w->metaObject()->className() == QCheckBox::staticMetaObject.className())
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QCheckBox *)w)->isChecked());
	}
	else
	if (w->metaObject()->className() == QPushButton::staticMetaObject.className())
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QPushButton *)w)->isChecked());
	}
	else
	if (w->metaObject()->className() == QRadioButton::staticMetaObject.className())
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QRadioButton *)w)->isChecked());
	}
	else
	if (w->metaObject()->className() == QTabWidget::staticMetaObject.className()
#ifdef __GXX_RTTI
		|| dynamic_cast<const QTabWidget*>(w)
#endif
			)
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QTabWidget *)w)->currentIndex());
	}
	else
	if (w->metaObject()->className() == QGroupBox::staticMetaObject.className())
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QGroupBox *)w)->isChecked());
	}
	else
	if (w->metaObject()->className() == QTreeWidget::staticMetaObject.className())
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QTreeWidget *)w)->header()->saveState());
	}
	else
	if ((w->metaObject()->className() == QTextEdit::staticMetaObject.className()) || (QString(w->metaObject()->className()) == "MyTextEdit"))
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QTextEdit *)w)->toPlainText());
	}
	else
	if (w->metaObject()->className() == QDateTimeEdit::staticMetaObject.className())
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QDateTimeEdit *)w)->dateTime());
	}
	else
	if (w->metaObject()->className() == QTableView::staticMetaObject.className()
#ifdef __GXX_RTTI
			|| dynamic_cast<const QTableView*>(w)
#endif
			)
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QTableView *)w)->horizontalHeader()->saveState());
		gRutSets->setValue(QString("%1/%2/%3").arg(prefix, w->objectName(), RUT_SETS_QTABLEWIDGET_DATA_PROPERTY), ((const QTableView *)w)->property(RUT_SETS_QTABLEWIDGET_DATA_PROPERTY));
	}
	else
	if (w->metaObject()->className() == QSplitter::staticMetaObject.className())
	{
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QSplitter *)w)->saveState());
	}
	else if (w->metaObject()->className() == QTreeView::staticMetaObject.className())
		gRutSets->setValue(QString("%1/%2").arg(prefix, w->objectName()), ((const QTreeView*)w)->header()->saveState());
#endif
}

const QString CStr_Widget_ = QStringLiteral("Widget_");
const QString CStr__ = QStringLiteral("_");

void RutSets::saveWidget(const QWidget *w, const QString &userPrefix)
{
#ifdef QT_GUI_LIB
	QString prefix = w->parent()->objectName();
	if (!userPrefix.isEmpty())
		prefix = prefix % CStr__ % userPrefix;
	ssaveWidget(prefix, w);
#endif
}

QString RutSets::makePrefix(const QWidget *w, const QWidget *maxParent)
{
	QString prefix;
#ifdef QT_GUI_LIB
	const QWidget *p = w;
	do {
		p = p->parentWidget();
		if (p)
			prefix = p->objectName() % CStr__ % prefix;
	} while (p && (p != maxParent));
#endif
	return CStr_Widget_ % prefix;
}

QString RutSets::saveWidget(const QWidget *w, const QWidget *maxParent)
{
	if (!w) return QString();
	QString prefix = makePrefix(w, maxParent);
	ssaveWidget(prefix, w);
	return prefix;
}

void RutSets::saveGroupWidgets(const QWidget *w, const QWidget *maxParent)
{
	for (auto* ww: w->children()) saveWidget(qobject_cast<QWidget*>(ww), maxParent);
}

void RutSets::saveWidgetData(const QWidget *w, const QWidget *maxParent, const QVariant& data)
{
	if (!w) return;
#ifdef QT_GUI_LIB
	gRutSets->setValue(QString("%1/%2_DATA").arg(makePrefix(w, maxParent), w->objectName()), data);
#endif
}

QVariant RutSets::loadWidgetData(QWidget *w, const QWidget *maxParent)
{
#ifdef QT_GUI_LIB
	if (!w) return QVariant();
	return gRutSets->value(QString("%1/%2_DATA").arg(makePrefix(w, maxParent), w->objectName()));
#else
	return QVariant();
#endif
}

void RutSets::lloadWidget(const QString &prefix, QWidget *w)
{
	if (!gRutSets) return;

#ifdef QT_GUI_LIB
	QString RutSetsName = QString("%1/%2").arg(prefix, w->objectName());
	QVariant v = gRutSets->value(RutSetsName);
	if (!v.isValid())
	{
		if (w->metaObject()->className() == QTableView::staticMetaObject.className()
#ifdef __GXX_RTTI
				|| dynamic_cast<const QTableView*>(w)
#endif
			)
		{
			// Подбор ширины колонки, с учетом шрифта и размера (Ubuntu, 16)
			auto* tv = (QTableView *)w;
			int colsCount = tv->model()->columnCount();
			int colsW = 350;
			if (colsCount < 3) colsW = 500; else
			if (colsCount == 5) colsW = 350; else
			if (colsCount > 6) colsW = 250;
			for (int i = 0; i < colsCount; i++)
			{
				int w = tv->model()->headerData(i, Qt::Horizontal, Qt::UserRole + 1).toInt();
				if (w > 0)
					tv->setColumnWidth(i, w);
				else
					tv->setColumnWidth(i, colsW);
			}
			//for (int i = 0; i < colsCount; i++) DLOG_PF("[%1] %2 %3", tv->objectName(), tv->model()->headerData(i, Qt::Horizontal, COLUMNWIDTHROLE).toInt(), tv->columnWidth(i));
		}
		return;
	}

	if (w->metaObject()->className() == QLineEdit::staticMetaObject.className())
	{
		((QLineEdit *)w)->setText(v.toString());
	}
	else
	if (w->metaObject()->className() == QSpinBox::staticMetaObject.className())
	{
		((QSpinBox *)w)->setValue(v.toInt());
	}
	else
	if (w->metaObject()->className() == QDoubleSpinBox::staticMetaObject.className())
	{
		((QDoubleSpinBox *)w)->setValue(v.toDouble());
	}
	else
	if (w->metaObject()->className() == QComboBox::staticMetaObject.className())
	{
		((QComboBox *)w)->setCurrentIndex(v.toInt());
	}
	else
	if (w->metaObject()->className() == QFontComboBox::staticMetaObject.className())
	{
		((QFontComboBox *)w)->setCurrentIndex(v.toInt());
	}
	else
	if (w->metaObject()->className() == QCheckBox::staticMetaObject.className())
	{
		((QCheckBox *)w)->setChecked(v.toBool());
	}
	else
	if (w->metaObject()->className() == QPushButton::staticMetaObject.className())
	{
		((QPushButton *)w)->setChecked(v.toBool());
	}
	else
	if (w->metaObject()->className() == QRadioButton::staticMetaObject.className())
	{
		((QRadioButton *)w)->setChecked(v.toBool());
	}
	else
	if (w->metaObject()->className() == QTabWidget::staticMetaObject.className()
#ifdef __GXX_RTTI
			|| dynamic_cast<const QTabWidget*>(w)
#endif
			)
	{
		((QTabWidget *)w)->setCurrentIndex(v.toInt());
	}
	else
	if (w->metaObject()->className() == QGroupBox::staticMetaObject.className())
	{
		((QGroupBox *)w)->setChecked(v.toBool());
	}
	else
	if (w->metaObject()->className() == QTreeWidget::staticMetaObject.className())
	{
		((QTreeWidget *)w)->header()->restoreState(v.toByteArray());
	}
	else
	if ((w->metaObject()->className() == QTextEdit::staticMetaObject.className()) || (QString(w->metaObject()->className()) == "MyTextEdit"))
	{
		((QTextEdit *)w)->setPlainText(v.toString());
	}
	else
	if (w->metaObject()->className() == QDateTimeEdit::staticMetaObject.className())
	{
		((QDateTimeEdit *)w)->setDateTime(v.toDateTime());
	}
	else
	if (w->metaObject()->className() == QTableView::staticMetaObject.className()
#ifdef __GXX_RTTI
		|| dynamic_cast<const QTableView*>(w)
#endif
		)
	{
		((QTableView *)w)->horizontalHeader()->restoreState(v.toByteArray());
		QVariant v = gRutSets->value(QString("%1/%2/%3").arg(prefix, w->objectName(), RUT_SETS_QTABLEWIDGET_DATA_PROPERTY));
		if (v.isValid())
			w->setProperty(RUT_SETS_QTABLEWIDGET_DATA_PROPERTY, v);
	}
	else
	if (w->metaObject()->className() == QSplitter::staticMetaObject.className())
	{
		auto* sp = (QSplitter *)w;
		sp->restoreState(v.toByteArray());
	}
	else if (w->metaObject()->className() == QTreeView::staticMetaObject.className())
		((QTreeView*)w)->header()->restoreState(v.toByteArray());
	else
		qDebug() << "RutSets ERROR LOAD:" << w->metaObject()->className() << w->objectName();
#endif
}

void RutSets::loadWidget(QWidget *w, const QString &userPrefix)
{
#ifdef QT_GUI_LIB
	if (!w) return;
	QString prefix = w->parent()->objectName();
	if (!userPrefix.isEmpty())
		prefix = QString("%1_%2").arg(prefix, userPrefix);
	lloadWidget(prefix, w);
#endif
}

void RutSets::loadGroupWidgets(QWidget *w, const QWidget *maxParent)
{
	for (auto* ww: w->children()) loadWidget(qobject_cast<QWidget*>(ww), maxParent);
}

QString RutSets::loadWidget(QWidget *w, const QWidget *maxParent)
{
#ifdef QT_GUI_LIB
	if (!w) return QString();
	w->blockSignals(true);
	QString prefix = makePrefix(w, maxParent);
	lloadWidget(prefix, w);
	w->blockSignals(false);
	return prefix;
#else
	return QString();
#endif
}

int RutSets::set(const QString &key, const QVariant& value)
{
	if (!gRutSets) return -1;
	gRutSets->setValue(key, value);
	emit changed(key, value);
	return 0;
}

QVariant RutSets::get(const QString &key, const QVariant &defaultValue)
{
	if (!gRutSets) return QVariant();

	QVariant val = gRutSets->value(key);
	if (val.isValid()) return val;

	if (defaultValue.isValid())	return defaultValue;
	return val;
}

void RutSets::sync()
{
	if (gRutSets) gRutSets->sync();
}
