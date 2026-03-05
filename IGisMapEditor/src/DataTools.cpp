#include "stdafx.h"
#include "DataTools.h"
//=========================================Data tools===============================================================

void CPointDataTool::LButtonDown(CPoint point,CZoomManager* pZoom)
{
	pt=pZoom->CPtoGP(point);
}
void CPointDataTool::LButtonUp(CPoint point,CZoomManager* pZoom)
{
	if(pdb->isConnection())
	{
		gisPoint gp;
		gp.point=pt;
		gp.pType=pType->ps.type;
		gp.size=pType->ps.size;
		gp.color=pType->ps.color;
		pdb->addPoint(gp,lname);
	}

}
void CPointDataTool::RButtonDown(CPoint point,CZoomManager* pZoom)
{

}
void CPointDataTool::RButtonUp(CPoint point,CZoomManager* pZoom)
{

}
void CPointDataTool::MouseMove(CPoint point,CZoomManager* pZoom)
{

}

void CLineDataTool::LButtonDown(CPoint point,CZoomManager* pZoom)
{

}
void CLineDataTool::LButtonUp(CPoint point,CZoomManager* pZoom)
{
	DPoint pt=pZoom->CPtoGP(point);

	if(!pType->ls.curve)
	pts.push_back(pt);
	else
	{
		if(pts.empty())
		{
			doing=0;pts.push_back(pt);
		}
		else
		{
			if(doing==0)
			{
				doing=1;
				DPoint p1,p2;
				p1=p2=pts.back();
				p2=MidPoint(p1,pt);
				p1=p2;
				pts.push_back(p1);
				pts.push_back(p2);
				pts.push_back(pt);
			}
			else if(doing==1)
			{
				doing=2;
				int n=pts.size()-1;
				pts[n-2]=pt;
			}
			else
			{
				doing=0;
				int n=pts.size()-1;
				pts[n-1]=pt;
			}
		}
	}
}
void CLineDataTool::RButtonDown(CPoint point,CZoomManager* pZoom)
{
	//DPoint pt=pZoom->CPtoGP(point);
	//pts.push_back(pt);
	if(!pType->ls.curve)
	{
		if(pts.size()<2)
		{
			while(!pts.empty())
				pts.clear();
			return;
		}
		if(pdb->isConnection())
		{
			int n=pts.size();
			gisLine gl(n);
			gl.width=pType->ls.width;
			int i=0;
			while(i<n)
			{
				gl.LinePoint[i]=pts[i];i++;
			}
			pts.clear();
			gl.color=pType->ls.color;
			gl.lType=pType->ls.PENTYPE;
			pdb->addLine(gl,lname);
		}
	}
	else
	{
		if(pts.size()<4)
		{
			while(!pts.empty())
				pts.clear();
			return;
		}
		if(pdb->isConnection())
		{
			int n=pts.size();
			gisLine gl(n);
			int a=1;
			gl.width=(-1)*pType->ls.width;
			int i=0;
			while(i<n)
			{
				gl.LinePoint[i]=pts[i];i++;
			}
			pts.clear();
			gl.color=pType->ls.color;
			gl.lType=pType->ls.PENTYPE;//|PS_GEOMETRIC;
			pdb->addLine(gl,lname);
		}
	}


}
void CLineDataTool::RButtonUp(CPoint point,CZoomManager* pZoom)
{

}
void CLineDataTool::MouseMove(CPoint point,CZoomManager* pZoom)
{

}

void CPolyDataTool::LButtonDown(CPoint point,CZoomManager* pZoom)
{

}
void CPolyDataTool::LButtonUp(CPoint point,CZoomManager* pZoom)
{
	DPoint pt=pZoom->CPtoGP(point);
	pts.push(pt);
}
void CPolyDataTool::RButtonDown(CPoint point,CZoomManager* pZoom)
{
	DPoint pt=pZoom->CPtoGP(point);
	pts.push(pt);
	if(pts.size()<3)
	{
		while(!pts.empty())
			pts.pop();
		return;
	}
	if(pdb->isConnection())
	{
		int n=pts.size();
		gisPolygon gp(n);
		gp.BorderColor=pType->rs.sidecolor;
		gp.BorderType=pType->rs.sideType;
		gp.color=pType->rs.fillcolor;
		int i=0;
		while(i<n)
		{
			gp.point[i++]=pts.front();
			pts.pop();
		}
		pdb->addPolygon(gp,lname);
	}

}
void CPolyDataTool::RButtonUp(CPoint point,CZoomManager* pZoom)
{

}
void CPolyDataTool::MouseMove(CPoint point,CZoomManager* pZoom)
{

}

void CNoteDataTool::LButtonDown(CPoint point,CZoomManager* pZoom)
{
	pt=pZoom->CPtoGP(point);
}
void CNoteDataTool::LButtonUp(CPoint point,CZoomManager* pZoom)
{
	if(pdb->isConnection())
	{
		gisMarker gm;
		gm.point=pt;
		gm.lfont=pType->ns.lfont;
		gm.text=pType->ns.text;
		gm.color=pType->ns.color;
		pdb->addMarker(gm,lname);
	}
}
void CNoteDataTool::RButtonDown(CPoint point,CZoomManager* pZoom)
{

}
void CNoteDataTool::RButtonUp(CPoint point,CZoomManager* pZoom)
{

}
void CNoteDataTool::MouseMove(CPoint point,CZoomManager* pZoom)
{

}
