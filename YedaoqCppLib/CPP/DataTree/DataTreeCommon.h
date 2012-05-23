/* ___________________________________________________________________________
@ 
@ file - DataTreeCommon.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-5-23
@ info -
@     
/* ___________________________________________________________________________*/
#pragma once

#include "tstring.h"
#include "..\Enumerator.h"

namespace nsYedaoqDataTree
{
	interface IDataTreeNode; 
	interface IDataTreeStorage;
	interface IDataTreeNodeInserter;

	// interface definition
	interface IDataTreeNode
	{
		virtual bool __export(IDataTreeStorage& storage) const = 0;
		virtual bool __import(IDataTreeStorage& storage) = 0;
	};

	interface IDataTreeStorage
	{
		virtual bool	get_attr(const tchar* name, tstring& val) = 0;
		virtual bool	set_attr(const tchar* name, tstring const& val) = 0;

		virtual bool	get_node(const tchar* name, IDataTreeNode& node) = 0;
		virtual bool	set_node(const tchar* name, IDataTreeNode& node) = 0;

		virtual bool	get_nodes(const tchar* name, IDataTreeNodeInserter& nodes) = 0;
		virtual bool	set_nodes(const tchar* name, IEnumerator<IDataTreeNode&>& nodes) = 0;
	};

	interface IDataTreeNodeInserter
	{
		virtual IDataTreeNode& InsertNew() = 0;
	};

	// IDataTreeNodeInserter implementation
	template<typename T>
	struct CDataTreeNodeInserter : public IDataTreeNodeInserter
	{
		CDataTreeNodeInserter(T& nodes)
			: Nodes(nodes)
		{}

		CDataTreeNodeInserter(const CDataTreeNodeInserter& other)
			: Nodes(other.Nodes)
		{}

		virtual IDataTreeNode& InsertNew()
		{
			Nodes.push_back(typename T::iterator::value_type());
			return *Nodes.rbegin();
		}

	protected:
		T& Nodes;

		CDataTreeNodeInserter& operator=(const CDataTreeNodeInserter&);
	};

	template<typename T>
	CDataTreeNodeInserter make_datanodeinserter(T& nodes)
	{
		return CDataTreeNodeInserter<T>(nodes);
	}

	struct CDTFriend : public IDataTreeNode
	{
		virtual bool Export(IDataTreeStorage& storage) const
		{
			storage.set_attr("name", Name);
		}

		virtual bool Import(IDataTreeStorage& storage)
		{
			Name = storage.get_attr("name");
		}

		tstring Name;
	};

	struct CDTConfig : public IDataTreeNode
	{
		virtual bool Export(IDataTreeStorage& storage, const tchar* nodename) const
		{
			storage.set_attr("name", Name);
			storage.set_attr("address", Address);
			storage.set_nodes("friends", make_datanodeinserter(Friends));
			storage.set_node("goodfriend", GoodFriend);
		}

		virtual bool Import(IDataTreeStorage& storage)
		{
			Name = storage.get_attr("name");
			Address = storage.get_attr("address");
			storage.get_nodes("friends", IEnumerator<IDataTreeNode&>& Friends);
			storage.get_node("goodfriend", GoodFriend);
		}

		tstring Name;
		tstring Address;
		CDTFriend GoodFriend;

		std::vector<CDTFriend> Friends;
	};



	template<typename T>
	struct CSingleValueSerializer
	{
		tstring Serialize(const T& val) const
		{
			return boost::lexical_cast<tstring>(val);
		}

		T Parse(tstring& val) const
		{
			return boost::lexical_cast<T>(val);
		}
	};

	template<>
	struct CSingleValueSerializer<tstring>
	{
		tstring Serialize(const tstring& val) const
		{
			return val;
		}

		tstring Parse(tstring& val) const
		{
			return val;
		}
	};

	template<>
	struct CSingleValueSerializer<tchar*>
	{
		// your should not use raw char ptr to storage a xml string
	};

	template<typename T>
	struct functor_SingleValue2Str
	{
		tstring operator()(const T& val) const
		{
			CSingleValueSerializer<T>().Serialize(val);
		}
	};

	template<typename T>
	struct CSingleValueSerializer<CXsList<T> >
	{
		tstring Serialize(const CXsList& val) const
		{
			return StringSplice(make_convert_enumerator(
				make_iterator_enumerator(val.begin(), val.end())
				,functor_XmlValue2Str<T>())
				, TEXT(" "));
		}

		tchar const* Parse(tstring& val) const
		{
			return boost::split(*this, val, std::isspace);
		}
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
