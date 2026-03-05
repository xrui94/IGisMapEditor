#include "stdafx.h"
#include "SearchTool.h"

void SearchByRect::LButtonDown(UINT nflags,CPoint point)
{
	this->LBtDown=true;
	this->lastPoint=this->clickPoint=point;
}
void SearchByRect::RButtonDown(UINT nflags,CPoint point)
{

}
void SearchByRect::LButtonUp(UINT nflags,CPoint point)
{
	int i=0;
	this->LBtDown=false;
	CDC * pDC = pView->GetDC();
	int mode = pDC->SetROP2(R2_NOTXORPEN);
	CRect r(this->clickPoint,point);
	pDC->Rectangle(&r);
	int max=0;
	switch(this->type)
	{
	case 1:
		max=this->pdb->getMaxID(this->layer);max++;
		this->gp=new gisPoint[max];
		this->pdb->SearchByRect(this->clickPoint,point,this->layer,this->gp,this->pZoom,count);
		break;
	case 2:
		max=this->pdb->getMaxID(this->layer);max++;
		this->gl = new gisLine[max];
		this->pdb->SearchByRect(this->clickPoint,point,this->layer,this->gl,this->pZoom,count);
		break;
	case 3:
		max=this->pdb->getMaxID(this->layer);max++;
		this->gPoly = new gisPolygon[max];
		this->pdb->SearchByRect(this->clickPoint,point,this->layer,this->gPoly,this->pZoom,count);
		break;
	case 4:
		max=this->pdb->getMaxID(this->layer);max++;
		this->gm = new gisMarker[max];
		this->pdb->SearchByRect(this->clickPoint,point,this->layer,this->gm,this->pZoom,count,pDC);
		break;
	}
	pDC->SetROP2(mode);
	pView->ReleaseDC(pDC);

}
void SearchByRect::RButtonUp(UINT nflags,CPoint point)
{

}
void SearchByRect::MouseMove(UINT nflags,CPoint point)
{
	if(this->LBtDown)
	{
		CDC * pDC = pView->GetDC();
		int mode = pDC->SetROP2(R2_NOTXORPEN);
		CRect * r=new CRect(this->clickPoint,this->lastPoint);
		pDC->Rectangle(r);
		r = new CRect(this->clickPoint,point);
		pDC->Rectangle(r);
		this->lastPoint=point;
		pDC->SetROP2(mode);
		pView->ReleaseDC(pDC);
	}
}

void SearchByRect::getPointSet(gisPoint * g)
{
	int i=0;
	while(i<this->count)
	{
		g[i]=this->gp[i];
		i++;
	}
}

void SearchByRect::getLineSet(gisLine * g)
{
	int i=0;
	while(i<this->count)
	{
		g[i]=this->gl[i];
		i++;
	}
}

void SearchByRect::getPolySet(gisPolygon * g)
{
	int i=0;
	while(i<this->count)
	{
		g[i]=this->gPoly[i];
		i++;
	}
}

void SearchByRect::getMarkerSet(gisMarker * g)
{
	int i=0;
	while(i<this->count)
	{
		g[i]=this->gm[i];
		i++;
	}
}
//////////////////////////////////////////////////////////////////////////////


void SearchByCircle::LButtonDown(UINT nflags,CPoint point)
{
	this->LBtDown=true;
	this->lastPoint=this->clickPoint=point;
}

void SearchByCircle::RButtonDown(UINT nflags,CPoint point)
{

}

void SearchByCircle::LButtonUp(UINT nflags,CPoint point)
{
	int i=0;
	this->LBtDown=false;
	CDC * pDC = pView->GetDC();
	int mode = pDC->SetROP2(R2_NOTXORPEN);
	CPoint center=CPoint((point.x+this->clickPoint.x)/2,(point.y+this->clickPoint.y)/2);
	double r=sqrt((double)(point.x-this->clickPoint.x)*(point.x-this->clickPoint.x)+(point.y-this->clickPoint.y)*(point.y-this->clickPoint.y))/2;
	pDC->Ellipse(CRect(CPoint(center.x-r,center.y-r),CPoint(center.x+r,center.y+r)));
	int max=0;
	switch(this->type)
	{
	case 1:
		max=this->pdb->getMaxID(this->layer);max++;
		this->gp=new gisPoint[max];
		this->pdb->SearchByCircle(CPoint(center.x-r,center.y-r),CPoint(center.x+r,center.y+r),this->layer,this->gp,this->pZoom,count);
		break;
	case 2:
		max=this->pdb->getMaxID(this->layer);max++;
		this->gl = new gisLine[max];
		this->pdb->SearchByCircle(CPoint(center.x-r,center.y-r),CPoint(center.x+r,center.y+r),this->layer,this->gl,this->pZoom,count);
		break;
	case 3:
		max=this->pdb->getMaxID(this->layer);max++;
		this->gPoly = new gisPolygon[max];
		this->pdb->SearchByCircle(CPoint(center.x-r,center.y-r),CPoint(center.x+r,center.y+r),this->layer,this->gPoly,this->pZoom,count);
		break;
	case 4:
		max=this->pdb->getMaxID(this->layer);max++;
		this->gm = new gisMarker[max];
		this->pdb->SearchByCircle(CPoint(center.x-r,center.y-r),CPoint(center.x+r,center.y+r),this->layer,this->gm,this->pZoom,count,pDC);
		break;
	}
	pDC->SetROP2(mode);
	pView->ReleaseDC(pDC);
}

void SearchByCircle::RButtonUp(UINT nflags,CPoint point)
{

}

void SearchByCircle::MouseMove(UINT nflags,CPoint point)
{
	if(this->LBtDown)
	{
		CDC * pDC = pView->GetDC();
		int mode = pDC->SetROP2(R2_NOTXORPEN);
		CPoint center=CPoint((this->lastPoint.x+this->clickPoint.x)/2,(this->lastPoint.y+this->clickPoint.y)/2);
		double r=sqrt((double)(this->lastPoint.x-this->clickPoint.x)*(this->lastPoint.x-this->clickPoint.x)+(this->lastPoint.y-this->clickPoint.y)*(this->lastPoint.y-this->clickPoint.y))/2;
		pDC->Ellipse(CRect(CPoint(center.x-r,center.y-r),CPoint(center.x+r,center.y+r)));
		center=CPoint((point.x+this->clickPoint.x)/2,(point.y+this->clickPoint.y)/2);
		r=sqrt((double)(point.x-this->clickPoint.x)*(point.x-this->clickPoint.x)+(point.y-this->clickPoint.y)*(point.y-this->clickPoint.y))/2;
		pDC->Ellipse(CRect(CPoint(center.x-r,center.y-r),CPoint(center.x+r,center.y+r)));
		this->lastPoint=point;
		pDC->SetROP2(mode);
		pView->ReleaseDC(pDC);
	}
}

void SearchByCircle::getPointSet(gisPoint * gp)
{
	int i=0;
	while(i<this->count)
	{
		gp[i]=this->gp[i];
		i++;
	}
}

void SearchByCircle::getLineSet(gisLine * g)
{
	int i=0;
	while(i<this->count)
	{
		g[i]=this->gl[i];
		i++;
	}
}

void SearchByCircle::getPolySet(gisPolygon * g)
{
	int i=0;
	while(i<this->count)
	{
		g[i]=this->gPoly[i];
		i++;
	}
}

void SearchByCircle::getMarkerSet(gisMarker * g)
{
	int i=0;
	while(i<this->count)
	{
		g[i]=this->gm[i];
		i++;
	}
}
///////////////////////////////////////////////////////////////////////////

void SearchByPolygon::LButtonDown(UINT nflags,CPoint point)
{
	if(pView!=NULL)
	{
		mouseMove=true;
		CDC* pDC=pView->GetDC();
		if(Q.empty())
		{
			lastmove=point;
			firstPoint=point;
		}
		else
		{
			pDC->MoveTo(Q.back());
			pDC->LineTo(point);
		}
		Q.push(point);
		pView->ReleaseDC(pDC);
	}
}

void SearchByPolygon::RButtonDown(UINT nflags,CPoint point)
{
	int i=0;
	CDC* pDC=pView->GetDC();
	if(!Q.empty())
	{
		pDC->MoveTo(Q.back());
		pDC->LineTo(point);
		Q.push(point);
		pDC->MoveTo(Q.back());
		pDC->LineTo(firstPoint);
	}
	pCount=Q.size();
	if(pCount>2)
	{		
		while(!Q.empty())
		{
			pt[i]=Q.front();
			Q.pop();
			i++;
		}
		int mode=pDC->SetROP2(R2_NOTXORPEN);
		pDC->Polygon(pt,pCount);
		pDC->SetROP2(mode);
		int max =0;
		switch(this->type)
		{
		case 1:
			max=this->pdb->getMaxID(this->layer);max++;
			this->gp=new gisPoint[max];
			this->pdb->SearchByPolygon(this->pt,this->pCount,this->layer,this->gp,this->pZoom,count);
			break;
		case 2:
			max=this->pdb->getMaxID(this->layer);max++;
			this->gl = new gisLine[max];
			this->pdb->SearchByPolygon(this->pt,this->pCount,this->layer,this->gl,this->pZoom,count);
			break;
		case 3:
			max=this->pdb->getMaxID(this->layer);max++;
			this->gPoly = new gisPolygon[max];
			this->pdb->SearchByPolygon(this->pt,this->pCount,this->layer,this->gPoly,this->pZoom,count);
			break;
		case 4:
			max=this->pdb->getMaxID(this->layer);max++;
			this->gm = new gisMarker[max];
			this->pdb->SearchByPolygon(this->pt,this->pCount,this->layer,this->gm,this->pZoom,count,pDC);
			break;
		}
	}
	mouseMove=false;
	pView->ReleaseDC(pDC);
}

void SearchByPolygon::LButtonUp(UINT nflags,CPoint point)
{

}

void SearchByPolygon::RButtonUp(UINT nflags,CPoint point)
{

}

void SearchByPolygon::MouseMove(UINT nflags,CPoint point)
{
	if(pView!=NULL&&mouseMove)
	{
		CDC* pDC=pView->GetDC();
		int mode=pDC->SetROP2(R2_NOTXORPEN);
		if(!Q.empty())
		{
			CPoint p=Q.back();
			pDC->MoveTo(p);
			pDC->LineTo(lastmove);
			pDC->MoveTo(p);
			pDC->LineTo(point);
			lastmove=point;
		}
		pDC->SetROP2(mode);
		pView->ReleaseDC(pDC);
	}
}


void SearchByPolygon::getPointSet(gisPoint * gp)
{
	int i=0;
	while(i<this->count)
	{
		gp[i]=this->gp[i];
		i++;
	}
}

void SearchByPolygon::getLineSet(gisLine * g)
{
	int i=0;
	while(i<this->count)
	{
		g[i]=this->gl[i];
		i++;
	}
}

void SearchByPolygon::getPolySet(gisPolygon * g)
{
	int i=0;
	while(i<this->count)
	{
		g[i]=this->gPoly[i];
		i++;
	}
}

void SearchByPolygon::getMarkerSet(gisMarker * g)
{
	int i=0;
	while(i<this->count)
	{
		g[i]=this->gm[i];
		i++;
	}
}