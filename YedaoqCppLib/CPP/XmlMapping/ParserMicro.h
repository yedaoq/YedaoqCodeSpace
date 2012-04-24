#pragma once

#ifdef XML_COMPLEXTYPE_BEGIN
	#undef XML_COMPLEXTYPE_BEGIN
#endif
#define XML_COMPLEXTYPE_BEGIN(cppname) void cppname::Parse(xnode_t& node){ xattr_t* attr; xnode_t* child;


#ifdef XML_COMPLEXTYPE_END
	#undef XML_COMPLEXTYPE_END
#endif
#define XML_COMPLEXTYPE_END }


#ifdef XML_ELE
	#undef XML_ELE
#endif
#define XML_ELE(xmlname, cppname, cpptype) child = node.first_node(TEXT(xmlname)); if(child){ name.Parse(*child); }


#ifdef XML_ELES
	#undef XML_ELES
#endif
#define XML_ELES(xmlname, cppname, cpptype)										\
	for(child = node.first_node(TEXT(xmlname)); child; child = child->next_sibling())	\
	{																				\
		cpptype sub;																\
		sub.Parse(*child);															\
		cppname.push_back(sub);														\
	}


#ifdef XML_ATTR
	#undef XML_ATTR
#endif
#define XML_ATTR(xmlname, cppname, cpptype) attr = node.first_attribute(TEXT(xmlname)); if(attr){ cppname = CXmlValueSerializer<cpptype>().Parse(attr->value());}

#ifdef XML_ENUMATTR
	#undef XML_ENUMATTR
#endif
#define XML_ENUMATTR(t, name) attr = node.first_attribute(TEXT(#name)); if(attr){ name = t::parse(attr->value(), TEXT(" "));}
