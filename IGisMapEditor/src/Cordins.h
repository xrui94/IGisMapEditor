#ifndef CORDINS_H
#define CORDINS_H

#include "math.h"
#include "GeoPoint.h"
//===================================================================================================================

class CZoomManager
{
public:
	CZoomManager(){times=1;nwidth=nheight=100;lt.x=0;lt.y=100;rb.x=100;rb.y=0;needInit=true;}
	CZoomManager(const CZoomManager &m){times=m.times;nwidth=m.nwidth;nheight=m.nheight;lt=m.lt;rb=m.rb;needInit=m.needInit;}
	~CZoomManager(){}
public:
	void recalc(int w,int h);//set new rect 
	DPoint CPtoGP(CPoint p);//Device CPoint to Geographical Point 
	CPoint GPtoCP(DPoint p);//Geographical Point to Device CPoint 
	inline bool resized(int w,int h){return !(w==nwidth&&h==nheight);} //see if rect resized
	void Init(double ltx,double lty,double rbx,double rby,int w,int h);//init area
	inline float ntimes(){return times;}
	void zoomin(CPoint clt,CPoint crb);///∑≈¥Û
	void zoomout(CPoint clt,CPoint crb);///Àı–°
	void move(CPoint p,CPoint q);
	bool Initalized(){return !needInit;}
	float xPrecision();//{return abs(rb.x-lt.x)/nwidth;}
	float yPrecision();//{return abs(lt.y-rb.y)/nheight;}
protected:
	float times;
	bool needInit;
public:
	int nwidth;
	int nheight;
	DPoint lt,rb;
};

//============================================ZOOM TOOLS===========================================================
class CZoomTool
{
public:
	CZoomTool(CView* pView=NULL){}
	~CZoomTool(){}
	virtual void LButtonDown(CPoint point,CZoomManager* pManager){}
	virtual void LButtonUp(CPoint point,CZoomManager* pManager){}
	virtual void RButtonDown(CPoint point,CZoomManager* pManager){}
	virtual void RButtonUp(CPoint point,CZoomManager* pManager){}
	virtual void MouseMove(CPoint point,CZoomManager* pManager){}
};

class CZoominTool:public CZoomTool
{
public:
	CZoominTool(CView* pView){noorg=true;p=pView;}
	virtual~CZoominTool(){}
	void LButtonDown(CPoint point,CZoomManager* pManager);
	void LButtonUp(CPoint point,CZoomManager* pManager);
	void RButtonDown(CPoint point,CZoomManager* pManager);
	void RButtonUp(CPoint point,CZoomManager* pManager);
	void MouseMove(CPoint point,CZoomManager* pManager);
private:
	CPoint org;
	bool noorg;
	CPoint last;
	CView* p;
};
class CZoomoutTool:public CZoomTool
{
public:
	CZoomoutTool(CView* pView){noorg=true;p=pView;}
	virtual~CZoomoutTool(){}
	void LButtonDown(CPoint point,CZoomManager* pManager);
	void LButtonUp(CPoint point,CZoomManager* pManager);
	void RButtonDown(CPoint point,CZoomManager* pManager);
	void RButtonUp(CPoint point,CZoomManager* pManager);
	void MouseMove(CPoint point,CZoomManager* pManager);
private:
	CPoint org;
	CView* p;
	bool noorg;
	CPoint last;
};
class CMoveTool:public CZoomTool
{
public:
	CMoveTool(CView* pView){noorg=true;p=pView;}
	virtual~CMoveTool(){}
	void LButtonDown(CPoint point,CZoomManager* pManager);
	void LButtonUp(CPoint point,CZoomManager* pManager);
	void RButtonDown(CPoint point,CZoomManager* pManager);
	void RButtonUp(CPoint point,CZoomManager* pManager);
	void MouseMove(CPoint point,CZoomManager* pManager);
private:
	CPoint org;
	bool noorg;
	CPoint last;
	CView* p;
};
//===================================================================================================================

#endif