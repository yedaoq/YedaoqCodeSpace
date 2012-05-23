#pragma once

#include "DataTreeCleanMicro.h"

#define BEGIN_DATATREE(cppname) bool cppname::__export(nsYedaoqDataTree::IDataTreeStorage& storage) const { tstring tmp;

#define END_DATATREE return true; }

#define ITEM_DATANODE(nodename, cppname, cpptype) storage.set_node(TEXT(#nodename), cppname);

#define ITEM_DATANODES(nodename, cppname, cpptype) storage.set_nodes(TEXT(#nodename), make_iterator_enumerator_ex<nsYedaoqDataTree::IDataTreeNode>(cppname.begin(), cppname.end()));																					

#define ITEM_ATTR(attrname, cppname, cpptype) storage.set_attr(TEXT(#attrname), nsYedaoqDataTree::CSingleValueSerializer<cpptype>().Serialize(cppname));

#define ITEM_ENUMATTR(attrname, cppname, cpptype) storage.set_attr(TEXT(#attrname), cppname.str());
