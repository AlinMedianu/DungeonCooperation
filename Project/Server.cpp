//#include <winsock2.h>
//#include <thread>
//#include <bit>
//
//unsigned __stdcall ReceiveFunction(void *pArguments)
//{
//	WSADATA WsaDat;
//	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
//	{
//		WSACleanup();
//		return 0;
//	}
//	SOCKET Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//	if (Socket == INVALID_SOCKET)
//	{
//		WSACleanup();
//		return 0;
//	}
//	SOCKADDR_IN serverInf;
//	serverInf.sin_family = AF_INET;
//	serverInf.sin_addr.s_addr = htonl(INADDR_ANY);
//	serverInf.sin_port = htons(8888);
//	if (bind(Socket, (SOCKADDR*)(&serverInf), sizeof(serverInf)) == SOCKET_ERROR)
//	{
//		WSACleanup();
//		return 0;
//	}
//	bool finished = false;
//	DWORD timeout(3000);
//	setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&timeout), sizeof(DWORD));
//	while (!finished)
//	{
//		const int buffer_size = 1024;
//		char buffer[buffer_size];		
//		SOCKADDR client_address{};
//		int client_address_size = sizeof(client_address);
//		int bytes_received = recvfrom(Socket, buffer, buffer_size, 0, &client_address, &client_address_size);
//		SOCKADDR_IN clientDebugAddress(std::bit_cast<SOCKADDR_IN>(client_address));
//		if (bytes_received == SOCKET_ERROR)
//		{	
//			WSAGetLastError();
//			std::this_thread::sleep_for(std::chrono::seconds(10));
//		}
//		else
//		{
//			std::string t = buffer;
//			if (t == "quit") finished = true;
//			EnterCriticalSection(&CriticalSection);
//			data_queue.push(t);
//			LeaveCriticalSection(&CriticalSection);
//			std::string data_to_send = "Message Received and Understood.";
//			strcpy(buffer, data_to_send.c_str());
//			sendto(Socket, buffer, buffer_size, 0, &client_address, client_address_size);
//		}
//	}
//	shutdown(Socket, SD_SEND);
//	closesocket(Socket);
//	WSACleanup();
//	_endthreadex(0);
//	return 0;
//}
//
//
//int main()
//{
//	HANDLE hThread;
//	unsigned threadID;
//	InitializeCriticalSectionAndSpinCount(&CriticalSection, 1000);
//	hThread = (HANDLE)_beginthreadex(NULL, 0, &ReceiveFunction, NULL, 0, &threadID);
//	bool done = false;
//	while (!done)
//	{
//		std::string d = "";
//		EnterCriticalSection(&CriticalSection);
//		if (data_queue.size() > 0)
//		{
//			d = data_queue.front();
//			data_queue.pop();
//		}
//		LeaveCriticalSection(&CriticalSection);
//		if (d == "quit") done = true;
//	}
//	WaitForSingleObject(hThread, INFINITE);
//	CloseHandle(hThread);
//	return 0;
//}
