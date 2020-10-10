#pragma once

#define _SQLNCLI_ODBC_
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
//#include <sqlncli.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <conio.h>

#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <thread>
#include "UserInfo.h"


// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib") //없으면 외부 참조 에러 발생됨


#define PACKET_SIZE	1024
#define DISPLAY_MAX 50          // Arbitrary limit on column width to display
#define DISPLAY_FORMAT_EXTRA 3  // Per column extra display bytes (| <data> )
#define DISPLAY_FORMAT      L"%c %*.*s "
#define DISPLAY_FORMAT_C    L"%c %-*.*s "
#define NULL_SIZE           6   // <NULL>
#define SQL_QUERY_SIZE      1000

#define PIPE                L'|'


using namespace std;

#define TRYODBC(h, ht, x)   {   RETCODE rc = x;\
                                if (rc != SQL_SUCCESS) \
                                { \
                                    HandleDiagnosticRecord (h, ht, rc); \
                                } \
                                if (rc == SQL_ERROR) \
                                { \
                                    fwprintf(stderr, L"Error in " L#x L"\n"); \
                                    goto Exit;  \
                                }  \
                            }

struct Client
{
	int num;
	thread m_Thread;
	SOCKET m_Socket;
};

typedef struct STR_BINDING {
	SQLSMALLINT         cDisplaySize;           /* size to display  */
	WCHAR* wszBuffer;             /* display buffer   */
	SQLLEN              indPtr;                 /* size or null     */
	BOOL                fChar;                  /* character col?   */
	struct STR_BINDING* sNext;                 /* linked list      */
} BINDING;

void HandleDiagnosticRecord(SQLHANDLE      hHandle,
	SQLSMALLINT    hType,
	RETCODE        RetCode);

void DisplayResults(HSTMT       hStmt,
	SQLSMALLINT cCols);

void AllocateBindings(HSTMT         hStmt,
	SQLSMALLINT   cCols,
	BINDING** ppBinding,
	SQLSMALLINT* pDisplay);


void DisplayTitles(HSTMT    hStmt,
	DWORD    cDisplaySize,
	BINDING* pBinding);

void SetConsole(DWORD   cDisplaySize,
	BOOL    fInvert);

static SHORT   gHeight = 80;       // Users screen height
class  CServer
{
public:
	static bool m_WorkingSignal;

	void static ClientWorkThread(Client* client);
	void static StaticTest(int);
	bool Login(string name, string ip);
	bool DB_FindUser(string name, UserInfo* user);
	void Listen(const char* ip, const uint16_t port);
	void Accept();
	void DisConnect();
	void ShowData();

	string GetAcceptIP(const SOCKET& acceptsocket);
	
	CServer();
	~CServer();
private:
	static const int MaxReceiveLength = 8192;
	SOCKET m_Socket;

	vector<Client*> m_AcceptSocketList;
	vector<string> m_StringList;
	vector<UserInfo*> m_UserList;

	thread m_ListenThread;
	int m_ClientCount = 0;
};

