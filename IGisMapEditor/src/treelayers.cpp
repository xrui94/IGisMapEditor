#include "stdafx.h"
#include "treelayers.h"
//===============================================================================================================
Layer* TreeItems::LayerPtr(int n)
{
	if(n<0||n>=v.size())return NULL;
	list<Layer>::iterator it=v.begin();
	int i=0;
	while(i<n)
	{
		it++;
		i++;
	}
	return &*it;
}
bool TreeItems::add(Layer& l)
{
	v.push_back(l);
	return true;
}
bool TreeItems::add_as(int n,Layer& l)
{
	if(n<0||n>v.size())return false;
	else if(n==0)
	{
		v.push_front(l);return true;
	}
	else if(n==v.size())
	{
		v.push_back(l);return true;
	}
	else
	{
		list<Layer>::iterator it=v.begin();
		int i=0;
		while(i<n)
		{
			it++;
			i++;
		}
		v.insert(it,l);	
		return true;
	}

}
bool TreeItems::remove(int n)
{
	if(n<0||n>=v.size())return false;
	else
	{
		list<Layer>::iterator it=v.begin();
		int i=0;
		while(i<n)
		{
			it++;
			i++;
		}
		v.erase(it);	
		return true;
	}
}
void TreeItems::remove(Layer* l)
{
	//int n=0;
	list<Layer>::iterator it=v.begin();
	while(&*it!=l)
	{
		it++;
	}
	v.erase(it);
}
bool TreeItems::change(int n,Layer& l)
{
	if(n<0||n>=v.size())return false;
	else
	{
		list<Layer>::iterator it=v.begin();
		int i=0;
		while(i<n)
		{
			it++;
			i++;
		}
		it->SetEx(l.GetType(),l.IsEditing(),l.IsVisible(),l.GetName());	
		return true;
	}
}
void TreeItems::up(Layer*p)
{
	list<Layer>::iterator it=v.begin();
	if(p==&*it)
	{return;}
	while(&*it!=p)
	{it++;}
	Layer temp1=*it;
	list<Layer>::iterator it2=it;
	it--;
	Layer temp2=*it;
	*it=temp1;
	*it2=temp2;
}
void TreeItems::down(Layer*p)
{
	list<Layer>::iterator it=v.begin();
	int n=0;
	while(&*it!=p)
	{it++;n++;}
	if(n+1==v.size()){return;}
	Layer temp1=*it;
	list<Layer>::iterator it2=it;
	it++;
	Layer temp2=*it;
	*it=temp1;
	*it2=temp2;
}

Layer * TreeItems:: GetLayerByName(CString name)
{
	list<Layer>::iterator it=v.begin();
	int i=0;
	while(i<this->size())
	{
		if(it->GetName()==name){ return &*it;break;}
		it++;
		i++;
	}
	return NULL;
}
//===============================================================================================================
Basepic* ImageItems::PicPtr(int n)
{
	if(n<0||n>=v.size())return NULL;
	list<Basepic>::iterator it=v.begin();
	int i=0;
	while(i<n)
	{
		it++;
		i++;
	}
	return &*it;
}
bool ImageItems::add(Basepic& l)
{
	v.push_back(l);
	return true;
}
bool ImageItems::remove(int n)
{
	if(n<0||n>=v.size())return false;
	else
	{
		list<Basepic>::iterator it=v.begin();
		int i=0;
		while(i<n)
		{
			it++;
			i++;
		}
		v.erase(it);	
		return true;
	}
}

void ImageItems::remove(Basepic* p)
{
	list<Basepic>::iterator it=v.begin();
	while(&*it!=p)
	{
		it++;
	}
	v.erase(it);
}
bool ImageItems::change(int n,Basepic& l)
{
	if(n<0||n>=v.size())return false;
	else
	{
		list<Basepic>::iterator it=v.begin();
		int i=0;
		while(i<n)
		{
			it++;
			i++;
		}
		it->SetCordin(l.ltx,l.lty,l.rbx,l.rby);
		it->SetEx(l.IsVisible(),l.GetName(),l.Width(),l.Height());
		return true;
	}
}
///=========================================================================================================================