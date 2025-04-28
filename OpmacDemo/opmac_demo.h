#pragma once

#include <QtWidgets/QMainWindow>
#include <QSerialPort>
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

private:
    void initSerialPortSettings() const;
    void initWritableVariableCbox() const;

    Ui::OpmacDemoClass ui;

    QSerialPort* m_serial = nullptr;
};
