#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	QString logModuleName() const { return objectName(); }

private slots:
	void on_pbAddToLog_clicked();

private:
	Ui::MainWindow *ui;
	void log(const QString& s);
};
#endif // MAINWINDOW_H
