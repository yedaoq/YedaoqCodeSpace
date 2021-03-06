/* ___________________________________________________________________________
@ 
@ file - DataTreeCommon.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-5-23
@ info -
@     
/* ___________________________________________________________________________*/

#ifndef DataTreeCommon_h__
#define DataTreeCommon_h__

#include "tstring.h"
#include "..\Enumerator.h"
#include <sstream>

namespace nsYedaoqDataTree
{
	interface IDataTreeNode; 
	interface IDataTreeExporter;
	interface IDataTreeImporter;
	interface IDataTreeStorage;
	interface IDataTreeNodeInserter;

	// interface definition
	interface IDataTreeNode
	{
		virtual bool __export(IDataTreeStorage& storage) const = 0;
		virtual bool __import(IDataTreeStorage& storage) = 0;
	};

	interface IDataTreeExporter
	{
		virtual bool set_attr(const tchar* name, tstring const& val) = 0;
		virtual bool set_attr(const tchar* name, bool val) = 0;
		virtual bool set_attr(const tchar* name, double val) = 0;
		virtual bool set_attr(const tchar* name, int val) = 0;
		virtual bool set_attr(const tchar* name, unsigned int val) = 0;

		virtual bool set_node(const tchar* name, IDataTreeNode const & node) = 0;
		virtual bool set_nodes(const tchar* name, IEnumerator<IDataTreeNode>& nodes) = 0;
	};

	interface IDataTreeImporter
	{
		virtual tstring get_attr(const tchar* name) = 0;
		virtual bool get_attr(const tchar* name) = 0;
		virtual double get_attr(const tchar* name) = 0;
		virtual int get_attr(const tchar* name) = 0;
		virtual unsigned int get_attr(const tchar* name) = 0;

		virtual bool get_node(const tchar* name, IDataTreeNode& node) = 0;
		virtual bool get_nodes(const tchar* name, IDataTreeNodeInserter& nodes) = 0;
	};

	interface IDataTreeStorage
	{
		virtual bool get_attr(const tchar* name, tstring& val) = 0;
		virtual bool set_attr(const tchar* name, tstring const& val) = 0;

		virtual bool get_attr(const tchar* name, bool& val) = 0;
		virtual bool set_attr(const tchar* name, bool val) = 0;

		virtual bool get_attr(const tchar* name, double& val) = 0;
		virtual bool set_attr(const tchar* name, double val) = 0;

		virtual bool get_attr(const tchar* name, int& val) = 0;
		virtual bool set_attr(const tchar* name, int val) = 0;

		virtual bool get_attr(const tchar* name, unsigned int& val) = 0;
		virtual bool set_attr(const tchar* name, unsigned int val) = 0;

		virtual bool get_node(const tchar* name, IDataTreeNode& node) = 0;
		virtual bool set_node(const tchar* name, IDataTreeNode const & node) = 0;

		virtual bool get_nodes(const tchar* name, IDataTreeNodeInserter& nodes) = 0;
		virtual bool set_nodes(const tchar* name, IEnumerator<IDataTreeNode>& nodes) = 0;
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
	CDataTreeNodeInserter<T> make_datanodeinserter(T& nodes)
	{
		return CDataTreeNodeInserter<T>(nodes);
	}

	template<typename T>
	struct CSingleValueSerializer
	{
		tstring Serialize(const T& val) const
		{
			tostringstream stream;
			stream<<val;
			return stream.str();
			//return boost::lexical_cast<tstring>(val);
		}

		T Parse(tstring& val) const
		{
			T ret;
			tistringstream stream(val);
			stream>>ret;
			return ret;
			//return boost::lexical_cast<T>(val);
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
}

#endif // DataTreeCommon_h__
