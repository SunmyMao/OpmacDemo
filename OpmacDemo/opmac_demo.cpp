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
    connect(ui.btnReadOnce, &QPushButton::clicked, this, &OpmacDemo::btnReadOnceClicked);
}

OpmacDemo::~OpmacDemo()
{}

void OpmacDemo::btnSerialPortOpenClicked(bool clicked)
{
    qDebug() << "open";

    m_client = modbus::Client::alloc(ui.cboxSerialPort->currentText(),
        static_cast<QSerialPort::BaudRate>(ui.cboxBaudrate->currentData().toInt()),
        static_cast<QSerialPort::Parity>(ui.cboxParity->currentData().toInt()),
        static_cast<QSerialPort::DataBits>(ui.cboxDataBits->currentData().toInt()),
        static_cast<QSerialPort::StopBits>(ui.cboxStopBits->currentData().toInt())
    );
    if (m_client != nullptr)
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

    modbus::Client::destory(&m_client);

    ui.btnReadLoop->setEnabled(false);
    ui.btnReadOnce->setEnabled(false);
    ui.btnWrite->setEnabled(false);
    ui.btnSerialClose->setEnabled(false);

    ui.btnSerialOpen->setEnabled(true);
}

void OpmacDemo::btnReadOnceClicked(bool clicked)
{
    QByteArray dest;
    modbus::Request req;
    req.set_slave_id(1);
    req.set_func_code(0x03);
    req.set_addr(0);
    modbus::Response* response = m_client->request(&req);
    if (response == nullptr)
        return;
    qDebug() << response->data_count();
    qDebug() << response->data_at(0);
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
