/* ___________________________________________________________________________
@ 
@ file - C:\YedaoqCodeSpace\YedaoqCppLib\CPP\DataTree\DataTreeExportMicro.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-9-6
@ info -
@     
/* ___________________________________________________________________________*/

#ifndef DataTreeExportMicro_h__
#define DataTreeExportMicro_h__

#ifndef _DATATREE_HELP_CHECK_UNDEFINE_HEADER_
	#error please undefine you header micro of config header file
#else
	#undef _DATATREE_HELP_CHECK_UNDEFINE_HEADER_
#endif

#include "DataTreeCleanMicro.h"

#define BEGIN_DATATREE(cppname) bool cppname::__export(nsYedaoqDataTree::IDataTreeStorage& storage) const { tstring tmp;

#define END_DATATREE return true; }

#define ITEM_DATANODE(nodename, cppname, cpptype) storage.set_node(TEXT(#nodename), cppname);

#define ITEM_DATANODES(nodename, cppname, cpptype) storage.set_nodes(TEXT(#nodename), make_iterator_enumerator_ex<nsYedaoqDataTree::IDataTreeNode>(cppname.begin(), cppname.end()));																					

#define ITEM_ATTR(attrname, cppname, cpptype) storage.set_attr(TEXT(#attrname), nsYedaoqDataTree::CSingleValueSerializer<cpptype>().Serialize(cppname));

#define ITEM_ENUMATTR(attrname, cppname, cpptype) storage.set_attr(TEXT(#attrname), cppname.str());

#endif // DataTreeExportMicro_h__
