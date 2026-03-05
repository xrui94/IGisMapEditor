#ifndef GEOPOINT_H
#define GEOPOINT_H

struct DPoint
{
	DPoint(double xx,double yy){x=xx;y=yy;}
	DPoint(){}
	bool operator ==(DPoint &p){return p.x==x&&p.y==y;}
	double x;
	double y;
};
#endif