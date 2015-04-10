#ifndef WINSOCKWRAP_H
#define WINSOCKWRAP_H

#include <string>
#include <sstream>
#include <vector>

#include <memory>

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#pragma comment(lib, "mpr.lib")
#pragma comment(lib, "Ws2_32.lib")

// Handles the WinSock init(and cleanup) process, making the process
//more OO.
class WinSockLibManager
{
public:
    // true -> success, false otherwise
    // maj min version number 2.2
    static bool init(int maj=2, int min=2);

    // true -> success, false otherwise
    static bool clean();

    // getters
    static bool is_init();
    static int last_error();
private:
    static bool _is_init;
    static int  _last_error;
};


// tiny wrapper for a ipv4 tcp "client-side" socket
class WinIPv4TcpSocket
{
public:
    WinIPv4TcpSocket();
    ~WinIPv4TcpSocket();    // automatically closes if it's open

    bool connectTo(const std::string& url, int port);
    bool disconnect();

    std::string read(int size, bool& err);
    bool send(const std::string& data);

    // getters
    bool is_valid() const;
    bool closed() const;
    bool connected() const;

	std::string peeraddress() const;
private:
    // trivial constructor for server to construct connected sockets
    explicit WinIPv4TcpSocket(SOCKET h, bool v, bool c);
    friend class WinIPv4TcpServer;

    SOCKET _handle;     // handle to the winsock

    bool _is_valid;
    bool _closed;
};



// tiny wrapper for a ipv4 tcp "server-side" socket
class WinIPv4TcpServer
{
public:
    WinIPv4TcpServer();
    ~WinIPv4TcpServer();    // automatically closes if it's open

    bool close();

    bool bind(const std::string& url, int port);
    bool listen(int maxconn=SOMAXCONN);
    std::unique_ptr<WinIPv4TcpSocket> accept();     // blocking


    // getters
    bool is_valid() const;
    bool bound() const;
    bool listening() const;
    bool closed() const;
private:
    SOCKET _handle;     // handle to the winsock

    bool _closed;
    bool _is_valid;
    bool _bound;
    bool _listening;
};

#endif // WINSOCKWRAP_H
