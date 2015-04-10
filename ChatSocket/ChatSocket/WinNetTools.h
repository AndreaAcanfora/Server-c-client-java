#pragma once

#include <WinSock2.h>
#include <winnetwk.h>
#include <WS2tcpip.h>
#include <Windows.h>

#include <vector>
#include <string>

namespace WinNetTools {
	std::vector<std::string> getAddressFromHostname(const std::string& hostname);

	std::vector<std::string> getNetworkResourceNames(LPNETRESOURCE lpnr);

	std::string wstringToString(const std::wstring &wstr);
}