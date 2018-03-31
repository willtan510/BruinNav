// The main.cpp you can use for testing will replace this file soon.
/*
#if defined(_MSC_VER)  &&  !defined(_DEBUG)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
~KeepWindowOpenUntilDismissed()
{
DWORD pids[1];
if (GetConsoleProcessList(pids, 1) == 1)
{
std::cout << "Press any key to continue . . . ";
_getch();
}
}
} keepWindowOpenUntilDismissed;
#endif
*/
#include "provided.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <queue>

std::string doubleToString(double d)
{
	std::ostringstream streamObj3;
	// Set Fixed -Point Notation
	streamObj3 << std::fixed;
	// Set precision to 2 digits
	streamObj3 << std::setprecision(3);
	//Add double to stream
	streamObj3 << d;
	std::string str = streamObj3.str();
	return str;
}

int main()
{
	using namespace std;
	cout << "Loading Map Data\n";
	std::queue<std::string> directions;
	Navigator n;
	n.loadMapData("mapdata.txt");

	vector<NavSegment> vn;
	string source, destination;
	cout << "Please enter a starting point:\n";
	getline(cin, source);
	if (n.attractionPresent(source) == false)
	{
		cout << "Invalid source\n";
		exit(1);
	}
	cout << "Please enter a destination:\n";
	getline(cin, destination);

	if (n.attractionPresent(destination) == false)
	{
		cout << "Invalid destination\n";
		exit(1);
	}
	n.navigate(source, destination, vn);

	double rdDist = 0;
	string prevDir = vn[0].m_direction;
	bool justTurned = false;
	for (int i = 0; i < vn.size(); i++)
	{
		int turn = vn[i].m_command;
		string curStr;
		if (turn == 0)
		{
			//rdDist += vn[i].m_distance;
			string curDir = vn[i].m_direction;
			if (vn[i].m_direction == prevDir || justTurned == true)
			{
				rdDist += vn[i].m_distance;
				if (justTurned)
					prevDir = curDir;
			}
			else
			{
				string distr = doubleToString(rdDist);
				curStr = "Proceed " + prevDir + " on " + vn[i].m_streetName + " for " + distr + " miles\n";
				rdDist = vn[i].m_distance;
				directions.push(curStr);
				prevDir = curDir;
			}
			justTurned = false;
		}
		else if (turn == 1)
		{
			justTurned = true;
			string distr = doubleToString(rdDist);
			curStr = "Proceed " + prevDir + " on " + vn[i - 1].m_streetName + " for " + distr + " miles\n";
			prevDir = "";
			rdDist = 0;
			directions.push(curStr);
			curStr = "Turn ";
			curStr += vn[i].m_direction + " on " + vn[i].m_streetName + "\n";
			directions.push(curStr);
		}
		if (i == vn.size() - 1)
		{
			string distr = doubleToString(rdDist);
			curStr = "Proceed " + prevDir + " on " + vn[i].m_streetName + " for " + distr + " miles\nYou've reached your destination!\n";
			directions.push(curStr);
		}
		/*curStr+= vn[i].m_direction + " on " + vn[i].m_streetName ;		//detailed version
		*/
	}
	int queueSize = directions.size();
	for (int i = 0; i < queueSize; i++)
	{
		cout << directions.front();
		directions.pop();
	}
}
