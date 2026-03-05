#ifndef DATATOOLS_H
#define DATATOOLS_H
//===============================
#include "Cordins.h"
#include <queue>
using namespace std;
//===============================
#include "Cordins.h"
#include "GraphStyles.h"
#include "GeoPoint.h"
#include "DataBase.h"
#include "DataTemplate.h"
//==============================================DATA TOOLS===========================================================
class CDataTool
{
public:CDataTool(CStyleManager *pStyle=NULL,CString layer=L""){}
	   ~CDataTool(){}
	   virtual void SetConn(DBConnection * db){}
	   virtual void SetDlg(CDialog * dlg){}
	   virtual void LButtonDown(CPoint point,CZoomManager* pZoom){}
	   virtual void LButtonUp(CPoint point,CZoomManager* pZoom){}
	   virtual void RButtonDown(CPoint point,CZoomManager* pZoom){}
	   virtual void RButtonUp(CPoint point,CZoomManager* pZoom){}
	   virtual void MouseMove(CPoint point,CZoomManager* pZoom){}
};

class CPointDataTool:public CDataTool
{
public:CPointDataTool(CStyleManager *pStyle,CString layer){lname=layer;pType=pStyle;}
	   ~CPointDataTool(){}
	   void LButtonDown(CPoint point,CZoomManager* pZoom);
	   void LButtonUp(CPoint point,CZoomManager* pZoom);
	   void RButtonDown(CPoint point,CZoomManager* pZoom);
	   void RButtonUp(CPoint point,CZoomManager* pZoom);
	   void MouseMove(CPoint point,CZoomManager* pZoom);
	   void SetConn(DBConnection * db){pdb=db;}
private:
	DPoint pt;
	CString lname;
	CStyleManager *pType;
	DBConnection * pdb;
};

class CLineDataTool:public CDataTool
{
public:CLineDataTool(CStyleManager *pStyle,CString layer){pType=pStyle;lname=layer;doing=0;}
	   ~CLineDataTool(){}
	   void LButtonDown(CPoint point,CZoomManager* pZoom);
	   void LButtonUp(CPoint point,CZoomManager* pZoom);
	   void RButtonDown(CPoint point,CZoomManager* pZoom);
	   void RButtonUp(CPoint point,CZoomManager* pZoom);
	   void MouseMove(CPoint point,CZoomManager* pZoom);
	   void SetConn(DBConnection * db){pdb=db;}
private:
	DPoint MidPoint(DPoint p1,DPoint p2)
	{
		DPoint p;
		p.x=(p1.x+p2.x)/2;
		p.y=(p1.y+p2.y)/2;
		return p;
	}
private:
	vector<DPoint> pts;
	int doing;
	CString lname;
	CStyleManager *pType;
	DBConnection * pdb;
};

class CPolyDataTool:public CDataTool
{
public:CPolyDataTool(CStyleManager *pStyle,CString layer){pType=pStyle;lname=layer;}
	   ~CPolyDataTool(){}
	   void LButtonDown(CPoint point,CZoomManager* pZoom);
	   void LButtonUp(CPoint point,CZoomManager* pZoom);
	   void RButtonDown(CPoint point,CZoomManager* pZoom);
	   void RButtonUp(CPoint point,CZoomManager* pZoom);
	   void MouseMove(CPoint point,CZoomManager* pZoom);
	   void SetConn(DBConnection * db){pdb=db;}
private:
	queue<DPoint> pts;
	CString lname;
	CStyleManager *pType;
	DBConnection * pdb;
};

class CNoteDataTool:public CDataTool
{
public:CNoteDataTool(CStyleManager *pStyle,CString layer){pType=pStyle;lname=layer;}
	   ~CNoteDataTool(){}
	   void LButtonDown(CPoint point,CZoomManager* pZoom);
	   void LButtonUp(CPoint point,CZoomManager* pZoom);
	   void RButtonDown(CPoint point,CZoomManager* pZoom);
	   void RButtonUp(CPoint point,CZoomManager* pZoom);
	   void MouseMove(CPoint point,CZoomManager* pZoom);
	  void SetConn(DBConnection *db){pdb=db;}
private:
	DPoint pt;
	CString lname;
	CStyleManager *pType;
	DBConnection * pdb;
};

//===============================================================================================================
#endif