#include "stdafx.h"
#ifndef GRAPHSTYLES_H
#define GRAPHSTYLES_H
class CPointSort
{
public:
	CString type;
	COLORREF color;
	int size;
public:CPointSort(){type=L"";size=1;color=RGB(0,0,0);}
	   CPointSort(CString sort,COLORREF cl,int sz){type=sort;color=cl;size=sz;}
};
class CLineSort
{
public:
	DWORD PENTYPE;
	COLORREF color;
	int width;
	bool curve;
	CLineSort(){PENTYPE=PS_SOLID;color=RGB(0,0,0);curve=false;width=1;}
	CLineSort(DWORD type,COLORREF clr,int Lwidth,bool bezier){PENTYPE=type;color=clr;width=Lwidth;curve=bezier;}
};

class CRgnSort
{
public:
	COLORREF sidecolor;
	COLORREF fillcolor;
	int sideType;
	CRgnSort(){sidecolor=fillcolor=RGB(0,0,0);sideType=PS_SOLID;}
	CRgnSort(int type,COLORREF clr1,COLORREF clr2){sideType=type;sidecolor=clr1;fillcolor=clr2;}
};

class CNoteSort
{
public:
	COLORREF color;
	LOGFONT lfont;
	CString text;
	CNoteSort()
	{
		color=RGB(0,0,0);text=L"";
		lfont.lfCharSet=134;
		lfont.lfClipPrecision=2;
		lfont.lfEscapement=0;
		lfont.lfHeight=16;
		lfont.lfItalic=FALSE;
		lfont.lfOrientation=0;
		lfont.lfOutPrecision=1;
		lfont.lfPitchAndFamily=34;
		lfont.lfQuality=2;
		lfont.lfStrikeOut=FALSE;
		lfont.lfUnderline=FALSE;
		lfont.lfWeight=700;
		lfont.lfWidth=7;
		lfont.lfFaceName[0]=83;lfont.lfFaceName[1]=121;
		lfont.lfFaceName[2]=115;lfont.lfFaceName[3]=116;
		lfont.lfFaceName[4]=101;lfont.lfFaceName[5]=109;
		lfont.lfFaceName[6]=0;lfont.lfFaceName[7]=845;
		lfont.lfFaceName[8]=63648;lfont.lfFaceName[9]=65535;
		lfont.lfFaceName[10]=498;lfont.lfFaceName[11]=0;
		lfont.lfFaceName[12]=0;lfont.lfFaceName[13]=0;
		lfont.lfFaceName[14]=52540;lfont.lfFaceName[15]=633;
		lfont.lfFaceName[16]=63616;lfont.lfFaceName[17]=65535;
		lfont.lfFaceName[18]=8;lfont.lfFaceName[19]=32344;
		lfont.lfFaceName[20]=63227;lfont.lfFaceName[21]=65535;
		lfont.lfFaceName[22]=0;lfont.lfFaceName[23]=0;
		lfont.lfFaceName[24]=0;lfont.lfFaceName[25]=0;
		lfont.lfFaceName[26]=52512;lfont.lfFaceName[27]=633;
		lfont.lfFaceName[28]=63616;lfont.lfFaceName[29]=65535;
		lfont.lfFaceName[30]=65535;lfont.lfFaceName[31]=0;
	}
	CNoteSort(COLORREF cl,CString str,LOGFONT &lf){color=cl;text=str;lfont=lf;}
};
//===================================================================================================================
class CStyleManager
{
public:
	CPointSort ps;
	CLineSort ls;
	CRgnSort rs;
	CNoteSort ns;
public:
	CStyleManager(){}
	~CStyleManager(){}
};
//====================================================================================================================
#endif