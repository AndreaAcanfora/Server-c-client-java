#include "winsockwrap.h"

#include <algorithm>

bool WinSockLibManager::_is_init   = false;
int WinSockLibManager::_last_error = 0;

bool WinSockLibManager::init(int maj, int min)
{
    if (_is_init)
        return false;

    _is_init = true;

    WSADATA wsaData;
    _last_error = WSAStartup(MAKEWORD(maj,min), &wsaData);

    return (_last_error==0);
}



bool WinSockLibManager::clean()
{
    if (!_is_init)
        return false;

    _is_init = false;

    _last_error = WSACleanup();

    return (_last_error==0);
}


bool WinSockLibManager::is_init()
{
    return _is_init;
}

int WinSockLibManager::last_error()
{
    return _last_error;
}




WinIPv4TcpSocket::WinIPv4TcpSocket()
{
    WinSockLibManager::init();

    _closed = true;

    _handle = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);     

    if (_handle == INVALID_SOCKET)
    {
        WinSockLibManager::clean();
        _is_valid = false;
    }
    else
        _is_valid = true;
}


WinIPv4TcpSocket::~WinIPv4TcpSocket()
{
    if (!_closed)
        disconnect();
}


bool WinIPv4TcpSocket::connectTo(const std::string& url, int port)
{
    if (!_closed || !_is_valid)
        return false;


    addrinfo hints, *res = NULL;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;


    std::stringstream ss;
    ss << port;

    int r = getaddrinfo(url.c_str(), ss.str().c_str(), &hints, &res);

    if (r != 0)
    {
        WinSockLibManager::clean();
        return false;
    }

    r = connect(_handle, res->ai_addr, (int)res->ai_addrlen);

    if (r == SOCKET_ERROR)
    {
        _handle = INVALID_SOCKET;
        _is_valid = false;
        closesocket(_handle);
        WinSockLibManager::clean();
    }
    else
        _closed = false;    

    freeaddrinfo(res);

    return !_closed;
}


bool WinIPv4TcpSocket::disconnect()
{
    if (_closed || !_is_valid)
        return false;

    _closed = true;

    int r = shutdown(_handle, SD_SEND);
    closesocket(_handle);

    if (r == SOCKET_ERROR)
    {
        WinSockLibManager::clean();
        return false;
    }
    return true;
}


bool WinIPv4TcpSocket::send(const std::string& data)
{
    if (_closed || !_is_valid)
        return false;

	int r = ::send(_handle, data.c_str(), data.size(), 0);
	
    return (r != SOCKET_ERROR);
}

std::string WinIPv4TcpSocket::read(int size, bool &err)
{
    if (_closed || !_is_valid)
    {
        err = true;
        return "";
    }
    err = false;

    char* buf_c = new char[size];
	
	int r = recv(_handle, buf_c, size, 0);

    if (r <= 0)
    {
        delete[] buf_c;
        err = true;
		_closed = true;
        return "";
    }

	using std::min;
    size = min(r, size);      

    std::string buf_s(buf_c, size);

    delete[] buf_c;

    return buf_s;
}


bool WinIPv4TcpSocket::is_valid() const
{
    return _is_valid;
}

bool WinIPv4TcpSocket::closed() const
{
    return _closed;
}

bool WinIPv4TcpSocket::connected() const
{
    return !_closed;    
}


std::string WinIPv4TcpSocket::peeraddress() const
{
	socklen_t len;
	struct sockaddr_storage addr;
	char ipstr[16];
	for (int i = 0; i < 16; ++i)
		ipstr[i] = 0;

	len = sizeof addr;
	getpeername(_handle, (struct sockaddr*)&addr, &len);

	struct sockaddr_in *s = (struct sockaddr_in *)&addr;
	inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);

	return std::string(ipstr);
}


WinIPv4TcpSocket::WinIPv4TcpSocket(SOCKET h, bool v, bool c)
{
    _handle = h;
    _is_valid = v;
    _closed = c;
}


WinIPv4TcpServer::WinIPv4TcpServer()
{
    WinSockLibManager::init();

    _bound = false;
    _listening = false;
    _closed = false;

    _handle = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);     

    if (_handle == INVALID_SOCKET)
    {
        WinSockLibManager::clean();
        _is_valid = false;
    }
    else
        _is_valid = true;

}

WinIPv4TcpServer::~WinIPv4TcpServer()
{
    if (!_closed)
        close();
}


bool WinIPv4TcpServer::close()
{
    if (!_closed) {
        _closed = true;
        closesocket(_handle);
        return true;
    }
    return false;
}

bool WinIPv4TcpServer::bind(const std::string &url, int port)
{
    if (_bound || _listening)
        return false;

    addrinfo hints, *res = NULL;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;


    std::stringstream ss;
    ss << port;

    int r = getaddrinfo(url.c_str(), ss.str().c_str(), &hints, &res);
    if (r != 0)
    {
        WinSockLibManager::clean();
        return false;
    }

    r = ::bind(_handle, res->ai_addr, (int)res->ai_addrlen);
    freeaddrinfo(res);

    if (r != 0)
    {
        _closed = true;
        closesocket(_handle);
       
        return false;
    }
    _bound = true;

    return true;
}


bool WinIPv4TcpServer::listen(int maxconn)
{
    if (!_bound || _listening)
        return false;

    if (::listen(_handle, maxconn) == SOCKET_ERROR)
        return false;

    _listening = true;
    return true;
}


std::unique_ptr<WinIPv4TcpSocket> WinIPv4TcpServer::accept()
{
    using ptr_t = std::unique_ptr<WinIPv4TcpSocket>;
    if (!_bound || !_listening)
        return ptr_t(nullptr);

    SOCKET s = INVALID_SOCKET;

    s = ::accept(_handle, NULL, NULL);
    if (s == INVALID_SOCKET)
        return ptr_t(nullptr);

    return ptr_t(new WinIPv4TcpSocket(s, true, false));
}

bool WinIPv4TcpServer::is_valid() const
{
    return _is_valid;
}

bool WinIPv4TcpServer::bound() const
{
    return _bound;
}

bool WinIPv4TcpServer::listening() const
{
    return _listening;
}

bool WinIPv4TcpServer::closed() const
{
    return _closed;
}
