#ifndef _CPPTEST_DATATREETEST_H_
#define _CPPTEST_DATATREETEST_H_

#include "DataTree/DataTreeHeaderMicro.h"

BEGIN_DATATREE(CfgFriend)
	ITEM_ATTR(name, Name, tstring)
	ITEM_ATTR(ship, Ship, int)
END_DATATREE

BEGIN_DATATREE(CfgPerson)
	ITEM_ATTR(name, Name, tstring)
	ITEM_ATTR(address, Address, tstring)
	ITEM_ATTR(age, Age, int)
	ITEM_DATANODE(goodfriend, GoodFriend, CfgFriend)
	ITEM_DATANODES(friends, Friends, CfgFriend)
END_DATATREE

#include "DataTree/DataTreeCleanMicro.h"

#endif



