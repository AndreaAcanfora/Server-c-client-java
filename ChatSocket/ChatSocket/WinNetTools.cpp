#include "WinNetTools.h"

#include <algorithm>

namespace WinNetTools {

	std::vector<std::string> getAddressFromHostname(const std::string& hostname) {
		std::vector<std::string> addresses;

		struct addrinfo *result = NULL;
		struct addrinfo hints;

		struct sockaddr_in  *sockaddr_ipv4;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_ADDRCONFIG;

		if (getaddrinfo(hostname.c_str(), nullptr, &hints, &result) != 0)
			return std::vector<std::string>();

		while (result != NULL) {
			sockaddr_ipv4 = (struct sockaddr_in *) result->ai_addr;
			addresses.push_back(std::string(inet_ntoa(sockaddr_ipv4->sin_addr)));
			result = result->ai_next;
		}

		freeaddrinfo(result);

		return addresses;
	}

	// enumerates all network resources (hostnames)
	std::vector<std::string> getNetworkResourceNames(LPNETRESOURCE lpnr)
	{
		std::vector<std::string> hostnames;

		DWORD dwResult, dwResultEnum;
		HANDLE hEnum;
		DWORD cbBuffer = 16384;
		DWORD cEntries = -1;
		LPNETRESOURCE lpnrLocal;

		dwResult = WNetOpenEnum(RESOURCE_GLOBALNET, // all network resources
								RESOURCETYPE_ANY,   // all resources
								0,					// enumerate all resources
								lpnr,				// (first time the function is called)
								&hEnum);			// handle to the resource


		lpnrLocal = (LPNETRESOURCE)GlobalAlloc(GPTR, cbBuffer);

		if (dwResult != NO_ERROR || lpnrLocal == NULL)
			return hostnames;

		do {
			// Initialize the buffer
			ZeroMemory(lpnrLocal, cbBuffer);

			// Enumerate the network resources
			dwResultEnum = WNetEnumResource(hEnum, &cEntries, lpnrLocal, &cbBuffer);

			if (dwResultEnum == NO_ERROR) {
				// loop through the structures
				for (int i = 0; i < cEntries; ++i) {
					std::string remoteName(wstringToString(lpnrLocal[i].lpRemoteName));

					// check if the resource name starts with '\\'
					if (remoteName.compare(0, 2, "\\\\") == 0) {
						remoteName = remoteName.substr(2);		// remove the first '\\'

						// find other slashes and remove them
						if (remoteName.find("\\") != std::string::npos)
							remoteName = remoteName.substr(0, remoteName.find("\\"));

						// add to the hostnames vector
						if (std::find(hostnames.begin(), hostnames.end(), remoteName) == hostnames.end())
							hostnames.push_back(remoteName);
					}

					// if the resource found is a resource container, call the function recursively
					if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONTAINER)) {
						std::vector<std::string> hn = getNetworkResourceNames(&lpnrLocal[i]);
						// append the vector returned by the recursive call to our vector
						if (!hn.empty())
							for (auto name : hn)
								if (std::find(hostnames.begin(), hostnames.end(), remoteName) == hostnames.end())
									hostnames.push_back(name);
					}
				}
			}
			else if (dwResultEnum != ERROR_NO_MORE_ITEMS)
				break;
		} while (dwResultEnum != ERROR_NO_MORE_ITEMS);

		// free the memory
		GlobalFree((HGLOBAL)lpnrLocal);

		// end enumeration
		dwResult = WNetCloseEnum(hEnum);

		if (dwResult != NO_ERROR)
			return std::vector<std::string>();		// empty vector

		return hostnames;
	}

	std::string wstringToString(const std::wstring &wstr)
	{
		std::string strTo;
		char *szTo = new char[wstr.length() + 1];
		szTo[wstr.size()] = '\0';
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
		strTo = szTo;
		delete[] szTo;
		return strTo;
	}
}