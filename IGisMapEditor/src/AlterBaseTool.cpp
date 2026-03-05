#include "stdafx.h"
#include "AlterBaseTool.h"

void AlterPointTool::LButtonDown(UINT nflags,CPoint point)
{
	this->LBtDown=true;
	CDC * pDC =pView->GetDC();
	this->found=false;
	if(this->pdb->SearchPointByClick(point,this->tempgp,this->layer,this->pZoom))
	{
		this->found=true;
		this->InitlizeObject(pDC);
		if(this->alterType!="Move"&&this->alterType!=L"")
		   pView->SetTimer(0,1000,NULL);
		if(this->alterType=="Delete")
		{
			if(AfxMessageBox(L"确定删除？",MB_YESNO)==IDYES)
			{
				pView->KillTimer(0);
				this->pdb->DeletePoint(this->gp,this->layer);
				this->hasInitlize=false;
				pView->Invalidate();
			}
			else
			{
				pView->KillTimer(0);
				this->hasInitlize=false;
				this->endflash(pDC);////
			}
		}
		else if(this->alterType=="Move"&&this->hasInitlize)
		{
			pView->KillTimer(0);
			this->lastPoint=pZoom->GPtoCP(this->gp.point);
			//pView->Invalidate();
		}
	}
	if(tempgp.ID!=this->gp.ID&&this->hasInitlize)///////点击在当前点外
	{
		pView->KillTimer(0);
		this->endflash(pDC);////
		this->found=false;
		this->hasInitlize=false;
	}
	pView->ReleaseDC(pDC);
}

void AlterPointTool::RButtonDown(UINT nflags,CPoint point)
{

}

void AlterPointTool::LButtonUp(UINT nflags,CPoint point)
{
	CDC * pDC = pView->GetDC();
	this->LBtDown=false;
	if(this->hasMove&&this->alterType=="Move"&&this->found)
	{
		this->gp.point=pZoom->CPtoGP(point);
		this->pdb->AlterPoint(this->gp,layer,pDC,pZoom);
		this->hasMove=false;
		pView->Invalidate();
	}
	pView->ReleaseDC(pDC);
}

void AlterPointTool::RButtonUp(UINT nflags,CPoint point)
{

}

void AlterPointTool::MouseMove(UINT nflags,CPoint point)
{
	int i=0;
	while(i<2)
	{
		if(this->LBtDown&&this->alterType=="Move"&&this->found)
	    {
			CDC * pDC = pView->GetDC();
			int times1=pZoom->ntimes()*(this->gp.size+5);
			if(times1==0) times1=1;
			int times2=pZoom->ntimes()*(this->gp.size+3);
			if(times2==0) times2=1;
			int times3=pZoom->ntimes()*(this->gp.size+1);
			if(times3==0) times3=1;
			int mode=pDC->SetROP2(R2_NOTXORPEN);
			CPen * pen = new CPen(PS_SOLID,1,this->gp.color);
			CPen * pOldPen = pDC->SelectObject(pen);
			CBrush * brush = new CBrush(this->gp.color);
			CBrush * pOldBrush = pDC->SelectObject(brush);
			if(this->gp.pType=="Circle")
					pDC->Ellipse(lastPoint.x-times1,lastPoint.y-times1,lastPoint.x+times1,lastPoint.y+times1);
			else if(this->gp.pType=="cRect")
				pDC->Rectangle(lastPoint.x-times1,lastPoint.y-times2,lastPoint.x+times1,lastPoint.y+times2);
			else if(this->gp.pType=="zRect")
				pDC->Rectangle(lastPoint.x-times1,lastPoint.y-times1,lastPoint.x+times1,lastPoint.y+times1);
			else if(this->gp.pType=="SJ")
			{
				CPoint _point[3];
				_point[0]=CPoint(lastPoint.x,lastPoint.y-times1);
				_point[1]=CPoint(lastPoint.x-times1,lastPoint.y+times1);
				_point[2]=CPoint(lastPoint.x+times1,lastPoint.y+times1);
				pDC->Polygon(_point,3);
			}
			else if(this->gp.pType=="Ellipes")
				pDC->Ellipse(lastPoint.x-times1,lastPoint.y-times3,lastPoint.x+times1,lastPoint.y+times3);
			this->lastPoint=point;
			this->hasMove=true;
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
			pDC->SetROP2(mode);
			pView->ReleaseDC(pDC);
		}
		i++;
	}
}

void AlterPointTool::flash(CDC* pDC)
{
	BYTE r=GetRValue(this->gp.color);
	BYTE g=GetGValue(this->gp.color);
	BYTE b=GetBValue(this->gp.color);
	int color = RGB(255-r,255-g,255-b);
	if(pDC->GetPixel(pZoom->GPtoCP(this->gp.point))==color)
		color=this->gp.color;
	int times1=pZoom->ntimes()*(this->gp.size+5);
	if(times1==0) times1=1;
	int times2=pZoom->ntimes()*(this->gp.size+3);
	if(times2==0) times2=1;
	int times3=pZoom->ntimes()*(this->gp.size+1);
	if(times3==0) times3=1;
	CPoint scrpoint=pZoom->GPtoCP(this->gp.point);
	CPen * pen = new CPen(PS_SOLID,1,color);
	CPen * pOldPen = pDC->SelectObject(pen);
	CBrush * brush = new CBrush(color);
	CBrush * pOldBrush = pDC->SelectObject(brush);
	if(this->gp.pType=="Circle")
			pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);
	else if(this->gp.pType=="cRect")
		pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times2,scrpoint.x+times1,scrpoint.y+times2);
	else if(this->gp.pType=="zRect")
		pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);
	else if(this->gp.pType=="SJ")
	{
		CPoint _point[3];
		_point[0]=CPoint(scrpoint.x,scrpoint.y-times1);
		_point[1]=CPoint(scrpoint.x-times1,scrpoint.y+times1);
		_point[2]=CPoint(scrpoint.x+times1,scrpoint.y+times1);
		pDC->Polygon(_point,3);
	}
	else if(this->gp.pType=="Ellipes")
		pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times3,scrpoint.x+times1,scrpoint.y+times3);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void AlterPointTool::endflash(CDC* pDC)
{
	int times1=pZoom->ntimes()*(this->gp.size+5);
	if(times1==0) times1=1;
	int times2=pZoom->ntimes()*(this->gp.size+3);
	if(times2==0) times2=1;
	int times3=pZoom->ntimes()*(this->gp.size+1);
	if(times3==0) times3=1;
	CPoint scrpoint=pZoom->GPtoCP(this->gp.point);
	CPen * pen = new CPen(PS_SOLID,1,this->gp.color);
	CPen * pOldPen = pDC->SelectObject(pen);
	CBrush * brush = new CBrush(this->gp.color);
	CBrush * pOldBrush = pDC->SelectObject(brush);
	if(this->gp.pType=="Circle")
			pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);
	else if(this->gp.pType=="cRect")
		pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times2,scrpoint.x+times1,scrpoint.y+times2);
	else if(this->gp.pType=="zRect")
		pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);
	else if(this->gp.pType=="SJ")
	{
		CPoint _point[3];
		_point[0]=CPoint(scrpoint.x,scrpoint.y-times1);
		_point[1]=CPoint(scrpoint.x-times1,scrpoint.y+times1);
		_point[2]=CPoint(scrpoint.x+times1,scrpoint.y+times1);
		pDC->Polygon(_point,3);
	}
	else if(this->gp.pType=="Ellipes")
		pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times3,scrpoint.x+times1,scrpoint.y+times3);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void AlterPointTool::InitlizeObject(CDC* pDC)
{
	if(!this->hasInitlize)
	{
		this->hasInitlize=true;
		this->gp=this->tempgp;
	}
	else if(!(fabs(this->gp.point.x-this->tempgp.point.x)<=(float)(this->tempgp.size+5)*pZoom->ntimes()&&fabs(this->gp.point.y-this->tempgp.point.y)<=(float)(this->tempgp.size+5)*pZoom->ntimes()))////
	{
		this->endflash(pDC);////
		this->gp=this->tempgp;
	}
}




//////////////////////////////////AlterLine/////////////////////////////////////////

void AlterLineTool::getLine(gisLine&gl)
{
	if(gl.LinePoint!=NULL)
		delete [] gl.LinePoint;
	gl.pCount=this->gl.pCount;
	gl.ID=this->gl.ID;
	gl.color=this->gl.color;
	gl.lType=this->gl.lType;
	gl.width=this->gl.width;
	gl.xMax=this->gl.xMax;
	gl.xMin=this->gl.xMin;
	gl.yMin=this->gl.yMin;
	gl.yMax=this->gl.yMax;
	gl.LinePoint = new DPoint[gl.pCount];
	for(int i=0;i<this->gl.pCount;i++)
		gl.LinePoint[i]=this->gl.LinePoint[i];
}



void AlterLineTool::InitlizeObject(CDC* pDC)
{
	if(!this->hasInitlize)
	{
		this->hasInitlize=true;
		this->gl=this->tempgl;
	}
	else if(this->gl.ID!=tempgl.ID)
	{
		this->endflash(pDC);
		this->gl=this->tempgl;
	}
}


void AlterLineTool::LButtonDown(UINT nflags,CPoint point)
{
	this->LBtDown=true;
	CDC * pDC =pView->GetDC();
	this->found=false;
	if(this->pdb->SearchLineByClick(point,this->tempgl,this->layer,this->pZoom))
	{
		this->found=true;
		this->InitlizeObject(pDC);
		if(this->alterType!="Move"&&this->alterType!=L"addPtToLine"&&this->alterType!=L"delPtFromLine"&&this->alterType!=L"MoveLinePt"&&this->alterType!=L"")
		   pView->SetTimer(0,1000,NULL);
		if(this->alterType=="Delete")
		{
			if(AfxMessageBox(L"确定删除？",MB_YESNO)==IDYES)
			{
				pView->KillTimer(0);
				this->pdb->DeleteLine(this->gl,this->layer);
				this->hasInitlize=false;
				pView->Invalidate();
			}
			else
			{
				pView->KillTimer(0);
				this->hasInitlize=false;
				this->endflash(pDC);
			}
		}
		else if(this->alterType=="Move"&&this->hasInitlize)
		{
			pView->KillTimer(0);
			this->lastPoint=point;
			this->moveLine=this->gl;

		}
		else if(this->alterType==L"addPtToLine"&&this->hasInitlize&&gl.width>=0)
		{
			pView->KillTimer(0);
			this->found=false;
			this->pos=-1;
			if(this->pdb->getLinePt(point,sp1,sp2,pos,this->layer,this->pZoom)&&pos!=-1)
			{
				this->found=true;
				this->lastPoint=sp2;
				this->pos=pos;
			}
		}
		else if(this->alterType==L"delPtFromLine"&&this->hasInitlize&&gl.width>=0)
		{
			pView->KillTimer(0);
			if(this->pdb->delPtFromLine(point,layer,this->gl,pZoom))
			{
				this->gl.pCount--;
				this->pdb->AlterLine(this->gl,layer,pDC,pZoom);
				this->hasInitlize=false;
				this->found=false;
			    pView->Invalidate();
			}
		}
		else if(this->alterType==L"MoveLinePt"&&this->hasInitlize&&gl.width>=0)
		{
			pView->KillTimer(0);
			this->found=false;
			this->pos=-1;
			if(this->pdb->MoveLinePt(point,this->sp1,this->sp2,this->pos,layer,this->gl,pZoom))
			{
				this->found=true;
				this->lastPoint=pZoom->GPtoCP(this->gl.LinePoint[pos]);
			}
		}
	}
	if(tempgl.ID!=this->gl.ID&&this->hasInitlize)////
	{
		pView->KillTimer(0);
		this->endflash(pDC);
		this->hasInitlize=false;
	}
	pView->ReleaseDC(pDC);
}

void AlterLineTool::RButtonDown(UINT nflags,CPoint point)
{

}

void AlterLineTool::LButtonUp(UINT nflags,CPoint point)
{
	CDC * pDC = pView->GetDC();
	this->LBtDown=false;
	if(this->alterType=="Move"&&this->hasMove&&this->found)
	{
		this->gl=this->moveLine;
		this->pdb->AlterLine(gl,layer,pDC,pZoom);
	    this->hasMove=false;
		this->found=false;
		pView->Invalidate();
	}
	else if(this->alterType==L"addPtToLine"&&this->found&&gl.width>=0)
	{
		int i=0,j=0;
		DPoint * pt =new DPoint[gl.pCount+1];
		for(;i<gl.pCount;i++,j++)
		{
			if(j==this->pos){pt[j]=pZoom->CPtoGP(point);i--;}
			else pt[j]=gl.LinePoint[i];
		}
		if(gl.LinePoint!=NULL) delete [] gl.LinePoint;
		gl.LinePoint = new DPoint[gl.pCount+1];
		gl.pCount++;
		for(i=0;i<gl.pCount;i++) 
			gl.LinePoint[i]=pt[i];
		this->pdb->AlterLine(gl,layer,pDC,pZoom);
		this->found=false;
		this->hasInitlize=false;
	}
	else if(this->alterType==L"MoveLinePt"&&this->found&&gl.width>=0)
	{
		this->gl.LinePoint[pos]=pZoom->CPtoGP(point);
		this->pdb->AlterLine(gl,layer,pDC,pZoom);
		this->found=false;
		this->hasInitlize=false;
	}
	pView->ReleaseDC(pDC);
}

void AlterLineTool::RButtonUp(UINT nflags,CPoint point)
{

}

void AlterLineTool::MouseMove(UINT nflags,CPoint point)
{
	int i=0;
	this->offsetX=this->offsetY=0;
	if(this->LBtDown&&this->alterType=="Move"&&this->found)
	{
		CDC * pDC = pView->GetDC();
		int mode=pDC->SetROP2(R2_NOTXORPEN);
		LOGBRUSH logBrush;
		logBrush.lbStyle = BS_SOLID;
		CPen pen;
		CPen * pOldPen;
		logBrush.lbColor = this->gl.color;
		pen.CreatePen(this->gl.lType|PS_GEOMETRIC,abs(int(this->gl.width))*pZoom->ntimes(),&logBrush);
		pOldPen=pDC->SelectObject(&pen);
		this->hasMove=true;
		while(i<2)
	    {
			
			if(gl.width>=0)
			{
				int j=0;
				pDC->MoveTo(pZoom->GPtoCP(this->moveLine.LinePoint[j++]));
				while(j<this->moveLine.pCount)
				{
					if(i==1)
					{
						this->moveLine.LinePoint[j].x+=offsetX;
						this->moveLine.LinePoint[j].y+=offsetY;
					}
					pDC->LineTo(pZoom->GPtoCP(this->moveLine.LinePoint[j++]));
				}
				if((++i)==1)
				{
					this->offsetX=pZoom->CPtoGP(point).x-pZoom->CPtoGP(this->lastPoint).x;
					this->offsetY=pZoom->CPtoGP(point).y-pZoom->CPtoGP(this->lastPoint).y;
					this->moveLine.LinePoint[0].x+=offsetX;
					this->moveLine.LinePoint[0].y+=offsetY;
				}
			}
			else
			{
				int nt=0;
				CPoint *pts=new CPoint [gl.pCount];
				while(nt<gl.pCount)
				{
					pts[nt]=pZoom->GPtoCP(moveLine.LinePoint[nt]);
					nt++;
				}
				//nt=0;
				pDC->PolyBezier(pts,gl.pCount);
				this->offsetX=pZoom->CPtoGP(point).x-pZoom->CPtoGP(this->lastPoint).x;
				this->offsetY=pZoom->CPtoGP(point).y-pZoom->CPtoGP(this->lastPoint).y;
				nt=0;
				while(nt<gl.pCount)
				{
					this->moveLine.LinePoint[nt].x+=offsetX;
					this->moveLine.LinePoint[nt].y+=offsetY;
					nt++;
				}
				nt=0;
				while(nt<gl.pCount)
				{
					pts[nt]=pZoom->GPtoCP(moveLine.LinePoint[nt]);
					nt++;
				}
				pDC->PolyBezier(pts,gl.pCount);
				delete []pts;
				i=2;
			}

		}
		this->lastPoint=point;
		pDC->SetROP2(mode);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
		pView->ReleaseDC(pDC);
	}
	else if(this->LBtDown&&this->alterType==L"addPtToLine"&&this->found&&gl.width>=0)
	{
		CDC * pDC = pView->GetDC();
		int mode=pDC->SetROP2(R2_NOTXORPEN);
		LOGBRUSH logBrush;
		logBrush.lbStyle = BS_SOLID;
		CPen pen;
		CPen * pOldPen;
		logBrush.lbColor = this->gl.color;
		pen.CreatePen(this->gl.lType|PS_GEOMETRIC,this->gl.width*pZoom->ntimes(),&logBrush);
		pOldPen=pDC->SelectObject(&pen);

		pDC->MoveTo(this->sp1);
		pDC->LineTo(this->lastPoint);
		pDC->LineTo(this->sp2);

		pDC->MoveTo(this->sp1);
		pDC->LineTo(point);
		pDC->LineTo(this->sp2);

		this->lastPoint=point;
		pen.DeleteObject();
		pDC->SetROP2(mode);
		pDC->SelectObject(pOldPen);
	}
	else if(this->LBtDown&&this->alterType==L"MoveLinePt"&&this->found&&gl.width>=0)
	{
		CDC * pDC = pView->GetDC();
		int mode=pDC->SetROP2(R2_NOTXORPEN);
		LOGBRUSH logBrush;
		logBrush.lbStyle = BS_SOLID;
		CPen pen;
		CPen * pOldPen;
		logBrush.lbColor = this->gl.color;
		pen.CreatePen(this->gl.lType|PS_GEOMETRIC,this->gl.width*pZoom->ntimes(),&logBrush);
		pOldPen=pDC->SelectObject(&pen);
		if(this->pos!=0&&this->pos!=-1&&this->pos!=this->gl.pCount-1)
		{
			pDC->MoveTo(this->sp1);
			pDC->LineTo(this->lastPoint);
			pDC->LineTo(this->sp2);
			//////////////////////////////
			pDC->MoveTo(this->sp1);
			pDC->LineTo(point);
			pDC->LineTo(this->sp2);
			this->lastPoint=point;
		}
		else if(this->pos==0)
		{
			pDC->MoveTo(this->lastPoint);
			pDC->LineTo(this->sp2);
			////
			pDC->MoveTo(point);
			pDC->LineTo(this->sp2);
			this->lastPoint=point;
		}
		else if(this->pos==this->gl.pCount-1)
		{
			pDC->MoveTo(this->sp1);
			pDC->LineTo(this->lastPoint);
			/////
			pDC->MoveTo(this->sp1);
			pDC->LineTo(point);
			this->lastPoint=point;
		}
		pen.DeleteObject();
		pDC->SetROP2(mode);
		pDC->SelectObject(pOldPen);
	}
}

void AlterLineTool::flash(CDC* pDC)
{
	CPen pen;
	CPen * pOldPen;
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	BYTE r=GetRValue(this->gl.color);
	BYTE g=GetGValue(this->gl.color);
	BYTE b=GetBValue(this->gl.color);
	int color = RGB(255-r,255-g,255-b);
	if(pDC->GetPixel(pZoom->GPtoCP(gl.LinePoint[0]))==color)
		color=gl.color;
	logBrush.lbColor = color;
	pen.CreatePen(gl.lType|PS_GEOMETRIC,abs(int(gl.width))*pZoom->ntimes(),&logBrush);
	pOldPen=pDC->SelectObject(&pen);
	if(gl.width>=0)
	{
		int j=0;
		pDC->MoveTo(pZoom->GPtoCP(gl.LinePoint[j++]));
		while(j<gl.pCount)
			pDC->LineTo(pZoom->GPtoCP(gl.LinePoint[j++]));
	}
	else
	{
		int nt=0;
		CPoint *pts=new CPoint[gl.pCount];
		while(nt<gl.pCount)
		{
			pts[nt]=pZoom->GPtoCP(gl.LinePoint[nt]);
			nt++;
		}
		pDC->PolyBezier(pts,gl.pCount);
		delete []pts;
	}
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void AlterLineTool::endflash(CDC* pDC)
{
	CPen pen;
	CPen * pOldPen;
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = this->gl.color;
	pen.CreatePen(gl.lType|PS_GEOMETRIC,abs(int(gl.width))*pZoom->ntimes(),&logBrush);
	pOldPen=pDC->SelectObject(&pen);
	if(gl.width>=0)
	{
		int j=0;
		pDC->MoveTo(pZoom->GPtoCP(gl.LinePoint[j++]));
		while(j<gl.pCount)
			pDC->LineTo(pZoom->GPtoCP(gl.LinePoint[j++]));
	}
	else
	{
		int nt=0;
		CPoint *pts=new CPoint[gl.pCount];
		while(nt<gl.pCount)
		{
			pts[nt]=pZoom->GPtoCP(gl.LinePoint[nt]);
			nt++;
		}
		pDC->PolyBezier(pts,gl.pCount);
		delete []pts;
	}
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}




//////////////////////////////////AlterPolygon/////////////////////////////////////////

void AlterPolygonTool::getPoly(gisPolygon&gp)
{
	if(gp.point!=NULL)
		delete [] gp.point;
	gp.pCount=this->gPoly.pCount;
	gp.ID=this->gPoly.ID;
	gp.BorderColor=this->gPoly.BorderColor;
	gp.BorderType=this->gPoly.BorderType;
	gp.color=this->gPoly.color;
	gp.xMax=this->gPoly.xMax;
	gp.xMin=this->gPoly.xMin;
	gp.yMin=this->gPoly.yMin;
	gp.yMax=this->gPoly.yMax;
	gp.point = new DPoint[gp.pCount];
	for(int i=0;i<gp.pCount;i++)
		gp.point[i]=this->gPoly.point[i];
}



void AlterPolygonTool::InitlizeObject(CDC* pDC)
{
	if(!this->hasInitlize)
	{
		this->hasInitlize=true;
		this->gPoly=this->tempgPoly;
	}
	else if(this->gPoly.ID!=this->tempgPoly.ID)
	{
		this->endflash(pDC);
		this->gPoly=this->tempgPoly;
	}
}


void AlterPolygonTool::LButtonDown(UINT nflags,CPoint point)
{
	this->LBtDown=true;
	CDC * pDC =pView->GetDC();
	this->found=false;
	if(this->pdb->SearchPolyByClick(point,this->tempgPoly,this->layer,this->pZoom))
	{
		this->found=true;
		this->InitlizeObject(pDC);
		if(this->alterType!="Move"&&this->alterType!=L"addPtToPolygon"&&this->alterType!=L"delPtFromPolygon"&&this->alterType!=L"MovePolygonPt"&&this->alterType!=L"")
		   pView->SetTimer(0,1000,NULL);
		if(this->alterType=="Delete")
		{
			if(AfxMessageBox(L"确定删除？",MB_YESNO)==IDYES)
			{
				pView->KillTimer(0);
				this->pdb->DeletePolygon(this->gPoly,this->layer);
				this->hasInitlize=false;
				pView->Invalidate();
			}
			else
			{
				pView->KillTimer(0);
				this->hasInitlize=false;
				this->endflash(pDC);
			}
		}
		else if(this->alterType=="Move"&&this->hasInitlize)
		{
			pView->KillTimer(0);
			this->lastPoint=point;
			this->movePolygon=this->gPoly;

		}
		else if(this->alterType==L"addPtToPolygon"&&this->hasInitlize)
		{
			pView->KillTimer(0);
			this->found=false;
			this->pos=-1;
			if(this->pdb->getPolygonPt(point,sp1,sp2,pos,this->layer,this->gPoly,this->pZoom))
			{
				this->found=true;
				this->lastPoint=sp2;
				this->pos=pos;
			}
		}
		else if(this->alterType==L"delPtFromPolygon"&&this->hasInitlize)
		{
			pView->KillTimer(0);
			if(this->pdb->delPtFromPolygon(point,layer,this->gPoly,pZoom))
			{
				this->gPoly.pCount--;
				this->pdb->AlterPolygon(this->gPoly,layer,pDC,pZoom);
				this->hasInitlize=false;
				this->found=false;
			    pView->Invalidate();
			}
		}
		else if(this->alterType==L"MovePolygonPt"&&this->hasInitlize)
		{
			pView->KillTimer(0);
			this->found=false;
			this->pos=-1;
			if(this->pdb->MovePolygonPt(point,this->sp1,this->sp2,this->pos,layer,this->gPoly,pZoom))
			{
				this->found=true;
				this->lastPoint=pZoom->GPtoCP(this->gPoly.point[pos]);
			}
		}
	}
	if(tempgPoly.ID!=this->gPoly.ID&&this->hasInitlize)///////点击在当前线外
	{
		pView->KillTimer(0);
		this->endflash(pDC);
		this->hasInitlize=false;
	}
	pView->ReleaseDC(pDC);
}

void AlterPolygonTool::RButtonDown(UINT nflags,CPoint point)
{

}

void AlterPolygonTool::LButtonUp(UINT nflags,CPoint point)
{
	CDC * pDC = pView->GetDC();
	this->LBtDown=false;
	if(this->alterType=="Move"&&this->hasMove&&this->found)
	{
		this->gPoly=this->movePolygon;
		this->pdb->AlterPolygon(this->gPoly,layer,pDC,pZoom);
	    this->hasMove=false;
		pView->Invalidate();
	}
	else if(this->alterType==L"addPtToPolygon"&&this->found)
	{
		int i=0,j=0;
		DPoint * pt =new DPoint[gPoly.pCount+1];
		for(;i<gPoly.pCount;i++,j++)
		{
			if(j==(this->pos)%this->gPoly.pCount)
			{
				pt[j]=pZoom->CPtoGP(point);
				i--;
			}
			else pt[j]=gPoly.point[i];
		}
		if(gPoly.point!=NULL) delete [] gPoly.point;
		gPoly.point = new DPoint[gPoly.pCount+1];
		gPoly.pCount++;
		for(i=0;i<gPoly.pCount;i++) 
			gPoly.point[i]=pt[i];
		this->pdb->AlterPolygon(gPoly,layer,pDC,pZoom);
		this->found=false;
		this->hasInitlize=false;
		pView->Invalidate();
	}
	else if(this->alterType==L"MovePolygonPt"&&this->found)
	{
		this->gPoly.point[pos%this->gPoly.pCount]=pZoom->CPtoGP(point);
		this->pdb->AlterPolygon(this->gPoly,layer,pDC,pZoom);
		this->found=false;
		this->hasInitlize=false;
		pView->Invalidate();
	}
	pView->ReleaseDC(pDC);
}

void AlterPolygonTool::RButtonUp(UINT nflags,CPoint point)
{

}

void AlterPolygonTool::MouseMove(UINT nflags,CPoint point)
{
	if(this->LBtDown&&this->alterType=="Move"&&this->found)
	{
		int i=0;
		this->offsetX=this->offsetY=0;
		CPoint * p = new CPoint[this->gPoly.pCount];
		while(i<2)
	    {
			CPen pen;
			CBrush * brush;
			CPen * pOldPen;
			CBrush * pOldBrush;
			CDC * pDC = pView->GetDC();
			int mode=pDC->SetROP2(R2_NOTXORPEN);
			pen.CreatePen(this->gPoly.BorderType,1,this->gPoly.BorderColor);
			pOldPen = pDC->SelectObject(&pen);
			brush =new CBrush(this->gPoly.color);
			pOldBrush =pDC->SelectObject(brush);
			int j=0;
			p[j]=pZoom->GPtoCP(this->movePolygon.point[j]);
			pDC->MoveTo(pZoom->GPtoCP(this->movePolygon.point[j++]));
			while(j<this->movePolygon.pCount)
			{
				if(i==1)
				{
					this->movePolygon.point[j].x+=this->offsetX;
					this->movePolygon.point[j].y+=this->offsetY;
				}
				p[j]=pZoom->GPtoCP(this->movePolygon.point[j]);
				pDC->LineTo(pZoom->GPtoCP(this->movePolygon.point[j++]));
			}
			pDC->LineTo(pZoom->GPtoCP(this->movePolygon.point[0]));
			if((++i)==1)
			{
				this->offsetX=pZoom->CPtoGP(point).x-pZoom->CPtoGP(this->lastPoint).x;
			    this->offsetY=pZoom->CPtoGP(point).y-pZoom->CPtoGP(this->lastPoint).y;
				this->movePolygon.point[0].x+=this->offsetX;
				this->movePolygon.point[0].y+=this->offsetY;
			}
			pDC->Polygon(p,this->gPoly.pCount);
			this->hasMove=true;
			pDC->SetROP2(mode);
			pDC->SelectObject(pOldPen);
			pView->ReleaseDC(pDC);
			this->lastPoint=point;
		}
		delete [] p;
	}
	else if(this->LBtDown&&this->alterType==L"addPtToPolygon"&&this->found)
	{
		CDC * pDC = pView->GetDC();
		int mode=pDC->SetROP2(R2_NOTXORPEN);
		CPen pen;
		CPen * pOldPen;
		pen.CreatePen(PS_SOLID,1,this->gPoly.BorderColor);
		pOldPen=pDC->SelectObject(&pen);

		pDC->MoveTo(this->sp1);
		pDC->LineTo(this->lastPoint);
		pDC->LineTo(this->sp2);

		pDC->MoveTo(this->sp1);
		pDC->LineTo(point);
		pDC->LineTo(this->sp2);

		this->lastPoint=point;
		pen.DeleteObject();
		pDC->SetROP2(mode);
		pDC->SelectObject(pOldPen);
	}
	else if(this->LBtDown&&this->alterType==L"MovePolygonPt"&&this->found)
	{
		CDC * pDC = pView->GetDC();
		int mode=pDC->SetROP2(R2_NOTXORPEN);
		CPen pen;
		CPen * pOldPen;
		pen.CreatePen(PS_SOLID,1,this->gPoly.BorderColor);
		pOldPen=pDC->SelectObject(&pen);
		if(this->pos!=-1)
		{
			pDC->MoveTo(this->sp1);
			pDC->LineTo(this->lastPoint);
			pDC->LineTo(this->sp2);
			//////////////////////////////
			pDC->MoveTo(this->sp1);
			pDC->LineTo(point);
			pDC->LineTo(this->sp2);
			this->lastPoint=point;
		}
		pen.DeleteObject();
		pDC->SetROP2(mode);
		pDC->SelectObject(pOldPen);
	}
}

void AlterPolygonTool::flash(CDC* pDC)
{
	CPen pen;
	CPen * pOldPen;
	CBrush * pOldBrush;
	CBrush * brush;
	CPoint * p = new CPoint[this->gPoly.pCount];
	BYTE r=GetRValue(this->gPoly.color);
	BYTE g=GetGValue(this->gPoly.color);
	BYTE b=GetBValue(this->gPoly.color);
	int color = RGB(255-r,255-g,255-b);
	if(pDC->GetPixel(pZoom->GPtoCP(this->gPoly.point[0]))==color)
		color=this->gPoly.color;
	brush=new CBrush(color);
	pOldBrush=pDC->SelectObject(brush);//
	pen.CreatePen(this->gPoly.BorderType,1,color);
	pOldPen=pDC->SelectObject(&pen);///
	int i=0;
	p[i]=pZoom->GPtoCP(this->gPoly.point[i]);
	pDC->MoveTo(pZoom->GPtoCP(this->gPoly.point[i++]));///
	while(i<this->gPoly.pCount)
	{
		p[i]=pZoom->GPtoCP(this->gPoly.point[i]);
		pDC->LineTo(pZoom->GPtoCP(this->gPoly.point[i++]));//
	}
	pDC->LineTo(pZoom->GPtoCP(this->gPoly.point[0]));///
	pDC->Polygon(p,this->gPoly.pCount);//
	pDC->SelectObject(pOldBrush);///
	pDC->SelectObject(pOldPen);//
	pen.DeleteObject();
	delete [] p;
}

void AlterPolygonTool::endflash(CDC* pDC)
{
	CPen pen;
	CPen * pOldPen;
	CBrush * brush;
	CBrush * pOldBrush;
	CPoint * p = new CPoint[this->gPoly.pCount];
	brush=new CBrush(this->gPoly.color);
	pOldBrush=pDC->SelectObject(brush);//
	pen.CreatePen(this->gPoly.BorderType,1,this->gPoly.BorderColor);
	pOldPen=pDC->SelectObject(&pen);///
	int i=0;
	p[i]=pZoom->GPtoCP(this->gPoly.point[i]);
	pDC->MoveTo(pZoom->GPtoCP(this->gPoly.point[i++]));///
	while(i<this->gPoly.pCount)
	{
		p[i]=pZoom->GPtoCP(this->gPoly.point[i]);
		pDC->LineTo(pZoom->GPtoCP(this->gPoly.point[i++]));//
	}
	pDC->LineTo(pZoom->GPtoCP(this->gPoly.point[0]));///
	pDC->Polygon(p,this->gPoly.pCount);//
	pDC->SelectObject(pOldBrush);///
	pDC->SelectObject(pOldPen);//
	pen.DeleteObject();
	delete [] p;
}



//////////////////////////////alterMarker///////////////////////////

void AlterMarkerTool::getMarker(gisMarker&gm)
{
	gm.color=this->gm.color;
	gm.ID=this->gm.ID;
	gm.lfont=this->gm.lfont;
	gm.point=this->gm.point;
	gm.text=this->gm.text;
}


void AlterMarkerTool::InitlizeObject(CDC* pDC)
{
	if(!this->hasInitlize)
	{
		this->hasInitlize=true;
		this->gm=this->tempgm;
	}
	else if(this->gm.ID!=this->tempgm.ID)
	{
		this->endflash(pDC);
		this->gm=this->tempgm;
	}
}


void AlterMarkerTool::LButtonDown(UINT nflags,CPoint point)
{
	this->LBtDown=true;
	CDC * pDC =pView->GetDC();
	this->found=false;
	if(this->pdb->SearchMarkerByClick(point,this->tempgm,this->layer,pDC,this->pZoom))
	{
		this->found=true;
		this->InitlizeObject(pDC);
		pView->SetTimer(0,1000,NULL);
		if(this->alterType=="Delete")
		{
			if(AfxMessageBox(L"确定删除？",MB_YESNO)==IDYES)
			{
				pView->KillTimer(0);
				this->pdb->DeleteMarker(this->gm,this->layer);
				this->hasInitlize=false;
				pView->Invalidate();
			}
			else
			{
				pView->KillTimer(0);
				this->hasInitlize=false;
				this->endflash(pDC);
			}
		}
		else if(this->alterType=="Move"&&this->hasInitlize)
		{
			this->moveMarker=this->gm;
			this->lastPoint=pZoom->GPtoCP(this->gm.point);
			HFONT hfont = ::CreateFontIndirect(&this->gm.lfont);
			CFont* pfont = CFont::FromHandle(hfont);
			CFont *oldfont=pDC->SelectObject(pfont);
			CSize size = pDC->GetTextExtent(this->gm.text);
			size.cx*=pZoom->ntimes();
			size.cy*=pZoom->ntimes();
			pDC->SelectObject(pfont);
			pDC->SelectStockObject(NULL_BRUSH);
			CRect * r = new CRect(pZoom->GPtoCP(this->gm.point),size);
			pDC->Rectangle(r);
		}
	}
	if(tempgm.ID!=this->gm.ID&&this->hasInitlize)
	{
		pView->KillTimer(0);
		this->endflash(pDC);
		this->hasInitlize=false;
	}
	pView->ReleaseDC(pDC);
}

void AlterMarkerTool::RButtonDown(UINT nflags,CPoint point)
{

}

void AlterMarkerTool::LButtonUp(UINT nflags,CPoint point)
{
	CDC * pDC = pView->GetDC();
	this->LBtDown=false;
	if(this->alterType=="Move"&&this->hasMove&&this->found)
	{
		this->gm=this->moveMarker;
		this->pdb->AlterMarker(this->gm,layer,pDC,pZoom);
	    this->hasMove=false;
		pView->KillTimer(0);
		pView->Invalidate();
	}
	pView->ReleaseDC(pDC);
}

void AlterMarkerTool::RButtonUp(UINT nflags,CPoint point)
{

}

void AlterMarkerTool::MouseMove(UINT nflags,CPoint point)
{
	if(this->LBtDown&&this->alterType=="Move"&&this->found)
	{
		LOGFONT lfont=moveMarker.lfont;
		int nwheight=int(this->moveMarker.lfont.lfHeight*pZoom->ntimes());
		if(abs(nwheight)<=5)
		{
			int a=nwheight/abs(nwheight);
			nwheight=a*5;
		}
		lfont.lfHeight=nwheight;
		CDC * pDC = pView->GetDC();
		int mode = pDC->SetROP2(R2_NOTXORPEN);
		HFONT hfont = ::CreateFontIndirect(&lfont);
		CFont* pfont = CFont::FromHandle(hfont);
		CFont *oldfont=pDC->SelectObject(pfont);
		CSize size = pDC->GetTextExtent(this->gm.text);
		pDC->SelectObject(pfont);
		pDC->SelectStockObject(NULL_BRUSH);
		CRect * r = new CRect(this->lastPoint,size);
		pDC->Rectangle(r);
		r = new CRect(point,size);
		pDC->Rectangle(r);
		this->lastPoint=point;
		this->moveMarker.point=pZoom->CPtoGP(point);
		this->hasMove=true;
		pDC->SetROP2(mode);
		::DeleteObject(hfont);
	}
}

void AlterMarkerTool::flash(CDC* pDC)
{
	BYTE r=GetRValue(this->gm.color);
	BYTE g=GetGValue(this->gm.color);
	BYTE b=GetBValue(this->gm.color);
	if(this->CurrentColor!=gm.color)
		this->CurrentColor=gm.color;
	else this->CurrentColor=RGB(255-r,255-g,255-b);
	LOGFONT lfont=gm.lfont;
	int nwheight=int(this->gm.lfont.lfHeight*pZoom->ntimes());
	if(abs(nwheight)<=5)
	{
		int a=nwheight/abs(nwheight);
		nwheight=a*5;
	}
	lfont.lfHeight=nwheight;
	HFONT hfont = ::CreateFontIndirect(&lfont);
	CFont* pfont = CFont::FromHandle(hfont);
	CFont *oldfont=pDC->SelectObject(pfont);
	COLORREF oldcolor=pDC->SetTextColor(this->CurrentColor);///
	int nbk=pDC->SetBkMode(TRANSPARENT);///
	pDC->TextOutW(pZoom->GPtoCP(gm.point).x,pZoom->GPtoCP(gm.point).y,gm.text);///
	pDC->SetTextColor(oldcolor);//
	pDC->SelectObject(oldfont);//
	pDC->SetBkMode(nbk);//
	::DeleteObject(hfont);
}

void AlterMarkerTool::endflash(CDC* pDC)
{
	int nwheight=int(this->gm.lfont.lfHeight*pZoom->ntimes());
	LOGFONT lfont=gm.lfont;
	if(abs(nwheight)<=5)
	{
		int a=nwheight/abs(nwheight);
		nwheight=a*5;
	}
	lfont.lfHeight=nwheight;
	HFONT hfont = ::CreateFontIndirect(&lfont);
	CFont* pfont = CFont::FromHandle(hfont);
	CFont *oldfont=pDC->SelectObject(pfont);
	COLORREF oldcolor=pDC->SetTextColor(gm.color);///
	int nbk=pDC->SetBkMode(TRANSPARENT);///
	pDC->TextOutW(pZoom->GPtoCP(gm.point).x,pZoom->GPtoCP(gm.point).y,gm.text);///
	pDC->SetTextColor(oldcolor);//
	pDC->SelectObject(oldfont);//
	pDC->SetBkMode(nbk);//
	::DeleteObject(hfont);
}



