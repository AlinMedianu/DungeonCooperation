//#include <winsock2.h>
//
//int Client()
//{
//	WSADATA WsaDat;
//	if (WSAStartup(MAKEWORD(2,2), &WsaDat) != 0)
//	{
//		WSACleanup();
//		return EXIT_FAILURE;
//	}
//	SOCKET Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//	if (Socket == INVALID_SOCKET)
//	{
//		WSACleanup();
//		return EXIT_FAILURE;
//	}
//	const int buffer_size = 1024;
//	char buffer[1024];
//	SecureZeroMemory(&buffer, buffer_size);
//	struct sockaddr_in server_address;
//	int server_address_size = (int)sizeof(server_address);
//	short port = 8888;
//	const char* server_ip_address = "127.0.0.1";
//	server_address.sin_family = AF_INET;
//	server_address.sin_port = htons(port);
//	server_address.sin_addr.s_addr = inet_addr(server_ip_address);
//	
//	std::cout << "Enter a string to send: ";
//	while (std::cin >> buffer)
//	{
//		int clientResult = sendto(Socket, buffer, buffer_size, 0, (SOCKADDR *)&server_address, server_address_size);
//		int bytes_received = recvfrom(Socket, buffer, buffer_size, 0, (SOCKADDR *)&server_address, &server_address_size);
//		if (bytes_received == SOCKET_ERROR)
//		{	
//			WSAGetLastError();
//		}
//		else
//		{
//			std::string acknowledge = buffer;
//			std::cout << acknowledge.c_str() << std::endl;
//		}
//	}
//	shutdown(Socket, SD_SEND);
//	closesocket(Socket);
//	WSACleanup();
//	return EXIT_SUCCESS;
//}
