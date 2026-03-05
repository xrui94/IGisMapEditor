#include "stdafx.h"
//=============================================================================
class CColorDlg
{
public:CColorDlg(COLORREF color=RGB(0,0,0))
	   {   
			rgbCurrent=color;
			for(int i=0;i<16;i++)
			{acrCustClr[i]=RGB(255,255,255);}   
			ZeroMemory(&cc, sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.lpCustColors = (LPDWORD) acrCustClr;
			cc.rgbResult = rgbCurrent;
			cc.hwndOwner=NULL;
			cc.hInstance=NULL;
			cc.Flags =CC_RGBINIT|CC_ANYCOLOR; 
			cc.lCustData=0;
			cc.lpfnHook =NULL;
			cc.lpTemplateName=NULL;	
	   }
	   ~CColorDlg(){}
	   int DoModal()
	   {
		   cc.rgbResult=rgbCurrent;
			BOOL r= ChooseColor(&cc);
			if(r==TRUE) {rgbCurrent=cc.rgbResult;return IDOK;}
			else
				return IDCANCEL;
	   }
	   COLORREF GetColor(){return cc.rgbResult;}
private:
	CHOOSECOLOR cc;            
	COLORREF acrCustClr[16];
	COLORREF rgbCurrent;
};