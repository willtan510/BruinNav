#include "support.h"
#include "provided.h"
#include <string>
using namespace std;


bool operator<(const GeoCoord &a, const GeoCoord &b)
{
	if (a.latitudeText < b.latitudeText)
		return true;
	else if (a.latitudeText > b.latitudeText)
		return false;
	else if (a.latitudeText == b.latitudeText)
	{
		if (a.longitudeText < b.longitudeText)
			return true;
		else
			return false;
	}
}


bool operator>(const GeoCoord &a, const GeoCoord &b)
{
	if (a.latitudeText > b.latitudeText)
		return true;
	else if (a.latitudeText < b.latitudeText)
		return false;
	else if (a.latitudeText == b.latitudeText)
	{
		if (a.longitudeText > b.longitudeText)
			return true;
		else
			return false;
	}
}

bool operator==(const GeoCoord &a, const GeoCoord &b)
{
	if (a.latitudeText == b.latitudeText && a.longitudeText == b.longitudeText)
		return true;
	return false;
}