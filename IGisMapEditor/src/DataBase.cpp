#include "StdAfx.h"
#include "math.h"
#include"DataTemplate.h"
#include "DataBase.h"


DBConnection::DBConnection(void)
{
	CoInitialize(NULL);
	pConn.CreateInstance("ADODB.Connection");
	pRst.CreateInstance(__uuidof(Recordset));
	this->m_isConnection=false;
	pos=-1;
}


DBConnection::~DBConnection(void)
{
	if(this->m_isConnection)
	{
	    pConn->Close();
	    pConn.Release();
	    CoUninitialize();
		this->m_isConnection=false;
	}
}

bool DBConnection::Open(CString server,CString dbname,CString uid,CString paw)
{
	CString str=_T("Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;");
	str+="User ID=";
	str+=uid;
	str+=";";
	str+="Initial Catalog=";
	str+=dbname;
	str+=";";
	str+="Data Source=";
	str+=server;
	str+=";";
	str+="Password=";
	str+=paw;
	str+=";";
	try
	{
		if(!this->m_isConnection)
		{
			pConn->ConnectionString=(_bstr_t)str;
			pConn->Open(pConn->ConnectionString,(_bstr_t)uid,(_bstr_t)paw,adConnectUnspecified);
		    this->m_isConnection=true;
			return true;
		}
		return false;
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
		this->m_isConnection=false;
		return false;
	}
}


_RecordsetPtr DBConnection::ExecuteSQL(CString sql)
{
	try
	{
	   pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	   return pRst;
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
		return NULL;
	}
}

CString DBConnection::GetResultByAttr(CString attribute)
{
	try
	{
		BSTR BSTRtemp=_T("");
		BSTRtemp=(BSTR)((_bstr_t)pRst->GetCollect((_variant_t)attribute));
		return (CString)BSTRtemp;
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
		return NULL;
	}
}


void DBConnection::disConnection()
{
	try
	{
		if(this->m_isConnection)
	    {
			if(pConn!=NULL)
			{
				pConn->Close();
				pConn.Release();
				pConn=NULL;
			}
			pConn.CreateInstance("ADODB.Connection");
			CoUninitialize();
			this->m_isConnection=false;
	    }
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
	}
}

int strtoi(CString str)
{
	int result=0;
	result=_tstoi(str);
	return result;
}

double strtof(CString str)
{
	int npos=str.Find(L'.');
	double result=0;
	if(npos!=-1)
	{
		CString intpart=str.Left(npos);
		int bits=str.GetLength()-npos-1;
		CString fraction=str.Right(bits);
		int i=_tstoi(intpart);
		int f=_tstoi(fraction);
		result=f/pow((float)10,bits);
		result=result+i;
	}
	else result=_tstoi(str);
	return result;
}

void DBConnection::addPoint(gisPoint&gp,CString table)
{
	CString pID;
	CString sql;
	sql.Format(_T("select * from [%s]"),table);
	try
	{
		pRst=ExecuteSQL(sql);
		if(!pRst->End)
		{
			sql.Format(_T("select max(ID)+1 pID from [%s]"),table);
			pRst=ExecuteSQL(sql);
			pID=(CString)(BSTR)((_bstr_t)pRst->GetCollect((_variant_t)"pID"));
		}
		else pID="1";
		sql.Format(_T("insert into [%s] values('%s','%f','%f','%d','%d','%s')"),table,pID,gp.point.x,gp.point.y,gp.color,gp.size,gp.pType);
		ExecuteSQL(sql);
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
	}
}


void DBConnection::addLine(gisLine&gl,CString table)
{
	int i=0;
	CString LID;
	CString tempPoint;
	CString sql;
	CString pointStr=L"";
	gl.xMin=gl.LinePoint[0].x;gl.yMin=gl.LinePoint[0].y;gl.xMax=gl.LinePoint[0].x;gl.yMax=gl.LinePoint[0].y;
	while(i<gl.pCount)
	{
		if(gl.LinePoint[i].x<gl.xMin) gl.xMin=gl.LinePoint[i].x;
		else if(gl.LinePoint[i].x>gl.xMax) gl.xMax=gl.LinePoint[i].x;
		if(gl.LinePoint[i].y<gl.yMin) gl.yMin=gl.LinePoint[i].y;
		else if(gl.LinePoint[i].y>gl.yMax) gl.yMax=gl.LinePoint[i].y;
		tempPoint.Format(L"%f,%f,",gl.LinePoint[i].x,gl.LinePoint[i].y);
		pointStr+=tempPoint;
		i++;
	}
	sql.Format(_T("select * from [%s]"),table);
	try
	{
		_RecordsetPtr pSet=ExecuteSQL(sql);
		if(!pSet->End)
		{
			sql.Format(_T("select max(ID)+1 LID from [%s]"),table);
			pSet=ExecuteSQL(sql);
			LID=(CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)"LID"));
		}
		else LID="1";
		sql.Format(_T("insert into [%s] values('%s','%d','%d','%d','%f','%f','%f','%f','%s','%d')"),table,LID,gl.color,gl.lType,gl.width,gl.xMin,gl.yMin,gl.xMax,gl.yMax,pointStr,gl.pCount);
		ExecuteSQL(sql);
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
	}	
}


void DBConnection::addPolygon(gisPolygon&gp,CString table)
{
	CString pID;
	CString tempPoint;
	CString sql;
	CString pointStr=L"";
	int i=0;
	gp.xMin=gp.point[0].x,gp.yMin=gp.point[0].y,gp.xMax=gp.point[0].x,gp.yMax=gp.point[0].y;
	while(i<gp.pCount)
	{
		if(gp.point[i].x<gp.xMin) gp.xMin=gp.point[i].x;
		else if(gp.point[i].x>gp.xMax) gp.xMax=gp.point[i].x;
		if(gp.point[i].y<gp.yMin) gp.yMin=gp.point[i].y;
		else if(gp.point[i].y>gp.yMax) gp.yMax=gp.point[i].y;
		tempPoint.Format(L"%f,%f,",gp.point[i].x,gp.point[i].y);
		pointStr+=tempPoint;
		i++;
	}
	sql.Format(_T("select * from [%s]"),table);
	try
	{
		pRst=ExecuteSQL(sql);
		if(!pRst->End)
		{
			sql.Format(_T("select max(ID)+1 pID from [%s]"),table);
			pRst=ExecuteSQL(sql);
			pID=(CString)(BSTR)((_bstr_t)pRst->GetCollect((_variant_t)"pID"));
		}
		else pID="1";
		sql.Format(_T("insert into [%s] values('%s','%d','%d','%d','%s','%d','%f','%f','%f','%f')"),table,pID,gp.pCount,gp.BorderType,gp.color,pointStr,gp.BorderColor,gp.xMin,gp.yMin,gp.xMax,gp.yMax);
		ExecuteSQL(sql);
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
	}
}

_RecordsetPtr DBConnection::getRecordSet(CString table)
{
	CString sql;
	sql.Format(_T("select * from [%s]"),table);
	try
	{
		pRst=this->ExecuteSQL(sql);
	    return pRst;
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
		return NULL;
	}
	
}

void DBConnection::OutputImagery(CDC *pDC,CString table,CZoomManager *pZoom,bool pixel)
{
	CString sql;
	CString LID;
	CString pID;
	CPen* pOldPen;
	CPen  pen;
	CBrush * brush;
	CBrush * pOldBrush;
	DPoint point;
	LOGFONT lof;
	int penStyle;
	int pCount;
	int color;
	int type;
	CString Type;
	int LWidth;
	_RecordsetPtr  LPRst;
	DPoint * nPoint;
	int size;
	int i=0;
	if(!this->isConnection())
	{
		AfxMessageBox(L"数据库未连接！");
		return;
	}
	sql.Format(_T("select * from CatalogTable where gisTable='%s'"),table);
	pRst=this->ExecuteSQL(sql);
	if(!pRst->End)
	{
		type=strtoi(GetResultByAttr(L"Type"));
		pRst=getRecordSet(table);
		switch(type)
		{
		case 1:
			while(!IsEnd())
			{
				color=strtoi(GetResultByAttr(_T("color")));
				point.x=strtof(GetResultByAttr(_T("pX")));
				point.y=strtof(GetResultByAttr(_T("pY")));
				size=strtoi(GetResultByAttr(_T("size")));
				Type=GetResultByAttr(_T("type"));
				pen.CreatePen(PS_SOLID,1,color);
				brush=new CBrush(color);
				pOldPen =pDC->SelectObject(&pen);
				pOldBrush=pDC->SelectObject(brush);
				CPoint scrpoint=pZoom->GPtoCP(point);
				int times1=pZoom->ntimes()*(size+5);
				if(times1==0) times1=1;
				int times2=pZoom->ntimes()*(size+3);
				if(times2==0) times2=1;
				int times3=pZoom->ntimes()*(size+1);
				if(times3==0) times3=1;
				if(!pixel)
				{
					CSize sz(times1*100,times2*100);
					pDC->HIMETRICtoDP(&sz);
					times1=sz.cx;
					times2=sz.cy;
					CSize sz2(times3*100,times2*100);
					pDC->HIMETRICtoDP(&sz2);
					times3=sz2.cx;
				}
				if(Type=="Circle")
					pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);///
				else if(Type=="cRect")
					pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times2,scrpoint.x+times1,scrpoint.y+times2);///
				else if(Type=="zRect")
					pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);//
				else if(Type=="SJ")
				{
					CPoint _point[3];
					_point[0]=CPoint(scrpoint.x,scrpoint.y-times1);
					_point[1]=CPoint(scrpoint.x-times1,scrpoint.y+times1);
					_point[2]=CPoint(scrpoint.x+times1,scrpoint.y+times1);
					pDC->Polygon(_point,3);///
				}
				else if(Type=="Ellipes")
					pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times3,scrpoint.x+times1,scrpoint.y+times3);///
				pDC->SelectObject(pOldBrush);//
				pDC->SelectObject(pOldPen);//
				pen.DeleteObject();
				MoveNext();
			}
			break;
		case 2:
			LOGBRUSH logBrush;
			logBrush.lbStyle = BS_SOLID;
			while(!this->IsEnd())
			{
				LID=(CString)this->GetResultByAttr(_T("ID"));
				pCount=strtoi(this->GetResultByAttr(_T("pCount")));
				color=strtoi(this->GetResultByAttr(_T("LColor")));
				logBrush.lbColor = color;
				Type=this->GetResultByAttr(_T("LType"));
				penStyle=strtoi(Type);
				LWidth=strtoi(this->GetResultByAttr(_T("LWidth")));
				bool curve=false;
				if(LWidth<0)
				{curve=true;LWidth=(-1)*LWidth;}
				nPoint=new DPoint[pCount];
				int viewWidth=int(pZoom->ntimes()*LWidth);
				if(viewWidth==0) viewWidth=1;
				LWidth=viewWidth;
				if(!pixel)
				{
					CSize sz(LWidth*100,LWidth*100);
					pDC->HIMETRICtoDP(&sz);
					LWidth=sz.cx;
				}
				pen.CreatePen(penStyle|PS_GEOMETRIC|PS_ENDCAP_ROUND|PS_JOIN_ROUND,LWidth,&logBrush);
				pOldPen=pDC->SelectObject(&pen);////
				this->readImgPoint(table,LID,nPoint,pCount);
				if(!curve)
				{
					pDC->MoveTo(pZoom->GPtoCP(nPoint[i++]));///
					while(i<pCount)
						pDC->LineTo(pZoom->GPtoCP(nPoint[i++]));///
					i=0;
				}
				else
				{
					i=0;
					CPoint *npt=new CPoint [pCount];
					while(i<pCount)
					{npt[i]=pZoom->GPtoCP(nPoint[i]);i++;}///
					i=0;
					pDC->PolyBezier(npt,pCount);
					delete []npt;
				}

				pDC->SelectObject(pOldPen);///
				pen.DeleteObject();
				MoveNext();
				delete [] nPoint;
			}
			break;
		case 3:
			int borderColor;
			while(!this->IsEnd())
			{
				pID=(CString)this->GetResultByAttr(_T("ID"));
				pCount=strtoi(this->GetResultByAttr(_T("pCount")));
				color=strtoi(this->GetResultByAttr(_T("color")));
				penStyle=strtoi(this->GetResultByAttr(_T("BorderType")));
				borderColor=strtoi(this->GetResultByAttr(_T("BorderColor")));
				nPoint=new DPoint[pCount];
				CPoint* scnPoint=new CPoint[pCount];
				int npw=1;
				if(!pixel)
				{
					CSize sz(100,100);
					pDC->HIMETRICtoDP(&sz);
					npw=sz.cx;
				}
				pen.CreatePen(penStyle,npw,borderColor);
				pOldPen=pDC->SelectObject(&pen);///
				this->readImgPoint(table,pID,nPoint,pCount);
				for(int psn=0;psn<pCount;psn++)
					scnPoint[psn]=pZoom->GPtoCP(nPoint[psn]);
				pDC->MoveTo(scnPoint[i++]);///
				while(i<pCount)
					pDC->LineTo(scnPoint[i++]);///
				pDC->LineTo(pZoom->GPtoCP(nPoint[0]));///
				brush=new CBrush(color);
				pOldBrush=pDC->SelectObject(brush);//
				pDC->Polygon(scnPoint,pCount);//
				pDC->SelectObject(pOldBrush);///
				pDC->SelectObject(pOldPen);//
				pen.DeleteObject();
				i=0;
				MoveNext();
				delete [] nPoint;
				delete [] scnPoint;
			}
			break;
		case 4:
			nPoint = new DPoint[1];
			CPoint scPoint;//=pZoom->GPtoCP(nPoint[0]);
			CString lable;
			while(!this->IsEnd())
			{
				pID=(CString)this->GetResultByAttr(_T("ID"));
				nPoint[0].x=strtof(this->GetResultByAttr(_T("pX")));
				nPoint[0].y=strtof(this->GetResultByAttr(_T("pY")));
				scPoint=pZoom->GPtoCP(nPoint[0]);
				color=strtoi(this->GetResultByAttr(_T("color")));
				lable=(CString)this->GetResultByAttr(_T("Lable"));
				this->readImgFont(table,pID,lof);
				int nwheight=0;
				nwheight=int(lof.lfHeight*pZoom->ntimes());
				if(abs(nwheight)<=5)
				{
					if(nwheight!=0)
					{
						int a=nwheight/abs(nwheight);
						nwheight=a*5;
					}
					else
					{
						nwheight=-2;
					}
				}
				if(!pixel)
				{
					CSize sz(nwheight*100,nwheight*100);
					pDC->HIMETRICtoDP(&sz);
					nwheight=sz.cx;
				}
				lof.lfHeight=nwheight;
				HFONT hfont = ::CreateFontIndirect(&lof);
				CFont* pfont = CFont::FromHandle(hfont);
				CFont *oldfont=pDC->SelectObject(pfont);///
				COLORREF oldcolor=pDC->SetTextColor(color);///
				int nbk=pDC->SetBkMode(TRANSPARENT);///
				pDC->TextOutW(scPoint.x,scPoint.y,lable);///
				pDC->SetTextColor(oldcolor);//
				pDC->SelectObject(oldfont);//
				pDC->SetBkMode(nbk);//
				::DeleteObject(hfont);
				MoveNext();
			}
			delete [] nPoint;
			break;
		}
	}
}

void DBConnection::DisplaySet(CDC * pDC,CZoomManager *pZoom,CString table)
{
	CString sql;
	CString LID;
	CString pID;
	CPen* pOldPen;
	CPen  pen;
	CBrush * brush;
	CBrush * pOldBrush;
	DPoint point;
	LOGFONT lof;
	int penStyle;
	int pCount;
	int color;
	int type;
	CString Type;
	int LWidth;
	_RecordsetPtr  LPRst;
	DPoint * nPoint;
	int size;
	int i=0;
	if(!this->isConnection())
	{
		AfxMessageBox(L"数据库未连接！");
		return;
	}
	sql.Format(_T("select * from CatalogTable where gisTable='%s'"),table);
	pRst=this->ExecuteSQL(sql);
	if(!pRst->End)
	{
		type=strtoi(GetResultByAttr(L"Type"));
		pRst=getRecordSet(table);
		switch(type)
		{
		case 1:
			while(!IsEnd())
			{
				color=strtoi(GetResultByAttr(_T("color")));
				point.x=strtof(GetResultByAttr(_T("pX")));
				point.y=strtof(GetResultByAttr(_T("pY")));
				size=strtoi(GetResultByAttr(_T("size")));
				Type=GetResultByAttr(_T("type"));
				pen.CreatePen(PS_SOLID,1,color);
				brush=new CBrush(color);
				pOldPen =pDC->SelectObject(&pen);
				pOldBrush=pDC->SelectObject(brush);
				CPoint scrpoint=pZoom->GPtoCP(point);
				int times1=pZoom->ntimes()*(size+5);
				if(times1==0) times1=1;
				int times2=pZoom->ntimes()*(size+3);
				if(times2==0) times2=1;
				int times3=pZoom->ntimes()*(size+1);
				if(times3==0) times3=1;
				if(Type=="Circle")
					pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);///
				else if(Type=="cRect")
					pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times2,scrpoint.x+times1,scrpoint.y+times2);///
				else if(Type=="zRect")
					pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);//
				else if(Type=="SJ")
				{
					CPoint _point[3];
					_point[0]=CPoint(scrpoint.x,scrpoint.y-times1);
					_point[1]=CPoint(scrpoint.x-times1,scrpoint.y+times1);
					_point[2]=CPoint(scrpoint.x+times1,scrpoint.y+times1);
					pDC->Polygon(_point,3);///
				}
				else if(Type=="Ellipes")
					pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times3,scrpoint.x+times1,scrpoint.y+times3);///
				pDC->SelectObject(pOldBrush);//
				pDC->SelectObject(pOldPen);//
				pen.DeleteObject();
				MoveNext();
			}
			break;
		case 2:
			LOGBRUSH logBrush;
			logBrush.lbStyle = BS_SOLID;
			while(!this->IsEnd())
			{
				LID=(CString)this->GetResultByAttr(_T("ID"));
				pCount=strtoi(this->GetResultByAttr(_T("pCount")));
				color=strtoi(this->GetResultByAttr(_T("LColor")));
				logBrush.lbColor = color;
				Type=this->GetResultByAttr(_T("LType"));
				penStyle=strtoi(Type);
				LWidth=strtoi(this->GetResultByAttr(_T("LWidth")));
				bool curve=false;
				if(LWidth<0)
				{curve=true;LWidth=(-1)*LWidth;}
				nPoint=new DPoint[pCount];
				int viewWidth=int(pZoom->ntimes()*LWidth);
				if(viewWidth==0) viewWidth=1;
				LWidth=viewWidth;
				pen.CreatePen(penStyle|PS_GEOMETRIC|PS_ENDCAP_ROUND|PS_JOIN_ROUND,LWidth,&logBrush);
				pOldPen=pDC->SelectObject(&pen);////
				this->readImgPoint(table,LID,nPoint,pCount);
				if(!curve)
				{
					pDC->MoveTo(pZoom->GPtoCP(nPoint[i++]));///
					while(i<pCount)
						pDC->LineTo(pZoom->GPtoCP(nPoint[i++]));///
					i=0;
				}
				else
				{
					i=0;
					CPoint *npt=new CPoint [pCount];
					while(i<pCount)
					{npt[i]=pZoom->GPtoCP(nPoint[i]);i++;}///
					i=0;
					pDC->PolyBezier(npt,pCount);
					delete []npt;
				}

				pDC->SelectObject(pOldPen);///
				pen.DeleteObject();
				MoveNext();
				delete [] nPoint;
			}
			break;
		case 3:
			int borderColor;
			while(!this->IsEnd())
			{
				pID=(CString)this->GetResultByAttr(_T("ID"));
				pCount=strtoi(this->GetResultByAttr(_T("pCount")));
				color=strtoi(this->GetResultByAttr(_T("color")));
				penStyle=strtoi(this->GetResultByAttr(_T("BorderType")));
				borderColor=strtoi(this->GetResultByAttr(_T("BorderColor")));
				nPoint=new DPoint[pCount];
				CPoint* scnPoint=new CPoint[pCount];
				pen.CreatePen(penStyle,1,borderColor);
				pOldPen=pDC->SelectObject(&pen);///
				this->readImgPoint(table,pID,nPoint,pCount);
				for(int psn=0;psn<pCount;psn++)
					scnPoint[psn]=pZoom->GPtoCP(nPoint[psn]);
				pDC->MoveTo(scnPoint[i++]);///
				while(i<pCount)
					pDC->LineTo(scnPoint[i++]);///
				pDC->LineTo(pZoom->GPtoCP(nPoint[0]));///
				brush=new CBrush(color);
				pOldBrush=pDC->SelectObject(brush);//
				pDC->Polygon(scnPoint,pCount);//
				pDC->SelectObject(pOldBrush);///
				pDC->SelectObject(pOldPen);//
				pen.DeleteObject();
				i=0;
				MoveNext();
				delete [] nPoint;
				delete [] scnPoint;
			}
			break;
		case 4:
			nPoint = new DPoint[1];
			CPoint scPoint;//=pZoom->GPtoCP(nPoint[0]);
			CString lable;
			while(!this->IsEnd())
			{
				pID=(CString)this->GetResultByAttr(_T("ID"));
				nPoint[0].x=strtof(this->GetResultByAttr(_T("pX")));
				nPoint[0].y=strtof(this->GetResultByAttr(_T("pY")));
				scPoint=pZoom->GPtoCP(nPoint[0]);
				color=strtoi(this->GetResultByAttr(_T("color")));
				lable=(CString)this->GetResultByAttr(_T("Lable"));
				this->readImgFont(table,pID,lof);
				int nwheight=0;
				nwheight=int(lof.lfHeight*pZoom->ntimes());
				if(abs(nwheight)<=5)
				{
					if(nwheight!=0)
					{
						int a=nwheight/abs(nwheight);
						nwheight=a*5;
					}
					else
					{
						nwheight=-5;
					}
				}
				lof.lfHeight=nwheight;
				HFONT hfont = ::CreateFontIndirect(&lof);
				CFont* pfont = CFont::FromHandle(hfont);
				CFont *oldfont=pDC->SelectObject(pfont);///
				COLORREF oldcolor=pDC->SetTextColor(color);///
				int nbk=pDC->SetBkMode(TRANSPARENT);///
				pDC->TextOutW(scPoint.x,scPoint.y,lable);///
				pDC->SetTextColor(oldcolor);//
				pDC->SelectObject(oldfont);//
				pDC->SetBkMode(nbk);//
				::DeleteObject(hfont);
				MoveNext();
			}
			delete [] nPoint;
			break;
		}
	}
}

void DBConnection::getBorderPoint(DPoint * p,CString table,int type)
{
	CString sql;
	try
	{
		switch(type)
		{
		case 1:
			sql.Format(_T("select min(pX)X,min(pY)Y from [%s] "),table);
			pRst=this->ExecuteSQL(sql);
			p[0].x=strtof(this->GetResultByAttr(_T("X")));
			p[0].y=strtof(this->GetResultByAttr(_T("Y")));
			sql.Format(_T("select max(pX)X,max(pY)Y from [%s]"),table);
			pRst=this->ExecuteSQL(sql);
			p[1].x=strtof(this->GetResultByAttr(_T("X")));
			p[1].y=strtof(this->GetResultByAttr(_T("Y")));
			break;
		case 2:
			sql.Format(_T("select min(xMin)X,min(yMin)Y from [%s] "),table);
			pRst=this->ExecuteSQL(sql);
			p[0].x=strtof(this->GetResultByAttr(_T("X")));
			p[0].y=strtof(this->GetResultByAttr(_T("Y")));
			sql.Format(_T("select max(xMax)X,max(yMax)Y from [%s]"),table);
			pRst=this->ExecuteSQL(sql);
			p[1].x=strtof(this->GetResultByAttr(_T("X")));
			p[1].y=strtof(this->GetResultByAttr(_T("Y")));
			break;
		}
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
	}
}

bool DBConnection::SearchTable(CString table)
{
	CString sql;
	if(!this->m_isConnection)
	{
		AfxMessageBox(L"数据库未连接!");
		return false;
	}
	sql.Format(_T("select ID from CatalogTable where gisTable='%s'"),table);
	pRst=ExecuteSQL(sql);
	if(!pRst->End)
		return true;
	return false;
}

bool DBConnection::CreateTable(CString table,int type)
{
	CString sql;
	CString ID;
	bool exit = SearchTable(table);
	if(!this->m_isConnection)
	{
		AfxMessageBox(L"数据库未连接!");
		return false;
	}
	else if(exit||type<1||type>4)
	{
		if(exit) AfxMessageBox(L"表已经存在！");
		return false;
	}
	else
	{
		sql.Format(_T("select * from CatalogTable"));
	    pRst=ExecuteSQL(sql);
		if(!pRst->End)
		{
			sql.Format(_T("select max(ID)+1 ID from CatalogTable"));
			pRst=ExecuteSQL(sql);
			ID=(CString)(BSTR)((_bstr_t)pRst->GetCollect((_variant_t)"ID"));
		}
		else ID="1";
		sql.Format(_T("insert into [CatalogTable] values('%s','%s','%d')"),ID,table,type);
		ExecuteSQL(sql);
		switch(type)
		{
		case 1:
			sql.Format(_T("create table [%s] (ID int primary key,pX float,pY float,color int,size int,type nvarchar(50))"),table);
			ExecuteSQL(sql);
			break;
		case 2:
			sql.Format(_T("create table [%s] (ID int primary key,LColor int,LType nvarchar(50),LWidth int,xMin float,yMin float,xMax float,yMax float,Point image,pCount int)"),table);
			ExecuteSQL(sql);
			break;
		case 3:
			sql.Format(_T("create table [%s] (ID int primary key,pCount int ,BorderType int,color int,Point image,BorderColor int,xMin float,yMin float,xMax float,yMax float)"),table);
			ExecuteSQL(sql);
			break;
		case 4:
			sql.Format(_T("create table [%s] (ID int primary key,pX float,pY float,Lable nvarchar(50),color int,FontStyle image)"),table);
			ExecuteSQL(sql);
			break;
		}
		return true;
	}
}


void DBConnection::readImgPoint(CString table,CString ID,DPoint * point,int n)
{
	CString     sql;
	FieldsPtr	pFldS;
	_RecordsetPtr  pSet;
	long		lDataSize;
	_variant_t	val;
	char		*pMove=NULL;
	char		*pBuf=NULL;
	CString     pointStr;
	int i=0;
	int xy=0;
	int num=0;
	sql.Format(L"select * from [%s] where ID='%s'",table,ID);
	try
	{
		pSet=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		pFldS = pSet ->GetFields();
		lDataSize = pFldS->GetItem("Point")->ActualSize;
		if (lDataSize>0)
		{
			val = pFldS->GetItem("Point")->GetChunk(lDataSize);
			if (val.vt == (VT_ARRAY|VT_UI1))
			 {
				pBuf = new char[lDataSize+1];
				SafeArrayAccessData(val.parray,(void **)&pMove); 
				memcpy(pBuf,pMove,lDataSize);
				SafeArrayUnaccessData(val.parray);
				while(i<lDataSize&&num<n)
				{
					if(pBuf[i]!=',')
					   pointStr+=pBuf[i];
					else
					{
						if(xy==0)
						{
							point[num].x=strtof(pointStr);
							xy++;
						}
						else
						{
							point[num].y=strtof(pointStr);
							xy=0;
							num++;
						}
						pointStr=L"";
					}
					i++;
				}
				delete [] pBuf;
			}
		}
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
	}
	
}

bool DBConnection::DropTable(CString table)
{
	CString sql;
	try
	{
		if(!SearchTable(table))
		    return false;
		else
		{
			sql.Format(L"drop table [%s]",table);
			ExecuteSQL(sql);
			sql.Format(L"delete from CatalogTable where gisTable='%s'",table);
			ExecuteSQL(sql);
			return true;
		}
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}

bool DBConnection::SearchPointByClick(CPoint point,gisPoint&gis_point,CString table,CZoomManager *pZoom)
{
	CString sql;
	DPoint scrpoint=pZoom->CPtoGP(point);
	float times=pZoom->ntimes();
	float cx,cy;
	cx=pZoom->xPrecision();
	cy=pZoom->yPrecision();
	sql.Format(L"select * from [%s] where abs(pX-%f)<=(size+3)*%f*%f and abs(pY-%f)<=(size+3)*%f*%f",table,scrpoint.x,times,cx,scrpoint.y,times,cy);
	try
	{
		pRst=ExecuteSQL(sql);
		gis_point.ID="";
		while(!pRst->End)
		{
			gis_point.ID=(CString)(BSTR)((_bstr_t)pRst->GetCollect(L"ID"));
			gis_point.color=strtoi((CString)(BSTR)((_bstr_t)pRst->GetCollect(L"color")));
			gis_point.point.x=strtof((CString)(BSTR)((_bstr_t)pRst->GetCollect(L"pX")));
			gis_point.point.y=strtof((CString)(BSTR)((_bstr_t)pRst->GetCollect(L"pY")));
			gis_point.size=strtoi((CString)(BSTR)((_bstr_t)pRst->GetCollect(L"size")));
			gis_point.pType=(CString)(BSTR)((_bstr_t)pRst->GetCollect(L"type"));
			pRst->MoveNext();
			return true;
		}
		return false;
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}

void DBConnection::addMarker(gisMarker&gm,CString table)
{
	CString pID;
	CString sql;
	CString strVal,temp;
	sql.Format(_T("select * from [%s]"),table);
	char * pLFont = new char[sizeof(gm.lfont)];
	try
	{
		pRst=ExecuteSQL(sql);
		if(!pRst->End)
		{
			sql.Format(_T("select max(ID)+1 pID from [%s]"),table);
			pRst=ExecuteSQL(sql);
			pID=(CString)(BSTR)((_bstr_t)pRst->GetCollect((_variant_t)"pID"));
		}
		else pID="1";
		memcpy(pLFont,&gm.lfont,sizeof(gm.lfont));
		for(int i=0;i<sizeof(gm.lfont);i++)
		{
			temp.Format(L"%d,",(int)pLFont[i]);
			strVal+=temp;
		}
		sql.Format(_T("insert into [%s] values('%s','%f','%f','%s','%d','%s')"),table,pID,gm.point.x,gm.point.y,gm.text,gm.color,strVal);
		ExecuteSQL(sql);
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
	}
	
}

void DBConnection::readImgFont(CString table,CString ID,LOGFONT & lfont)
{
	CString     sql;
	CString     valStr;
	_RecordsetPtr  pSet;
	FieldsPtr	pFldS;
	long		lDataSize;
	_variant_t	val;
	char		*pMove=NULL;
	char		*pBuf=NULL;
	char        *charVal;
	int         i=0;
	int         index=0;
	int         lflen=sizeof(lfont);
	int         valnum=0;
	bool        v=false;
	sql.Format(L"select * from [%s] where ID='%s'",table,ID);
	try
	{
		pSet=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		pFldS = pSet ->GetFields();
		lDataSize = pFldS->GetItem("FontStyle")->ActualSize;
		if (lDataSize>0)
		{
			val = pFldS->GetItem("FontStyle")->GetChunk(lDataSize);
			if (val.vt == (VT_ARRAY|VT_UI1))
			 {
				pBuf = new char[lDataSize+1];
				charVal = new char[lflen];
				SafeArrayAccessData(val.parray,(void **)&pMove); 
				memcpy(pBuf,pMove,lDataSize);
				SafeArrayUnaccessData(val.parray);
				while(i<lDataSize&&valnum<lflen)
				{
					if(pBuf[i]!=',')
					{
						if(pBuf[i]=='-') v=true;
						else valStr+=pBuf[i];
					}
					else 
					{
						if(v) charVal[valnum]=-strtoi(valStr);
						else charVal[valnum]=strtoi(valStr);
						v=false;
						valnum++;
						valStr=L"";
					}
					i++;
				}
				memcpy(&lfont,charVal,lflen);
				delete [] pBuf;
				delete [] charVal;
			}
		}
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
	}
}


int DBConnection::GetDataBaseTable(CatalogTable * tableSet)
{
	int tableCount=0;
	int index=0;
	CString sql=L"select * from [CatalogTable] where Type<=4";
	try
	{
		pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pRst->End)
		{
			tableSet[tableCount].tableName=(CString)(BSTR)((_bstr_t)pRst->GetCollect(L"gisTable"));
			tableSet[tableCount].ID=(CString)(BSTR)((_bstr_t)pRst->GetCollect(L"ID"));
			tableSet[tableCount].tableType=strtoi((CString)(BSTR)((_bstr_t)pRst->GetCollect(L"Type")));
			tableCount++;
			pRst->MoveNext();
		}
		return tableCount;
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
		return 0;
	}
}

bool DBConnection::GetTableMsg(CatalogTable&tableMsg,int id)
{
	CString sql;
	sql.Format(L"select gisTable,Type from [CatalogTable] where ID='%d'",id);
	try
	{
		pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		if(!pRst->End)
		{
			tableMsg.tableName=(CString)(BSTR)((_bstr_t)pRst->GetCollect((_variant_t)L"gisTable"));
			tableMsg.tableType=strtoi((CString)(BSTR)((_bstr_t)pRst->GetCollect((_variant_t)L"Type")));
			return true;
		}
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}

bool DBConnection::CreateDataBase(CString server,CString db_name,CString user,CString paw,CString db_path)
{
   CString str;
   CString iniDataBase=L"master";
   CString createDataBase=L"CREATE DATABASE "+db_name+L" ON(NAME="+db_name+L",FILENAME='"+db_path+db_name+L".mdf',SIZE=20MB,MAXSIZE=50MB,FILEGROWTH=10%);";
   str.Format(L"Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;User ID=%s;Password=%s;Initial Catalog=%s;Data Source=%s;",user,paw,iniDataBase,server);
	try
	{
		if(!this->m_isConnection)
		{
			pConn->ConnectionString=(_bstr_t)str;
			pConn->Open(pConn->ConnectionString,(_bstr_t)user,(_bstr_t)paw,adConnectUnspecified);
			pConn->Execute((_bstr_t)createDataBase,NULL,adCmdText);
			pConn->Close();
	        pConn.Release();
	        CoUninitialize();
			CoInitialize(NULL);
	        pConn.CreateInstance("ADODB.Connection");
	        pRst.CreateInstance(__uuidof(Recordset));
			str.Format(L"Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=%s;Data Source=%s;User ID=%s,Password=%s",db_name,server,user,paw);
			pConn->ConnectionString=(_bstr_t)str;
			pConn->Open(pConn->ConnectionString,(_bstr_t)user,(_bstr_t)paw,adConnectUnspecified);
			pConn->Execute((_bstr_t)"create table CatalogTable(ID int primary key,gisTable nvarchar(50),Type int)",NULL,adCmdText);
			this->m_isConnection=true;
			return true;
		}
		return false;
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}

void DBConnection::AlterPoint(gisPoint& gis_point,CString table,CDC * pDC,CZoomManager * pZoom)
{
	try
	{
		int i=0;
		CString sql;
		int times1=pZoom->ntimes()*(gis_point.size+5);
		if(times1==0) times1=1;
		int times2=pZoom->ntimes()*(gis_point.size+3);
		if(times2==0) times2=1;
		int times3=pZoom->ntimes()*(gis_point.size+1);
		if(times3==0) times3=1;
		CPoint scrpoint=pZoom->GPtoCP(gis_point.point);
		sql.Format(L"update [%s] set pX='%f',pY='%f',color='%d',size='%d',type='%s' where ID='%s';",table,gis_point.point.x,gis_point.point.y,gis_point.color,gis_point.size,gis_point.pType,gis_point.ID);
		pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		CPen * pen = new CPen(PS_SOLID,1,gis_point.color);
		CPen * pOldPen = pDC->SelectObject(pen);
		CBrush * brush = new CBrush(gis_point.color);
		CBrush * pOldBrush = pDC->SelectObject(brush);
		if(gis_point.pType=="Circle")
				pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);
		else if(gis_point.pType=="cRect")
			pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times2,scrpoint.x+times1,scrpoint.y+times2);
		else if(gis_point.pType=="zRect")
			pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);
		else if(gis_point.pType=="SJ")
		{
			CPoint _point[3];
			_point[0]=CPoint(scrpoint.x,scrpoint.y-times1);
			_point[1]=CPoint(scrpoint.x-times1,scrpoint.y+times1);
			_point[2]=CPoint(scrpoint.x+times1,scrpoint.y+times1);
			pDC->Polygon(_point,3);
		}
		else if(gis_point.pType=="Ellipes")
			pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times3,scrpoint.x+times1,scrpoint.y+times3);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}

void DBConnection::DeletePoint(gisPoint& gp,CString table)
{
	try
	{
		CString sql;
		sql.Format(L"delete from [%s] where ID='%s';",table,gp.ID);
		pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}

void DBConnection::swap(CPoint&p1,CPoint&p2)
{
	CPoint temp=p1;
	p1=p2;
	p2=temp;
}

bool DBConnection::SearchLineByClick(CPoint point,gisLine& gl,CString table,CZoomManager *pZoom)
{
	CString sql;
	int i=1;
	double s,s1,s2,s3,p,h;
	bool hasFind=false;
	//CPoint p1,p2;
	sql.Format(L"select * from [%s] where %f>xMin and %f<xMax and %f>yMin and %f<yMax;",table,pZoom->CPtoGP(point).x,pZoom->CPtoGP(point).x,pZoom->CPtoGP(point).y,pZoom->CPtoGP(point).y);
	_RecordsetPtr pSet;
	try
	{
		pos=-1;
		pSet=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pSet->End)
		{
			pos=1;
			if(gl.LinePoint!=NULL)
			   delete [] gl.LinePoint;
			gl.pCount=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"pCount")));
			gl.LinePoint = new DPoint[gl.pCount];
			gl.ID=(CString)(BSTR)((_bstr_t)pSet->GetCollect(L"ID"));
			int w=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"LWidth")));
			readImgPoint(table,gl.ID,gl.LinePoint,gl.pCount);
			if(w>=0)
			{	
				
				while(pos<gl.pCount)
				{
					sp1=pZoom->GPtoCP(gl.LinePoint[pos-1]);
					sp2=pZoom->GPtoCP(gl.LinePoint[pos]);
					if(sp1.x>sp2.x)
						swap(sp1,sp2);
					if((sp1.x-sp2.x)&&(sp1.y-sp2.y))////不是竖直线或水平线
					{
						if(sp1.y<sp2.y)
							hasFind=(point.y>=sp1.y)&&(point.y<=sp2.y)&&(point.x>=sp1.x)&&(point.x<=sp2.x);
						else if(sp1.y>sp2.y)
							hasFind=(point.y<=sp1.y)&&(point.y>=sp2.y)&&(point.x>=sp1.x)&&(point.x<=sp2.x);
					}
					else if(sp1.x-sp2.x==0)////竖直线
						hasFind=(point.y>=sp1.y)&&(point.y<=sp2.y);////竖直线，且满足求点到直线距离的条件
					else if(sp1.y-sp2.y==0)///水平线
						hasFind=(point.x>=sp1.x)&&(point.x<=sp2.x);
					if(hasFind)
					{
						s1=sqrt(pow((double)sp1.x-sp2.x,2)+pow((double)sp1.y-sp2.y,2));
						s2=sqrt(pow((double)sp1.x-point.x,2)+pow((double)sp1.y-point.y,2));
						s3=sqrt(pow((double)sp2.x-point.x,2)+pow((double)sp2.y-point.y,2));
						p=(s1+s2+s3)/2;
						s=sqrt(p*(p-s1)*(p-s2)*(p-s3));
						h=2*s/s1;
						gl.width=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"LWidth")));
						if(h<=gl.width*pZoom->ntimes())
						{
							gl.color=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"LColor")));
							gl.lType=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"LType")));
							gl.xMax=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"xMax")));
							gl.xMin=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"xMin")));
							gl.yMin=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"yMin")));
							gl.yMax=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"yMax")));
							return true;
						}
					}
					pos++;
				}
				pSet->MoveNext();
				pos=1;
			}
			else
			{
				CPoint* npt=new CPoint[gl.pCount];
				int nt=0;
				while(nt<gl.pCount)
				{
					npt[nt]=pZoom->GPtoCP(gl.LinePoint[nt]);
					nt++;
				}
				nt=3;
				while(nt<gl.pCount)
				{
					CPoint t1=npt[nt-2];
					CPoint t2=npt[nt-1];
					npt[nt-2]=CPoint((npt[nt-3].x+(t1.x-npt[nt-3].x)/3),(npt[nt-3].y+(t1.y-npt[nt-3].y)/3));
					npt[nt-1]=CPoint((npt[nt].x+(t2.x-npt[nt].x)/3),(npt[nt].y+(t2.y-npt[nt].y)/3));
					nt+=3;
				}
				while(i<gl.pCount)
				{
					sp1=npt[i-1];//pZoom->GPtoCP(gl.LinePoint[i-1]);
					sp2=npt[i];//pZoom->GPtoCP(gl.LinePoint[i]);
					if(sp1.x>sp2.x)
						swap(sp1,sp2);
					if((sp1.x-sp2.x)&&(sp1.y-sp2.y))////不是竖直线或水平线
					{
						if(sp1.y<sp2.y)
							hasFind=(point.y>=sp1.y)&&(point.y<=sp2.y)&&(point.x>=sp1.x)&&(point.x<=sp2.x);
						else if(sp1.y>sp2.y)
							hasFind=(point.y<=sp1.y)&&(point.y>=sp2.y)&&(point.x>=sp1.x)&&(point.x<=sp2.x);
					}
					else if(sp1.x-sp2.x==0)////竖直线
						hasFind=(point.y>=sp1.y)&&(point.y<=sp2.y);////竖直线，且满足求点到直线距离的条件
					else if(sp1.y-sp2.y==0)///水平线
						hasFind=(point.x>=sp1.x)&&(point.x<=sp2.x);
					if(hasFind)
					{
						s1=sqrt(pow((double)sp1.x-sp2.x,2)+pow((double)sp1.y-sp2.y,2));
						s2=sqrt(pow((double)sp1.x-point.x,2)+pow((double)sp1.y-point.y,2));
						s3=sqrt(pow((double)sp2.x-point.x,2)+pow((double)sp2.y-point.y,2));
						p=(s1+s2+s3)/2;
						s=sqrt(p*(p-s1)*(p-s2)*(p-s3));
						h=2*s/s1;
						gl.width=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"LWidth")));
						if(h<=abs(int(gl.width))*pZoom->ntimes())
						{
							gl.color=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"LColor")));
							gl.lType=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"LType")));
							gl.xMax=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"xMax")));
							gl.xMin=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"xMin")));
							gl.yMin=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"yMin")));
							gl.yMax=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"yMax")));
							return true;
						}
					}
					i++;
				}
				delete []npt;
				pSet->MoveNext();
				i=1;
			}

		}
		return false;
    }
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}

void DBConnection::DeleteLine(gisLine& gl,CString table)
{
	try
	{
		CString sql;
		sql.Format(L"delete from [%s] where ID='%s';",table,gl.ID);
		pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}

void DBConnection::AlterLine(gisLine& gl,CString table,CDC * pDC,CZoomManager *pZoom)
{
	int i=0;
	CString tempPoint;
	CString sql;
	CString pointStr=L"";
	gl.xMin=gl.LinePoint[0].x;gl.yMin=gl.LinePoint[0].y;gl.xMax=gl.LinePoint[0].x;gl.yMax=gl.LinePoint[0].y;
	while(i<gl.pCount)
	{
		if(gl.LinePoint[i].x<gl.xMin) gl.xMin=gl.LinePoint[i].x;
		else if(gl.LinePoint[i].x>gl.xMax) gl.xMax=gl.LinePoint[i].x;
		if(gl.LinePoint[i].y<gl.yMin) gl.yMin=gl.LinePoint[i].y;
		else if(gl.LinePoint[i].y>gl.yMax) gl.yMax=gl.LinePoint[i].y;
		tempPoint.Format(L"%f,%f,",gl.LinePoint[i].x,gl.LinePoint[i].y);
		pointStr+=tempPoint;
		i++;
	}
	sql.Format(_T("update [%s] set LColor='%d',LType='%d',LWidth='%d',xMin='%f',yMin='%f',xMax='%f',yMax='%f',Point='%s',pCount='%d' where ID='%s';"),table,gl.color,gl.lType,gl.width,gl.xMin,gl.yMin,gl.xMax,gl.yMax,pointStr,gl.pCount,gl.ID);
	try
	{
		pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		LOGBRUSH logBrush;
		logBrush.lbStyle = BS_SOLID;
		CPen pen;
		CPen * pOldPen;
		logBrush.lbColor = gl.color;
		int w=gl.width;
		pen.CreatePen(gl.lType|PS_GEOMETRIC,abs(int(gl.width))*pZoom->ntimes(),&logBrush);
		pOldPen=pDC->SelectObject(&pen);////
		if(w>=0)
		{
			pDC->MoveTo(pZoom->GPtoCP(gl.LinePoint[0]));///
			i=1;
			while(i<gl.pCount)
				pDC->LineTo(pZoom->GPtoCP(gl.LinePoint[i++]));///
		}
		else
		{
			int nt=0;
			CPoint *pts=new CPoint [gl.pCount];
			while(nt<gl.pCount)
			{
				pts[nt]=pZoom->GPtoCP(gl.LinePoint[nt]);
				nt++;
			}
			pDC->PolyBezier(pts,gl.pCount);
			delete []pts;
		}
		pDC->SelectObject(pOldPen);///
		pen.DeleteObject();
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
	}	
}

bool DBConnection::getLinePt(CPoint point,CPoint& p1,CPoint& p2,int& pos,CString table,CZoomManager * pZoom)
{

	p1=this->sp1;
	p2=this->sp2;
	pos=this->pos;
	return true;
}

bool DBConnection::delPtFromLine(CPoint point,CString table,gisLine&gl,CZoomManager * pZoom)
{
	int i=0;
	bool search=false;
	while(i<gl.pCount)
	{
		if(abs(point.x-pZoom->GPtoCP(gl.LinePoint[i]).x)<4&&abs(point.y-pZoom->GPtoCP(gl.LinePoint[i]).y)<4)
		{
			search=true;
			break;
		}
		i++;
	}
	if(search)
	{
		i++;
		while(i<gl.pCount)
		{
			gl.LinePoint[i-1]=gl.LinePoint[i];
			i++;
		}
	}
	return search;
}


bool DBConnection::MoveLinePt(CPoint point,CPoint&p1,CPoint&p2,int&pos,CString table,gisLine&gl,CZoomManager * pZoom)
{
	int i=0;
	bool search=false;
	while(i<gl.pCount)
	{
		if(abs(point.x-pZoom->GPtoCP(gl.LinePoint[i]).x)<4&&abs(point.y-pZoom->GPtoCP(gl.LinePoint[i]).y)<4)
		{
			search=true;
			break;
		}
		i++;
	}
	if(search)
	{
		if(i!=0&&i!=gl.pCount-1)
		{
			p1=pZoom->GPtoCP(gl.LinePoint[i-1]);
		    p2=pZoom->GPtoCP(gl.LinePoint[i+1]);
		}
		else if(i==0)
			p2=pZoom->GPtoCP(gl.LinePoint[i+1]);
		else if(i==gl.pCount-1)
			p1=pZoom->GPtoCP(gl.LinePoint[i-1]);
		pos=i;
	}
	return search;
}

void DBConnection::getInfo(CPoint&p1,CPoint&p2,double& minX,double&maxX,double&maxY,double&minY)
{
	minX=maxX=p1.x;
	maxY=minY=p1.y;
	if(p2.x>p1.x) maxX=p2.x;
	else minX=p2.x;
	if(p2.y>p1.y) maxY=p2.y;
	else minY=p2.y;
}

bool DBConnection::SearchPolyByClick(CPoint&point,gisPolygon& gPoly,CString table,CZoomManager *pZoom)
{
	CString sql;
	CPoint p1,p2;
	double xMin,xMax,yMax,yMin;
	double k,b;
	int line;	 
	sql.Format(L"select * from [%s] where %f>=xMin and %f<=xMax and %f>=yMin and %f<=yMax;",table,pZoom->CPtoGP(point).x,pZoom->CPtoGP(point).x,pZoom->CPtoGP(point).y,pZoom->CPtoGP(point).y);
	_RecordsetPtr pSet;
	try
	{
		pSet=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pSet->End)
		{
			delete [] gPoly.point;
			line=0;
			gPoly.pCount=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"pCount")));
			gPoly.point = new DPoint[gPoly.pCount];
			gPoly.ID=(CString)(BSTR)((_bstr_t)pSet->GetCollect(L"ID"));
			readImgPoint(table,gPoly.ID,gPoly.point,gPoly.pCount);
			for(int i=1;i<gPoly.pCount+1;i++)
			{
				p1=pZoom->GPtoCP(gPoly.point[i-1]);
				p2=pZoom->GPtoCP(gPoly.point[i%gPoly.pCount]);
				this->getInfo(p1,p2,xMin,xMax,yMax,yMin);
				if(xMin==xMax&&point.x==xMin){line++; continue;}//竖直线
				if(point.x<xMin||point.x>xMax) continue;////点x的坐标超过直线的x范围
				if((yMin==yMax)&&(point.y>yMax)){line++;continue;}///点x的坐标在直线的x范围内且点在线的下方
				///y=kx+b,判断点是否在直线的下方
				k=(double)(p1.y-p2.y)/(p1.x-p2.x);
				b=(double)(p1.y-k*p1.x);
				if((double)point.y>=k*point.x+b)
					line++;
			}
			if(line%2!=0)
			{
				gPoly.BorderColor=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"BorderColor")));
				gPoly.BorderType=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"BorderType")));
				gPoly.color=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"color")));
				gPoly.xMax=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"xMax")));
				gPoly.xMin=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"xMin")));
				gPoly.yMin=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"yMin")));
				gPoly.yMax=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"yMax")));
				return true;
			}
			pSet->MoveNext();
		}
		return false;
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}


void DBConnection::AlterPolygon(gisPolygon& gPoly,CString table,CDC*pDC,CZoomManager *pZoom)
{
	int i=0;
	CString tempPoint;
	CString sql;
	CString pointStr=L"";
	gPoly.xMin=gPoly.point[0].x;gPoly.yMin=gPoly.point[0].y;gPoly.xMax=gPoly.point[0].x;gPoly.yMax=gPoly.point[0].y;
	while(i<gPoly.pCount)
	{
		if(gPoly.point[i].x<gPoly.xMin) gPoly.xMin=gPoly.point[i].x;
		else if(gPoly.point[i].x>gPoly.xMax) gPoly.xMax=gPoly.point[i].x;
		if(gPoly.point[i].y<gPoly.yMin) gPoly.yMin=gPoly.point[i].y;
		else if(gPoly.point[i].y>gPoly.yMax) gPoly.yMax=gPoly.point[i].y;
		tempPoint.Format(L"%f,%f,",gPoly.point[i].x,gPoly.point[i].y);
		pointStr+=tempPoint;
		i++;
	}
	sql.Format(_T("update [%s] set pCount='%d',BorderType='%d',color='%d',Point='%s',BorderColor='%d',xMin='%f',yMin='%f',xMax='%f',yMax='%f' where ID='%s';"),table,gPoly.pCount,gPoly.BorderType,gPoly.color,pointStr,gPoly.BorderColor,gPoly.xMin,gPoly.yMin,gPoly.xMax,gPoly.yMax,gPoly.ID);
	try
	{
		pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		CPen pen;
		CPen * pOldPen;
		CBrush * brush = new CBrush(gPoly.color);
		CBrush * pOldBrush;
		pen.CreatePen(gPoly.BorderType,1,gPoly.BorderColor);
		pOldPen=pDC->SelectObject(&pen);
		pOldBrush=pDC->SelectObject(brush);
		i=0;
		CPoint *  p = new CPoint[gPoly.pCount];
		p[i]=pZoom->GPtoCP(gPoly.point[i]);
		pDC->MoveTo(pZoom->GPtoCP(gPoly.point[i++]));///
		while(i<gPoly.pCount)
		{
			p[i]=pZoom->GPtoCP(gPoly.point[i]);
			pDC->LineTo(pZoom->GPtoCP(gPoly.point[i++]));///
		}
		pDC->LineTo(pZoom->GPtoCP(gPoly.point[0]));///
		pDC->Polygon(p,gPoly.pCount);//
		pDC->SelectObject(pOldBrush);///
		pDC->SelectObject(pOldPen);//
		pen.DeleteObject();
		delete [] p;
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
	}	
}

void DBConnection::DeletePolygon(gisPolygon& gPoly,CString table)
{
	try
	{
		CString sql;
		sql.Format(L"delete from [%s] where ID='%s';",table,gPoly.ID);
		pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}

bool DBConnection::getPolygonPt(CPoint point,CPoint& p1,CPoint& p2,int& pos,CString table,gisPolygon&gl,CZoomManager * pZoom)
{
	pos=1;
	bool hasFind=false;
	double s,s1,s2,s3,h,p;
	while(pos<gl.pCount+1)
	{
		p1=pZoom->GPtoCP(gl.point[pos-1]);
		p2=pZoom->GPtoCP(gl.point[pos%gl.pCount]);
		if(p1.x>p2.x)
			swap(p1,p2);
		if((p1.x-p2.x)&&(p1.y-p2.y))////不是竖直线或水平线
		{
			if(p1.y<p2.y)
				hasFind=(point.y>=p1.y)&&(point.y<=p2.y)&&(point.x>=p1.x)&&(point.x<=p2.x);
			else if(p1.y>p2.y)
				hasFind=(point.y<=p1.y)&&(point.y>=p2.y)&&(point.x>=p1.x)&&(point.x<=p2.x);
		}
		else if(p1.x-p2.x==0)////竖直线
			hasFind=(point.y>=p1.y)&&(point.y<=p2.y);////竖直线，且满足求点到直线距离的条件
		else if(p1.y-p2.y==0)///水平线
			hasFind=(point.x>=p1.x)&&(point.x<=p2.x);
		if(hasFind)
		{
			s1=sqrt(pow((double)p1.x-p2.x,2)+pow((double)p1.y-p2.y,2));
			s2=sqrt(pow((double)p1.x-point.x,2)+pow((double)p1.y-point.y,2));
			s3=sqrt(pow((double)p2.x-point.x,2)+pow((double)p2.y-point.y,2));
			p=(s1+s2+s3)/2;
			s=sqrt(p*(p-s1)*(p-s2)*(p-s3));
			h=2*s/s1;
			if(h<=4) return true;
		}
		pos++;
	}
	return false;
}

bool DBConnection::delPtFromPolygon(CPoint point,CString table,gisPolygon&gl,CZoomManager * pZoom)
{
	int i=0;
	bool search=false;
	while(i<gl.pCount)
	{
		if(abs(point.x-pZoom->GPtoCP(gl.point[i]).x)<4&&abs(point.y-pZoom->GPtoCP(gl.point[i]).y)<4)
		{
			search=true;
			break;
		}
		i++;
	}
	if(search)
	{
		i++;
		while(i<gl.pCount)
		{
			gl.point[i-1]=gl.point[i];
			i++;
		}
	}
	return search;
}


bool DBConnection::MovePolygonPt(CPoint point,CPoint&p1,CPoint&p2,int&pos,CString table,gisPolygon&gl,CZoomManager * pZoom)
{
	int i=0;
	bool search=false;
	while(i<gl.pCount)
	{
		if(abs(point.x-pZoom->GPtoCP(gl.point[i]).x)<4&&abs(point.y-pZoom->GPtoCP(gl.point[i]).y)<4)
		{
			search=true;
			break;
		}
		i++;
	}
	if(search)
	{
		if(i!=0&&i!=gl.pCount-1)
		{
			p1=pZoom->GPtoCP(gl.point[i-1]);
		    p2=pZoom->GPtoCP(gl.point[i+1]);
		}
		else if(i==0)
		{
			p1=pZoom->GPtoCP(gl.point[gl.pCount-1]);
			p2=pZoom->GPtoCP(gl.point[i+1]);
		}
		else if(i==gl.pCount-1)
		{
			p1=pZoom->GPtoCP(gl.point[i-1]);
			p2=pZoom->GPtoCP(gl.point[0]);
		}
		pos=i;
	}
	return search;
}

bool DBConnection::SearchMarkerByClick(CPoint point,gisMarker&gm,CString table,CDC*pDC,CZoomManager *pZoom)
{
	CString sql;
	_RecordsetPtr pSet;
	sql.Format(L"select * from [%s]",table);
	HFONT hfont ;
	CFont* pfont ;
	CFont *oldfont;
	CSize size;
	try
	{
		pSet=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pSet->End)
		{
			gm.text=(CString)(BSTR)((_bstr_t)pSet->GetCollect(L"Lable"));
			gm.ID=(CString)(BSTR)((_bstr_t)pSet->GetCollect(L"ID"));
			gm.point.x=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"pX")));
			gm.point.y=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"pY")));
			gm.color=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect(L"color")));
			this->readImgFont(table,gm.ID,gm.lfont);
			hfont = ::CreateFontIndirect(&gm.lfont);
			pfont = CFont::FromHandle(hfont);
		    oldfont=pDC->SelectObject(pfont);
			size=pDC->GetTextExtent(gm.text);
			size.cx*=pZoom->ntimes();
			size.cy*=pZoom->ntimes();
			pDC->SelectObject(oldfont);
			if(point.x>=pZoom->GPtoCP(gm.point).x&&point.x<=pZoom->GPtoCP(gm.point).x+size.cx&&point.y>=pZoom->GPtoCP(gm.point).y&&point.y<=pZoom->GPtoCP(gm.point).y+size.cy)
				return true;
			pSet->MoveNext();
		}
		return false;
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}


void DBConnection::AlterMarker(gisMarker&gm,CString table,CDC* pDC,CZoomManager *pZoom)
{
	CString sql;
	CString strVal,temp;
	char * pLFont = new char[sizeof(gm.lfont)];
	memcpy(pLFont,&gm.lfont,sizeof(gm.lfont));
	for(int i=0;i<sizeof(gm.lfont);i++)
	{
		temp.Format(L"%d,",(int)pLFont[i]);
		strVal+=temp;
	}
	sql.Format(_T("update [%s] set pX='%f',pY='%f',Lable='%s',color='%d',FontStyle='%s' where ID=%s"),table,gm.point.x,gm.point.y,gm.text,gm.color,strVal,gm.ID);
	try
	{
		pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		HFONT hfont = ::CreateFontIndirect(&gm.lfont);
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
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
	}
}

void DBConnection::DeleteMarker(gisMarker&gm,CString table)
{
	try
	{
		CString sql;
		sql.Format(L"delete from [%s] where ID='%s';",table,gm.ID);
		pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}

bool DBConnection::getTableInfo(double&xMin,double&yMin,double&xMax,double&yMax)
{
	CString sql=L"select * from CatalogTable";
	xMin=yMin=xMax=yMax=0;
	int type;
	CString table;
	_RecordsetPtr pSet(__uuidof(Recordset));
	bool hasInit=false;
	bool data=false;
	double temp;
	try
	{
		pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		if(pRst->End) return false;
		while(!pRst->End)
		{
			type=strtoi((CString)(BSTR)((_bstr_t)pRst->GetCollect((_variant_t)"Type")));
			table=(CString)(BSTR)((_bstr_t)pRst->GetCollect((_variant_t)"gisTable"));
			switch(type)
			{
				case 2:
				case 3:
					{
						sql.Format(L"select * from [%s] ",table);
						_RecordsetPtr pRecord(__uuidof(Recordset));
						pRecord=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
						if(pRecord->End)
						{data=false;}
						else
						{data=true;}
					}
					sql.Format(L"select Min(xMin) xMin,Min(yMin) yMin ,Max(xMax) xMax,Max(yMax) yMax from [%s] ",table);
					pSet=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
					break;
				case 1:
				case 4:
					{
						sql.Format(L"select * from [%s] ",table);
						_RecordsetPtr pRecord(__uuidof(Recordset));
						pRecord=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
						if(pRecord->End)
						{data=false;}
						else
						{data=true;}
					}
					sql.Format(L"select Min(pX) xMin,Min(pY) yMin ,Max(pX) xMax,Max(pY) yMax from [%s] ",table);
					pSet=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
					break;
			}
			if(!hasInit&&data)
			{
				xMin=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)"xMin")));
				yMin=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)"yMin")));
				xMax=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)"xMax")));
				yMax=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)"yMax")));
				hasInit=true;
			}
			else if(data)
			{
				temp=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)"xMin")));
				if(xMin>temp) xMin=temp;
				temp=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)"yMin")));
				if(yMin>temp) yMin=temp;
				temp=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)"xMax")));
				if(xMax<temp) xMax=temp;
				temp=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)"yMax")));
				if(yMax<temp) yMax=temp;
			}
			else
			{;}
			pRst->MoveNext();
		}
		return true;
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}


void DBConnection::SearchByRect(CPoint p1,CPoint p2,CString table,gisPoint * gp,CZoomManager * pZoom,int& count)
{
	double x,y;
	CPoint point;
	CString ID;
	count=0;
	pRst=getRecordSet(table);
	while(!pRst->End)
	{
		x=strtof(GetResultByAttr(_T("pX")));
	    y=strtof(GetResultByAttr(_T("pY")));
		ID=GetResultByAttr(_T("ID"));
		point=pZoom->GPtoCP(DPoint(x,y));
		if(point.x>=p1.x&&point.x<=p2.x&&point.y>=p1.y&&point.y<=p2.y)
		{
			gp[count].point=pZoom->CPtoGP(point);
			gp[count].ID=ID;
			gp[count].color=strtoi(this->GetResultByAttr(L"color"));
			gp[count].size=strtoi(this->GetResultByAttr(L"size"));
			gp[count].pType=this->GetResultByAttr(L"type");
			count++;
		}
		pRst->MoveNext();
	}
	if(count==0) AfxMessageBox(L"没有找到相关记录");
}


void DBConnection::SearchByRect(CPoint p1,CPoint p2,CString table,gisLine * gl,CZoomManager * pZoom,int& count)
{
	double xMin,yMin,xMax,yMax;
	int pCount,i;
	CString ID;
	count=0;
	CString sql;
	DPoint * point=NULL;
	CPoint lt,rb;
	try
	{
		sql.Format(L"select * from [%s]",table);
		pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pRst->End)
		{
			xMin=strtof(this->GetResultByAttr(L"xMin"));
			yMin=strtof(this->GetResultByAttr(L"yMin"));
			xMax=strtof(this->GetResultByAttr(L"xMax"));
			yMax=strtof(this->GetResultByAttr(L"yMax"));
			lt=pZoom->GPtoCP(DPoint(xMin,yMax));
			rb=pZoom->GPtoCP(DPoint(xMax,yMin));
			if(p1.x>rb.x||p2.x<lt.x||p1.y>rb.y||p2.y<lt.y);
			else
			{
				pCount = strtoi(this->GetResultByAttr(L"pCount"));
				int w = strtoi(this->GetResultByAttr(L"LWidth"));
				ID=this->GetResultByAttr(L"ID");
				if(point!=NULL) delete [] point;
				point = new DPoint[pCount];
				this->readImgPoint(table,ID,point,pCount);
				if(w<0)
				{
					int nt=3;
					while(nt<pCount)
					{
						DPoint t1=point[nt-2];
						DPoint t2=point[nt-1];
						point[nt-2]=DPoint((point[nt-3].x+(t1.x-point[nt-3].x)/3),(point[nt-3].y+(t1.y-point[nt-3].y)/3));
						point[nt-1]=DPoint((point[nt].x+(t2.x-point[nt].x)/3),(point[nt].y+(t2.y-point[nt].y)/3));
						nt+=3;
					}
				}
				bool search=false;
				double b, k;
				i=1;
				while(i<pCount)
				{
					this->getInfo(pZoom->GPtoCP(point[i-1]),pZoom->GPtoCP(point[i]),xMin,xMax,yMax,yMin);
					lt=CPoint(xMin,yMin);
					rb=CPoint(xMax,yMax);
					if(rb.x<p1.x||lt.x>p2.x||lt.y>p2.y||rb.y<p1.y);
					else if((rb.x==lt.x&&lt.x>=p1.x&&lt.x<=p2.x)||(lt.y==rb.y&&lt.y>=p1.y&&lt.y<=p2.y)) search = true;
					else
					{
						k=(double)(pZoom->GPtoCP(point[i]).y-pZoom->GPtoCP(point[i-1]).y)/(pZoom->GPtoCP(point[i]).x-pZoom->GPtoCP(point[i-1]).x);
						b=(double)(pZoom->GPtoCP(point[i]).y-k*pZoom->GPtoCP(point[i]).x);
						if(k<0&&(p1.y<k*p1.x+b)&&(p2.y>k*p2.x+b)) search = true;
						else if(k>0&&(p1.y<k*p2.x+b)&&(p2.y>k*p1.x+b)) search = true;
					}
					if(search)
					{
						gl[count].pCount=pCount;
						gl[count].ID=ID;
						gl[count].color=strtoi(this->GetResultByAttr(L"LColor"));
						gl[count].lType=strtoi(this->GetResultByAttr(L"LType"));
						gl[count].width=strtoi(this->GetResultByAttr(L"LWidth"));
						gl[count].xMin=strtoi(this->GetResultByAttr(L"xMin"));
						gl[count].yMin=strtoi(this->GetResultByAttr(L"yMin"));
						gl[count].xMax=strtoi(this->GetResultByAttr(L"xMax"));
						gl[count].yMax=strtoi(this->GetResultByAttr(L"yMax"));
						if(gl[count].LinePoint!=NULL) delete [] gl[count].LinePoint;
						gl[count].LinePoint= new DPoint[pCount];
						this->readImgPoint(table,ID,gl[count].LinePoint,pCount);
						count++;
						break;

					}
					i++;
				}
			}
			pRst->MoveNext();
		}
		if(count==0) AfxMessageBox(L"没有找到相关记录!");
		if(point!=NULL) delete [] point;
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}


void DBConnection::SearchByRect(CPoint p1,CPoint p2,CString table,gisPolygon * gp,CZoomManager * pZoom,int& count)
{
	double xMin,yMin,xMax,yMax;
	int pCount,i;
	CString ID;
	count=0;
	CString sql;
	DPoint * point=NULL;
	CPoint lt,rb;
	gisPolygon gpp;
	try
	{
		sql.Format(L"select * from [%s]",table);
		pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pRst->End)
		{
			xMin=strtof(this->GetResultByAttr(L"xMin"));
			yMin=strtof(this->GetResultByAttr(L"yMin"));
			xMax=strtof(this->GetResultByAttr(L"xMax"));
			yMax=strtof(this->GetResultByAttr(L"yMax"));
			lt=pZoom->GPtoCP(DPoint(xMin,yMax));
			rb=pZoom->GPtoCP(DPoint(xMax,yMin));
			if(p1.x>rb.x||p2.x<lt.x||p1.y>rb.y||p2.y<lt.y);
			else
			{
				pCount = strtoi(this->GetResultByAttr(L"pCount"));
				ID=this->GetResultByAttr(L"ID");
				if(point!=NULL) delete [] point;
				point = new DPoint[pCount];
				this->readImgPoint(table,ID,point,pCount);
				bool search=false;
				double b, k;
				i=1;
				while(i<pCount)
				{
					this->getInfo(pZoom->GPtoCP(point[i-1]),pZoom->GPtoCP(point[i]),xMin,xMax,yMax,yMin);
					lt=CPoint(xMin,yMin);
					rb=CPoint(xMax,yMax);
					if(rb.x<p1.x||lt.x>p2.x||lt.y>p2.y||rb.y<p1.y);
					else if((rb.x==lt.x&&lt.x>=p1.x&&lt.x<=p2.x)||(lt.y==rb.y&&lt.y>=p1.y&&lt.y<=p2.y)) search = true;
					else if(this->SearchPolyByClick(p1,gpp,table,pZoom)||this->SearchPolyByClick(p2,gpp,table,pZoom)) search=true;
					else
					{
						k=(double)(pZoom->GPtoCP(point[i]).y-pZoom->GPtoCP(point[i-1]).y)/(pZoom->GPtoCP(point[i]).x-pZoom->GPtoCP(point[i-1]).x);
						b=(double)(pZoom->GPtoCP(point[i]).y-k*pZoom->GPtoCP(point[i]).x);
						if(k<0&&(p1.y<k*p1.x+b)&&(p2.y>k*p2.x+b)) search = true;
						else if(k>0&&(p1.y<k*p2.x+b)&&(p2.y>k*p1.x+b)) search = true;
					}
					if(search)
					{
						gp[count].pCount=pCount;
						gp[count].ID=ID;
						gp[count].color=strtoi(this->GetResultByAttr(L"color"));
						gp[count].BorderColor=strtoi(this->GetResultByAttr(L"BorderColor"));
						gp[count].BorderType=strtoi(this->GetResultByAttr(L"BorderType"));
						gp[count].xMin=strtoi(this->GetResultByAttr(L"xMin"));
						gp[count].yMin=strtoi(this->GetResultByAttr(L"yMin"));
						gp[count].xMax=strtoi(this->GetResultByAttr(L"xMax"));
						gp[count].yMax=strtoi(this->GetResultByAttr(L"yMax"));
						if(gp[count].point!=NULL) delete [] gp[count].point;
						gp[count].point= new DPoint[pCount];
						this->readImgPoint(table,ID,gp[count].point,pCount);
						count++;
						break;
					}
					i++;
				}
			}
			pRst->MoveNext();
		}
		if(count==0) AfxMessageBox(L"没有找到相关记录!");
		if(point!=NULL) delete [] point;
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}

void DBConnection::SearchByRect(CPoint p1,CPoint p2,CString table,gisMarker * gm,CZoomManager * pZoom,int& count,CDC * pDC)
{
	double x,y;
	CString ID;
	count=0;
	CString sql,text;
	CPoint lt,rb;
	LOGFONT lfont;
	HFONT hfont ;
	CFont* pfont ;
	CFont *oldfont;
	CSize size;
	gisPolygon gpp;
	try
	{
		sql.Format(L"select * from [%s]",table);
		pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pRst->End)
		{
			ID=this->GetResultByAttr(L"ID");
			text=this->GetResultByAttr(L"Lable");
			x=strtof(this->GetResultByAttr(L"pX"));
			y=strtof(this->GetResultByAttr(L"pY"));
			this->readImgFont(table,ID,lfont);
			hfont = ::CreateFontIndirect(&lfont);
			pfont = CFont::FromHandle(hfont);
		    oldfont=pDC->SelectObject(pfont);
			size=pDC->GetTextExtent(text);
			size.cx*=pZoom->ntimes();
			size.cy*=pZoom->ntimes();
			pDC->SelectObject(oldfont);
			lt=pZoom->GPtoCP(DPoint(x,y));
			rb=CPoint(lt.x+size.cx,lt.y+size.cy);
			if(p1.x>rb.x||p2.x<lt.x||p1.y>rb.y||p2.y<lt.y);
			else
			{
         		gm[count].color=strtoi(this->GetResultByAttr(L"color"));
				gm[count].ID=ID;
				gm[count].lfont=lfont;
				gm[count].point=DPoint(x,y);
				gm[count].text=text;
				count++;
			}
			pRst->MoveNext();
		}
		if(count==0) AfxMessageBox(L"没有找到相关记录!");
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}

void DBConnection::SearchByCircle(CPoint p1,CPoint p2,CString table,gisPoint * gp,CZoomManager * pZoom,int& count)
{
	CPoint center=CPoint((p2.x+p1.x)/2,(p2.y+p1.y)/2);
	double r=(p2.x-p1.x)/2;
	double x,y,dist;
	CPoint point;
	count=0;
	pRst=getRecordSet(table);
	while(!pRst->End)
	{
		x=strtof(GetResultByAttr(_T("pX")));
	    y=strtof(GetResultByAttr(_T("pY")));
		point=pZoom->GPtoCP(DPoint(x,y));
		if(point.x>center.x-r&&point.x<center.x+r&&point.y>center.y-r&&point.y<center.y+r)
		{
			dist=sqrt((double)(center.x-point.x)*(center.x-point.x)+(center.y-point.y)*(center.y-point.y));
			if(dist<r)
			{
				gp[count].point=pZoom->CPtoGP(point);
				gp[count].ID=this->GetResultByAttr(L"ID");
				gp[count].color=strtoi(this->GetResultByAttr(L"color"));
				gp[count].size=strtoi(this->GetResultByAttr(L"size"));
				gp[count].pType=this->GetResultByAttr(L"type");
				count++;
			}
		}
		pRst->MoveNext();
	}
	if(count==0) AfxMessageBox(L"没有找到相关记录");
}


void DBConnection::SearchByCircle(CPoint p1,CPoint p2,CString table,gisLine * gl,CZoomManager * pZoom,int& count)
{
	double xMin,yMin,xMax,yMax,s,s1,s2,s3,h,p;
	double r=(p2.x-p1.x)/2;
	int pCount,i;
	CString ID;
	count=0;
	CString sql;
	DPoint * point=NULL;
	CPoint lt,rb,p3,p4,center=CPoint((p2.x+p1.x)/2,(p2.y+p1.y)/2);
	try
	{
		sql.Format(L"select * from [%s]",table);
		pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pRst->End)
		{
			xMin=strtof(this->GetResultByAttr(L"xMin"));
			yMin=strtof(this->GetResultByAttr(L"yMin"));
			xMax=strtof(this->GetResultByAttr(L"xMax"));
			yMax=strtof(this->GetResultByAttr(L"yMax"));
			lt=pZoom->GPtoCP(DPoint(xMin,yMax));
			rb=pZoom->GPtoCP(DPoint(xMax,yMin));
			if(center.x-r>rb.x||center.x+r<lt.x||center.y-r>rb.y||center.y+r<lt.y);
			else
			{
				i=1;
				pCount = strtoi(this->GetResultByAttr(L"pCount"));
				int w= strtoi(this->GetResultByAttr(L"LWidth"));
				ID=this->GetResultByAttr(L"ID");
				if(point!=NULL) delete [] point;
				point = new DPoint[pCount];
				this->readImgPoint(table,ID,point,pCount);
				bool search=false;
				if(w>=0)
				{
					while(i<pCount)
					{
						this->getInfo(pZoom->GPtoCP(point[i-1]),pZoom->GPtoCP(point[i]),xMin,xMax,yMax,yMin);
						lt=CPoint(xMin,yMin);
						rb=CPoint(xMax,yMax);
						if(rb.x<center.x-r||lt.x>center.x+r||lt.y>center.y+r||rb.y<center.y-r);
						else
						{
							p3=pZoom->GPtoCP(point[i-1]);p4=pZoom->GPtoCP(point[i]);
							s1=sqrt(pow((double)p3.x-p4.x,2)+pow((double)p3.y-p4.y,2));
							s2=sqrt(pow((double)p3.x-center.x,2)+pow((double)p3.y-center.y,2));
							s3=sqrt(pow((double)p4.x-center.x,2)+pow((double)p4.y-center.y,2));
							p=(s1+s2+s3)/2;
							s=sqrt(p*(p-s1)*(p-s2)*(p-s3));
							h=2*s/s1;
							if(h<r) search =true;
						}
						if(search)
						{
							gl[count].pCount=pCount;
							gl[count].ID=ID;
							gl[count].color=strtoi(this->GetResultByAttr(L"LColor"));
							gl[count].lType=strtoi(this->GetResultByAttr(L"LType"));
							gl[count].width=strtoi(this->GetResultByAttr(L"LWidth"));
							gl[count].xMin=strtoi(this->GetResultByAttr(L"xMin"));
							gl[count].yMin=strtoi(this->GetResultByAttr(L"yMin"));
							gl[count].xMax=strtoi(this->GetResultByAttr(L"xMax"));
							gl[count].yMax=strtoi(this->GetResultByAttr(L"yMax"));
							if(gl[count].LinePoint!=NULL) delete [] gl[count].LinePoint;
							gl[count].LinePoint= new DPoint[pCount];
							this->readImgPoint(table,ID,gl[count].LinePoint,pCount);
							count++;
							break;
						}
						i++;
					}
				}
				else
				{
					CPoint* npt=new CPoint[pCount];
					int nt=0;
					while(nt<pCount)
					{
						npt[nt]=pZoom->GPtoCP(point[nt]);
						nt++;
					}
					nt=3;
					while(nt<pCount)
					{
						CPoint t1=npt[nt-2];
						CPoint t2=npt[nt-1];
						npt[nt-2]=CPoint((npt[nt-3].x+(t1.x-npt[nt-3].x)/3),(npt[nt-3].y+(t1.y-npt[nt-3].y)/3));
						npt[nt-1]=CPoint((npt[nt].x+(t2.x-npt[nt].x)/3),(npt[nt].y+(t2.y-npt[nt].y)/3));
						nt+=3;
					}
					while(i<pCount)
					{
						this->getInfo(npt[i-1],npt[i],xMin,xMax,yMax,yMin);
						lt=CPoint(xMin,yMin);
						rb=CPoint(xMax,yMax);
						if(rb.x<center.x-r||lt.x>center.x+r||lt.y>center.y+r||rb.y<center.y-r);
						else
						{
							p3=npt[i-1];p4=npt[i];
							s1=sqrt(pow((double)p3.x-p4.x,2)+pow((double)p3.y-p4.y,2));
							s2=sqrt(pow((double)p3.x-center.x,2)+pow((double)p3.y-center.y,2));
							s3=sqrt(pow((double)p4.x-center.x,2)+pow((double)p4.y-center.y,2));
							p=(s1+s2+s3)/2;
							s=sqrt(p*(p-s1)*(p-s2)*(p-s3));
							h=2*s/s1;
							if(h<r) search =true;
						}
						if(search)
						{
							gl[count].pCount=pCount;
							gl[count].ID=ID;
							gl[count].color=strtoi(this->GetResultByAttr(L"LColor"));
							gl[count].lType=strtoi(this->GetResultByAttr(L"LType"));
							gl[count].width=strtoi(this->GetResultByAttr(L"LWidth"));
							gl[count].xMin=strtoi(this->GetResultByAttr(L"xMin"));
							gl[count].yMin=strtoi(this->GetResultByAttr(L"yMin"));
							gl[count].xMax=strtoi(this->GetResultByAttr(L"xMax"));
							gl[count].yMax=strtoi(this->GetResultByAttr(L"yMax"));
							if(gl[count].LinePoint!=NULL) delete [] gl[count].LinePoint;
							gl[count].LinePoint= new DPoint[pCount];
							this->readImgPoint(table,ID,gl[count].LinePoint,pCount);
							count++;
							break;
						}
						i++;
					}
				}
			}
			pRst->MoveNext();
		}
		if(count==0) AfxMessageBox(L"没有找到相关记录!");
		if(point!=NULL) delete [] point;
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}

void DBConnection::SearchByCircle(CPoint p1,CPoint p2,CString table,gisPolygon * gp,CZoomManager * pZoom,int& count)
{
	double xMin,yMin,xMax,yMax,s,s1,s2,s3,h,p;
	double r=(p2.x-p1.x)/2;
	int pCount,i;
	CString ID;
	count=0;
	CString sql;
	DPoint * point=NULL;
	CPoint * pt=NULL;
	CPoint lt,rb,p3,p4,center=CPoint((p2.x+p1.x)/2,(p2.y+p1.y)/2);;
	try
	{
		sql.Format(L"select * from [%s]",table);
		pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pRst->End)
		{
			bool search=false;
			xMin=strtof(this->GetResultByAttr(L"xMin"));
			yMin=strtof(this->GetResultByAttr(L"yMin"));
			xMax=strtof(this->GetResultByAttr(L"xMax"));
			yMax=strtof(this->GetResultByAttr(L"yMax"));
			lt=pZoom->GPtoCP(DPoint(xMin,yMax));
			rb=pZoom->GPtoCP(DPoint(xMax,yMin));
			if(center.x-r>rb.x||center.x+r<lt.x||center.y-r>rb.y||center.y+r<lt.y);
			else
			{
				i=1;
				pCount = strtoi(this->GetResultByAttr(L"pCount"));
				ID=this->GetResultByAttr(L"ID");
				if(point!=NULL) delete [] point;
				point = new DPoint[pCount];
				pt = new CPoint[pCount];
				this->readImgPoint(table,ID,point,pCount);
				this->convertDPtoCP(point,pt,pCount,pZoom);
				if(this->PointInsidePolygon(p1,pt,pCount)||this->PointInsidePolygon(p2,pt,pCount)){search = true;goto hasSearch;}
				while(i<pCount)
				{
					this->getInfo(pZoom->GPtoCP(point[i-1]),pZoom->GPtoCP(point[i]),xMin,xMax,yMax,yMin);
					lt=CPoint(xMin,yMin);
					rb=CPoint(xMax,yMax);
					if(rb.x<center.x-r||lt.x>center.x+r||lt.y>center.y+r||rb.y<center.y-r);
					else
					{
						p3=pZoom->GPtoCP(point[i-1]);p4=pZoom->GPtoCP(point[i]);
						s1=sqrt(pow((double)p3.x-p4.x,2)+pow((double)p3.y-p4.y,2));
						s2=sqrt(pow((double)p3.x-center.x,2)+pow((double)p3.y-center.y,2));
						s3=sqrt(pow((double)p4.x-center.x,2)+pow((double)p4.y-center.y,2));
						p=(s1+s2+s3)/2;
						s=sqrt(p*(p-s1)*(p-s2)*(p-s3));
					    h=2*s/s1;
						if(h<=r){search =true;break;}
					}
					i++;
				}
hasSearch:		if(search)
				{
					gp[count].pCount=pCount;
					gp[count].ID=ID;
					gp[count].color=strtoi(this->GetResultByAttr(L"color"));
					gp[count].BorderColor=strtoi(this->GetResultByAttr(L"BorderColor"));
					gp[count].BorderType=strtoi(this->GetResultByAttr(L"BorderType"));
					gp[count].xMin=strtoi(this->GetResultByAttr(L"xMin"));
					gp[count].yMin=strtoi(this->GetResultByAttr(L"yMin"));
					gp[count].xMax=strtoi(this->GetResultByAttr(L"xMax"));
					gp[count].yMax=strtoi(this->GetResultByAttr(L"yMax"));
					if(gp[count].point!=NULL) delete [] gp[count].point;
					gp[count].point= new DPoint[pCount];
					this->readImgPoint(table,ID,gp[count].point,pCount);
					count++;
				}
			}
			pRst->MoveNext();
		}
		if(count==0) AfxMessageBox(L"没有找到相关记录!");
		if(point!=NULL) delete []point;
		if(pt!=NULL) delete [] pt;
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}


void DBConnection::SearchByCircle(CPoint p1,CPoint p2,CString table,gisMarker * gm,CZoomManager * pZoom,int& count,CDC * pDC)
{
	CPoint center=CPoint((p2.x+p1.x)/2,(p2.y+p1.y)/2);
	double r=(p2.x-p1.x)/2;
	double x,y;
	CPoint p3,p4;
	gisPolygon gpp;
	CString ID;
	count=0;
	CString sql,text;
	CPoint lt,rb;
	LOGFONT lfont;
	HFONT hfont ;
	CFont* pfont ;
	CFont *oldfont;
	CSize size;
	try
	{
		sql.Format(L"select * from [%s]",table);
		pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pRst->End)
		{
			bool search=false;
			ID=this->GetResultByAttr(L"ID");
			text=this->GetResultByAttr(L"Lable");
			x=strtof(this->GetResultByAttr(L"pX"));
			y=strtof(this->GetResultByAttr(L"pY"));
			this->readImgFont(table,ID,lfont);
			hfont = ::CreateFontIndirect(&lfont);
			pfont = CFont::FromHandle(hfont);
		    oldfont=pDC->SelectObject(pfont);
			size=pDC->GetTextExtent(text);
			size.cx*=pZoom->ntimes();
			size.cy*=pZoom->ntimes();
			pDC->SelectObject(oldfont);
			lt=pZoom->GPtoCP(DPoint(x,y));
			rb=CPoint(lt.x+size.cx,lt.y+size.cy);
			p3=CPoint(lt.x,rb.y);p3=CPoint(rb.x,lt.y);
			if(center.x-r>rb.x||center.x+r<lt.x||center.y-r>rb.y||center.y+r<lt.y);
			else
			{
				if(sqrt((double)(center.x-lt.x)*(center.x-lt.x)+(center.y-lt.y)*(center.y-lt.y))<=r)
					search =true;
				else if(sqrt((double)(center.x-rb.x)*(center.x-rb.x)+(center.y-rb.y)*(center.y-rb.y))<=r)
					search =true;
				else if(sqrt((double)(center.x-p3.x)*(center.x-p3.x)+(center.y-p3.y)*(center.y-p3.y))<=r)
					search =true;
				else if(sqrt((double)(center.x-p4.x)*(center.x-p4.x)+(center.y-p4.y)*(center.y-p4.y))<=r)
					search =true;
			}
			if(search)
			{
				gm[count].color=strtoi(this->GetResultByAttr(L"color"));
				gm[count].ID=ID;
				gm[count].lfont=lfont;
				gm[count].point=DPoint(x,y);
				gm[count].text=text;
				count++;
			}
			pRst->MoveNext();
		}
		if(count==0) AfxMessageBox(L"没有找到相关记录!");
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}


void DBConnection::SearchByPolygon(CPoint * point,int n,CString table,gisPoint * gp,CZoomManager * pZoom,int& count)////射线法
{
	double xMin,xMax,yMax,yMin,x,y,k,b;
	CPoint p;
	count=0;
	int line=0;
	try
	{
		pRst=getRecordSet(table);
		while(!pRst->End)
		{
			line=0;
			x=strtof(GetResultByAttr(_T("pX")));
			y=strtof(GetResultByAttr(_T("pY")));
			p=pZoom->GPtoCP(DPoint(x,y));
			for(int i=1;i<n+1;i++)
			{
				this->getInfo(point[i-1],point[i%n],xMin,xMax,yMax,yMin);
				if(xMin==xMax) continue;//竖直线
				if(p.x<xMin||p.x>xMax) continue;////点x的坐标超过直线的x范围
				if((yMin==yMax)&&(p.y>yMax)){line++;continue;}///点x的坐标在直线的x范围内且点在线的下方
				///y=kx+b,判断点是否在直线的下方
				k=(double)(point[i-1].y-point[i%n].y)/(point[i-1].x-point[i%n].x);
				b=(double)(point[i-1].y-k*point[i-1].x);
				if((double)p.y>=k*p.x+b)
					line++;
			}
			if(line%2!=0)////射线与多边形的交点为奇数，则点在多边形内
			{
				gp[count].point=pZoom->CPtoGP(p);
				gp[count].ID=this->GetResultByAttr(L"ID");
				gp[count].color=strtoi(this->GetResultByAttr(L"color"));
				gp[count].size=strtoi(this->GetResultByAttr(L"size"));
				gp[count].pType=this->GetResultByAttr(L"type");
				count++;		
			}
			pRst->MoveNext();
		}
		if(count==0) AfxMessageBox(L"没有找到相关记录");
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}

void DBConnection::SearchByPolygon(CPoint * point,int n,CString table,gisLine * gl,CZoomManager * pZoom,int& count)
{
	int pCount,j,i=0;
	double r,r1,r2,t;
	CString ID;
	count=0;
	CString sql;
	CPoint lt,rb;
	DPoint * dpt=NULL;
	try
	{
		sql.Format(L"select * from [%s]",table);
		pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pRst->End)
		{
			i=j=1;
			bool search=false;
			pCount = strtoi(this->GetResultByAttr(L"pCount"));
			int w= strtoi(this->GetResultByAttr(L"LWidth"));
			ID=this->GetResultByAttr(L"ID");
			dpt = new DPoint[pCount];
			this->readImgPoint(table,ID,dpt,pCount);
			if(w<0)
			{
				int nt=3;
				while(nt<pCount)
				{
					DPoint t1=dpt[nt-2];
					DPoint t2=dpt[nt-1];
					dpt[nt-2]=DPoint((dpt[nt-3].x+(t1.x-dpt[nt-3].x)/3),(dpt[nt-3].y+(t1.y-dpt[nt-3].y)/3));
					dpt[nt-1]=DPoint((dpt[nt].x+(t2.x-dpt[nt].x)/3),(dpt[nt].y+(t2.y-dpt[nt].y)/3));
					nt+=3;
				}
			}
			while(i<pCount)
			{
				j=1;
				while(j<n+1)
				{
					r1=point[j-1].y*(point[j%n].x-point[j-1].x)-pZoom->GPtoCP(dpt[i-1]).y*(point[j%n].x-point[j-1].x)+(pZoom->GPtoCP(dpt[i-1]).x-point[j-1].x)*(point[j%n].y-point[j-1].y);
					r2=(pZoom->GPtoCP(dpt[i]).y-pZoom->GPtoCP(dpt[i-1]).y)*(point[j%n].x-point[j-1].x)-(pZoom->GPtoCP(dpt[i]).x-pZoom->GPtoCP(dpt[i-1]).x)*(point[j%n].y-point[j-1].y);
					r=r1/r2;
					t=(pZoom->GPtoCP(dpt[i-1]).x-point[j-1].x+r*(pZoom->GPtoCP(dpt[i]).x-pZoom->GPtoCP(dpt[i-1]).x))/(point[j%n].x-point[j-1].x);
					if(0<r&&r<1&&0<t&&t<1){ search = true;break;}
					j++;
				}
				if(search) break;
				i++;
			}
			if(!search&&this->PointInsidePolygon(pZoom->GPtoCP(dpt[0]),point,n))///判断线是否在多边形内部
				search=true;
        	if(search)
			{
				gl[count].pCount=pCount;
				gl[count].ID=ID;
				gl[count].color=strtoi(this->GetResultByAttr(L"LColor"));
				gl[count].lType=strtoi(this->GetResultByAttr(L"LType"));
				gl[count].width=strtoi(this->GetResultByAttr(L"LWidth"));
				gl[count].xMin=strtoi(this->GetResultByAttr(L"xMin"));
				gl[count].yMin=strtoi(this->GetResultByAttr(L"yMin"));
				gl[count].xMax=strtoi(this->GetResultByAttr(L"xMax"));
				gl[count].yMax=strtoi(this->GetResultByAttr(L"yMax"));
				if(gl[count].LinePoint!=NULL) delete [] gl[count].LinePoint;
				gl[count].LinePoint= new DPoint[pCount];
				this->readImgPoint(table,ID,gl[count].LinePoint,pCount);
				count++;
			}
		    pRst->MoveNext();
		}
		if(count==0) AfxMessageBox(L"没有找到相关记录!");
		if(dpt!=NULL) delete [] dpt;
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}


void DBConnection::SearchByPolygon(CPoint * point,int n,CString table,gisPolygon * gp,CZoomManager * pZoom,int& count)
{
	int pCount,j,i=0;
	double r,r1,r2,t;
	CString ID;
	count=0;
	CString sql;
	CPoint lt,rb;
	DPoint * dpt=NULL;
	CPoint * pt=NULL;
	try
	{
		sql.Format(L"select * from [%s]",table);
		pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pRst->End)
		{
			i=j=1;
			bool search=false;
			pCount = strtoi(this->GetResultByAttr(L"pCount"));
			ID=this->GetResultByAttr(L"ID");
			dpt = new DPoint[pCount];
			pt=new CPoint[pCount];
			this->readImgPoint(table,ID,dpt,pCount);
			this->convertDPtoCP(dpt,pt,pCount,pZoom);
			while(i<pCount+1)
			{
				j=1;
				while(j<n+1)
				{
					r1=point[j-1].y*(point[j%n].x-point[j-1].x)-pZoom->GPtoCP(dpt[i-1]).y*(point[j%n].x-point[j-1].x)+(pZoom->GPtoCP(dpt[i-1]).x-point[j-1].x)*(point[j%n].y-point[j-1].y);
					r2=(pZoom->GPtoCP(dpt[i%pCount]).y-pZoom->GPtoCP(dpt[i-1]).y)*(point[j%n].x-point[j-1].x)-(pZoom->GPtoCP(dpt[i%pCount]).x-pZoom->GPtoCP(dpt[i-1]).x)*(point[j%n].y-point[j-1].y);
					r=r1/r2;
					t=(pZoom->GPtoCP(dpt[i-1]).x-point[j-1].x+r*(pZoom->GPtoCP(dpt[i%pCount]).x-pZoom->GPtoCP(dpt[i-1]).x))/(point[j%n].x-point[j-1].x);
					if(0<r&&r<1&&0<t&&t<1){ search = true;break;}
					j++;
				}
				if(search) break;
				i++;
			}
			if(!search&&this->PointInsidePolygon(pt[0],point,n)) ///目标区在查找多边形内
				search=true;
			else if(!search&&this->PointInsidePolygon(point[0],pt,pCount))///查找多边形在目标区内
				search=true;
        	if(search)
			{
				gp[count].pCount=pCount;
				gp[count].ID=ID;
				gp[count].color=strtoi(this->GetResultByAttr(L"color"));
				gp[count].BorderColor=strtoi(this->GetResultByAttr(L"BorderColor"));
				gp[count].BorderType=strtoi(this->GetResultByAttr(L"BorderType"));
				gp[count].xMin=strtoi(this->GetResultByAttr(L"xMin"));
				gp[count].yMin=strtoi(this->GetResultByAttr(L"yMin"));
				gp[count].xMax=strtoi(this->GetResultByAttr(L"xMax"));
				gp[count].yMax=strtoi(this->GetResultByAttr(L"yMax"));
				if(gp[count].point!=NULL) delete [] gp[count].point;
				gp[count].point= new DPoint[pCount];
				this->readImgPoint(table,ID,gp[count].point,pCount);
				count++;
			}
		    pRst->MoveNext();
		}
		if(count==0) AfxMessageBox(L"没有找到相关记录!");
		if(dpt!=NULL) delete [] dpt;
		if(pt!=NULL) delete [] pt;
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}


void DBConnection::SearchByPolygon(CPoint * point,int n,CString table,gisMarker * gm,CZoomManager * pZoom,int& count,CDC* pDC)
{
	double x,y,r,r1,r2,t;
	int i=0,j=1;
	CString ID;
	count=0;
	CString sql,text;
	LOGFONT lfont;
	HFONT hfont ;
	CFont* pfont ;
	CFont *oldfont;
	CSize size;
	CPoint dpt[4];
	try
	{
		sql.Format(L"select * from [%s]",table);
		pRst=pConn->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!pRst->End)
		{
			i=j=1;
			bool search=false;
			ID=this->GetResultByAttr(L"ID");
			text=this->GetResultByAttr(L"Lable");
			x=strtof(this->GetResultByAttr(L"pX"));
			y=strtof(this->GetResultByAttr(L"pY"));
			this->readImgFont(table,ID,lfont);
			hfont = ::CreateFontIndirect(&lfont);
			pfont = CFont::FromHandle(hfont);
		    oldfont=pDC->SelectObject(pfont);
			size=pDC->GetTextExtent(text);
			size.cx*=pZoom->ntimes();
			size.cy*=pZoom->ntimes();
			pDC->SelectObject(oldfont);
			dpt[0]=pZoom->GPtoCP(DPoint(x,y));
			dpt[2]=CPoint(dpt[0].x+size.cx,dpt[0].y+size.cy);
			dpt[1]=CPoint(dpt[2].x,dpt[0].y);
			dpt[3]=CPoint(dpt[0].x,dpt[2].y);
			while(i<5)
			{
				j=1;
				while(j<n+1)
				{
					r1=point[j-1].y*(point[j%n].x-point[j-1].x)-dpt[i-1].y*(point[j%n].x-point[j-1].x)+(dpt[i-1].x-point[j-1].x)*(point[j%n].y-point[j-1].y);
					r2=(dpt[i%4].y-dpt[i-1].y)*(point[j%n].x-point[j-1].x)-(dpt[i%4].x-dpt[i-1].x)*(point[j%n].y-point[j-1].y);
					r=r1/r2;
					t=(dpt[i-1].x-point[j-1].x+r*(dpt[i%4].x-dpt[i-1].x))/(point[j%n].x-point[j-1].x);
					if(0<r&&r<1&&0<t&&t<1){ search = true;break;}
					j++;
				}
				if(search) break;
				i++;
			}
			if(!search&&this->PointInsidePolygon(dpt[0],point,n))
				search=true;
			else if(!search&&this->PointInsidePolygon(point[0],dpt,4))
				search=true;
        	if(search)
			{
				gm[count].color=strtoi(this->GetResultByAttr(L"color"));
				gm[count].ID=ID;
				gm[count].lfont=lfont;
				gm[count].point=DPoint(x,y);
				gm[count].text=text;
				count++;
			}
			pRst->MoveNext();
		}
		if(count==0) AfxMessageBox(L"没有找到相关记录!");
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
}


void DBConnection::convertDPtoCP(const DPoint * dt,CPoint * pt,int n,CZoomManager  * pZoom)
{
	int i=0;
	while(i<n)
	{
		pt[i]=pZoom->GPtoCP(dt[i]);
		i++;
	}
}

bool DBConnection::PointInsidePolygon(CPoint p,CPoint * pt,int n)
{
/*	double xMin,xMax,yMax,yMin,k,b;
	int line=0;
	for(int i=1;i<n+1;i++)
	{
		this->getInfo(pt[i-1],pt[i%n],xMin,xMax,yMax,yMin);
		if(xMin==xMax&&p.x==xMin){line++; continue;}//竖直线
		if(p.x<xMin||p.x>xMax) continue;////点x的坐标超过直线的x范围
		if((yMin==yMax)&&(p.y>yMax)){line++;continue;}///点x的坐标在直线的x范围内且点在线的下方
		///y=kx+b,判断点是否在直线的下方
		k=(double)(pt[i-1].y-pt[i%n].y)/(double)(pt[i-1].x-pt[i%n].x);
		b=(double)(pt[i-1].y-k*pt[i-1].x);
		if((double)p.y>=(double)k*p.x+b)
			line++;
	}
	if(line%2!=0)////射线与多边形的交点为奇数，则点在多边形内
		return true;
	return false;*/
	CRgn r;
	r.CreatePolygonRgn(pt,n,ALTERNATE);//WINDING);
	return r.PtInRegion(p);
}

bool DBConnection::LineDjPolygon(CString lineTable,CString PolyTable,CString ResultTable,CZoomManager  * pZoom)
{
	_RecordsetPtr pLineSet , pPolySet;
	int lpCount,ppCount,count=0;
	int color,width,type;
	double xMin_l,yMin_l,xMax_l,yMax_l,xMin_p,yMin_p,xMax_p,yMax_p;
	double r,r1,r2,t;
	CPoint lt_l,rb_l,lt_p,rb_p,p1,p2,p3,p4;
	CString sql,lID,pID;
	gisLine  gl(2);
	DPoint * lpt=NULL,* ppt=NULL;
	CPoint * jp = NULL;
	sql.Format(L"select * from [%s]",lineTable);
	pLineSet = pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	sql.Format(L"select * from [%s]",PolyTable);
	pPolySet = pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	sql.Format(L"select * from [CatalogTable] where gisTable='%s'",ResultTable);
	pRst = pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	if(pLineSet->End){AfxMessageBox(L"线表为空！"); return false;}
	else if(pPolySet->End){AfxMessageBox(L"区表为空！"); return false;}
	if(!pRst->End){AfxMessageBox(L"目标表已存在，请重新命名目标表！"); return false;}
	else{this->CreateTable(ResultTable,2);}
	while(!pLineSet->End)
	{
		lpCount = strtoi((CString)(BSTR)((_bstr_t)pLineSet->GetCollect((_variant_t)L"pCount")));
		xMin_l=strtof((CString)(BSTR)((_bstr_t)pLineSet->GetCollect((_variant_t)L"xMin")));
		yMin_l=strtof((CString)(BSTR)((_bstr_t)pLineSet->GetCollect((_variant_t)L"yMin")));
		xMax_l=strtof((CString)(BSTR)((_bstr_t)pLineSet->GetCollect((_variant_t)L"xMax")));
		yMax_l=strtof((CString)(BSTR)((_bstr_t)pLineSet->GetCollect((_variant_t)L"yMax")));
		color = strtoi((CString)(BSTR)((_bstr_t)pLineSet->GetCollect((_variant_t)L"LColor")));
		type=strtoi((CString)(BSTR)((_bstr_t)pLineSet->GetCollect((_variant_t)L"LType")));
		width=strtoi((CString)(BSTR)((_bstr_t)pLineSet->GetCollect((_variant_t)L"LWidth")));
		lt_l=pZoom->GPtoCP(DPoint(xMin_l,yMax_l));
		rb_l=pZoom->GPtoCP(DPoint(xMax_l,yMin_l));
		pPolySet->MoveFirst();
		while(!pPolySet->End)
		{
			xMin_p=strtof((CString)(BSTR)((_bstr_t)pPolySet->GetCollect((_variant_t)L"xMin")));
			yMin_p=strtof((CString)(BSTR)((_bstr_t)pPolySet->GetCollect((_variant_t)L"yMin")));
			xMax_p=strtof((CString)(BSTR)((_bstr_t)pPolySet->GetCollect((_variant_t)L"xMax")));
			yMax_p=strtof((CString)(BSTR)((_bstr_t)pPolySet->GetCollect((_variant_t)L"yMax")));
			ppCount = strtoi((CString)(BSTR)((_bstr_t)pPolySet->GetCollect((_variant_t)L"pCount")));
			lt_p=pZoom->GPtoCP(DPoint(xMin_p,yMax_p));
			rb_p=pZoom->GPtoCP(DPoint(xMax_p,yMin_p));
			if(lt_l.x>=rb_p.x||lt_l.y>=rb_p.y||rb_l.x<=lt_p.x||rb_l.y<=lt_p.y);///线的外包矩形和区的外包矩形不相交
			else
			{
				int i=1,j=1;
				count=0;
				lID=(CString)(BSTR)((_bstr_t)pLineSet->GetCollect((_variant_t)L"ID"));
				pID=(CString)(BSTR)((_bstr_t)pPolySet->GetCollect((_variant_t)L"ID"));
				if(lpt!=NULL) delete [] lpt;
				if(ppt!=NULL) delete [] ppt;
				if(jp!=NULL) delete [] jp;
				lpt = new DPoint[lpCount];
				ppt = new DPoint[ppCount];
				jp=new CPoint[ppCount-1];
				this->readImgPoint(lineTable,lID,lpt,lpCount);
				int w=strtoi((CString)(BSTR)((_bstr_t)pLineSet->GetCollect((_variant_t)L"LWidth")));
				this->readImgPoint(PolyTable,pID,ppt,ppCount);
				if(w<0)
				{
					int nt=3;
					while(nt<lpCount)
					{
						DPoint t1=lpt[nt-2];
						DPoint t2=lpt[nt-1];
						lpt[nt-2]=DPoint((lpt[nt-3].x+(t1.x-lpt[nt-3].x)/3),(lpt[nt-3].y+(t1.y-lpt[nt-3].y)/3));
						lpt[nt-1]=DPoint((lpt[nt].x+(t2.x-lpt[nt].x)/3),(lpt[nt].y+(t2.y-lpt[nt].y)/3));
						nt+=3;
					}					
				}/**/
				CPoint * p = new CPoint[ppCount];
			    this->convertDPtoCP(ppt,p,ppCount,pZoom);
				while(i<lpCount)////线线段
				{
					count=0;j=1;
					p1=pZoom->GPtoCP(lpt[i-1]);p2=pZoom->GPtoCP(lpt[i]);
					while(j<ppCount+1)///区线段
					{
						p3=pZoom->GPtoCP(ppt[j-1]);p4=pZoom->GPtoCP(ppt[j%ppCount]);
						r1=p3.y*(p4.x-p3.x)-p1.y*(p4.x-p3.x)+(p1.x-p3.x)*(p4.y-p3.y);
						r2=(p2.y-p1.y)*(p4.x-p3.x)-(p2.x-p1.x)*(p4.y-p3.y);
						r=r1/r2;
						t=(p1.x-p3.x+r*(p2.x-p1.x))/(p4.x-p3.x);
						if(0<r&&r<1&&0<t&&t<1)///线段相交
						{
							///求交点
							double d=(double)(p2.y-p1.y)*(double)(p4.x-p3.x)-(double)(p4.y-p3.y)*(double)(p2.x-p1.x);
                            double x=((double)(p2.x-p1.x)*(double)(p4.x-p3.x)*(double)(p3.y-p1.y)+(double)(p2.y-p1.y)*(double)(p4.x-p3.x)*(double)p1.x-(double)(p4.y-p3.y)*(double)(p2.x-p1.x)*(double)p3.x)/d;
							double y=((double)(p2.y-p1.y)*(double)(p4.y-p3.y)*(double)(p3.x-p1.x)+(double)(p2.x-p1.x)*(double)(p4.y-p3.y)*(double)p1.y-(double)(p4.x-p3.x)*(double)(p2.y-p1.y)*(double)p3.y)/(-d);
							jp[count++]=CPoint(x,y);
						}
						j++;
					}
					if(count==0&&this->PointInsidePolygon(p1,p,ppCount)&&this->PointInsidePolygon(p2,p,ppCount))///线段在多边形内
					{
						gl.LinePoint[0]=pZoom->CPtoGP(p1);
						gl.LinePoint[1]=pZoom->CPtoGP(p2);
						BYTE r=GetRValue(color);
						BYTE g=GetGValue(color);
						BYTE b=GetBValue(color);
						gl.color=RGB(255-r,255-g,255-b);
						gl.lType=type;
						gl.width=abs(int(width))+1;
						this->addLine(gl,ResultTable);
					}
					else if(count==1&&(this->PointInsidePolygon(p1,p,ppCount)||this->PointInsidePolygon(p2,p,ppCount)))///线段的一个点在多边形内
					{
						if(this->PointInsidePolygon(p1,p,ppCount))
							gl.LinePoint[count]=pZoom->CPtoGP(p1);
						else if(this->PointInsidePolygon(p2,p,ppCount))
							gl.LinePoint[count]=pZoom->CPtoGP(p2);
						gl.LinePoint[0]=pZoom->CPtoGP(jp[count-1]);
						BYTE r=GetRValue(color);
						BYTE g=GetGValue(color);
						BYTE b=GetBValue(color);
						gl.color=RGB(255-r,255-g,255-b);
						gl.lType=type;
						gl.width=abs(int(width))+1;
						count=0;
						this->addLine(gl,ResultTable);
					}
					else if(count>1)
					{
						if(this->PointInsidePolygon(p1,p,ppCount))
							jp[count++]=p1;
						if(this->PointInsidePolygon(p2,p,ppCount))
							jp[count++]=p2;
						j=1;
						if(p1.x==p2.x) SortPointByY(jp,count);
						else SortPointByX(jp,count);
						while(j<count)
						{
							CPoint temp = CPoint((jp[j-1].x+jp[j].x)/2,(jp[j-1].y+jp[j].y)/2);
							if(this->PointInsidePolygon(temp,p,ppCount))
							{
								gl.LinePoint[0]=pZoom->CPtoGP(jp[j-1]);
								gl.LinePoint[1]=pZoom->CPtoGP(jp[j]);
								BYTE r=GetRValue(color);
								BYTE g=GetGValue(color);
								BYTE b=GetBValue(color);
								gl.color=RGB(255-r,255-g,255-b);
								gl.lType=type;
								gl.width=abs(int(width))+1;
								this->addLine(gl,ResultTable);
							}
							j++;
						}
						count=0;
					}
					i++;
				}
				delete [] ppt;ppt=NULL;
				delete [] lpt;lpt=NULL;
				delete [] jp;jp=NULL;
				delete [] p;
			}
			pPolySet->MoveNext();
		}
		pLineSet->MoveNext();
	}
	return true;
}

void DBConnection::PolyDjPoly(CPoint * pPoly1,CPoint * pPoly2,int n1,int n2,int color,int BorderType,CString ResultTable,CZoomManager  * pZoom)
{
	int i=1,j=1,count=0;
	CPoint p1,p2,p3,p4;
	double r,r1,r2,t;
	gisLine gl(2);
	CPoint * jp = new CPoint[n2];
	while(i<n1+1)////区1
	{
		j=1;
		p1=pPoly1[i-1];p2=pPoly1[i%n1];
		count=0;
		while(j<n2+1)///区2
		{
			p3=pPoly2[j-1];p4=pPoly2[j%n2];
			r1=p3.y*(p4.x-p3.x)-p1.y*(p4.x-p3.x)+(p1.x-p3.x)*(p4.y-p3.y);
			r2=(p2.y-p1.y)*(p4.x-p3.x)-(p2.x-p1.x)*(p4.y-p3.y);
			r=r1/r2;
			t=(p1.x-p3.x+r*(p2.x-p1.x))/(p4.x-p3.x);
			if(0<r&&r<1&&0<t&&t<1)///线段相交
			{
				///求交点
				double d=(double)(p2.y-p1.y)*(double)(p4.x-p3.x)-(double)(p4.y-p3.y)*(double)(p2.x-p1.x);
                double x=((double)(p2.x-p1.x)*(double)(p4.x-p3.x)*(double)(p3.y-p1.y)+(double)(p2.y-p1.y)*(double)(p4.x-p3.x)*(double)p1.x-(double)(p4.y-p3.y)*(double)(p2.x-p1.x)*(double)p3.x)/d;
				double y=((double)(p2.y-p1.y)*(double)(p4.y-p3.y)*(double)(p3.x-p1.x)+(double)(p2.x-p1.x)*(double)(p4.y-p3.y)*(double)p1.y-(double)(p4.x-p3.x)*(double)(p2.y-p1.y)*(double)p3.y)/(-d);
				jp[count++]=CPoint(x,y);
			}
			j++;
		}
		if(count==0&&this->PointInsidePolygon(p1,pPoly2,n2)&&this->PointInsidePolygon(p2,pPoly2,n2))///线段在多边形内
		{
			gl.LinePoint[0]=pZoom->CPtoGP(p1);
			gl.LinePoint[1]=pZoom->CPtoGP(p2);
			BYTE r=GetRValue(color);
			BYTE g=GetGValue(color);
			BYTE b=GetBValue(color);
			gl.color=RGB(255-r,255-g,255-b);
			gl.lType=BorderType;
			gl.width=3;
			this->addLine(gl,ResultTable);
		}
		else if(count==1&&(this->PointInsidePolygon(p1,pPoly2,n2)||this->PointInsidePolygon(p2,pPoly2,n2)))///线段的一个点在多边形内
		{
			if(this->PointInsidePolygon(p1,pPoly2,n2))
				gl.LinePoint[count]=pZoom->CPtoGP(p1);
			else if(this->PointInsidePolygon(p2,pPoly2,n2))
				gl.LinePoint[count]=pZoom->CPtoGP(p2);
			gl.LinePoint[0]=pZoom->CPtoGP(jp[count-1]);
			BYTE r=GetRValue(color);
			BYTE g=GetGValue(color);
			BYTE b=GetBValue(color);
			gl.color=RGB(255-r,255-g,255-b);
			gl.lType=BorderType;
			gl.width=3;
			count=0;
			this->addLine(gl,ResultTable);
		}
		else if(count>1)
		{
			if(this->PointInsidePolygon(p1,pPoly2,n2))
				jp[count++]=p1;
			if(this->PointInsidePolygon(p2,pPoly2,n2))
				jp[count++]=p2;
			j=1;
			if(p1.x==p2.x) SortPointByY(jp,count);
			else SortPointByX(jp,count);
			while(j<count)
			{
				CPoint temp = CPoint((jp[j-1].x+jp[j].x)/2,(jp[j-1].y+jp[j].y)/2);
				if(this->PointInsidePolygon(temp,pPoly2,n2))
				{
					gl.LinePoint[0]=pZoom->CPtoGP(jp[j-1]);
					gl.LinePoint[1]=pZoom->CPtoGP(jp[j]);
					BYTE r=GetRValue(color);
					BYTE g=GetGValue(color);
					BYTE b=GetBValue(color);
					gl.color=RGB(255-r,255-g,255-b);
					gl.lType=BorderType;
					gl.width=3;
					this->addLine(gl,ResultTable);
				}
				j++;
			}
			count=0;
		}
		i++;
	}
	delete [] jp;
}

bool DBConnection::PolyDjPoly(CString PolyTable1,CString PolyTable2,CString ResultTable,CZoomManager  * pZoom)
{
	_RecordsetPtr pPolySet2, pPolySet1;
	CString ID1,ID2;
	int pCount1,pCount2,count=0;
	int color1,type1,color2,type2;
	double xMin_1,yMin_1,xMax_1,yMax_1,xMin_2,yMin_2,xMax_2,yMax_2;
	CPoint lt_1,rb_1,lt_2,rb_2;
	DPoint * dpt1=NULL,* dpt2=NULL;
	CPoint * p1=NULL,*p2=NULL;
	CString sql,lID,pID;
	sql.Format(L"select * from [%s]",PolyTable1);
	pPolySet1 = pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	sql.Format(L"select * from [%s]",PolyTable2);
	pPolySet2 = pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	sql.Format(L"select * from [CatalogTable] where gisTable='%s'",ResultTable);
	pRst = pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	if(pPolySet1->End){AfxMessageBox(L"区表1为空！"); return false;}
	else if(pPolySet2->End){AfxMessageBox(L"区表2为空！"); return false;}
	if(!pRst->End){AfxMessageBox(L"目标表已存在，请重新命名目标表！"); return false;}
	else{this->CreateTable(ResultTable,2);}
	while(!pPolySet1->End)
	{
		ID1=(CString)(BSTR)((_bstr_t)pPolySet1->GetCollect((_variant_t)L"ID"));
		pCount1 = strtoi((CString)(BSTR)((_bstr_t)pPolySet1->GetCollect((_variant_t)L"pCount")));
		xMin_1=strtof((CString)(BSTR)((_bstr_t)pPolySet1->GetCollect((_variant_t)L"xMin")));
		yMin_1=strtof((CString)(BSTR)((_bstr_t)pPolySet1->GetCollect((_variant_t)L"yMin")));
		xMax_1=strtof((CString)(BSTR)((_bstr_t)pPolySet1->GetCollect((_variant_t)L"xMax")));
		yMax_1=strtof((CString)(BSTR)((_bstr_t)pPolySet1->GetCollect((_variant_t)L"yMax")));
		color1 = strtoi((CString)(BSTR)((_bstr_t)pPolySet1->GetCollect((_variant_t)L"BorderColor")));
		type1=strtoi((CString)(BSTR)((_bstr_t)pPolySet1->GetCollect((_variant_t)L"BorderType")));
		lt_1=pZoom->GPtoCP(DPoint(xMin_1,yMax_1));
		rb_1=pZoom->GPtoCP(DPoint(xMax_1,yMin_1));
		if(dpt1!=NULL) delete []dpt1;
		dpt1 = new DPoint[pCount1];
		if(p1!=NULL) delete [] p1;
		p1 = new CPoint[pCount1];
		this->readImgPoint(PolyTable1,ID1,dpt1,pCount1);
		this->convertDPtoCP(dpt1,p1,pCount1,pZoom);
		pPolySet2->MoveFirst();
		while(!pPolySet2->End)
		{
			ID2=(CString)(BSTR)((_bstr_t)pPolySet2->GetCollect((_variant_t)L"ID"));
			xMin_2=strtof((CString)(BSTR)((_bstr_t)pPolySet2->GetCollect((_variant_t)L"xMin")));
			yMin_2=strtof((CString)(BSTR)((_bstr_t)pPolySet2->GetCollect((_variant_t)L"yMin")));
			xMax_2=strtof((CString)(BSTR)((_bstr_t)pPolySet2->GetCollect((_variant_t)L"xMax")));
			yMax_2=strtof((CString)(BSTR)((_bstr_t)pPolySet2->GetCollect((_variant_t)L"yMax")));
			pCount2 = strtoi((CString)(BSTR)((_bstr_t)pPolySet2->GetCollect((_variant_t)L"pCount")));
			color2 = strtoi((CString)(BSTR)((_bstr_t)pPolySet2->GetCollect((_variant_t)L"BorderColor")));
		    type2=strtoi((CString)(BSTR)((_bstr_t)pPolySet2->GetCollect((_variant_t)L"BorderType")));
			lt_2=pZoom->GPtoCP(DPoint(xMin_2,yMax_2));
			rb_2=pZoom->GPtoCP(DPoint(xMax_2,yMin_2));
			if(lt_1.x>=rb_2.x||lt_1.y>=rb_2.y||rb_1.x<=lt_2.x||rb_1.y<=lt_2.y);///区的外包矩形不相交
			else
			{
				if(dpt2!=NULL) delete [] dpt2;
				dpt2 = new DPoint[pCount2];
				if(p2!=NULL) delete [] p2;
				p2 = new CPoint[pCount2];
				this->readImgPoint(PolyTable2,ID2,dpt2,pCount2);
			    this->convertDPtoCP(dpt2,p2,pCount2,pZoom);
				this->PolyDjPoly(p1,p2,pCount1,pCount2,color1,type1,ResultTable,pZoom);
				this->PolyDjPoly(p2,p1,pCount2,pCount1,color2,type2,ResultTable,pZoom);
				if(dpt2!=NULL) delete []dpt2;dpt2 = NULL;
	            if(p2!=NULL) delete [] p2;p2=NULL;
			}
			pPolySet2->MoveNext();
		}
		if(dpt1!=NULL) delete [] dpt1;dpt1 = NULL;
	    if(p1!=NULL) delete [] p1;p1=NULL;
		pPolySet1->MoveNext();
	}

	return true;
}


void DBConnection::SortPointByY(CPoint * pt,int n)
{
	for(int i=1;i<n;i++)
	{
		CPoint t=pt[i];
		for(int j=i;j>0;j--)
		{
			if(pt[j-1].y>pt[j].y)
			{
				pt[j]=pt[j-1];
				pt[j-1]=t;
			}
		}
	}
}

void DBConnection::SortPointByX(CPoint * pt,int n)
{
	for(int i=1;i<n;i++)
	{
		CPoint t=pt[i];
		for(int j=i;j>0;j--)
		{
			if(pt[j-1].x>pt[j].x)
			{
				pt[j]=pt[j-1];
				pt[j-1]=t;
			}
		}
	}
}

void DBConnection::GetPointTable(CString table,gisPoint * gp,int & count)
{
	_RecordsetPtr pSet;
	count=0;
	CString sql;
	sql.Format(L"select * from [%s]",table);
	pSet = pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	while(!pSet->End)
	{
		gp[count].ID=(CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"ID"));
		gp[count].color=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"color")));
		gp[count].point.x=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"pX")));
		gp[count].point.y=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"pY")));
		gp[count].pType=(CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"type"));
		gp[count].size=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"size")));
		count++;
		pSet->MoveNext();
	}
}

void DBConnection::GetLineTable(CString table,gisLine * gl,int & count)
{
	_RecordsetPtr pSet;
	count=0;
	CString sql;
	sql.Format(L"select * from [%s]",table);
	pSet = pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	while(!pSet->End)
	{
		gl[count].ID=(CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"ID"));
		gl[count].color=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"LColor")));
		gl[count].lType=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"LType")));
		gl[count].pCount=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"pCount")));
		gl[count].width=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"LWidth")));
		gl[count].xMin=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"xMin")));
		gl[count].yMin=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"yMin")));
		gl[count].xMax=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"xMax")));
		gl[count].yMax=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"yMax")));
		if(gl[count].LinePoint!=NULL) delete [] gl[count].LinePoint;
		gl[count].LinePoint=new DPoint[gl[count].pCount];
		this->readImgPoint(table,gl[count].ID,gl[count].LinePoint,gl[count].pCount);
		count++;
		pSet->MoveNext();
	}
}

void DBConnection::GetPolyTable(CString table,gisPolygon * gPoly,int & count)
{
	_RecordsetPtr pSet;
	count=0;
	CString sql;
	sql.Format(L"select * from [%s]",table);
	pSet = pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	while(!pSet->End)
	{
		gPoly[count].ID=(CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"ID"));
		gPoly[count].color=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"color")));
		gPoly[count].BorderType=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"BorderType")));
		gPoly[count].pCount=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"pCount")));
		gPoly[count].BorderColor=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"BorderColor")));
		gPoly[count].xMin=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"xMin")));
		gPoly[count].yMin=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"yMin")));
		gPoly[count].xMax=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"xMax")));
		gPoly[count].yMax=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"yMax")));
		if(gPoly[count].point!=NULL) delete [] gPoly[count].point;
		gPoly[count].point=new DPoint[gPoly[count].pCount];
		this->readImgPoint(table,gPoly[count].ID,gPoly[count].point,gPoly[count].pCount);
		count++;
		pSet->MoveNext();
	}
}

void DBConnection::GetMarkerTable(CString table,gisMarker * gm,int & count)
{
	_RecordsetPtr pSet;
	count=0;
	CString sql;
	sql.Format(L"select * from [%s]",table);
	pSet = pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	while(!pSet->End)
	{
		gm[count].ID=(CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"ID"));
		gm[count].color=strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"color")));
		gm[count].point.x=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"pX")));
		gm[count].point.y=strtof((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"pY")));
		gm[count].text=(CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"Lable"));
		this->readImgFont(table,gm[count].ID,gm[count].lfont);
		count++;
		pSet->MoveNext();
	}
}

int DBConnection::getMaxID(CString table)
{
	_RecordsetPtr pSet;
	CString sql;
	sql.Format(L"select max(ID)ID from [%s]",table);
	pSet = pConn->Execute((_bstr_t)sql,NULL,adCmdText);
	if(!pSet->End)
	   return strtoi((CString)(BSTR)((_bstr_t)pSet->GetCollect((_variant_t)L"ID"))); 
	return 0;
}