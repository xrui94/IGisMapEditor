
#ifndef ADO_DLL_READY
#define ADO_DLL_READY
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
#endif

#ifndef DLLEX
#define DLLEX _declspec(dllexport)
#endif
#ifndef DLLIN
#define DLLIN _declspec(dllimport)
#endif
#ifndef DATAENGINE_H
#define DATAENGINE_H

#include "DataTmplate.h"
#include "Database.h"
#include "Cordins.h"
#include "GraphStyles.h"
#include "GeoPoint.h"

namespace cmdb
{
	class DLLEX AdoDb
	{
	public:AdoDb()
		   {
			   connected=false;
			   CoInitialize(NULL);
			   pConn.CreateInstance(__uuidof(Connection));
			   pRec.CreateInstance(__uuidof(Recordset));
		   }
		   ~AdoDb()
		   {
			   pConn->Close();
			   pRec.Release();
			   pConn.Release();
			   CoUninitialize();
		   }
		   bool Open(CString server,CString db,CString user,CString pin);
		   bool ExecuteSql(CString sql);
		   inline bool IsConnected(){return connected;}
	private:
		_ConnectionPtr pConn;
		_RecordsetPtr pRec;
		bool connected;
	};
}

#endif