#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString a="COM4";
    osc=new Synthesizer(a);
    if(osc->openSerialPort()==false)
    {
       ui->panel->insertPlainText("Port açılamadı");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_FreqBox_editingFinished()
{

    osc->setFrequency(ui->FreqBox->value()*10e6);
}

void MainWindow::on_AmpBox_editingFinished()
{

    osc->setAmplitude(ui->AmpBox->value());
}

void MainWindow::on_DiagButtn_clicked()
{

    QList<QByteArray> diag;
    osc->readDiagnostic(diag);
    for(int i=0;i<diag.length();i++)
    {
         ui->panel->insertPlainText(diag[i]);
    }
}

void MainWindow::on_ConfigButton_clicked()
{
    QList<QByteArray> config;
       osc->readConfiguration(config);
       for(int i=0;i<config.length();i++)
       {
            ui->panel->insertPlainText(config[i]);
       }


}
