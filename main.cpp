#include "provided.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <queue>
using namespace std;
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

void getInputs(string & source, string & destination, bool & detailed, Navigator & n)
{
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

	
	string details;
	cout << "Would you like a detailed directions?(Answer yes/no)\n";
	getline(cin, details);
	for (int i=0; i < details.size(); i++)
	{
		details[i] = tolower(details[i]);
	}
	if (details == "yes")
		detailed = true;
	else if (details == "no")
		detailed = false;
	else
	{
		cout << "You didn't put yes or no\n";
		exit(1);
	}
	if (source == destination)
	{
		cout << "You've reached your destination!\n";
		return;
	}
}

void direct(const vector<NavSegment> & vn, bool detailed)
{
	std::queue<std::string> directions;
	double rdDist = 0;
	string prevDir = vn[0].m_direction;
	bool justTurned = false;
	if (detailed == false)
	{
		for (int i = 0; i < vn.size(); i++)
		{
			int turn = vn[i].m_command;
			string curStr;
			if (turn == 0)
			{
				string distr = doubleToString(rdDist);
				rdDist += vn[i].m_distance;
			}
			else
			{
				string distr = doubleToString(rdDist);
				curStr = "Proceed on " + vn[i - 1].m_streetName + " for " + distr + " miles\n";
				rdDist = 0;
				directions.push(curStr);
				curStr = "Turn ";
				curStr += vn[i].m_direction + " on " + vn[i].m_streetName + "\n";
				directions.push(curStr);
			}
			if (i == vn.size() - 1)
			{
				string distr = doubleToString(rdDist);
				curStr = "Proceed on " + vn[i].m_streetName + " for " + distr + " miles\nYou've reached your destination!\n";
				directions.push(curStr);
			}
		}
	}
	else {
		for (int i = 0; i < vn.size(); i++)
		{
			int turn = vn[i].m_command;
			string curStr;
			if (turn == 0)
			{
				//rdDist += vn[i].m_distance;
				string curDir = vn[i].m_direction;
				if (vn[i].m_direction == prevDir || justTurned == true)	//check if the previous direction is same as current dirrection, if so, keep incrementing
				{
					rdDist += vn[i].m_distance;
					if (justTurned)
						prevDir = curDir;
				}
				else													//else must output the total distance must travel in this certain direction in 1 line
				{
					string distr = doubleToString(rdDist);
					curStr = "Proceed " + prevDir + " on " + vn[i].m_streetName + " for " + distr + " miles\n";
					rdDist = vn[i].m_distance;
					directions.push(curStr);
					prevDir = curDir;
				}
				justTurned = false;
			}
			else if (turn == 1)											//if turn, must output the direction and distance traveled in 1 line, and output the turn in another line
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

		}
	}
	int queueSize = directions.size();
	for (int i = 0; i < queueSize; i++)
	{
		cout << directions.front();
		directions.pop();
	}
}

int main()
{

	cout << "Loading Map Data\n";

	Navigator n;
	n.loadMapData("mapdata.txt");

	vector<NavSegment> vn;
	string source, destination;
	bool detailed;
	getInputs(source, destination, detailed, n);

	n.navigate(source, destination, vn);
	direct(vn,detailed);
}
