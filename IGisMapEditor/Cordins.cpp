#include "stdafx.h"
#include "Cordins.h"
//================================================================================================================
void CZoomManager::recalc(int w,int h)
{
	if(w==nwidth&&h==nheight)return;
	else
	{
		rb.x=lt.x+((rb.x-lt.x)*(float)w/nwidth);
		rb.y=lt.y+((rb.y-lt.y)*(float)h/nheight);
		nwidth=w;
		nheight=h;
		return ;
	}
}

void CZoomManager::Init(double ltx,double lty,double rbx,double rby,int w,int h)
{
	lt.x=ltx;
	lt.y=lty;
	rb.x=rbx;
	rb.y=rby;
	nwidth=w;
	nheight=h;
	times=1.0;
	needInit=false;
}

DPoint CZoomManager::CPtoGP(CPoint p)
{
	DPoint pt;
	float xcor=(float)p.x/nwidth;
	float ycor=(float)p.y/nheight;
	pt.x=lt.x+(rb.x-lt.x)*xcor;
	pt.y=lt.y+(rb.y-lt.y)*ycor;
	return pt;
}

CPoint CZoomManager::GPtoCP(DPoint p)
{
	CPoint pt;
	pt.x=int(nwidth*(p.x-lt.x)/(rb.x-lt.x));
	pt.y=int(nheight*(p.y-lt.y)/(rb.y-lt.y));
	return pt;
}

void CZoomManager::zoomin(CPoint clt,CPoint crb)///�Ŵ�
{
	int newW=crb.x-clt.x;
	int newH=crb.y-clt.y;
	if(newW>=newH)
	{
		float tg=(float)nheight/nwidth;
		newH=int(newW*tg);
	}
	else
	{
		float tg=(float)nwidth/nheight;
		newW=int(newH*tg);
	}
	times*=float(nwidth)/newW;
	crb.x=clt.x+newW;
	crb.y=clt.y+newH;
	DPoint tp1,tp2;
	tp1=CPtoGP(clt);
	tp2=CPtoGP(crb);
	lt=tp1;
	rb=tp2;
	return;
}
	   
void CZoomManager::zoomout(CPoint clt,CPoint crb)///��С
{
	int newW=crb.x-clt.x;
	int newH=crb.y-clt.y;
	if(newW>=newH)
	{
		float tg=(float)nheight/nwidth;
		newH=int(newW*tg);
	}
	else
	{
		float tg=(float)nwidth/nheight;
		newW=int(newH*tg);
	}
	times*=float(newW)/nwidth;
	crb.x=clt.x+newW;
	crb.y=clt.y+newH;
	DPoint tp1=lt;DPoint tp2=rb;
	double xop=(rb.x-lt.x)/newW;
	double yop=(rb.y-lt.y)/newH;
	lt.x=tp1.x-clt.x*xop;
	lt.y=tp1.y-clt.y*yop;
	rb.x=tp2.x+(nwidth-crb.x)*xop;
	rb.y=tp2.y+(nheight-crb.y)*yop;
	return;
	
}	   
void CZoomManager::move(CPoint p,CPoint q)
{
	DPoint p1,p2;
	p1=CPtoGP(p);
	p2=CPtoGP(q);
	double xoff=p2.x-p1.x;
	double yoff=p2.y-p1.y;
	lt.x=lt.x-xoff;
	lt.y=lt.y-yoff;
	rb.x=rb.x-xoff;
	rb.y=rb.y-yoff;
}
float CZoomManager::xPrecision()
{
	return fabs(rb.x-lt.x)/nwidth;
}
float CZoomManager::yPrecision()
{
	return fabs(lt.y-rb.y)/nheight;
}
//===================================================================================================================

//----------------zoomin tool------------------------------

void CZoominTool::LButtonDown(CPoint point,CZoomManager* pManager)
{
	if(noorg)
	{
		org=point;
		noorg=false;
		last=org;
	}
}
void CZoominTool::LButtonUp(CPoint point,CZoomManager* pManager)
{
	if(!noorg&&(point.x>org.x)&&(point.y>org.y))
	{
		CDC* pDC=p->GetDC();
		int nmode=pDC->SetROP2(R2_NOTXORPEN);
		pDC->Rectangle(org.x,org.y,last.x,last.y);
		pDC->SetROP2(nmode);
		p->ReleaseDC(pDC);
		pManager->zoomin(org,point);
		noorg=true;
		org=last=CPoint(0,0);
	}
}
void CZoominTool::RButtonDown(CPoint point,CZoomManager* pManager)
{

}
void CZoominTool::RButtonUp(CPoint point,CZoomManager* pManager)
{

}
void CZoominTool::MouseMove(CPoint point,CZoomManager* pManager)
{
	if(!noorg&&(point.x>org.x)&&(point.y>org.y))
	{
		CDC* pDC=p->GetDC();
		int nmode=pDC->SetROP2(R2_NOTXORPEN);
		pDC->Rectangle(org.x,org.y,last.x,last.y);
		pDC->Rectangle(org.x,org.y,point.x,point.y);
		pDC->SetROP2(nmode);
		p->ReleaseDC(pDC);
		last=point;
	}
}
//----------------zoomout tool------------------------------

void CZoomoutTool::LButtonDown(CPoint point,CZoomManager* pManager)
{
	if(noorg)
	{
		org=point;
		noorg=false;
		last=org;
	}
}
void CZoomoutTool::LButtonUp(CPoint point,CZoomManager* pManager)
{
	if(!noorg&&(point.x>org.x)&&(point.y>org.y))
	{
		CDC* pDC=p->GetDC();
		int nmode=pDC->SetROP2(R2_NOTXORPEN);
		pDC->Rectangle(org.x,org.y,last.x,last.y);
		pDC->SetROP2(nmode);
		p->ReleaseDC(pDC);
		pManager->zoomout(org,point);
		noorg=true;
		org=last=CPoint(0,0);
	}
}
void CZoomoutTool::RButtonDown(CPoint point,CZoomManager* pManager)
{

}
void CZoomoutTool::RButtonUp(CPoint point,CZoomManager* pManager)
{

}
void CZoomoutTool::MouseMove(CPoint point,CZoomManager* pManager)
{
	if(!noorg&&(point.x>org.x)&&(point.y>org.y))
	{
		CDC* pDC=p->GetDC();
		int nmode=pDC->SetROP2(R2_NOTXORPEN);
		pDC->Rectangle(org.x,org.y,last.x,last.y);
		pDC->Rectangle(org.x,org.y,point.x,point.y);
		pDC->SetROP2(nmode);
		p->ReleaseDC(pDC);
		last=point;
	}
}
//----------------move tool------------------------------
void CMoveTool::LButtonDown(CPoint point,CZoomManager* pManager)
{
	if(noorg)
	{
		org=point;
		noorg=false;
		last=org;
	}
}
void CMoveTool::LButtonUp(CPoint point,CZoomManager* pManager)
{
	if(!noorg)
	{
		CDC* pDC=p->GetDC();
		int nmode=pDC->SetROP2(R2_NOTXORPEN);
		pDC->MoveTo(org);
		pDC->LineTo(last);
		pDC->SetROP2(nmode);
		p->ReleaseDC(pDC);
		pManager->move(org,point);
		noorg=true;
		org=last=CPoint(0,0);
	}
}
void CMoveTool::RButtonDown(CPoint point,CZoomManager* pManager)
{

}
void CMoveTool::RButtonUp(CPoint point,CZoomManager* pManager)
{

}
void CMoveTool::MouseMove(CPoint point,CZoomManager* pManager)
{
	if(!noorg)
	{
		CDC* pDC=p->GetDC();
		int nmode=pDC->SetROP2(R2_NOTXORPEN);
		pDC->MoveTo(org);
		pDC->LineTo(last);
		pDC->MoveTo(org);
		pDC->LineTo(point);
		pDC->SetROP2(nmode);
		p->ReleaseDC(pDC);
		last=point;
	}
}
//==========================================