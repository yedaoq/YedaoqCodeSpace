#pragma once

#include "DataTreeCommon.h"

#ifndef BEGIN_DATATREE
	#define BEGIN_DATATREE(cppname) struct cppname : public nsYedaoqDataTree::IDataTreeNode { virtual bool __export(nsYedaoqDataTree::IDataTreeStorage& storage) const; virtual bool __import(nsYedaoqDataTree::IDataTreeStorage& storage);
#endif

#ifndef END_DATATREE
	#define END_DATATREE };
#endif

#ifndef ITEM_DATANODE
	#define ITEM_DATANODE(nodename, cppname, cpptype) cpptype cppname;
#endif

#ifndef ITEM_DATANODES
	#define ITEM_DATANODES(nodename, cppname, cpptype) std::vector<cpptype> cppname;
#endif

#ifndef ITEM_ATTR
	#define ITEM_ATTR(nodename, cppname, cpptype) cpptype cppname;
#endif

#ifndef ITEM_ENUMATTR
	#define ITEM_ENUMATTR(xmlname, cppname, cpptype) Enum##cpptype cppname;
#endif