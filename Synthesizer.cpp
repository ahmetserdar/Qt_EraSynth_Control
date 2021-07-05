//Created on 20/06/2021

#include"Synthesizer.h"
#include<QDebug>
Synthesizer::Synthesizer(const QString PortName)
{
    serial = new QSerialPort(this);
    port=PortName;
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()),this,SLOT(readData()));
}

Synthesizer::~Synthesizer()
{
    if(serial->isOpen())
    {
        serial->close();
    }
    delete serial;
}

bool Synthesizer::openSerialPort()
{
    serial->setPortName(port);
    serial->setBaudRate(115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite))
    {
        setDefaultSettings();
        return true;
    }

    return false;
}

void Synthesizer::setDefaultSettings()
{
    if(serial->isOpen())
    {

       //default settings for ERASynth++

       //ESP8266 MODULE OFF
       writeData(">PE00\r");

       // RF ON
       writeData(">P01\r");

       //MODULATION OFF
       writeData(">MS0\r");

       // SWEEP OFF
       writeData(">SS0\r");

       //INTERNAL REFERENCE
       writeData(">P10\r");

       //INTERNAL REFERENCE SOURCE:OCXO
       writeData(">P51\r\n");

       // SYNTHESIZER MODE:LOW SPURIOUS MODE
       writeData(">P90\r");
       status=OTHERS;

    }
}


void Synthesizer::readData()
{

    data =data.append( serial->readAll());

    if(data[data.length()-1]=='\n')
    {
          qDebug()<<"ReadData:"<<data;
          data.clear();
    }
}

bool Synthesizer::writeData(const QByteArray &command)
{
     if(command.length()==serial->write(command))
     {
         return true;
     }
     return false;
}

void Synthesizer::closeSerialPort()
{
    if(serial->isOpen())
    {
        serial->close();
    }
}

bool Synthesizer::setFrequency(int Frequency)
{
    if(serial->isOpen())
    {
        if(Frequency>250e3&&Frequency<20e9)
        {
           disconnect(serial, SIGNAL(readyRead()),this,SLOT(readData()));
           QByteArray command=">F";
           command.append(QByteArray::number(Frequency));
           command.append("\r");
           if(writeData(command)==false)
           {
               return false;
           }
           QByteArray response;
           response.clear();
           do
           {
               serial->waitForReadyRead(-1);
               if(serial->bytesAvailable()>0)
               {
                   response.append(serial->readAll());
               }
           }
           while(response[response.length()-1] != '\n');
           //qDebug()<<"response:"<<response;
           return true;
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Connection Error"),"Settings couldn't be sent");
    }
    return false;
}

bool Synthesizer::setAmplitude(float Amplitude)
{
    if(serial->isOpen())
    {
        if(Amplitude>=-60.0&&Amplitude<=15.0)
        {
           QByteArray command=">A";
           command.append(QByteArray::number(Amplitude));
           command.append("\r");
           if(writeData(command)==false)
           {
               return false;
           }

           QByteArray response;
           response.clear();
           do
           {
               serial->waitForReadyRead(-1);
               if(serial->bytesAvailable()>0)
               {
                   response.append(serial->readAll());
               }
           }
           while(response[response.length()-1] != '\n');
           //qDebug()<<"response:"<<response;
           return true;
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Connection Error"),"Settings couldn't be sent");
    }
    return false;
}

bool Synthesizer::readDiagnostic(QList<QByteArray>& Diagnostics)
{
    if(serial->isOpen())
    {
        disconnect(serial, SIGNAL(readyRead()),this,SLOT(readData()));
        if(writeData(">RD\r")==false)
        {
            return false;
        }
        QByteArray response;
        response.clear();
        do
        {
            serial->waitForReadyRead(-1);
            if(serial->bytesAvailable()>0)
            {
                response.append(serial->readAll());
            }
        }
        while(response[response.length()-1] != '\n');
        int first_char = response.indexOf("{");
        int last_char = response.lastIndexOf("}");
        response = response.mid(first_char, (response.size() - first_char - (response.size() - last_char - 1)));
        response = response.replace("{", "").replace("}", "").replace("\"", "");
        QList<QByteArray> values = response.split(',');
        for (int i = 0; i < values.size(); i++)
        {
           Diagnostics.append(values[i].split(':')[1]);
        }
        connect(serial, SIGNAL(readyRead()),this,SLOT(readData()));
    }
    else
    {
        QMessageBox::warning(this,tr("Connection Error"),"Settings couldn't be sent");
    }
     return false;
}

bool Synthesizer::readConfiguration(QList<QByteArray>& Configuration)
{
    if(serial->isOpen())
    {
        disconnect(serial, SIGNAL(readyRead()),this,SLOT(readData()));
        if(writeData(">RA\r")==false)
        {
            return false;
        }
        QByteArray response;
        response.clear();
        do
        {
            serial->waitForReadyRead(-1);
            if(serial->bytesAvailable()>0)
            {
                response.append(serial->readAll());
            }
        }
        while(response[response.length()-1] != '\n');
        int first_char = response.indexOf("{");
        int last_char = response.lastIndexOf("}");
        response = response.mid(first_char, (response.size() - first_char - (response.size() - last_char - 1)));
        response = response.replace("{", "").replace("}", "").replace("\"", "");
        QList<QByteArray> values = response.split(',');
        for (int i = 0; i < values.size(); i++)
        {
           Configuration.append(values[i].split(':')[1]);
        }
        connect(serial, SIGNAL(readyRead()),this,SLOT(readData()));
    }
    else
    {
        QMessageBox::warning(this,tr("Connection Error"),"Settings couldn't be sent");
    }
     return false;
}


void Synthesizer::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
