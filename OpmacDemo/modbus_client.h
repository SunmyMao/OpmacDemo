#pragma once

#include <list>
#include <string>
#include <thread>
#include <QSerialPort>


namespace modbus
{
class Observer
{
    virtual void response(uint64_t msgid, int32_t errNo, std::string msg);
};
class Request
{
    
};

class Response
{

};

class Client : public QObject
{
    Q_OBJECT
private:
    Client(QObject* parent = nullptr): QObject(parent) {}
    virtual ~Client() {}

public:
    static Client* alloc(const QString& name, QSerialPort::BaudRate baud,
        QSerialPort::Parity parity, QSerialPort::DataBits data, QSerialPort::StopBits stop);
    static void destory(Client** o);

    int32_t read_bits(uint8_t slave, uint16_t addr, uint16_t nb, QByteArray& dest);
    int32_t read_input_bits(uint8_t addr, int nb, uint8_t* dest) { return 0; }
    int32_t read_registers(uint8_t slave, uint16_t addr, int16_t nb, QByteArray& dest);
    int32_t read_input_registers(uint8_t slave, uint16_t addr, int nb, QByteArray& dest);
    int32_t write_bit(uint8_t coil_addr, int status) { return 0; }
    int32_t write_register(uint8_t slave, uint16_t addr, int16_t nb, QByteArray& dest);
    int32_t write_bits(uint8_t addr, int nb, const uint8_t* data) { return 0; }

private:
    std::list<Request> m_requests;
    std::list<Observer*> m_observers;

    
    QSerialPort m_serial;
};
}
