#include "CServer.h"

bool CServer::m_WorkingSignal;

//SHORT   gHeight = 80;
void static TEST22()
{
	cout << "TEST..." << endl;
}
void CServer::Listen(const char* ip, const uint16_t port)
{
	m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_Socket == -1)
	{
		std::cout << "Server Socket Error" << std::endl;
		return;
	}
	std::cout << "Server Socket Succece" << std::endl;

	sockaddr_in m_Endpoint;
	m_Endpoint.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &m_Endpoint.sin_addr);
	m_Endpoint.sin_port = htons(port);

	int bindresult = bind(m_Socket, (sockaddr*)&m_Endpoint, sizeof(m_Endpoint));
	if (bindresult < 0)
	{
		std::cout << "Bind Failed" << std::endl;
		return;
	}

	listen(m_Socket, 5000);
	std::cout << "SERVER Listen" << std::endl;
	m_WorkingSignal = true;
	m_ListenThread = thread(
		[&]() {
		Accept();
		});
	m_ListenThread.join();

}

void CServer::Accept()
{
	std::cout << "Accept Start !!" << std::endl;
	while (m_WorkingSignal)
	{
		SOCKET acceptsocket = accept(m_Socket, NULL, 0);
		if (acceptsocket == -1)
		{
			std::cout << "Accept Socket Error" << std::endl;
			return;
		}
		m_ClientCount++;

		std::cout << "Accept !!!" << std::endl;
		Client* client = new  Client;
		client->m_Socket = acceptsocket;
		client->num = m_ClientCount;
		client->m_Thread = thread(ClientWorkThread, client);
		
		//client->m_Thread.join();
		m_AcceptSocketList.push_back(client);
	}	
}

void CServer::StaticTest(int num)
{
	cout << "StaticTest : " << num << endl;

}

bool CServer::Login(string name, string ip)
{
	//TODO :: Find User from DB
	//TODO :: Check New User
	//
	bool newuser = false;
	//string local = FindLocalByIP(ip);

	//UserInfo* user;
	//newuser = DB_FindUser(name, user);
	//if (newuser)
	//{
	//	user = new UserInfo(name, false, ip ,"Location Test");
	//}
	//
	////TODO :: watch out CriticalSection
	//m_UserList.push_back(user);

	return true;
}

bool CServer::DB_FindUser(string name, UserInfo* user)
{
	bool result = false;


	return result;
}

void  CServer::ClientWorkThread(Client* client)
{
	char m_receiveBuffer[MaxReceiveLength];

	while (m_WorkingSignal)
	{
		string receivedData;
		int result = (int)recv(client->m_Socket, m_receiveBuffer, MaxReceiveLength, 0);

		if (result == 0)
		{
			cout << client->num << "Connection closed.\n";
			break;
		}
		else if (result < 0)
		{
			cout << client->num << " : result zero.\n";
			break;
		}
		
		//string str = (string)client->num + "Received =" + m_receiveBuffer;

		cout << client->num << " : Received = " << m_receiveBuffer << endl;
	}
	closesocket(client->m_Socket);
}
string CServer::GetAcceptIP(const SOCKET& acceptsocket)
{
	sockaddr_in ret;
	ret.sin_family = AF_INET;
	socklen_t retLength = sizeof(ret);

	std::cout << "retLength  : " << retLength << std::endl;

	getpeername(acceptsocket, (sockaddr*)&ret, &retLength);

	char addrString[1000];
	addrString[0] = 0;
	inet_ntop(AF_INET, &ret.sin_addr, addrString, sizeof(addrString) - 1);

	char finalString[1000];
	//sprintf(finalString, "%s:%d", addrString, htons(ret.sin_port));
	sprintf_s(finalString, "%s:%d", addrString, htons(ret.sin_port));
	std::cout << "Socket from  : " << finalString << std::endl;
	string result = finalString;

	return result;

}


void CServer::ShowData()
{
	//char m_receiveBuffer[MaxReceiveLength];

	//while (m_WorkingSignal)
	//{
	//	for (size_t i = 0; i < m_AcceptSocketList.size(); i++)
	//	{
	//		string receivedData;
	//		cout << "Receiving data...\n";
	//		int result = (int)recv(m_AcceptSocketList[i], m_receiveBuffer, MaxReceiveLength, 0);

	//		if (result == 0)
	//		{
	//			cout << "Connection closed.\n";
	//			break;
	//		}
	//		else if (result < 0)
	//		{
	//			cout << "Connect lost: " << endl;
	//			closesocket(m_AcceptSocketList[i]);
	//			m_AcceptSocketList.erase(m_AcceptSocketList.begin()+i);
	//			break;
	//		}
	//		// 수신된 데이터를 꺼내서 출력합니다. 송신자는 "hello\0"을 보냈으므로 sz가 있음을 그냥 믿고 출력합니다.
	//		// (실전에서는 클라이언트가 보낸 데이터는 그냥 믿으면 안됩니다. 그러나 여기는 독자의 이해를 위해 예외로 두겠습니다.)
	//		cout << "Received: " << m_receiveBuffer << endl;
	//	}
	//}
}


void CServer::DisConnect()
{
	cout << "Server DisConnect" << endl;
	closesocket(m_Socket);
	WSACleanup();
}
CServer::CServer()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err = 0;

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		//WSAStartup Error
		std::cout << "WSAStartup failed with error : " << err << std::endl;
		return;
	}

//	SQLHENV     hEnv = NULL;
//	SQLHDBC     hDbc = NULL;
//	SQLHSTMT    hStmt = NULL;
//	WCHAR* pwszConnStr;
//	WCHAR       wszInput[SQL_QUERY_SIZE];
//
//	// Allocate an environment
//
//	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) == SQL_ERROR)
//	{
//		fwprintf(stderr, L"Unable to allocate an environment handle\n");
//		exit(-1);
//	}
//
//	TRYODBC(hEnv,
//		SQL_HANDLE_ENV,
//		SQLSetEnvAttr(hEnv,
//			SQL_ATTR_ODBC_VERSION,
//			(SQLPOINTER)SQL_OV_ODBC3,
//			0));
//	TRYODBC(hEnv,
//		SQL_HANDLE_ENV,
//		SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc));
//
//
//
//Exit:
//
//	// Free ODBC handles and exit
//
//	if (hStmt)
//	{
//		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
//	}
//
//	if (hDbc)
//	{
//		SQLDisconnect(hDbc);
//		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
//	}
//
//	if (hEnv)
//	{
//		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
//	}
//
//	wprintf(L"\nDisconnected.");

}
CServer::~CServer()
{
	
	//delete m_ListenThread;
}


void HandleDiagnosticRecord(SQLHANDLE      hHandle,
	SQLSMALLINT    hType,
	RETCODE        RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	WCHAR       wszMessage[1000];
	WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];


	if (RetCode == SQL_INVALID_HANDLE)
	{
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}

	while (SQLGetDiagRec(hType,
		hHandle,
		++iRec,
		wszState,
		&iError,
		wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)),
		(SQLSMALLINT*)NULL) == SQL_SUCCESS)
	{
		// Hide data truncated..
		if (wcsncmp(wszState, L"01004", 5))
		{
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
}



void DisplayResults(HSTMT       hStmt,
	SQLSMALLINT cCols)
{
	BINDING* pFirstBinding, * pThisBinding;
	SQLSMALLINT     cDisplaySize;
	RETCODE         RetCode = SQL_SUCCESS;
	int             iCount = 0;

	// Allocate memory for each column

	AllocateBindings(hStmt, cCols, &pFirstBinding, &cDisplaySize);

	// Set the display mode and write the titles

	DisplayTitles(hStmt, cDisplaySize + 1, pFirstBinding);


	// Fetch and display the data

	bool fNoData = false;

	do {
		// Fetch a row

		if (iCount++ >= gHeight - 2)
		{
			int     nInputChar;
			bool    fEnterReceived = false;

			while (!fEnterReceived)
			{
				wprintf(L"              ");
				SetConsole(cDisplaySize + 2, TRUE);
				wprintf(L"   Press ENTER to continue, Q to quit (height:%hd)", gHeight);
				SetConsole(cDisplaySize + 2, FALSE);

				nInputChar = _getch();
				wprintf(L"\n");
				if ((nInputChar == 'Q') || (nInputChar == 'q'))
				{
					goto Exit;
				}
				else if ('\r' == nInputChar)
				{
					fEnterReceived = true;
				}
				// else loop back to display prompt again
			}

			iCount = 1;
			DisplayTitles(hStmt, cDisplaySize + 1, pFirstBinding);
		}

		TRYODBC(hStmt, SQL_HANDLE_STMT, RetCode = SQLFetch(hStmt));

		if (RetCode == SQL_NO_DATA_FOUND)
		{
			fNoData = true;
		}
		else
		{

			// Display the data.   Ignore truncations

			for (pThisBinding = pFirstBinding;
				pThisBinding;
				pThisBinding = pThisBinding->sNext)
			{
				if (pThisBinding->indPtr != SQL_NULL_DATA)
				{
					wprintf(pThisBinding->fChar ? DISPLAY_FORMAT_C : DISPLAY_FORMAT,
						PIPE,
						pThisBinding->cDisplaySize,
						pThisBinding->cDisplaySize,
						pThisBinding->wszBuffer);
				}
				else
				{
					wprintf(DISPLAY_FORMAT_C,
						PIPE,
						pThisBinding->cDisplaySize,
						pThisBinding->cDisplaySize,
						L"<NULL>");
				}
			}
			wprintf(L" %c\n", PIPE);
		}
	} while (!fNoData);

	SetConsole(cDisplaySize + 2, TRUE);
	wprintf(L"%*.*s", cDisplaySize + 2, cDisplaySize + 2, L" ");
	SetConsole(cDisplaySize + 2, FALSE);
	wprintf(L"\n");

Exit:
	// Clean up the allocated buffers

	while (pFirstBinding)
	{
		pThisBinding = pFirstBinding->sNext;
		free(pFirstBinding->wszBuffer);
		free(pFirstBinding);
		pFirstBinding = pThisBinding;
	}
}

void AllocateBindings(HSTMT         hStmt,
	SQLSMALLINT   cCols,
	BINDING** ppBinding,
	SQLSMALLINT* pDisplay)
{
	SQLSMALLINT     iCol;
	BINDING* pThisBinding, * pLastBinding = NULL;
	SQLLEN          cchDisplay, ssType;
	SQLSMALLINT     cchColumnNameLength;

	*pDisplay = 0;

	for (iCol = 1; iCol <= cCols; iCol++)
	{
		pThisBinding = (BINDING*)(malloc(sizeof(BINDING)));
		if (!(pThisBinding))
		{
			fwprintf(stderr, L"Out of memory!\n");
			exit(-100);
		}

		if (iCol == 1)
		{
			*ppBinding = pThisBinding;
		}
		else
		{
			pLastBinding->sNext = pThisBinding;
		}
		pLastBinding = pThisBinding;


		// Figure out the display length of the column (we will
		// bind to char since we are only displaying data, in general
		// you should bind to the appropriate C type if you are going
		// to manipulate data since it is much faster...)

		TRYODBC(hStmt,
			SQL_HANDLE_STMT,
			SQLColAttribute(hStmt,
				iCol,
				SQL_DESC_DISPLAY_SIZE,
				NULL,
				0,
				NULL,
				&cchDisplay));


		// Figure out if this is a character or numeric column; this is
		// used to determine if we want to display the data left- or right-
		// aligned.

		// SQL_DESC_CONCISE_TYPE maps to the 1.x SQL_COLUMN_TYPE.
		// This is what you must use if you want to work
		// against a 2.x driver.

		TRYODBC(hStmt,
			SQL_HANDLE_STMT,
			SQLColAttribute(hStmt,
				iCol,
				SQL_DESC_CONCISE_TYPE,
				NULL,
				0,
				NULL,
				&ssType));

		pThisBinding->fChar = (ssType == SQL_CHAR ||
			ssType == SQL_VARCHAR ||
			ssType == SQL_LONGVARCHAR);

		pThisBinding->sNext = NULL;

		// Arbitrary limit on display size
		if (cchDisplay > DISPLAY_MAX)
			cchDisplay = DISPLAY_MAX;

		// Allocate a buffer big enough to hold the text representation
		// of the data.  Add one character for the null terminator

		pThisBinding->wszBuffer = (WCHAR*)malloc((cchDisplay + 1) * sizeof(WCHAR));

		if (!(pThisBinding->wszBuffer))
		{
			fwprintf(stderr, L"Out of memory!\n");
			exit(-100);
		}

		// Map this buffer to the driver's buffer.   At Fetch time,
		// the driver will fill in this data.  Note that the size is
		// count of bytes (for Unicode).  All ODBC functions that take
		// SQLPOINTER use count of bytes; all functions that take only
		// strings use count of characters.

		TRYODBC(hStmt,
			SQL_HANDLE_STMT,
			SQLBindCol(hStmt,
				iCol,
				SQL_C_TCHAR,
				(SQLPOINTER)pThisBinding->wszBuffer,
				(cchDisplay + 1) * sizeof(WCHAR),
				&pThisBinding->indPtr));


		// Now set the display size that we will use to display
		// the data.   Figure out the length of the column name

		TRYODBC(hStmt,
			SQL_HANDLE_STMT,
			SQLColAttribute(hStmt,
				iCol,
				SQL_DESC_NAME,
				NULL,
				0,
				&cchColumnNameLength,
				NULL));

		pThisBinding->cDisplaySize = max((SQLSMALLINT)cchDisplay, cchColumnNameLength);
		if (pThisBinding->cDisplaySize < NULL_SIZE)
			pThisBinding->cDisplaySize = NULL_SIZE;

		*pDisplay += pThisBinding->cDisplaySize + DISPLAY_FORMAT_EXTRA;

	}

	return;

Exit:

	exit(-1);

	return;
}

void DisplayTitles(HSTMT     hStmt,
	DWORD     cDisplaySize,
	BINDING* pBinding)
{
	WCHAR           wszTitle[DISPLAY_MAX];
	SQLSMALLINT     iCol = 1;

	SetConsole(cDisplaySize + 2, TRUE);

	for (; pBinding; pBinding = pBinding->sNext)
	{
		TRYODBC(hStmt,
			SQL_HANDLE_STMT,
			SQLColAttribute(hStmt,
				iCol++,
				SQL_DESC_NAME,
				wszTitle,
				sizeof(wszTitle), // Note count of bytes!
				NULL,
				NULL));

		wprintf(DISPLAY_FORMAT_C,
			PIPE,
			pBinding->cDisplaySize,
			pBinding->cDisplaySize,
			wszTitle);
	}

Exit:

	wprintf(L" %c", PIPE);
	SetConsole(cDisplaySize + 2, FALSE);
	wprintf(L"\n");

}

void SetConsole(DWORD dwDisplaySize,
	BOOL  fInvert)
{
	HANDLE                          hConsole;
	CONSOLE_SCREEN_BUFFER_INFO      csbInfo;

	// Reset the console screen buffer size if necessary

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hConsole != INVALID_HANDLE_VALUE)
	{
		if (GetConsoleScreenBufferInfo(hConsole, &csbInfo))
		{
			if (csbInfo.dwSize.X < (SHORT)dwDisplaySize)
			{
				csbInfo.dwSize.X = (SHORT)dwDisplaySize;
				SetConsoleScreenBufferSize(hConsole, csbInfo.dwSize);
			}

			gHeight = csbInfo.dwSize.Y;
		}

		if (fInvert)
		{
			SetConsoleTextAttribute(hConsole, (WORD)(csbInfo.wAttributes | BACKGROUND_BLUE));
		}
		else
		{
			SetConsoleTextAttribute(hConsole, (WORD)(csbInfo.wAttributes & ~(BACKGROUND_BLUE)));
		}
	}
}
