#include "iocp_Server.h"

int main()
{

	iocp_Server server = iocp_Server("0.0.0.0", 7272, 1);

	//server.Worker();


	return 0;
}