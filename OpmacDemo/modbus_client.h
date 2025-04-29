#pragma once

#include <list>
#include <string>
#include <thread>
#include <QSerialPort>


namespace modbus
{
class Request
{
public:
    uint8_t slave_id() const { return _slave_id; }
    void set_slave_id(uint8_t id) { _slave_id = id; }

    uint8_t func_code() const { return _func_code; }
    void set_func_code(uint8_t code) { _func_code = code; }

    uint16_t register_coil_count() const { return  _register_coil_count; }
    void set_register_coil_count(uint16_t count) { _register_coil_count = count; }

    void set_addr(uint16_t addr) { _addr = addr; }

    uint16_t response_buffer_size();
    const uint8_t* build_cmd(uint16_t& length);

private:
    uint8_t _slave_id = 0x01;
    uint8_t _func_code = 0x03;
    uint16_t _addr = 0x00;
    uint16_t _register_coil_count = 0x01;

#define MAX_MESSAGE_LENGTH 260
    uint8_t _buffer[MAX_MESSAGE_LENGTH] = { 0 };
#undef MAX_MESSAGE_LENGTH
};

class Response
{
public:
    static Response* alloc(const uint8_t* buffer, uint16_t buffer_size);
    uint8_t err_code() const { return _err_no; };

    uint8_t slave_id() const { return _slave_id; }
    uint8_t func_code() const { return _func_code; };

    uint8_t data_count() const { return static_cast<uint8_t>(_data_array.size()); };
    uint16_t data_at(uint8_t idx) { return _data_array[idx]; };

private:
    uint8_t _err_no = 0;

    uint8_t _slave_id = 0x01;
    uint8_t _func_code = 0x0;
    std::vector<uint16_t> _data_array;

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

    Response* request(Request* req);

private:
    std::list<Request> m_requests;

    
    QSerialPort m_serial;
};
}
