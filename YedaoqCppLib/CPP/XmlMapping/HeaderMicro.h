#pragma once

#ifndef XML_COMPLEXTYPE_BEGIN
	#define XML_COMPLEXTYPE_BEGIN(cppname) struct cppname{ xnode_t* Serialize(xdoc_t* doc, tchar const* elename); void Parse(xnode_t& node);
#endif

#ifndef XML_COMPLEXTYPE_END
	#define XML_COMPLEXTYPE_END };
#endif

#ifndef XML_ELE
	#define XML_ELE(xmlname, cppname, cpptype) cpptype cppname;
#endif

#ifndef XML_ELES
	#define XML_ELES(xmlname, cppname, cpptype) std::vector<cpptype> cppname;
#endif

#ifndef XML_ATTR
	#define XML_ATTR(xmlname, cppname, cpptype) cpptype cppname;
#endif

#ifndef XML_ENUMATTR
	#define XML_ENUMATTR(xmlname, cppname, cpptype) Enum##cpptype cppname;
#endif