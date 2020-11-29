#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <stdio.h>
#include <iostream>
#include <vector>
// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

volatile bool m_Working = true;

int main()
{
    //----------------------------------------
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult = 0;
    BOOL bRetVal = FALSE;

    HANDLE hCompPort;
    HANDLE hCompPort2;
    
    LPFN_ACCEPTEX lpfnAcceptEx = NULL;
    GUID GuidAcceptEx = WSAID_ACCEPTEX;
    WSAOVERLAPPED olOverlap;
    WSAOVERLAPPED tempoverlap;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET AcceptSocket = INVALID_SOCKET;

    std::vector<SOCKET> m_ConnectList;
    
    
    sockaddr_in service;
    char lpOutputBuf[1024];
    int outBufLen = 1024;
    DWORD dwBytes;
    DWORD recivedata = 0;


    char m_receiveBuffer[1024];

    hostent* thisHost;
    char* ip;
    u_short port;
    


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"Error at WSAStartup\n");
        return 1;
    }

    // Create a handle for the completion port
    hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long)0, 1);
    if (hCompPort == NULL) {
        wprintf(L"CreateIoCompletionPort failed with error: %u\n",
            GetLastError());
        WSACleanup();
        return 1;
    }

    // Create a listening socket
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        wprintf(L"Create of ListenSocket socket failed with error: %u\n",
            WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Associate the listening socket with the completion port
    CreateIoCompletionPort((HANDLE)ListenSocket, hCompPort, (u_long)0, 1);

    //----------------------------------------
    // Bind the listening socket to the local IP address
    // and port 27015
    port = 7272;
    thisHost = gethostbyname("127.0.0.1");
   
    ip = inet_ntoa(*(struct in_addr*)*thisHost->h_addr_list);

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(ip);
    //service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(port);

   /* sockaddr_in m_Endpoint;
    m_Endpoint.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &m_Endpoint.sin_addr);
    m_Endpoint.sin_port = htons(port);
*/

    if (bind(ListenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        wprintf(L"bind failed with error: %u\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    //----------------------------------------
    // Start listening on the listening socket
    iResult = listen(ListenSocket, 100);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"listen failed with error: %u\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

   // wprintf(L"Listening on address: %s:%d\n", ip, port);

    // Load the AcceptEx function into memory using WSAIoctl.
    // The WSAIoctl function is an extension of the ioctlsocket()
    // function that can use overlapped I/O. The function's 3rd
    // through 6th parameters are input and output buffers where
    // we pass the pointer to our AcceptEx function. This is used
    // so that we can call the AcceptEx function directly, rather
    // than refer to the Mswsock.lib library.
    iResult = WSAIoctl(ListenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
        &GuidAcceptEx, 
        sizeof(GuidAcceptEx),
        &lpfnAcceptEx, 
        sizeof(lpfnAcceptEx),
        &dwBytes,
        NULL, 
        NULL);

    if (iResult == SOCKET_ERROR) {
        wprintf(L"WSAIoctl failed with error: %u\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Create an accepting socket
    AcceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (AcceptSocket == INVALID_SOCKET) {
        wprintf(L"Create accept socket failed with error: %u\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Empty our overlapped structure and accept connections.
    memset(&olOverlap, 0, sizeof(olOverlap));
    ZeroMemory(&tempoverlap, sizeof(tempoverlap));
    
    char ignored[200];
    DWORD ignored2 = 0;

    //bRetVal = lpfnAcceptEx(ListenSocket, 
    //    AcceptSocket, 
    //    lpOutputBuf,
    //    outBufLen - ((sizeof(sockaddr_in) + 16) * 2),
    //    sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
    //    &dwBytes, 
    //    &olOverlap);
    bRetVal = lpfnAcceptEx(ListenSocket, 
        AcceptSocket, 
        &ignored,
        0,
        50,
        50,
        &dwBytes, 
        &olOverlap);

  

    // if (bRetVal == FALSE) 기존 코드임 
    // 실행시 bRetVal 은 false 에러 처리가 책 예제의 경우 false 이면 true 가 되도록 구현되어 있음
    // 게임서버 예제에선 WSAGetLastError() 도 997이 뜨는 것을 확인
    //  ERROR_IO_PENDING 이 아닐 경우 에러 처리로 되어 있음을 확인
    if (!bRetVal == FALSE) {
        wprintf(L"AcceptEx failed with error: %u\n", WSAGetLastError());
        closesocket(AcceptSocket);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Associate the accept socket with the completion port
    hCompPort2 = CreateIoCompletionPort((HANDLE)AcceptSocket, hCompPort, (u_long)0, 1);
    // hCompPort2 should be hCompPort if this succeeds
    if (hCompPort2 == NULL) {
        wprintf(L"CreateIoCompletionPort associate failed with error: %u\n",
            GetLastError());
        closesocket(AcceptSocket);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Continue on to use send, recv, TransmitFile(), etc.,.
    //...
    int eventsize = 0;
    PULONG size = new ULONG(0);
    int eventcount = 0;
    //https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-error-codes-2 
    // 소켓 오류 코드
    while (m_Working)
    {
        OVERLAPPED_ENTRY m_events[10];

        //BOOL r = GetQueuedCompletionStatusEx(hCompPort, m_events, 10, size, 100, TRUE);
        BOOL r = GetQueuedCompletionStatusEx(hCompPort, m_events, 10, (PULONG)&eventcount, 100, FALSE);
        //PULONG 으로 접근시 오는 값은 1임 결과에 따라 값을 초기화 해주는것으로 보임 false 이면 일단 실패
        // GetQueuedCompletionStatusEx 스펙상 기본이 1 인것으로 보임 
        if (!r)
        {
            eventcount = 0;
        }
        //else
        //{
        //    //클라이언트 연결 확인
        //    eventcount = (int)(*size);
        //}

        int errcode = GetLastError();
        std::cout << "working... :" << eventcount << std::endl;

        for (size_t i = 0; i < eventcount; i++)
        {
            // 리슨 소켓일때
            std::cout << "lpCompletionKey : " << m_events[i].lpCompletionKey << std::endl;

            if (m_events[i].lpCompletionKey == 0)
            {
                std::cout << "Connect!!" << std::endl;
                //SOCKET m_accept = AcceptSocket;

                sockaddr_in ignore1;
                sockaddr_in ignore3;
                INT ignore2, ignore4;

                char ignore[1000];
                GetAcceptExSockaddrs(ignore,
                    0,
                    50,
                    50,
                    (sockaddr**)&ignore1,
                    &ignore2,
                    (sockaddr**)&ignore3,
                    &ignore4);

                int optresult = setsockopt(AcceptSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
                    (char*)&ListenSocket, sizeof(ListenSocket));

                if (optresult == 0)
                {
                    
                    //m_ConnectList.push_back(m_accept);
                    WSABUF b;
                    b.buf = m_receiveBuffer;
                    b.len = 1024;

                    //// overlapped I/O가 진행되는 동안 여기 값이 채워집니다.
                    int recvresult = WSARecv(AcceptSocket, &b, 1, NULL, &recivedata, &tempoverlap, NULL);
                    std::cout << "recde result :" << recvresult << std::endl;
                    int wsarecverror = WSAGetLastError();
                    std::cout << "error code " << wsarecverror << std::endl;


                }
                else
                {

                }  
            }
            else
            {
                std::cout << "recv" << std::endl;
               /* CHAR* m_receiveBuffer[1024];

                WSABUF b;*/
                //b.buf = m_receiveBuffer;
                //b.len = MaxReceiveLength;

                //// overlapped I/O가 진행되는 동안 여기 값이 채워집니다.
                //m_readFlags = 0;

                //WSARecv(m_accept, &b, 1, NULL, &m_readFlags, &m_readOverlappedStruct, NULL);

                std::cout << "Socket recive Data :" << m_receiveBuffer << std::endl;
            }
        }
    }

    return 0;
}
