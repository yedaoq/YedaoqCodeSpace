/* ___________________________________________________________________________
@ 
@ file - C:\YedaoqCodeSpace\YedaoqCppLib\CPP\DataTreeJsonStorage.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-9-13
@ info -
@     
/* ___________________________________________________________________________*/

#ifndef DataTreeJsonStorage_h__
#define DataTreeJsonStorage_h__

#include "DataTreeCommon.h"
#include "jsoncpp/json.h"
#include <fstream>

namespace nsYedaoqDataTree
{
	class CDataTreeJsonStorage : public IDataTreeExporter, public IDataTreeImporter
	{
	public:
		CDataTreeJsonStorage(Json::Value* doc, const tchar* nodename)
			: Root_(doc)
		{
			Current_ = doc;
		}

		virtual tstring get_attr(const tchar* name, tstring def)
		{
			Current_->get()
			pugi::xml_attribute attr = Current_.attribute(name);
			if(attr)
			{
				return attr.value();
			}
			else
			{
				return def;
			}
		}

		virtual bool set_attr(const tchar* name, tstring const& val)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(!attr)
			{
				attr = Current_.append_attribute(name);
			}

			attr.set_value(val.c_str());
			return true;
		}

		virtual bool get_attr(const tchar* name, bool def)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(attr)
			{
				return attr.as_bool();
			}
			else
			{
				return def;
			}
		}

		virtual bool set_attr(const tchar* name, bool val)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(!attr)
			{
				attr = Current_.append_attribute(name);
			}

			attr.set_value(val);
			return true;
		}

		virtual double get_attr(const tchar* name, double def)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(attr)
			{
				return attr.as_double();
			}
			else
			{
				return def;
			}
		}

		virtual bool set_attr(const tchar* name, double val)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(!attr)
			{
				attr = Current_.append_attribute(name);
			}

			attr.set_value(val);
			return true;
		}

		virtual int get_attr(const tchar* name, int def)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(attr)
			{
				return attr.as_int();
			}
			else
			{
				return def;
			}
		}

		virtual bool set_attr(const tchar* name, int val)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(!attr)
			{
				attr = Current_.append_attribute(name);
			}

			attr.set_value(val);
			return true;
		}

		virtual unsigned int get_attr(const tchar* name, unsigned int def)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(attr)
			{
				return attr.as_int();
			}
			else
			{
				return def;
			}
		}

		virtual bool set_attr(const tchar* name, unsigned int val)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(!attr)
			{
				attr = Current_.append_attribute(name);
			}

			attr.set_value(val);
			return true;
		}

		virtual bool get_node(const tchar* name, IDataTreeNode& node)
		{
			pugi::xml_node xnode = GetChild(Current_, name, false);
			if(xnode)
			{
				Current_ = xnode;
				node.__import(*this);
				Current_ = Current_.parent();
			}

			return false;
		}

		virtual bool set_node(const tchar* name, IDataTreeNode const & node)
		{
			Current_ = GetChild(Current_, name, true);
			node.__export(*this);
			Current_ = Current_.parent();
			return true;
		}

		virtual bool get_nodes(const tchar* name, IDataTreeNodeInserter& nodes)
		{
			for(pugi::xml_node xnode = Current_.child(name); xnode; xnode = xnode.next_sibling(name))
			{
				Current_ = xnode;
				IDataTreeNode& node = nodes.InsertNew();
				node.__import(*this);
				Current_ = xnode.parent();
			}

			return true;
		}

		virtual bool set_nodes(const tchar* name, IEnumerator<IDataTreeNode>& nodes)
		{
			// 将数据输出到结点
			pugi::xml_node xnode = GetChild(Current_, name, true);
			pugi::xml_node tmp = Current_;
			while (xnode && nodes.MoveNext())
			{
				Current_ = xnode;
				nodes.Current().__export(*this);
				xnode = GetNeighbour(Current_, name, true);
			}
			Current_ = tmp;

			// 移除多余的同名结点
			while(tmp = xnode.next_sibling(name))
			{
				Current_.remove_child(xnode);
				xnode = tmp;
			}
			Current_.remove_child(xnode); // the last empty node

			return true;
		}

	protected:
		pugi::xml_node GetChild(pugi::xml_node& cur, const tchar* nodename, bool autoCreate = false)
		{
			pugi::xml_node node = cur.child(nodename);
			if(!node && autoCreate)
			{
				node = cur.append_child(nodename);
			}
			return node;
		}

		pugi::xml_node GetNeighbour(pugi::xml_node& cur, const tchar* nodename, bool autoCreate = false)
		{
			pugi::xml_node node = cur.next_sibling(nodename);
			if(!node && autoCreate)
			{
				node = cur.parent().append_child(nodename);
			}
			return node; 
		}

		CDataTreeJsonStorage& operator=(const CDataTreeJsonStorage&);

	protected:
		Json::Value*		Root_;
		Json::Value*		Current_;
	};


#endif // DataTreeJsonStorage_h__