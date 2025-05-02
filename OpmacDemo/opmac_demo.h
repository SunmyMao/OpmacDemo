#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "ui_main.h"
#include "../libmodbus/modbus.h"

class OpmacDemo : public QMainWindow
{
    Q_OBJECT

public:
    OpmacDemo(QWidget *parent = nullptr);
    ~OpmacDemo();

private slots:
    void btnSerialPortOpenClicked(bool clicked);
    void btnSerialPortCloseClicked(bool clicked);

    void btnReadOnceClicked(bool clicked);
    void btnReadLoopStartClicked(bool clicked);
    void btnReadLoopStopClicked(bool clicked);

    void btnWriteClicked(bool clicked);
    void readTimeout();

private:
    void initSerialPortSettings() const;
    void initWritableVariableCbox() const;
    void batchRead() const;

    Ui::OpmacDemoClass ui;
    QTimer m_timer;

    modbus_t* _modbus_ctx = nullptr;
};
