#pragma once

#ifndef GRAPHICTOOL_H
#define GRAPHICTOOL_H
//---------------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include "GraphStyles.h"
#include <queue>
using namespace std;
//---------------------------------------------------------------------------------------------------------------
class CBaseGisTool
{
public:
	CBaseGisTool(CView* p=NULL,CStyleManager *pType=NULL,float ntimes=1.0){}
	virtual ~CBaseGisTool(){}
public:
	virtual int LButtonDown(UINT nflags,CPoint point){return 0;}
	virtual int RButtonDown(UINT nflags,CPoint point){return 0;}
	virtual int LButtonUp(UINT nflags,CPoint point){return 0;}
	virtual int RButtonUp(UINT nflags,CPoint point){return 0;}
	virtual int MouseMove(UINT nflags,CPoint point){return 0;}
};

class CPointTool :public CBaseGisTool
{
public:
	CPointTool(CView* p,CStyleManager *pType,float ntimes=1.0)
	{
		pView=p;
		pStyle=pType;
		times=ntimes;
		color=pStyle->ps.color; 
		type=pStyle->ps.type;
		radius=pType->ps.size;
	}
	virtual ~CPointTool(){}
	CView *pView;
	float times;
	int radius;
	CStyleManager* pStyle;
	COLORREF color;
	CString type;
public:
	int LButtonDown(UINT nflags,CPoint point);
	int RButtonDown(UINT nflags,CPoint point);
	int LButtonUp(UINT nflags,CPoint point);
	int RButtonUp(UINT nflags,CPoint point);
	int MouseMove(UINT nflags,CPoint point);
};

class CLineTool:public CBaseGisTool
{
public:
	CLineTool(CView* p,CStyleManager *pType,float ntimes=1.0)
	{
		pView=p;
		pStyle=pType;
		times=ntimes;
		LOGBRUSH logBrush;  
		memset(&logBrush, 0, sizeof(logBrush)); 
		logBrush.lbStyle = BS_SOLID;
		logBrush.lbColor = pStyle->ls.color;
		int nwidth=int(pStyle->ls.width*ntimes);
		if(nwidth==0) nwidth++;
		pen.CreatePen(pStyle->ls.PENTYPE|PS_GEOMETRIC|PS_JOIN_ROUND|PS_ENDCAP_FLAT,nwidth,&logBrush);
		mouseMove=false;
		doing=0;
	}
	virtual ~CLineTool(){}
	CView *pView;
	float times;
	CStyleManager* pStyle;
	vector<CPoint> q;
	CPoint lastmove;
	CPen pen;
	CPen * pOldPen;
	bool mouseMove;
	int mode;
	int doing;
public:
	int LButtonDown(UINT nflags,CPoint point);
	int RButtonDown(UINT nflags,CPoint point);
	int LButtonUp(UINT nflags,CPoint point);
	int RButtonUp(UINT nflags,CPoint point);
	int MouseMove(UINT nflags,CPoint point);
private:
	int PLButtonDown(UINT nflags,CPoint point);
	int PRButtonDown(UINT nflags,CPoint point);
	int PLButtonUp(UINT nflags,CPoint point);
	int PRButtonUp(UINT nflags,CPoint point);
	int PMouseMove(UINT nflags,CPoint point);
	int CLButtonDown(UINT nflags,CPoint point);
	int CRButtonDown(UINT nflags,CPoint point);
	int CLButtonUp(UINT nflags,CPoint point);
	int CRButtonUp(UINT nflags,CPoint point);
	int CMouseMove(UINT nflags,CPoint point);
	CPoint MidPoint(CPoint p1,CPoint p2)
	{
		CPoint p;
		p.x=(p1.x+p2.x)/2;
		p.y=(p1.y+p2.y)/2;
		return p;
	}
};

class CPolyTool:public CBaseGisTool
{
public:
	CPolyTool(CView* p,CStyleManager *pType,float ntimes=1.0)
	{
		pStyle=pType;
		times=ntimes;
		pView=p;
		pen.CreatePen(pStyle->rs.sideType,1,pStyle->rs.sidecolor);
		brush.CreateSolidBrush(pStyle->rs.fillcolor);
		mouseMove=false;
	}
	virtual ~CPolyTool(){}
	CView *pView;
	CStyleManager* pStyle;
	float times;
	CPen pen;
	CPen * pOldPen;
	CBrush brush;
	CBrush * pOldBrush;
	queue<CPoint> Q;
	CPoint lastmove;
	CPoint firstPoint;
	int mode;
	bool mouseMove;
public:
	int LButtonDown(UINT nflags,CPoint point);
	int RButtonDown(UINT nflags,CPoint point);
	int LButtonUp(UINT nflags,CPoint point);
	int RButtonUp(UINT nflags,CPoint point);
	int MouseMove(UINT nflags,CPoint point);
};

class CNoteTool :public CBaseGisTool
{
public:
	CNoteTool(CView* p,CStyleManager *pType,float ntimes=1.0)
	{
		pView=p;
		pStyle=pType;
		times=ntimes;
	}
	CView *pView;
	float times;
	CStyleManager* pStyle;
public:
	int LButtonDown(UINT nflags,CPoint point);
	int RButtonDown(UINT nflags,CPoint point);
	int LButtonUp(UINT nflags,CPoint point);
	int RButtonUp(UINT nflags,CPoint point);
	int MouseMove(UINT nflags,CPoint point);
};

//---------------------------------------------------------------------------------------------------------------
#endif