#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H
#include <QtSerialPort/QSerialPort>
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtConcurrent/QtConcurrent>
#include <QThreadPool>
#include <QJsonArray>
#include <QThread>
#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
enum ConfigurationMap
{
    rfoutput=0,
    frequency=1,
    amplitude=2,
    modulation_on_off=3,
    modulation_type=4,
    modulation_source=5,
    modulation_signal_waveform=6,
    modulation_freq=7,
    modulation_fm_deviation=8,
    modulation_am_depth=9,
    modulation_pulse_period=10,
    modulation_pulse_width=11,
    sweep_start_stop=12,
    sweep_start=13,
    sweep_stop=14,
    sweep_step=15,
    sweep_dwell=16,
    sweep_trigger=17,
    reference_int_ext=18,
    reference_tcxo_ocxo=19,
    wifi_mode=20,
    wifi_sta_ssid=21,
    wifi_sta_password=22,
    wifi_ap_ssid=23,
    wifi_ap_password=24,
    wifi_ip_address=25,
    wifi_gateway_address=26,
    wifi_subnet_address=27,
    phase_noise_mode=28,
    max_phase=29
};

enum DiagnosticMap
{
    temperature=0,
    lock_xtal=1,
    lock_lmx1=2,
    lock_lmx2=3,
    current=4,
    voltage=5,
    rssi=6,//wifi received signal power
    em=7,//firmware version
    ESP8266em=8,//ESP8266 embedded version
    serial_number=9,
    model=10
};

enum CommandStatus
{
    READY,
    FREQ_SET,
    AMPL_SET,
    DIAG_QUERY,
    CONFIG_QUERY,
    OTHERS
};

class Synthesizer : public QMainWindow
{
 Q_OBJECT
public:
    Synthesizer(const QString PortName);
    ~Synthesizer();
    bool openSerialPort();
    void closeSerialPort();
    //1Hz Resolution,unit is Hz,Frequency Range:250 kHz to 20 GHz
    bool setFrequency(int Frequency);
    //Unit is dBm,Amplitude Range: -60 to +15 dBm
    bool setAmplitude(float Amplitude);
    bool readDiagnostic(QList<QByteArray>& Diag);
    bool readConfiguration(QList<QByteArray>& Config);
    bool writeData(const QByteArray &command);
    void setDefaultSettings();

private slots:
    void handleError(QSerialPort::SerialPortError error);
    void readData();
private:
     QByteArray data;
     QSerialPort *serial;
     QString port;
     CommandStatus status;

};
#endif // SYNTHESIZER_H
