#include "stdafx.h"
#include "GraphTool.h"
//========================================================================================================
int CPointTool:: LButtonDown(UINT nflags,CPoint point)
{
	CPen pen;
	pen.CreatePen(PS_SOLID,1,color);
	CBrush brush(color);
	CDC*pDC=pView->GetDC();
	CPen* pOldPen =pDC->SelectObject(&pen);
	CBrush* pOldBrush=pDC->SelectObject(&brush);
	int nradius=radius+4;	
	nradius=int(nradius*times);
	if(nradius==0) nradius=1;
	if(type==L"Circle")
		pDC->Ellipse(point.x-nradius,point.y-nradius,point.x+nradius,point.y+nradius);///
	else if(type==L"cRect")
		pDC->Rectangle(point.x-nradius,point.y-nradius-int(2*times),point.x+nradius,point.y+nradius+int(2*times));///
	else if(type==L"zRect")
		pDC->Rectangle(point.x-nradius,point.y-nradius,point.x+nradius,point.y+nradius);//
	else if(type==L"SJ")
	{
		CPoint _point[3];
		_point[0]=CPoint(point.x,point.y-nradius);
		_point[1]=CPoint(point.x-nradius,point.y+nradius);
		_point[2]=CPoint(point.x+nradius,point.y+nradius);
		pDC->Polygon(_point,3);///
	}
	else if(type==L"Ellipes")
		pDC->Ellipse(point.x-nradius,point.y-nradius-int(3*times),point.x+nradius,point.y+nradius+int(3*times));///
	pDC->SelectObject(pOldBrush);//
	pDC->SelectObject(pOldPen);//
	pen.DeleteObject();
	pView->ReleaseDC(pDC);
	return 0;
}
int CPointTool:: RButtonDown(UINT nflags,CPoint point)
{
	return 0;
}
int CPointTool:: LButtonUp(UINT nflags,CPoint point)
{
	return 0;
}
int CPointTool:: RButtonUp(UINT nflags,CPoint point)
{
	return 0;
}
int CPointTool:: MouseMove(UINT nflags,CPoint point)
{
	return 0;
}
//--------------------------------------------------------------------------------------------------------
int CLineTool::PLButtonUp(UINT nflags,CPoint point)
{
	if(pView!=NULL)
	{
		CDC *pDC=pView->GetDC();
		pOldPen = pDC->SelectObject(&pen);
		mouseMove=true;
		if(q.empty())
		{
			q.push_back(point);
			lastmove=point;
		}
		else
		{
			pDC->MoveTo(q.back());
			pDC->LineTo(point);
			q.push_back(point);
		}
		pDC->SelectObject(pOldPen);
		pView->ReleaseDC(pDC);
		return 1;
	}
	return 0;
}
int CLineTool::PRButtonUp(UINT nflags,CPoint point)
{
	return 0;
}
int CLineTool::PLButtonDown(UINT nflags,CPoint point)
{
	return 0;
}
int CLineTool::PRButtonDown(UINT nflags,CPoint point)
{
	if(pView!=NULL)
	{
		CDC *pDC=pView->GetDC();
		pOldPen = pDC->SelectObject(&pen);
		if(!q.empty())
		{
		   pDC->MoveTo(q.back());
		   pDC->LineTo(point);
		   while(!q.empty())
			   q.pop_back();
		}
		pDC->SelectObject(pOldPen);
		pDC->SetROP2(mode);
		mouseMove=false;
		pView->ReleaseDC(pDC);
		return 1;
	}
	return 0;
}
int CLineTool::PMouseMove(UINT nflags,CPoint point)
{
	if(pView!=NULL&&mouseMove)
	{
		if(!q.empty())
		{
			CDC *pDC=pView->GetDC();
			pOldPen = pDC->SelectObject(&pen);
			mode=pDC->SetROP2(R2_NOTXORPEN);
			if(!q.empty())
			{
				pDC->MoveTo(q.back());
				pDC->LineTo(lastmove);
				pDC->MoveTo(q.back());
				pDC->LineTo(point);
				lastmove=point;
			}
			pDC->SelectObject(pOldPen);
			pDC->SetROP2(mode);
			pView->ReleaseDC(pDC);
			return 1;
		}
	}
	return 0;
}
int CLineTool::CLButtonUp(UINT nflags,CPoint point)
{
	if(pView!=NULL)
	{
		CDC *pDC=pView->GetDC();
		pDC->SelectObject(&pen);
//		CBrush b(RGB(255,0,0));
//		pDC->SelectObject(&b);
		if(q.empty())
		{
			q.push_back(point);lastmove=point;doing=0;
			//pDC->Ellipse(point.x-5,point.y-5,point.x+5,point.y+5);//
		}
		else
		{
			if(doing==0)
			{
				doing=1;
				CPoint p1,p2;CPoint or;
				or=p1=p2=q.back();
				p2=MidPoint(p1,point);
				p1=p2;
				q.push_back(p1);
				q.push_back(p2);
				q.push_back(point);
				//pDC->Ellipse(point.x-5,point.y-5,point.x+5,point.y+5);//
				lastmove=p1;
				CPen dashpen(PS_SOLID,1,RGB(0,0,0));
				pDC->SelectObject(&dashpen);
				int nmode=pDC->SetROP2(R2_NOTXORPEN);
				CPoint pp[4]={or,p1,p2,point};
				pDC->PolyBezier(pp,4);
			}
			else if(doing==1)
			{
				doing=2;
				int n=q.size()-1;
				q[n-2]=point;
				//pDC->Ellipse(point.x-5,point.y-5,point.x+5,point.y+5);
				lastmove=q[n-1];
				
			}
			else if(doing==2)
			{
				doing=0;
				int n=q.size()-1;
				q[n-1]=point;
				//pDC->Ellipse(point.x-5,point.y-5,point.x+5,point.y+5);//				
				lastmove=point;
			}
			else
			{q.clear();doing=0;AfxMessageBox(L"lerror");}
			//lastmove=point;
		}
		return 1;
	}
	else 
		return 0;
}
int CLineTool::CRButtonUp(UINT nflags,CPoint point)
{return 0;}
int CLineTool::CLButtonDown(UINT nflags,CPoint point)
{return 0;}
int CLineTool::CRButtonDown(UINT nflags,CPoint point)
{
	if(pView!=NULL)
	{
		
		if(!q.empty())
		{
			CDC *pDC=pView->GetDC();
			CPen dashpen(PS_SOLID,1,RGB(0,0,0));
			pDC->SelectObject(&dashpen);
			int nmode=pDC->SetROP2(R2_NOTXORPEN);
			int sz=q.size();
			if(sz>=4)
			{			
				CPoint* pts=new CPoint[sz];
				for(int i=0;i<sz;i++)
				{
					pts[i]=q[i];
				}
				if(doing==0)
				{
				}
				else if(doing==1)
				{
					pts[sz-3]=lastmove;
				}
				else if(doing==2)
				{
					pts[sz-2]=lastmove;
				}
				else
				{;}
				pDC->PolyBezier(pts,sz);
				delete[]pts;
			}
			else
			{
				q.clear();
			}
			pDC->SetROP2(nmode);
			pDC->SelectObject(&pen);
			if(q.size()>=4)
			{			
				CPoint* pts=new CPoint[q.size()];
				for(int i=0;i<q.size();i++)
				{
					pts[i]=q[i];
				}
				pDC->PolyBezier(pts,q.size());
				delete[]pts;
				q.clear();
				doing=0;
			}
			return 1;
		}
		else
			return 0;
	}
	else
	return 0;
}
int CLineTool::CMouseMove(UINT nflags,CPoint point)
{
	if(pView!=NULL)
	{	
		if(!q.empty())
		{
			CDC *pDC=pView->GetDC();
			CPen dashpen(PS_SOLID,1,RGB(0,0,0));
			pDC->SelectObject(&dashpen);
			int nmode=pDC->SetROP2(R2_NOTXORPEN);
			int sz=q.size();
			if(sz>=4)
			{			
				CPoint* pts=new CPoint[sz];
				for(int i=0;i<sz;i++)
				{
					pts[i]=q[i];
				}
				if(doing==0)
				{
					//pDC->MoveTo(q.back());
					//pDC->LineTo(lastmove);
				}
				else if(doing==1)
				{
					pts[sz-3]=lastmove;
				}
				else if(doing==2)
				{
					pts[sz-2]=lastmove;
				}
				else
				{;}
				pDC->PolyBezier(pts,sz);
				delete []pts;
			}	
//			pDC->SetROP2(nmode);
			lastmove=point;
			if(q.size()>=4)
			{			
				CPoint* pts=new CPoint[sz];
				for(int i=0;i<sz;i++)
				{
					pts[i]=q[i];
				}
				if(doing==0)
				{
					//pDC->MoveTo(q.back());
					//pDC->LineTo(lastmove);
				}
				else if(doing==1)
				{
					pts[sz-3]=lastmove;
				}
				else if(doing==2)
				{
					pts[sz-2]=lastmove;
				}
				else
				{;}
				pDC->PolyBezier(pts,sz);
				delete []pts;
			}
			
			return 1;
		}
		else
			return 0;
	}
	else
	return 0;
}
int CLineTool::LButtonDown(UINT nflags,CPoint point)
{
	int res;
	if(pStyle->ls.curve)
	{res=CLButtonDown(nflags,point);}
	else
	{res=PLButtonDown(nflags,point);}
	return res;
}
int CLineTool::RButtonDown(UINT nflags,CPoint point)
{
	int res;
	if(pStyle->ls.curve)
	{res=CRButtonDown(nflags,point);}
	else
	{res=PRButtonDown(nflags,point);}
	return res;
}
int CLineTool::LButtonUp(UINT nflags,CPoint point)
{
	int res;
	if(pStyle->ls.curve)
	{res=CLButtonUp(nflags,point);}
	else
	{res=PLButtonUp(nflags,point);}
	return res;
}
int CLineTool::RButtonUp(UINT nflags,CPoint point)
{
	int res;
	if(pStyle->ls.curve)
	{res=CRButtonUp(nflags,point);}
	else
	{res=PRButtonUp(nflags,point);}
	return res;
}
int CLineTool::MouseMove(UINT nflags,CPoint point)
{
	int res;
	if(pStyle->ls.curve)
	{res=CMouseMove(nflags,point);}
	else
	{res=PMouseMove(nflags,point);}
	return res;
}
//---------------------------------------------------------------------------------------------------------

int CPolyTool::LButtonUp(UINT nflags,CPoint point)
{
	if(pView!=NULL)
	{
		CDC* pDC=pView->GetDC();
		pOldPen = pDC->SelectObject(&pen);
		mouseMove=true;
		if(Q.empty())
		{
			Q.push(point);
			lastmove=point;
			firstPoint=point;
		}
		else
		{
			pDC->MoveTo(Q.back());
			pDC->LineTo(point);
		}
		Q.push(point);
		pDC->SelectObject(pOldPen);
		pView->ReleaseDC(pDC);
		return 1;
	}
	return 0;
}
int CPolyTool::RButtonUp(UINT nflags,CPoint point)
{
	return 0;
}
int CPolyTool::LButtonDown(UINT nflags,CPoint point)
{
	return 0;
}
int CPolyTool::RButtonDown(UINT nflags,CPoint point)
{
	if(pView!=NULL)
	{
		CDC* pDC=pView->GetDC();
		pOldPen = pDC->SelectObject(&pen);
		pOldBrush = pDC->SelectObject(&brush);
		if(!Q.empty())
		{
			if(Q.back()!=point&&point!=firstPoint)
			{
				pDC->MoveTo(Q.back());
				pDC->LineTo(point);
				Q.push(point);
				pDC->MoveTo(Q.back());
				pDC->LineTo(firstPoint);
			}
		}
		int pointNumber=Q.size();
		int i=0;
		if(pointNumber>2)
		{		
			CPoint * point = new CPoint[pointNumber];
			while(!Q.empty())
			{
				point[i]=Q.front();
				Q.pop();
				i++;
			}
			pDC->Polygon(point,pointNumber);
			pDC->SelectObject(pOldBrush);
		}
		mouseMove=false;
		pDC->SelectObject(pOldPen);
		pDC->SetROP2(mode);
		pView->ReleaseDC(pDC);
		return 1;
	}
	return 0;
}
int CPolyTool::MouseMove(UINT nflags,CPoint point)
{
	if(pView!=NULL&&mouseMove)
	{
		CDC* pDC=pView->GetDC();
		pOldPen = pDC->SelectObject(&pen);
		mode=pDC->SetROP2(R2_NOTXORPEN);
		if(!Q.empty())
		{
			CPoint p=Q.back();
			pDC->MoveTo(p);
			pDC->LineTo(lastmove);
			pDC->MoveTo(p);
			pDC->LineTo(point);
			lastmove=point;
		}
		pDC->SelectObject(pOldPen);
		pDC->SetROP2(mode);
		pView->ReleaseDC(pDC);
		return 1;
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------------
int CNoteTool:: LButtonDown(UINT nflags,CPoint point)
{
	CDC * pDC = pView->GetDC();
	int nwheight=0;
	nwheight=int(pStyle->ns.lfont.lfHeight*times);
	if(abs(nwheight)<=5)
	{
		int a=nwheight/abs(nwheight);
		nwheight=a*5;
	}
	pStyle->ns.lfont.lfHeight=nwheight;
	CFont* pfont = CFont::FromHandle(CreateFontIndirect(&pStyle->ns.lfont));
	CFont *poldf=pDC->SelectObject(pfont);
	COLORREF oldcl=pDC->SetTextColor(pStyle->ns.color);
	int nbkmode=pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW(point.x,point.y,pStyle->ns.text);
	pDC->SelectObject(poldf);
	pDC->SetTextColor(oldcl);
	pDC->SetBkMode(nbkmode);
	pView->ReleaseDC(pDC);
	return 0;
}
int CNoteTool:: RButtonDown(UINT nflags,CPoint point)
{
	return 0;
}
int CNoteTool:: LButtonUp(UINT nflags,CPoint point)
{

	return 0;
}
int CNoteTool:: RButtonUp(UINT nflags,CPoint point)
{
	return 0;
}
int CNoteTool:: MouseMove(UINT nflags,CPoint point)
{
	return 0;
}