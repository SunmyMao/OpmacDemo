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

    connect(ui.btnReadLoop, &QPushButton::clicked, this, &OpmacDemo::btnReadLoopStartClicked);
    connect(ui.btnReadStop, &QPushButton::clicked, this, &OpmacDemo::btnReadLoopStopClicked);

    connect(ui.btnWrite, &QPushButton::clicked, this, &OpmacDemo::btnWriteClicked);

    connect(&m_timer, &QTimer::timeout, this, &OpmacDemo::readTimeout);
}

OpmacDemo::~OpmacDemo()
{}

void OpmacDemo::btnSerialPortOpenClicked(bool clicked)
{
    qDebug() << "open";

    // _modbus_ctx = modbus_new_rtu(.toStdString().c_str(),
    QString device = ui.cboxSerialPort->currentText();
#ifdef _WIN32
    if (device.length() > QString("COMX").length())
        device = QString("\\\\.\\").append(device);
#endif

    _modbus_ctx = modbus_new_rtu(device.toStdString().c_str(),
        ui.cboxBaudrate->currentData().toInt(),
        ui.cboxParity->currentData().toChar().toLatin1(),
        ui.cboxDataBits->currentData().toInt(),
        ui.cboxStopBits->currentData().toInt());
    // modbus_set_debug(_modbus_ctx, 1);
    if (_modbus_ctx != nullptr && modbus_connect(_modbus_ctx) != -1)
    {
        ui.btnReadLoop->setEnabled(true);
        ui.btnReadOnce->setEnabled(true);
        ui.btnWrite->setEnabled(true);
        ui.btnSerialClose->setEnabled(true);

        ui.btnSerialOpen->setEnabled(false);
    }
    modbus_set_slave(_modbus_ctx, 1);
}

void OpmacDemo::btnSerialPortCloseClicked(bool clicked)
{
    qDebug() << "close";

    m_timer.stop();

    modbus_close(_modbus_ctx);
    modbus_free(_modbus_ctx);

    ui.btnReadLoop->setEnabled(false);
    ui.btnReadOnce->setEnabled(false);
    ui.btnWrite->setEnabled(false);
    ui.btnSerialClose->setEnabled(false);
    ui.btnReadStop->setEnabled(false);

    ui.btnSerialOpen->setEnabled(true);
}

void OpmacDemo::btnReadOnceClicked(bool clicked)
{
    Q_UNUSED(clicked)
    batchRead();
}

void OpmacDemo::btnReadLoopStartClicked(bool clicked)
{
    m_timer.start(1000);

    ui.btnReadStop->setEnabled(true);
    ui.btnReadLoop->setEnabled(false);
    ui.btnReadOnce->setEnabled(false);
}

void OpmacDemo::btnReadLoopStopClicked(bool clicked)
{
    m_timer.stop();

    ui.btnReadStop->setEnabled(false);
    ui.btnReadLoop->setEnabled(true);
    ui.btnReadOnce->setEnabled(true);
}

void OpmacDemo::btnWriteClicked(bool clicked)
{
    int addr_table[] = { 0x4a, 0x46, 0x47, 0x48 };
    uint16_t value;
    if (ui.cboxVariable->currentIndex() != 0)
        value = ui.editValue->text().toDouble() * 1000;
    else
        value = ui.editValue->text().toUInt();
    modbus_write_register(_modbus_ctx, addr_table[ui.cboxVariable->currentIndex()], value);
}

void OpmacDemo::readTimeout()
{
    batchRead();
}

void OpmacDemo::initSerialPortSettings() const
{
    int i = 0;
    for (const auto& info : QSerialPortInfo::availablePorts())
        ui.cboxSerialPort->insertItem(i++, info.portName(), info.portName());

    i = 0;
    ui.cboxBaudrate->insertItem(i++, QString::number(1200), 1200);
    ui.cboxBaudrate->insertItem(i++, QString::number(2400), 2400);
    ui.cboxBaudrate->insertItem(i++, QString::number(4800), 4800);
    ui.cboxBaudrate->insertItem(i++, QString::number(9600), 9600);
    ui.cboxBaudrate->insertItem(i++, QString::number(19200),19200);
    ui.cboxBaudrate->insertItem(i++, QString::number(38400),38400);
    ui.cboxBaudrate->insertItem(i++, QString::number(57600),57600);
    ui.cboxBaudrate->insertItem(i++, QString::number(115200), 115200);
    ui.cboxBaudrate->setCurrentIndex(3);

    i = 0;
    ui.cboxDataBits->insertItem(i++, QString::number(i + 5), i + 5);
    ui.cboxDataBits->insertItem(i++, QString::number(i + 5), i + 5);
    ui.cboxDataBits->insertItem(i++, QString::number(i + 5), i + 5);
    ui.cboxDataBits->insertItem(i++, QString::number(i + 5), i + 5);
    ui.cboxDataBits->setCurrentIndex(3);

    i = 0;
    ui.cboxStopBits->insertItem(i++, QString::number(1), 1);
    ui.cboxStopBits->insertItem(i++, QString::number(1.5), 3);
    ui.cboxStopBits->insertItem(i++, QString::number(2), 2);
    ui.cboxStopBits->setCurrentIndex(0);

    i = 0;
    ui.cboxParity->insertItem(i++, "None", 'N');
    ui.cboxParity->insertItem(i++, "Even", 'E');
    ui.cboxParity->insertItem(i++, "Odd", 'O');
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

void OpmacDemo::batchRead() const
{
    uint16_t* tab_reg = (uint16_t*)malloc(MODBUS_MAX_READ_REGISTERS * sizeof(uint16_t));
    
    if (modbus_read_input_registers(_modbus_ctx, 0x41, 5, tab_reg) >= 0)
    {
        uint8_t idx = 0;
        ui.editDiameterAverage->setText(QString::number(static_cast<float>(tab_reg[idx++]) / 1000));
        ui.editDiameterX->setText(QString::number(static_cast<float>(tab_reg[idx++]) / 1000));
        ui.editDiameterY->setText(QString::number(static_cast<float>(tab_reg[idx++]) / 1000));
        ui.editPositionX->setText(QString::number(int16_t(tab_reg[idx++])));
        ui.editPositionY->setText(QString::number(int16_t(tab_reg[idx++])));
    }

    if (modbus_read_registers(_modbus_ctx, 0x46, 3, tab_reg) != -1)
    {
        uint8_t idx = 0;
        ui.editDiameterReferred->setText(QString::number(static_cast<float>(tab_reg[idx++]) / 1000));
        ui.editDeviationSettedUpper->setText(QString::number(static_cast<float>(tab_reg[idx++]) / 1000));
        ui.editDeviationSettedLower->setText(QString::number(static_cast<float>(tab_reg[idx]) / 1000));
    }

    if (modbus_read_input_registers(_modbus_ctx, 0x49, 1, tab_reg) != -1)
    {
        uint8_t idx = 0;
        ui.editPIDCtlOutput->setText(QString::number((int16_t)tab_reg[idx]));
    }

    if (modbus_read_registers(_modbus_ctx, 0x4a, 9, tab_reg) != -1)
    {
        uint8_t idx = 0;
        ui.editBuzzerSwitch->setText(QString::number(tab_reg[idx++]));
        ui.editAverageTimes->setText(QString::number(tab_reg[idx++]));
        ui.editParamP->setText(QString::number(tab_reg[idx++]));
        ui.editParamI->setText(QString::number(tab_reg[idx++]));
        ui.editParamD->setText(QString::number(tab_reg[idx++]));
        ui.editParamAt->setText(QString::number(tab_reg[idx++]));
        ui.editVoltagePolority->setText(QString::number(tab_reg[idx++]));
        ui.editCtlMode->setText(QString::number(tab_reg[idx++]));
        ui.editInputSwitch->setText(QString::number(tab_reg[idx++]));
    }

    if (modbus_read_input_registers(_modbus_ctx, 0x54, 3, tab_reg) != -1)
    {
        uint8_t idx = 0;
        ui.editAddr->setText(QString::number(tab_reg[idx++]));
        ui.editBaudrate->setText(QString::number(tab_reg[idx++]));
        ui.editCommMode->setText(QString::number(tab_reg[idx]));
    }

    delete tab_reg;
}
