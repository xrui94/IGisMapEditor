#pragma once
#include "GeoPoint.h"

class _declspec(dllexport) CatalogTable
{
public:
	CatalogTable(){tableType=-1;tableName=ID=L"";}
	int tableType;
	CString tableName;
	CString ID;
};


class _declspec(dllexport)gisPoint
{
public:
	CString pType;
	DPoint point;
	int color;
	int size;
	CString ID;
};

class _declspec(dllexport)gisLine
{
public:
	gisLine(int pNum){pCount=pNum;LinePoint=new DPoint[pCount];xMin=yMin=xMax=yMax=0;}
	gisLine(gisLine &l)
	{
		this->lType=l.lType;
		this->ID=l.ID;
		this->pCount=l.pCount;
		this->width=l.width;
		this->color=l.color;
		this->xMin=l.xMin;
		this->xMax=l.xMax;
		this->yMin=l.yMin;
		this->yMax=l.yMax;
		this->LinePoint=new DPoint [pCount];
		int i=0;
		while(i<pCount)
		{
			LinePoint[i]=l.LinePoint[i];
			i++;
		}
	}
	~gisLine(){if(LinePoint!=NULL) delete [] LinePoint;LinePoint=NULL;}
	gisLine(){LinePoint=NULL;}
	void operator =(gisLine&gl)
	{
		if(LinePoint!=NULL)
			delete [] LinePoint;
		this->pCount=gl.pCount;
		this->ID=gl.ID;
		this->color=gl.color;
		this->lType=gl.lType;
		this->width=gl.width;
		this->xMax=gl.xMax;
		this->xMin=gl.xMin;
		this->yMin=gl.yMin;
		this->yMax=gl.yMax;
		LinePoint = new DPoint[this->pCount];
		for(int i=0;i<this->pCount;i++)
			this->LinePoint[i]=gl.LinePoint[i];
	}
	int lType;
	CString ID;
	DPoint * LinePoint;
	int color;
	int width;
	int pCount;
	double xMin;
	double yMin;
	double xMax;
	double yMax;
};

class _declspec(dllexport)gisPolygon
{
public:
	gisPolygon(int count){pCount=count;point=new DPoint[pCount];}
	gisPolygon(gisPolygon &l)
	{
		this->BorderType=l.BorderType;
		this->ID=l.ID;
		this->pCount=l.pCount;
		this->color=l.color;
		this->BorderColor=l.BorderColor;
		this->xMin=l.xMin;
		this->xMax=l.xMax;
		this->yMin=l.yMin;
		this->yMax=l.yMax;
		this->point=new DPoint [pCount];
		int i=0;
		while(i<pCount)
		{
			point[i]=l.point[i];
			i++;
		}
	}
	~gisPolygon(){if(point!=NULL){delete [] point;point=NULL;}}
	gisPolygon(){point=NULL;}
	void operator =(gisPolygon&gp)
	{
		if(point!=NULL)
			delete [] point;
		this->pCount=gp.pCount;
		this->ID=gp.ID;
		this->BorderColor=gp.BorderColor;
		this->BorderType=gp.BorderType;
		this->color=gp.color;
		this->xMax=gp.xMax;
		this->xMin=gp.xMin;
		this->yMin=gp.yMin;
		this->yMax=gp.yMax;
		point = new DPoint[gp.pCount];
		for(int i=0;i<this->pCount;i++)
			this->point[i]=gp.point[i];
	}
	int pCount;
	CString ID;
	DPoint *point;
	int BorderType;
	int color;
	int BorderColor;
	double xMin;
	double yMin;
	double xMax;
	double yMax;
};


class _declspec(dllexport)gisMarker
{
public:
	DPoint point;
	int color;
	CString text;
	CString ID;
	LOGFONT lfont;
};