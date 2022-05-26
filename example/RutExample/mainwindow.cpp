#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Log.h"
#include "Sets.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	restoreState(RUT_SETS_GET_WINDOWS_STA);
	restoreGeometry(RUT_SETS_GET_WINDOWS_GEO);
	RUT_SETS_LOAD_WIDGET(ui->leNewLogLine);
	LOG("main window created");
}

MainWindow::~MainWindow()
{
	RUT_SETS_SAVE_WIDGET(ui->leNewLogLine);
	RUT_SETS_SAVE_WINDOWS_GEO;
	RUT_SETS_SAVE_WINDOWS_STA;
	delete ui;
}

void MainWindow::on_pbAddToLog_clicked()
{
	log(ui->leNewLogLine->text());
}

void MainWindow::log(const QString& s)
{
	auto ss = LOG(s);
	ss.replace("\033[031m", "<span style=\"color: red;\">");
	ss.replace("\033[032m", "<span style=\"color: green;\">");
	ss.replace("\033[033m", "<span style=\"color: #e97541;\">");
	ss.replace("\033[034m", "<span style=\"color: blue;\">");
	ss.replace("\033[035m", "<span style=\"color: magenta;\">");
	ss.replace("\033[036m", "<span style=\"color: #20B2AA;\">");
	ss.replace("\033[037m", "<span style=\"color: gray;\">");
	ss.replace("\033[1m", "<span style=\"font-weight: bold;\">");
	ss.replace("\033[0m", "</span>");
	ui->lwLog->insertItem(ui->lwLog->count(), "");
	ui->lwLog->setItemWidget(ui->lwLog->item(ui->lwLog->count() - 1), new QLabel(ss, ui->lwLog));
	ui->lwLog->scrollToBottom();
}
