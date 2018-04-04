#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "provided.h"

using namespace std;

class MapLoaderImpl
{
public:									//Class parses through mapData.txt to compile a vector of StreetSegments
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;



private:
	vector<StreetSegment*> mapData;
};



MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream file(mapFile);
	if (!file)
		return false;
	string s;
	int count = 0;
	while (getline(file, s))						//Parses through format of mapData.txt to compile necessary geospatial and string data
	{
		mapData.push_back(new StreetSegment());
		mapData[count]->streetName = s;
		getline(file, s);
		string latText;
		int stringIndex = 0;
		for (stringIndex; s[stringIndex] != ','; stringIndex++)
		{
			latText += s[stringIndex];
		}
		stringIndex++;
		while (s[stringIndex] == ' ')
		{
			stringIndex++;
		}
		string longText;
		for (stringIndex; s[stringIndex] != ' '; stringIndex++)
		{
			longText += s[stringIndex];
		}
		mapData[count]->segment.start.latitudeText = latText;
		mapData[count]->segment.start.longitudeText = longText;
		while (s[stringIndex] == ' ')
			stringIndex++;
		latText = "";
		longText = "";
		for (stringIndex; s[stringIndex] != ','; stringIndex++)
		{
			latText += s[stringIndex];
		}
		stringIndex++;
		for (stringIndex; stringIndex<s.size(); stringIndex++)
		{
			longText += s[stringIndex];
		}
		mapData[count]->segment.end.latitudeText = latText;
		mapData[count]->segment.end.longitudeText = longText;

		double value = stod(mapData[count]->segment.start.latitudeText);
		mapData[count]->segment.start.latitude = value;
		value = stod(mapData[count]->segment.start.longitudeText);
		mapData[count]->segment.start.longitude = value;

		value = stod(mapData[count]->segment.end.latitudeText);
		mapData[count]->segment.end.latitude = value;
		value = stod(mapData[count]->segment.end.longitudeText);
		mapData[count]->segment.end.longitude = value;

		getline(file, s);
		int n = stoi(s);
		for (int i = 0; i < n; i++)
		{
			getline(file, s);
			string attraction;
			int ind = 0;
			for (ind; s[ind] != '|'; ind++)
			{
				attraction += s[ind];
			}

			ind++;
			latText = "";
			for (ind; s[ind] != ','; ind++)
			{
				latText += s[ind];
			}
			ind++;
			longText = "";
			while (s[ind] == ' ')
				ind++;
			for (ind; ind<s.size(); ind++)
			{
				longText += s[ind];
			}
			mapData[count]->attractions.push_back(Attraction());
			mapData[count]->attractions[i].name = attraction;
			mapData[count]->attractions[i].geocoordinates.latitudeText = latText;
			mapData[count]->attractions[i].geocoordinates.longitudeText = longText;

			value = stod(mapData[count]->attractions[i].geocoordinates.latitudeText);
			mapData[count]->attractions[i].geocoordinates.latitude = value;
			value = stod(mapData[count]->attractions[i].geocoordinates.longitudeText);
			mapData[count]->attractions[i].geocoordinates.longitude = value;
		}
		count++;

	}

	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return mapData.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < 0 || segNum >= mapData.size())
		return false;
	seg = *mapData[segNum];
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
	return m_impl->getSegment(segNum, seg);
}
