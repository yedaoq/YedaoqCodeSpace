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

#include "DataTreeCleanMicro.h"

using namespace nsYedaoqDataTree;

#define BEGIN_DATATREE(cppname) bool cppname::__export(IDataTreeExporter& storage) const { tstring tmp;

#define END_DATATREE return true; }

#define ITEM_DATANODE(nodename, cppname, cpptype) storage.set_node(TEXT(#nodename), cppname);

#define ITEM_DATANODES(nodename, cppname, cpptype) storage.set_nodes(TEXT(#nodename), make_iterator_enumerator_ex<IDataTreeNode>(cppname.begin(), cppname.end()));																					

#define ITEM_ATTR(attrname, cppname, cpptype) storage.set_attr(TEXT(#attrname), Serialize_TypeTrait<cpptype>::ToSerializeType(cppname));

#define ITEM_ENUMATTR(attrname, cppname, cpptype) storage.set_attr(TEXT(#attrname), cppname.str());

#endif // DataTreeExportMicro_h__
