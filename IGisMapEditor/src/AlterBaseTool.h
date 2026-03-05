#pragma once

#ifndef ALTERBASETOOL_H
#define ALTERBASETOOL_H

#include "stdafx.h"
#include "DataTemplate.h"
#include "DataBase.h"
#include "Cordins.h"
#include "math.h"
using namespace std;

class  _declspec(dllexport) AlterBaseTool
{
public:
	AlterBaseTool(CView * view,CZoomManager * zoom,DBConnection * db,CString ly)
	{pView=view;pZoom=zoom;pdb=db;layer=ly;alterType=L"";}
	AlterBaseTool(){}
	virtual void LButtonDown(UINT nflags,CPoint point){}
	virtual void RButtonDown(UINT nflags,CPoint point){}
	virtual void LButtonUp(UINT nflags,CPoint point){}
	virtual void RButtonUp(UINT nflags,CPoint point){}
	virtual void MouseMove(UINT nflags,CPoint point){}
	virtual void flash(CDC* pDC){}
	virtual void endflash(CDC* pDC){}
	virtual void InitlizeObject(CDC* pDC){}
	virtual bool getHasInit(){return false;}
	virtual void setInit(bool bl){}
	virtual void setAlterType(CString type){}
	virtual bool hasFound(){return false;}
	virtual gisPoint  getPoint(){return gisPoint();}
	virtual void  getLine(gisLine&gl){}
	virtual void  getPoly(gisPolygon&gp){}
	virtual void  getMarker(gisMarker&gm){}
	virtual CString getAlterType(){return alterType;}
private:
	DBConnection * pdb;
	CView *pView;
	CZoomManager * pZoom;
	CString layer;
	CString alterType;
};

class  _declspec(dllexport) AlterPointTool:public AlterBaseTool
{
public:
	AlterPointTool(CView * view,CZoomManager * zoom,DBConnection * db,CString ly)
	{pView=view;pZoom=zoom;pdb=db;layer=ly;hasInitlize=false;this->hasMove=false;LBtDown=false;alterType=L"";found=false;}
	void LButtonDown(UINT nflags,CPoint point);
	void RButtonDown(UINT nflags,CPoint point);
	void LButtonUp(UINT nflags,CPoint point);
	void RButtonUp(UINT nflags,CPoint point);
	void MouseMove(UINT nflags,CPoint point);
	void flash(CDC* pDC);
	void endflash(CDC* pDC);
	void InitlizeObject(CDC* pDC);
	bool getHasInit(){return this->hasInitlize;}
	void setInit(bool bl){this->hasInitlize=bl;}
	void setAlterType(CString type){alterType=type;}
	bool hasFound(){return found;}
	CString getAlterType(){return alterType;}
	gisPoint  getPoint(){return this->gp;}
private:
	DBConnection * pdb;
	CView *pView;
	CZoomManager * pZoom;
	CString layer;
	bool hasInitlize;
	bool LBtDown;
	bool hasMove;
	bool found;
	CString alterType;
	CPoint lastPoint;
	gisPoint gp;
	gisPoint tempgp;
};

class  _declspec(dllexport) AlterLineTool:public AlterBaseTool
{
public:
	AlterLineTool(CView * view,CZoomManager * zoom,DBConnection * db,CString ly)
	{pView=view;pZoom=zoom;pdb=db;layer=ly;alterType=L"";found=false;this->hasInitlize=false;this->hasMove=false;this->LBtDown=false;pos=-1;}
	void LButtonDown(UINT nflags,CPoint point);
	void RButtonDown(UINT nflags,CPoint point);
	void LButtonUp(UINT nflags,CPoint point);
	void RButtonUp(UINT nflags,CPoint point);
	void MouseMove(UINT nflags,CPoint point);
	void flash(CDC* pDC);
	void endflash(CDC* pDC);
	void InitlizeObject(CDC* pDC);
	bool getHasInit(){return this->hasInitlize;}
	void setInit(bool bl){this->hasInitlize=bl;}
	void setAlterType(CString type){alterType=type;}
	void getLine(gisLine&gl);
	bool hasFound(){return found;}
	CString getAlterType(){return alterType;}
private:
	CView *pView;
	CZoomManager * pZoom;
	DBConnection * pdb;
	CString layer;
	bool hasInitlize;
	bool LBtDown;
	bool hasMove;
	bool found;
	CPoint lastPoint;
	CString alterType;
	gisLine moveLine;
	gisLine tempgl;
	gisLine gl;
	double offsetX;
	double offsetY;
	CPoint sp1,sp2;
	int pos;
};

class  _declspec(dllexport) AlterPolygonTool:public AlterBaseTool
{
public:
	AlterPolygonTool(CView * view,CZoomManager * zoom,DBConnection * db,CString ly)
	{pView=view;pZoom=zoom;pdb=db;layer=ly;alterType=L"";this->found=false;this->hasInitlize=false;this->LBtDown=false;this->hasMove=false;}
	void LButtonDown(UINT nflags,CPoint point);
	void RButtonDown(UINT nflags,CPoint point);
	void LButtonUp(UINT nflags,CPoint point);
	void RButtonUp(UINT nflags,CPoint point);
	void MouseMove(UINT nflags,CPoint point);
	void flash(CDC* pDC);
	void endflash(CDC * pDC);
	void InitlizeObject(CDC* pDC);
	bool getHasInit(){return this->hasInitlize;}
	void setInit(bool bl){this->hasInitlize=bl;}
	void setAlterType(CString type){alterType=type;}
	void getPoly(gisPolygon&gp);
	bool hasFound(){return found;}
	CString getAlterType(){return alterType;}
private:
	CView *pView;
	CZoomManager * pZoom;
	DBConnection * pdb;
	CString layer;
	bool hasInitlize;
	bool LBtDown;
	bool hasMove;
	bool found;
	CPoint lastPoint;
	CString alterType;
	gisPolygon  movePolygon;
	gisPolygon  gPoly;
	gisPolygon  tempgPoly;
	double offsetX;
	double offsetY;
	CPoint sp1,sp2;
	int pos;
};


class  _declspec(dllexport) AlterMarkerTool:public AlterBaseTool
{
public:
	AlterMarkerTool(CView * view,CZoomManager * zoom,DBConnection * db,CString ly)
	{pView=view;pZoom=zoom;pdb=db;layer=ly;alterType=L"";found=false;this->LBtDown=false;this->hasMove=false;this->hasInitlize=false;this->CurrentColor=0;}
	void LButtonDown(UINT nflags,CPoint point);
	void RButtonDown(UINT nflags,CPoint point);
	void LButtonUp(UINT nflags,CPoint point);
	void RButtonUp(UINT nflags,CPoint point);
	void MouseMove(UINT nflags,CPoint point);
	void flash(CDC* pDC);
	void endflash(CDC * pDC);
	void InitlizeObject(CDC* pDC);
	bool getHasInit(){return this->hasInitlize;}
	void setInit(bool bl){this->hasInitlize=bl;}
	void setAlterType(CString type){alterType=type;}
	void getMarker(gisMarker&gm);
	bool hasFound(){return found;}
	CString getAlterType(){return alterType;}
private:
	CView *pView;
	CZoomManager * pZoom;
	DBConnection * pdb;
	CString layer;
	bool hasInitlize;
	bool LBtDown;
	bool hasMove;
	bool found;
	CString alterType;
	gisMarker moveMarker;
	gisMarker gm;
	gisMarker tempgm;
	int CurrentColor;
	CPoint lastPoint;
};

#endif