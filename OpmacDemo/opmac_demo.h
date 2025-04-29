#pragma once

#include <QtWidgets/QMainWindow>
#include "modbus_client.h"
#include "ui_main.h"

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

private:
    void initSerialPortSettings() const;
    void initWritableVariableCbox() const;

    Ui::OpmacDemoClass ui;

    modbus::Client* m_client = nullptr;
};
