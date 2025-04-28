#include "opmac_demo.h"
#include <QSerialPortInfo>

OpmacDemo::OpmacDemo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    initSerialPortSettings();
    initWritableVariableCbox();

    ui.btnSerialClose->setEnabled(false);
    ui.btnReadOnce->setEnabled(false);
    ui.btnReadStop->setEnabled(false);
    ui.btnReadLoop->setEnabled(false);
    ui.btnWrite->setEnabled(false);

    connect(ui.btnSerialOpen, &QPushButton::clicked, this,  &OpmacDemo::btnSerialPortOpenClicked);
    connect(ui.btnSerialClose, &QPushButton::clicked, this, &OpmacDemo::btnSerialPortCloseClicked);
}

OpmacDemo::~OpmacDemo()
{}

void OpmacDemo::btnSerialPortOpenClicked(bool clicked)
{
    qDebug() << "open";
    m_serial = new QSerialPort();
    m_serial->setPortName(ui.cboxSerialPort->currentText());
    m_serial->setBaudRate(ui.cboxBaudrate->currentData().toInt());
    m_serial->setDataBits(static_cast<QSerialPort::DataBits>(ui.cboxDataBits->currentData().toInt()));
    m_serial->setParity(static_cast<QSerialPort::Parity>(ui.cboxParity->currentData().toInt()));
    m_serial->setStopBits(static_cast<QSerialPort::StopBits>(ui.cboxStopBits->currentData().toInt()));
    if (!m_serial->open(QIODeviceBase::ReadWrite))
    {
        delete m_serial;
        m_serial = nullptr;
    }
    if (m_serial != nullptr)
    {
        ui.btnReadLoop->setEnabled(true);
        ui.btnReadOnce->setEnabled(true);
        ui.btnWrite->setEnabled(true);
        ui.btnSerialClose->setEnabled(true);

        ui.btnSerialOpen->setEnabled(false);
    }
}

void OpmacDemo::btnSerialPortCloseClicked(bool clicked)
{
    qDebug() << "close";

    m_serial->close();
    delete m_serial;
    m_serial = nullptr;

    ui.btnReadLoop->setEnabled(false);
    ui.btnReadOnce->setEnabled(false);
    ui.btnWrite->setEnabled(false);
    ui.btnSerialClose->setEnabled(false);

    ui.btnSerialOpen->setEnabled(true);
}

void OpmacDemo::initSerialPortSettings() const
{
    int i = 0;
    for (const auto& info : QSerialPortInfo::availablePorts())
    {
        ui.cboxSerialPort->insertItem(i++, info.portName(), info.portName());
    }

    i = 0;
    ui.cboxBaudrate->insertItem(i++, QString::number(QSerialPort::Baud1200), QSerialPort::Baud1200);
    ui.cboxBaudrate->insertItem(i++, QString::number(QSerialPort::Baud2400), QSerialPort::Baud2400);
    ui.cboxBaudrate->insertItem(i++, QString::number(QSerialPort::Baud4800), QSerialPort::Baud4800);
    ui.cboxBaudrate->insertItem(i++, QString::number(QSerialPort::Baud9600), QSerialPort::Baud9600);
    ui.cboxBaudrate->insertItem(i++, QString::number(QSerialPort::Baud19200), QSerialPort::Baud19200);
    ui.cboxBaudrate->insertItem(i++, QString::number(QSerialPort::Baud38400), QSerialPort::Baud38400);
    ui.cboxBaudrate->insertItem(i++, QString::number(QSerialPort::Baud57600), QSerialPort::Baud57600);
    ui.cboxBaudrate->insertItem(i++, QString::number(QSerialPort::Baud115200), QSerialPort::Baud115200);
    ui.cboxBaudrate->setCurrentIndex(3);

    i = 0;
    ui.cboxDataBits->insertItem(i++, QString::number(QSerialPort::Data5), QSerialPort::Data5);
    ui.cboxDataBits->insertItem(i++, QString::number(QSerialPort::Data6), QSerialPort::Data6);
    ui.cboxDataBits->insertItem(i++, QString::number(QSerialPort::Data7), QSerialPort::Data7);
    ui.cboxDataBits->insertItem(i++, QString::number(QSerialPort::Data8), QSerialPort::Data8);
    ui.cboxDataBits->setCurrentIndex(3);

    i = 0;
    ui.cboxStopBits->insertItem(i++, QString::number(QSerialPort::OneStop), QSerialPort::OneStop);
    ui.cboxStopBits->insertItem(i++, QString::number(1.5), QSerialPort::OneAndHalfStop);
    ui.cboxStopBits->insertItem(i++, QString::number(QSerialPort::TwoStop), QSerialPort::TwoStop);
    ui.cboxStopBits->setCurrentIndex(0);

    i = 0;
    ui.cboxParity->insertItem(i++, "None", QSerialPort::NoParity);
    ui.cboxParity->insertItem(i++, "Even", QSerialPort::EvenParity);
    ui.cboxParity->insertItem(i++, "Odd", QSerialPort::OddParity);
    ui.cboxParity->setCurrentIndex(0);
}

void OpmacDemo::initWritableVariableCbox() const
{
    int i = 0;
    ui.cboxVariable->insertItem(0, "\u8702\u9e23\u5f00\u5173", 0);
    ui.cboxVariable->insertItem(1, "\u53c2\u8003\u76f4\u5f84", 1);
    ui.cboxVariable->insertItem(2, "\u4e0a\u504f\u5dee\u9884\u7f6e", 2);
    ui.cboxVariable->insertItem(3, "\u4e0b\u504f\u5dee\u9884\u7f6e", 3);
}
