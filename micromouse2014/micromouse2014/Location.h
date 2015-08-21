#pragma once
#ifndef __LOCATION_H__
#define __LOCATION_H__

namespace Data
{
	class Location
	{
	public:
		Location(int xx, int yy){ x = xx; y = yy; }
		int x, y;

		bool operator == (Location loc) { return (x == loc.x && y == loc.y); }

		// publicly accessible goal
		//static Location goal
	};
}
#endif