#ifndef TREELAYERS_H
#define TREELAYERS_H
//=================================================================================================================
//includes
//#include "stdafx.h"
#include <list>
using namespace std;
//=================================================================================================================
class Outofbounds
{
public:Outofbounds(){s=L"search out of bounds";}
	   void call(){AfxMessageBox(s);}
private:
	CString s;
};

//=================================================================================================================
//classes
class Layer
{
public:	Layer(){type=-1;editing=visible=false;name=L"";}
		Layer(int Ltype,bool edit,bool view,CString Lname){type=Ltype;editing=edit;visible=view;name=Lname;}
		Layer(const Layer &l){type=l.type;editing=l.editing;visible=l.visible;name=l.name;}
		int GetType(){return type;}
		bool IsEditing(){return editing;}
		bool IsVisible(){return visible;}
		CString GetName(){return name;}
		void SetType(int t){type=t;}
		void SetEdit(bool e){editing=e;}
		void SetVisible(bool v){visible=v;}
		void SetName(CString str){name=str;}
		void SetEx(int Ltype,bool edit,bool view,CString Lname){type=Ltype;editing=edit;visible=view;name=Lname;}
		Layer & operator =(const Layer &l){type=l.type;editing=l.editing;visible=l.visible;name=l.name;return *this;}

protected:
		int type;
		bool editing;
		bool visible;
		CString name;
};
//=================================================================================================================
class Basepic
{
public:	Basepic(){visible=false;fname=L"";ltx=lty=1.1;rbx=rby=0.9;w=h=0;}
		Basepic(bool view,CString Lname){visible=view;fname=Lname;}
		Basepic(const Basepic &l){visible=l.visible;fname=l.fname;ltx=l.ltx;lty=l.lty;rbx=l.rbx;rby=l.rby;w=l.w;h=l.h;}
		bool IsVisible(){return visible;}
		CString GetName(){return fname;}
		void SetVisible(bool v){visible=v;}
		void SetName(CString str){fname=str;}
		void SetBM(int nw,int nh){w=nw;h=nh;}
		int Height(){return h;}
		int Width(){return w;}
		void SetEx(bool view,CString Lname,int nw,int nh){visible=view;fname=Lname;w=nw;h=nh;}
		void SetCordin(double lt_x,double lt_y,double rb_x,double rb_y){ltx=lt_x;rbx=rb_x;lty=lt_y;rby=rb_y;}
		Basepic & operator =(const Basepic &l){visible=l.visible;fname=l.fname;ltx=l.ltx;lty=l.lty;rbx=l.rbx;rby=l.rby;w=l.w;h=l.h;return *this;}
		CPoint GetCP(double x,double y)
		{
			double xop=(x-ltx)/(rbx-ltx);
			double yop=(y-lty)/(rby-lty);
			CPoint p;
			p.x=LONG(xop*w);
			p.y=LONG(yop*h);
			return p;
		}
public:	double ltx,lty,rbx,rby;

protected:
		bool visible;
		CString fname;
		int w,h;
		
};
//=================================================================================================================
class TreeItems
{
public:TreeItems(){}
	   TreeItems(TreeItems &tree){v=tree.v;}
	   ~TreeItems(){}
public:Layer* LayerPtr(int n);
	   bool add(Layer& l);
	   bool remove(int n);
	   void remove(Layer* p);
	   bool change(int n,Layer& l);
	   void clear(){v.clear();}
	   bool empty(){return v.empty();}
	   int size(){return v.size();}
	   void up(Layer*p);
	   void down(Layer*p);
	   Layer *  GetLayerByName(CString name);
private:
	list<Layer> v;
	bool add_as(int n,Layer& l);
};
//=================================================================================================================
class ImageItems
{
public:ImageItems(){}
	   ImageItems(const ImageItems& ii){v=ii.v;}
	   ~ImageItems(){}
	   Basepic* PicPtr(int n);
	   bool add(Basepic& l);
	   bool remove(int n);
	   void remove(Basepic* p);
	   bool change(int n,Basepic& l);
	   void clear(){v.clear();}
	   bool empty(){return v.empty();}
	   int size(){return v.size();}
private:
	list<Basepic> v;
};
//=================================================================================================================
#endif