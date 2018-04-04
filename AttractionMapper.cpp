#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <string>
using namespace std;

string lower(string s)
{
	for (int i = 0; i < s.size(); i++)
		s[i] = tolower(s[i]);
	return s;
}

class AttractionMapperImpl						//class implementing map of possible attractions(source and destinations)
{									//associates each attraction string to a GeoCoord
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;

private:
	 MyMap<string,GeoCoord> attract;

};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)		//associates each attraction string to its geocoord
{								//from information parsed from MapLoader 
	
	for (int i = 0; i < ml.getNumSegments(); i++)	
	{
		StreetSegment s;
		ml.getSegment(i, s);
		if (s.attractions.size() > 0)
		{
			for (int j = 0; j < s.attractions.size(); j++)
			{
				string lowerS = lower(s.attractions[j].name);
				attract.associate(lowerS, s.attractions[j].geocoordinates);
			}
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	for (int i = 0; i < attraction.size(); i++)
	{
		if (attraction[i]>0 && isalpha(attraction[i]) != 0)
		{
			attraction[i]=tolower(attraction[i]);
		}
	}
	auto i= attract.find(attraction);
	if (i != nullptr)
	{
		gc = *i;
		return true;
	}
	return false;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
