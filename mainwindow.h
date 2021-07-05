#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Synthesizer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_FreqBox_editingFinished();

    void on_AmpBox_editingFinished();

    void on_DiagButtn_clicked();

    void on_ConfigButton_clicked();

private:
    Ui::MainWindow *ui;
    Synthesizer *osc;
};
#endif // MAINWINDOW_H
