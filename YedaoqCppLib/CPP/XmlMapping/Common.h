#pragma once

#include "\rapidxml-1.13\rapidxml.hpp"
#include "\rapidxml-1.13\rapidxml_print.hpp"
#include "tstring.h"
#include <fstream>

namespace nsYedaoqXmlMapping
{
	typedef rapidxml::xml_node<tchar>		xnode_t;
	typedef rapidxml::xml_attribute<tchar>	xattr_t;
	typedef rapidxml::xml_document<tchar>	xdoc_t;

	template<typename T>
	struct CXmlValueSerializer
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
	struct CXmlValueSerializer<tstring>
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
	struct CXmlValueSerializer<tchar*>
	{
		tstring Serialize(tchar const* val) const
		{
			return val;
		}

		tchar const* Parse(tstring& val) const
		{
			return val.c_str();
		}
	};

	template<>
	struct CXmlValueSerializer<tchar*>
	{
		tstring Serialize(tchar const* val) const
		{
			return val;
		}

		tchar const* Parse(tstring& val) const
		{
			return val.c_str();
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
