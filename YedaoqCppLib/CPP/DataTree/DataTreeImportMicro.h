#pragma once

#ifdef XML_COMPLEXTYPE_BEGIN
	#undef XML_COMPLEXTYPE_BEGIN
#endif
#define XML_COMPLEXTYPE_BEGIN(cppname) xnode_t* cppname::Serialize(xdoc_t* doc, tchar const* elename){ xnode_t* node = doc->allocate_node(rapidxml::node_type::node_element, TEXT(elename));


#ifdef XML_COMPLEXTYPE_END
	#undef XML_COMPLEXTYPE_END
#endif
#define XML_COMPLEXTYPE_END return node; }


#ifdef XML_ELE
	#undef XML_ELE
#endif
#define XML_ELE(xmlname, cppname, cpptype) node.append_node(cppname.Serialize(doc, xmlname));


#ifdef XML_ELES
	#undef XML_ELES
#endif
#define XML_ELES(xmlname, cppname, cpptype)														\
	for(std::vector<cpptype>::iterator iter = cppname.begin(); iter != cppname.end(); ++iter)	\
	{																							\
		node.append_node(iter->Serialize(doc, xmlname));										\
	}																						


#ifdef XML_ATTR
	#undef XML_ATTR
#endif
#define XML_ATTR(xmlname, cppname, cpptype) node.append_attribute(doc->allocate_attribute(TEXT(xmlname), CConfigValueSerializer<cpptype>().Serialize(cppname)));

#ifdef XML_ENUMATTR
	#undef XML_ENUMATTR
#endif
#define XML_ENUMATTR(t, name) node.append_attribute(doc->allocate_attribute(TEXT(#name), name.str()));
