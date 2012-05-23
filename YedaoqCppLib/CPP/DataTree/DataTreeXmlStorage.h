/* ___________________________________________________________________________
@ 
@ file - DataTreeXmlStorage.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-5-23
@ info -
@     
/* ___________________________________________________________________________*/
#pragma once

#include "DataTreeCommon.h"
#include "pugixml.hpp"
#include <fstream>

namespace nsYedaoqDataTree
{
	class CDataTreeXmlStorage : IDataTreeStorage
	{
		struct ScopedCurrentNode
		{
			ScopedCurrentNode(pugi::xml_node& node, pugi::xml_node& scopedVal)
				: Node(node), Value(node)
			{
				Node = scopedVal;
			}

			~ScopedCurrentNode()
			{
				Node = Value;
			}

			pugi::xml_node& Node;
			pugi::xml_node	Value;
		};

	public:
		CDataTreeXmlStorage(pugi::xml_node& doc, const tchar* nodename)
			: Root_(doc)
		{
			Current_ = GetChild(doc, nodename, true);
		}

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
				node = cur.parent.append_child(nodename);
			}
			return node; 
		}

		virtual bool get_attr(const tchar* name, tstring& val)
		{
			pugi::xml_attribute attr = Current_.attribute(name);
			if(attr)
			{
				val = attr.value();
			}
			return attr;
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

		virtual bool get_node(const tchar* name, IDataTreeNode& node)
		{
			//ScopedCurrentNode tmp(Current_, Current_, GetChild(Current_, name, false));

			pugi::xml_node xnode = GetChild(Current_, name, false);
			if(xnode)
			{
				Current_ = xnode;
				return node.__import(*this);
				Current_ = Current_.parent();
			}

			return false;
		}

		virtual bool set_node(const tchar* name, IDataTreeNode& node)
		{
			//ScopedCurrentNode tmp(Current_, Current_, GetChild(Current_, name, true));

			Current_ = GetChild(Current_, name, true);
			return node.__export(*this);
			Current_ = Current_.parent();

		}

		virtual bool get_nodes(const tchar* name, IDataTreeNodeInserter& nodes)
		{
			//ScopedCurrentNode tmp(Current_, Current_, Current_);

			for(pugi::xml_node xnode = Current_.child(name); xnode; xnode = xnode.next_sibling(name))
			{
				Current_ = xnode;
				IDataTreeNode& node = nodes.InsertNew();
				node.__import(*this);
				Current_ = xnode.parent();
			}

			return false;
		}

		virtual bool set_nodes(const tchar* name, IEnumerator<IDataTreeNode&>& nodes)
		{
			//ScopedCurrentNode tmp(Current_, Current_, GetChild(Current_, name, true));

			pugi::xml_node xnode = GetChild(Current_, name, true);
			while (xnode && nodes.MoveNext())
			{
				Current_ = xnode;
				nodes.Current().__export(*this);
				xnode = GetNeighbour(Current_, name, true);
				Current_ = xnode.parent();
			}

			return true;
		}

	protected:
		pugi::xml_node&		Root_;
		pugi::xml_node		Current_;
	};
}
