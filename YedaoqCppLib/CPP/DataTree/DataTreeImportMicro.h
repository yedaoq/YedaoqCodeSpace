#pragma once

#include "DataTreeCleanMicro.h"

#define BEGIN_DATATREE(cppname) bool cppname::__import(nsYedaoqDataTree::IDataTreeStorage& storage){ tstring tmp;

#define END_DATATREE return true; }

#define ITEM_DATANODE(nodename, cppname, cpptype) storage.get_node(TEXT(#nodename), cppname);

#define ITEM_DATANODES(nodename, cppname, cpptype) cppname.clear(); storage.get_nodes(TEXT(#nodename), nsYedaoqDataTree::make_datanodeinserter(cppname));																					

#define ITEM_ATTR(attrname, cppname, cpptype) if(storage.get_attr(TEXT(#attrname), tmp)) { cppname = nsYedaoqDataTree::CSingleValueSerializer<cpptype>().Parse(tmp); }

#define ITEM_ENUMATTR(attrname, cppname, cpptype) if(storage.get_attr(TEXT(#attrname), tmp)) { cppname = Enum##cpptype::Parse(tmp); }

