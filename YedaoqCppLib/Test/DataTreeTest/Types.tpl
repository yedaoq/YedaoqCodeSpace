BEGIN_DATATREE(CfgChapter)
	ITEM_ATTR(idx, Idx, int)
	ITEM_ATTR(title, Title, tstring)
END_DATATREE

BEGIN_DATATREE(CfgSourceBook)
	ITEM_ATTR(source_id, SourceID, int)
	ITEM_ATTR(source_book_id, SourceBookID, int)
	ITEM_DATANODES(chapter, Chapters, CfgChapter)
	ITEM_DATANODE(cur_chapter, CurChapter, CfgChapter)
END_DATATREE

BEGIN_DATATREE(CfgBook)
	ITEM_ATTR(name, Name, tstring)
	ITEM_ATTR(id, ID, int)
	ITEM_ATTR(ended, Ended, bool)
	ITEM_DATANODES(sources, Sources, CfgSourceBook)
END_DATATREE

