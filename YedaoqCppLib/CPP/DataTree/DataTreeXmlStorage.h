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

#ifdef UNICODE
#define PUGIXML_WCHAR_MODE
#endif

#include "DataTreeCommon.h"
#include "pugixml\pugixml.hpp"
#include <fstream>

namespace nsYedaoqDataTree
{
	class CDataTreeXmlStorage : public IDataTreeStorage
	{
	public:
		CDataTreeXmlStorage(pugi::xml_node& doc, const tchar* nodename)
			: Root_(doc)
		{
			Current_ = GetChild(doc, nodename, true);
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
			pugi::xml_node xnode = GetChild(Current_, name, true);
			while (xnode && nodes.MoveNext())
			{
				Current_ = xnode;
				nodes.Current().__export(*this);
				xnode = GetNeighbour(Current_, name, true);
				Current_ = xnode.parent();
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

		CDataTreeXmlStorage& operator=(const CDataTreeXmlStorage&);

	protected:
		pugi::xml_node&		Root_;
		pugi::xml_node		Current_;
	};

	template<typename T>
	struct CXmlFile
	{
		T Object;

		bool Load(tchar const* file);
		bool Save(tchar const* file);
	};

	template<typename T>
	bool CXmlFile<T>::Save( tchar const* file )
	{
		xdoc_t doc;
		doc.append_node(Object.Serialize(&doc));

		tstring content;
		rapidxml::print(std::back_inserter(content), doc, 0);

		std::locale prev_loc = std::locale::global(std::locale("chs"));
		std::tofstream f(file, std::ios_base::out);

		f<<content;
		f.flush();
		f.close();

		return true;
	}

	template<typename T>
	bool CXmlFile<T>::Load( tchar const* file )
	{
		std::locale prev_loc = std::locale::global(std::locale("chs"));
		std::tifstream f(file, std::ios_base::in);

		tstring content;
		std::getline(f, content, EOF);

		tchar* bufXml = new tchar[content.size() + 1];
		content.copy(bufXml, content.size());
		bufXml[content.size()] = 0;

		xdoc_t doc;
		doc.parse<0>(bufXml);

		Object.Parse(doc.first_node());

		return true;
	}
}
