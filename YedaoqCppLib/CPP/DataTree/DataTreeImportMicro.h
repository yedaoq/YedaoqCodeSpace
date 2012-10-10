/* ___________________________________________________________________________
@ 
@ file - C:\YedaoqCodeSpace\YedaoqCppLib\CPP\DataTree\DataTreeImportMicro.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-9-6
@ info -
@     
/* ___________________________________________________________________________*/

#ifndef DataTreeImportMicro_h__
#define DataTreeImportMicro_h__

#include "DataTreeCleanMicro.h"

using namespace nsYedaoqDataTree;

#define BEGIN_DATATREE(cppname) bool cppname::__import(IDataTreeImporter& storage){

#define END_DATATREE return true; }

#define ITEM_DATANODE(nodename, cppname, cpptype) storage.get_node(TEXT(#nodename), cppname);

#define ITEM_DATANODES(nodename, cppname, cpptype) cppname.clear(); storage.get_nodes(TEXT(#nodename), make_datanodeinserter(cppname));																					

#define ITEM_ATTR(attrname, cppname, cpptype) cppname = Serialize_TypeTrait<cpptype>::FromSerializeType(storage.get_attr(TEXT(#attrname), Serialize_TypeTrait<cpptype>::Default_Value()));

#define ITEM_ENUMATTR(attrname, cppname, cpptype) cppname = Enum##cpptype::Parse(storage.get_attr(attrname, tstring()));

#endif // DataTreeImportMicro_h__
