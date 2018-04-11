#include "provided.h"
#include "support.h"
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "MyMap.h"

using namespace std;

bool operator==(const StreetSegment &a, const StreetSegment &b)
{
	if (a.segment.start == b.segment.start && a.segment.end == b.segment.end)
		return true;
	return false;
}

class Node
{
public:
	Node(GeoCoord geo, GeoCoord *endGeo, StreetSegment street, Node* parent)
	{
		m_geo = geo;
		m_endGeo = endGeo;
		m_parent = parent;
		m_street = street;
		m_distance = distanceEarthMiles(geo, *endGeo);
		m_walk = 0;
		if (parent != nullptr)
		{
			m_walk = distanceEarthMiles(geo, parent->m_geo);
			m_cost = m_distance + m_walk;
		}
		else
			m_cost = m_distance;
	}

	GeoCoord m_geo;
	GeoCoord *m_endGeo;
	double m_distance;
	StreetSegment m_street;
	Node* m_parent;
	double m_walk;
	double m_cost;
	Node(const Node &old)
	{
		m_geo = old.m_geo;
		m_endGeo = old.m_endGeo;
		m_parent = old.m_parent;
		m_street = old.m_street;
		m_distance = old.m_distance;
		m_walk = old.m_walk;
		m_cost = old.m_cost;
	}
	Node &operator=(const Node &old)
	{
		m_geo = old.m_geo;
		m_endGeo = old.m_endGeo;
		m_parent = old.m_parent;
		m_street = old.m_street;
		m_distance = old.m_distance;
		m_walk = old.m_walk;
		m_cost = old.m_cost;
		return (*this);
	}

};

class CompareClass
{
public:
	bool operator()(Node a, Node b)
	{
		if (a.m_cost > b.m_cost)
			return true;
		else
			return false;
	}
};

string angleTurn(GeoCoord &start1, GeoCoord &end1, GeoCoord &start2, GeoCoord &end2)
{
	GeoSegment line1(start1, end1);
	GeoSegment line2(start2, end2);
	double angle = angleBetween2Lines(line1, line2);
	if (angle < 180)
		return "left";
	else if (angle >= 180)
		return "right";
}

string angleDirection(GeoCoord &start, GeoCoord &end)
{
	GeoSegment line(start, end);
	double angle = angleOfLine(line);
	int iAngle = (int)(angle / 22.5);
	switch (iAngle)
	{
	case 0:
		return "east";
		break;
	case 1:
	case 2:
		return "northeast";
		break;
	case 3:
	case 4:
		return "north";
		break;
	case 5:
	case 6:
		return "northwest";
		break;
	case 7:
	case 8:
		return "west";
		break;
	case 9:
	case 10:
		return "southwest";
		break;
	case 11:
	case 12:
		return "south";
		break;
	case 13:
	case 14:
		return "southeast";
		break;
	case 15:
	case 16:
		return "east";
		break;
	}

}

int dirToNum(string dir)
{
	if (dir == "north")
		return 0;
	else if (dir == "northeast")
		return 1;
	else if (dir == "east")
		return 2;
	else if (dir == "southeast")
		return 3;
	else if (dir == "south")
		return 4;
	else if (dir == "southwest")
		return 5;
	else if (dir == "west")
		return 6;
	else if (dir == "northwest")
		return 7;
}

string turn2(string curDir, string nextDir)
{
	int cur = dirToNum(curDir);
	int next = dirToNum(nextDir);
	if ((cur - next) > 0)
		return "left";
	else
		return "right";
}

class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
	bool attractionPresent(string attraction);

private:
	MapLoader mapData;
	AttractionMapper AM;
	SegmentMapper SM;
	vector<NavSegment> directions;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	if (mapData.load(mapFile) == true)
	{
		AM.init(mapData);
		SM.init(mapData);
		return true;
	}
	return false;  // This compiles, but may not be correct
}

bool NavigatorImpl::attractionPresent(string attraction)
{
	GeoCoord temp;
	return AM.getGeoCoord(attraction, temp);
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	if (directions.size() != 0)	//ensures vector is empty
		directions.clear();
	priority_queue<Node, vector<Node>, CompareClass> minDist;	//priority queue used to pop top value's geocoord into checked
	MyMap<GeoCoord,int> checked;								//map of checked geocoordinates, used only to ensure a geocoordinate isn't revisted, less big O for find
	stack<Node> closed;											//stack of nodes, used to process top node and hold all processed nodes, we only needed nodes that will be traversed backwards from each parent pointer to get the final route once the destination is found.

	GeoCoord geoStart;
	if(AM.getGeoCoord(start, geoStart)==false)
		return NAV_BAD_SOURCE;	//get geoCoords from starting and ending attractions
	GeoCoord geoEnd;
	if(AM.getGeoCoord(end, geoEnd)==false)
		return NAV_BAD_DESTINATION;
	vector<StreetSegment> endSegs = SM.getSegments(geoEnd);		//gets vector of segments at end geoCoord
	StreetSegment endSeg = endSegs[0];							//should only be one, because coord is in middle of street (attraction)
	vector<StreetSegment> startSeg = SM.getSegments(geoStart);
	Node startNode(geoStart, &geoEnd, startSeg[0], nullptr);	//initialize start node, null parent pointer
	checked.associate(geoStart,0);								//will not reevaluate this geocoordinate
	closed.push(startNode);				

	Node nodeTemp1(startNode.m_street.segment.start, &geoEnd, startNode.m_street, &startNode);	//first two Nodes representing ends of segment the beginning attraction is on
	minDist.push(nodeTemp1);
	Node nodeTemp2(startNode.m_street.segment.end, &geoEnd, startNode.m_street, &startNode);
	minDist.push(nodeTemp2);																	//push both nodes into priority queue, lower cost one will be on top
	int iterations = 0;																			
	double minimum=1000;
	double scale = startNode.m_distance / 10;				//used to determine if there is no route
	for (;;)
	{
		iterations++;
		if (iterations > 1000)
		{
			if (minDist.top().m_distance >= minimum + scale)		//once the loop has run quite long
				return NAV_NO_ROUTE;								//if the lowest recorded distance to the end coord + leeway of distance from start to end divided by 10
		}															//is less than the dist from the top node's coord to the end, there is no route

		closed.push(minDist.top());					//push top Node into closed
		if (minDist.top().m_distance < minimum)
			minimum = minDist.top().m_distance;		//record minimum dist
		minDist.pop();
		checked.associate(closed.top().m_geo, 0);	//push top Node's geocoord into checked
		if (closed.top().m_street == endSeg)		//if the top Node's street segment equals the end node's street segment, end found. Direction vector set up:
		{
			for (int k = 0; k < endSeg.attractions.size(); k++)			
			{
				if (endSeg.attractions[k].name == end)		//iterate across attraction vector of the found end segment
				{
					Node temp(endSeg.attractions[k].geocoordinates, &geoEnd, endSeg, &closed.top());	//push last node onto closed stack, contains end destination coord
					closed.push(temp);
				}
			}
			stack<Node> directList;					//used to reorder the nodes in correct order start->end
			Node* parent = &closed.top();	//use top of stack as the basis for the path
			while (parent != nullptr)
			{
				directList.push(*parent);			//follows the parent of the end node all the way to the start node
				parent = parent->m_parent;
			}
			Node cur = directList.top();			//this would be the start node
			directList.pop();
			Node cur2 = directList.top();			//the node directly after start node, always must exist 2 nodes if found
			directList.pop();
			string dir = angleDirection(cur.m_geo, cur2.m_geo);			//find angle between start coord and the coord after it
			double dist = distanceEarthMiles(cur.m_geo, cur2.m_geo);	//find distance between these two coords
			GeoSegment startSegment(cur.m_geo, cur2.m_geo);				//first segment between the start coord and start or end of the street
			NavSegment startDirection(dir, cur.m_street.streetName, dist, startSegment);	//first NavSegment direction to be stuck into directions vecotr
			directions.push_back(startDirection);
			double totDist = 0;
			string curStreet = cur.m_street.streetName;
			while (directList.size() != 0)	//while still nodes left to act as directions:
			{
				cur = directList.top();			//get next node and pop it
				directList.pop();
				if (cur.m_street.streetName != curStreet)	//a turn necessitates a turn NavSegment and a Proceed NavSegment to be produced
				{
					curStreet = cur.m_street.streetName;
					string turn;
		

					if (cur.m_parent->m_street.segment.start == cur.m_street.segment.end)
					{
						turn = angleTurn(cur.m_parent->m_street.segment.end, cur.m_parent->m_street.segment.start, cur.m_street.segment.end, cur.m_street.segment.start);
					}
					else if (cur.m_parent->m_street.segment.start == cur.m_street.segment.start)
					{
						turn = angleTurn(cur.m_parent->m_street.segment.end, cur.m_parent->m_street.segment.start, cur.m_street.segment.start, cur.m_street.segment.end);
					}
					else if (cur.m_parent->m_street.segment.end==cur.m_street.segment.start)
					{
						turn = angleTurn(cur.m_parent->m_street.segment.start, cur.m_parent->m_street.segment.end, cur.m_street.segment.start, cur.m_street.segment.end);
					}
					else if (cur.m_parent->m_street.segment.end == cur.m_street.segment.end)
					{
						turn = angleTurn(cur.m_parent->m_street.segment.start, cur.m_parent->m_street.segment.end, cur.m_street.segment.end, cur.m_street.segment.start);
					}

					NavSegment NS(turn, cur.m_street.streetName);
					directions.push_back(NS);

					dir = angleDirection(cur.m_parent->m_geo, cur.m_geo);
					dist = distanceEarthMiles(cur.m_geo, cur.m_parent->m_geo);
					totDist += dist;
					GeoSegment segment(cur.m_parent->m_geo, cur.m_geo);					//parent is always the geocoord before the current one
					NavSegment NS1(dir, cur.m_street.streetName, dist, segment);
					directions.push_back(NS1);
					if (directList.size() != 0)
					{
						cur = directList.top();
						directList.pop();
					}
				}
				if (cur.m_parent != nullptr)
				{
					if (cur.m_street.streetName==cur.m_parent->m_street.streetName) 		
						//if the cur and parent streetname is the same, it's a proceed navsegment
					{
						dir = angleDirection(cur.m_parent->m_geo, cur.m_geo);
						dist = distanceEarthMiles(cur.m_geo, cur.m_parent->m_geo);
						totDist += dist;
						GeoSegment segment(cur.m_parent->m_geo, cur.m_geo);					//parent is always the geocoord before the current one
						NavSegment NS(dir, cur.m_street.streetName, dist, segment);
						directions.push_back(NS);
					}
				}
			}
			return NAV_SUCCESS;		//once there's no more nodes to be used as directions, return success
		}
		
		vector<StreetSegment> segs = SM.getSegments(closed.top().m_geo);	//get the streetsegments at given geocoord

		for (int i = 0; i < segs.size(); i++)	//for every streetsegment at geo coord
		{
			if (closed.top().m_geo == segs[i].segment.start && checked.find(segs[i].segment.end)==nullptr)	//if the current geo coord equals the start of the current segment
			{																								//and this coord hasn't been inspected yet
				Node temp(segs[i].segment.end,&geoEnd, segs[i], &closed.top());			//creates node with geocoord of the end of the segment, parent is the Node on top of closed staack
				minDist.push(temp);														//this node pushed into priority queue
			}
			if (closed.top().m_geo == segs[i].segment.end && checked.find(segs[i].segment.start)==nullptr)
			{
				Node temp(segs[i].segment.start, &geoEnd, segs[i], &closed.top());		//same with start coord of the segment
				minDist.push(temp);
			}
		}
	}
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
	m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
	delete m_impl;
}

bool Navigator::attractionPresent(string attraction)
{
	return m_impl->attractionPresent(attraction);
}

bool Navigator::loadMapData(string mapFile)
{
	return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
	return m_impl->navigate(start, end, directions);
}
