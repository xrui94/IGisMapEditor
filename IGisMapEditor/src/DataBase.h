
#pragma once

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","End")

#include "DataTemplate.h"
#include "Cordins.h"

int _declspec(dllexport) strtoi(CString str);
double  _declspec(dllexport) strtof(CString str);

class _declspec(dllexport)DBConnection
{
public:
	DBConnection(void);
	~DBConnection(void);
	_RecordsetPtr ExecuteSQL(CString sql);
	bool Open(CString server,CString dbname,CString uid=NULL,CString paw=NULL);
	void disConnection();
	bool isConnection(){return m_isConnection;}
	
	void MoveNext(){pRst->MoveNext();}
	void MoveFirst(){pRst->MoveFirst();}
	void MoveLast(){pRst->MoveLast();}
	void MovePrevious(){pRst->MovePrevious();}
	bool IsEnd(){return pRst->End;}
	void operator = (_RecordsetPtr  _pRst){pRst = _pRst;};
	_RecordsetPtr getRecordSet(CString table);
	void DisplaySet(CDC * pDC,CZoomManager* pZoom,CString table);
	CString GetResultByAttr(CString attribute);

	void addPoint(gisPoint&gp,CString table);
	bool SearchPointByClick(CPoint point,gisPoint&gis_point,CString table,CZoomManager *pZoom);
	void AlterPoint(gisPoint&gis_point,CString table,CDC * pDC,CZoomManager *pZoom);
	void DeletePoint(gisPoint&gp,CString table);

	void addLine(gisLine&gl,CString table);
	bool SearchLineByClick(CPoint point,gisLine& gl,CString table,CZoomManager *pZoom);
	void AlterLine(gisLine& gl,CString table,CDC * pDC,CZoomManager *pZoom);
	void DeleteLine(gisLine& gl,CString table);
	bool getLinePt(CPoint point,CPoint& p1,CPoint& p2,int& pos,CString table,CZoomManager * pZoom);
	bool delPtFromLine(CPoint point,CString table,gisLine&gl,CZoomManager * pZoom);
	bool MoveLinePt(CPoint point,CPoint&p1,CPoint&p2,int&pos,CString table,gisLine&gl,CZoomManager * pZoom);
	void CutLine(gisLine&gl);

	void addPolygon(gisPolygon&gp,CString table);
	bool SearchPolyByClick(CPoint&point,gisPolygon& gPoly,CString table,CZoomManager *pZoom);
	void DeletePolygon(gisPolygon& gPoly,CString table);
	void AlterPolygon(gisPolygon& gPoly,CString table,CDC*pDC,CZoomManager *pZoom);
	bool getPolygonPt(CPoint point,CPoint& p1,CPoint& p2,int& pos,CString table,gisPolygon&gl,CZoomManager * pZoom);
	bool delPtFromPolygon(CPoint point,CString table,gisPolygon&gl,CZoomManager * pZoom);
	bool MovePolygonPt(CPoint point,CPoint&p1,CPoint&p2,int&pos,CString table,gisPolygon&gl,CZoomManager * pZoom);

	void addMarker(gisMarker&gm,CString table);
	bool SearchMarkerByClick(CPoint point,gisMarker&gm,CString table,CDC*pDC,CZoomManager *pZoom);
	void AlterMarker(gisMarker&gm,CString table,CDC* pDC,CZoomManager *pZoom);
	void DeleteMarker(gisMarker&gm,CString table);

	bool CreateTable(CString table,int type);
	bool DropTable(CString table);
	bool SearchTable(CString table);
	int GetDataBaseTable(CatalogTable * tableSet);
	bool GetTableMsg(CatalogTable&tableMsg,int id);
	void GetPointTable(CString table,gisPoint * gp,int & count);
	void GetLineTable(CString table,gisLine * gl,int & count);
	void GetPolyTable(CString table,gisPolygon * gPoly,int & count);
	void GetMarkerTable(CString table,gisMarker * gm,int & count);
	bool CreateDataBase(CString server,CString db_name,CString user,CString paw,CString db_path);

	_RecordsetPtr get_pRst(){return pRst;}
	_ConnectionPtr get_pConn(){return pConn;}

	void readImgPoint(CString table,CString ID,DPoint * point,int n);
	void readImgFont(CString table,CString ID,LOGFONT & lfont);
	void getBorderPoint(DPoint * p,CString table,int type=1);
	bool getTableInfo(double&xMin,double&yMin,double&xMax,double&yMax);
	int getMaxID(CString table);

	void swap(CPoint&p1,CPoint&p2);
	void SortPointByY(CPoint * pt,int n);
	void SortPointByX(CPoint * pt,int n);
	void getInfo(CPoint&p1,CPoint&p2,double& minX,double&maxX,double&maxY,double&minY);
	void convertDPtoCP(const DPoint * dt,CPoint * pt,int n,CZoomManager * pZoom);
	bool PointInsidePolygon(CPoint,CPoint * pt,int n);

	void SearchByRect(CPoint p1,CPoint p2,CString table,gisPoint * gp,CZoomManager * pZoom,int& count);
	void SearchByRect(CPoint p1,CPoint p2,CString table,gisLine * gl,CZoomManager * pZoom,int& count);
	void SearchByRect(CPoint p1,CPoint p2,CString table,gisPolygon * gPoly,CZoomManager * pZoom,int& count);
	void SearchByRect(CPoint p1,CPoint p2,CString table,gisMarker * gm,CZoomManager * pZoom,int& count,CDC * pDC);

	void SearchByCircle(CPoint p1,CPoint p2,CString table,gisPoint * gp,CZoomManager * pZoom,int& count);
	void SearchByCircle(CPoint p1,CPoint p2,CString table,gisLine * gl,CZoomManager * pZoom,int& count);
	void SearchByCircle(CPoint p1,CPoint p2,CString table,gisPolygon * gPoly,CZoomManager * pZoom,int& count);
	void SearchByCircle(CPoint p1,CPoint p2,CString table,gisMarker * gm,CZoomManager * pZoom,int& count,CDC * pDC);

	void SearchByPolygon(CPoint * point,int n,CString table,gisPoint * gp,CZoomManager * pZoom,int& count);
	void SearchByPolygon(CPoint * point,int n,CString table,gisLine * gl,CZoomManager * pZoom,int& count);
	void SearchByPolygon(CPoint * point,int n,CString table,gisPolygon * gPoly,CZoomManager * pZoom,int& count);
	void SearchByPolygon(CPoint * point,int n,CString table,gisMarker * gm,CZoomManager * pZoom,int& count,CDC * pDC);

	bool LineDjPolygon(CString lineTable,CString PolyTable,CString ResultTable,CZoomManager  * pZoom);
	bool PolyDjPoly(CString PolyTable1,CString PolyTable2,CString ResultTable,CZoomManager  * pZoom);
	void PolyDjPoly(CPoint * pPoly1,CPoint * pPoly2,int n1,int n2,int color,int BorderType,CString ResultTable,CZoomManager  * pZoom);
	void HcAnalyse(gisPoint&gp);
	void HcAnalyse(gisLine&gl);
	void HcAnalyse(gisPolygon&gPoly);
	
	void OutputImagery(CDC *pDC,CString table,CZoomManager *pZoom,bool pixel);
public:
	bool m_isConnection;
	_RecordsetPtr  pRst;
	_ConnectionPtr  pConn;
	_bstr_t error;
private:
	CPoint sp1,sp2;
	int pos;
};

