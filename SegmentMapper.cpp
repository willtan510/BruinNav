#include "provided.h"
#include "MyMap.h"
#include <vector>
#include "support.h"
using namespace std;


class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;

private:
	MyMap<GeoCoord, vector<StreetSegment>> segmentMap;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		StreetSegment s;
		ml.getSegment(i, s);
		GeoCoord geoStart(s.segment.start.latitudeText, s.segment.start.longitudeText);
		auto it = segmentMap.find(geoStart);		//start coord
		if (it == nullptr)					
		{

			vector<StreetSegment> vs;
			vs.push_back(s);
			segmentMap.associate(geoStart, vs);
		}
		else
		{
			it->push_back(s);
		}
		GeoCoord geoEnd(s.segment.end.latitudeText, s.segment.end.longitudeText);
		it = segmentMap.find(geoEnd);			//end coord
		if (it == nullptr)
		{
			vector<StreetSegment> vs;
			vs.push_back(s);
			segmentMap.associate(geoEnd, vs);
		}
		else
		{
			it->push_back(s);
		}
		for (int j = 0; j < s.attractions.size(); j++)
		{
			GeoCoord attractGeo(s.attractions[j].geocoordinates.latitudeText, s.attractions[j].geocoordinates.longitudeText);
			it = segmentMap.find(attractGeo);
			if (it == nullptr)
			{
				vector<StreetSegment> vs;
				vs.push_back(s);
				segmentMap.associate(attractGeo, vs);
			}
			else
			{
				it->push_back(s);
			}
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> vs;
	auto it = segmentMap.find(gc);
	if (it == nullptr)
		return vs;
	return *it;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
