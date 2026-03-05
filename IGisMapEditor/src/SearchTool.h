#pragma once

#ifndef SEARCHTOOL_H
#define SEARCHTOOL_H

#include "stdafx.h"
#include "DataTemplate.h"
#include "DataBase.h"
#include "Cordins.h"
#include "math.h"
#include <queue>
using namespace std;

class SearchTool
{
public:
	SearchTool(CView * view,CZoomManager * zoom,DBConnection * db,CString ly,int type)
	{pView=view;pZoom=zoom;pdb=db;layer=ly;this->type=type+1;}
	SearchTool(){}
	virtual void LButtonDown(UINT nflags,CPoint point){}
	virtual void RButtonDown(UINT nflags,CPoint point){}
	virtual void LButtonUp(UINT nflags,CPoint point){}
	virtual void RButtonUp(UINT nflags,CPoint point){}
	virtual void MouseMove(UINT nflags,CPoint point){}
	virtual void SetLayer(CString layer){}
	virtual void SetType(int type){}
	virtual int getCount(){return 0;}
	virtual void getPointSet(gisPoint * gp){}
	virtual void getLineSet(gisLine * gl){}
	virtual void getPolySet(gisPolygon * gl){}
	virtual void getMarkerSet(gisMarker * gm){}
	virtual void setCount(int i){}
private:
	DBConnection * pdb;
	CView *pView;
	CZoomManager * pZoom;
	CString layer;
	int type;
};

class SearchByRect:public SearchTool
{
public:
	SearchByRect(CView * view,CZoomManager * zoom,DBConnection * db,CString ly,int type)
	{pView=view;pZoom=zoom;pdb=db;layer=ly;gp=NULL;gl=NULL;gPoly=NULL;gm=NULL;LBtDown=false;this->type=type+1;count=0;}
	~SearchByRect()
	{
		if(this->gp!=NULL){delete [] this->gp;this->gp=NULL;}
		if(this->gl!=NULL){delete [] this->gl;this->gl=NULL;}
		if(this->gPoly!=NULL){delete [] this->gPoly;this->gPoly=NULL;}
		if(this->gm!=NULL){delete [] this->gm;this->gm=NULL;}
	}
	void LButtonDown(UINT nflags,CPoint point);
	void RButtonDown(UINT nflags,CPoint point);
	void LButtonUp(UINT nflags,CPoint point);
	void RButtonUp(UINT nflags,CPoint point);
	void MouseMove(UINT nflags,CPoint point);
	void SetLayer(CString layer){this->layer=layer;}
	void SetType(int type){this->type=type+1;}
	int getCount(){return count;}
	void getPointSet(gisPoint * gp);
	void getLineSet(gisLine * gl);
	void getPolySet(gisPolygon * gl);
	void getMarkerSet(gisMarker * gm);
	void setCount(int i){count=i;}
private:
	DBConnection * pdb;
	CView *pView;
	CZoomManager * pZoom;
	CPoint lastPoint,clickPoint;
	CString layer;
	int type;
	gisPoint * gp;
	gisLine * gl;
	gisPolygon * gPoly;
	gisMarker * gm;
	bool LBtDown;
	int count;
};

class SearchByCircle:public SearchTool
{
public:
	SearchByCircle(CView * view,CZoomManager * zoom,DBConnection * db,CString ly,int type)
	{pView=view;pZoom=zoom;pdb=db;layer=ly;gp=NULL;gl=NULL;gPoly=NULL;gm=NULL;LBtDown=false;this->type=type+1;count=0;}
	~SearchByCircle()
	{
		if(this->gp!=NULL){delete [] this->gp;this->gp=NULL;}
		if(this->gl!=NULL){delete [] this->gl;this->gl=NULL;}
		if(this->gPoly!=NULL){delete [] this->gPoly;this->gPoly=NULL;}
		if(this->gm!=NULL){delete [] this->gm;this->gm=NULL;}
	}
	void LButtonDown(UINT nflags,CPoint point);
	void RButtonDown(UINT nflags,CPoint point);
	void LButtonUp(UINT nflags,CPoint point);
	void RButtonUp(UINT nflags,CPoint point);
	void MouseMove(UINT nflags,CPoint point);
	void SetLayer(CString layer){this->layer=layer;}
	void SetType(int type){this->type=type+1;}
	int getCount(){return count;}
	void getPointSet(gisPoint * gp);
	void getLineSet(gisLine * gl);
	void getPolySet(gisPolygon * gl);
	void getMarkerSet(gisMarker * gm);
	void setCount(int i){count=i;}
private:
	DBConnection * pdb;
	CView *pView;
	CZoomManager * pZoom;
	CPoint lastPoint,clickPoint;
	CString layer;
	int type;
	gisPoint * gp;
	gisLine * gl;
	gisPolygon * gPoly;
	gisMarker * gm;
	bool LBtDown;
	int count;
};

class SearchByPolygon:public SearchTool
{
public:
	SearchByPolygon(CView * view,CZoomManager * zoom,DBConnection * db,CString ly,int type)
	{pView=view;pZoom=zoom;pdb=db;layer=ly;gp=NULL;gl=NULL;gPoly=NULL;gm=NULL;this->type=type+1;count=0;pCount=0;mouseMove=false;}
	SearchByPolygon()
	{
		if(this->gp!=NULL){delete [] this->gp;this->gp=NULL;}
		if(this->gl!=NULL){delete [] this->gl;this->gl=NULL;}
		if(this->gPoly!=NULL){delete [] this->gPoly;this->gPoly=NULL;}
		if(this->gm!=NULL){delete [] this->gm;this->gm=NULL;}
	}
	void LButtonDown(UINT nflags,CPoint point);
	void RButtonDown(UINT nflags,CPoint point);
	void LButtonUp(UINT nflags,CPoint point);
	void RButtonUp(UINT nflags,CPoint point);
	void MouseMove(UINT nflags,CPoint point);
	void SetLayer(CString layer){this->layer=layer;}
	void SetType(int type){this->type=type+1;}
	int getCount(){return count;}
	void getPointSet(gisPoint * gp);
	void getLineSet(gisLine * gl);
	void getPolySet(gisPolygon * gl);
	void getMarkerSet(gisMarker * gm);
	void setCount(int i){count=i;}
private:
	DBConnection * pdb;
	CView *pView;
	CZoomManager * pZoom;
	CPoint lastmove,firstPoint;
	CString layer;
	int type;
	gisPoint * gp;
	gisLine * gl;
	gisPolygon * gPoly;
	gisMarker * gm;
	CPoint pt[100];
	queue<CPoint> Q;
	int pCount;
	int count;
	bool mouseMove;
};


#endif