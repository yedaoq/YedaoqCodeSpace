
// DwarfDoc.cpp : CDwarfDoc 类的实现
//

#include "stdafx.h"
#include "Dwarves.h"

#include "DwarfDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDwarfDoc

IMPLEMENT_DYNCREATE(CDwarfDoc, CDocument)

BEGIN_MESSAGE_MAP(CDwarfDoc, CDocument)
END_MESSAGE_MAP()


// CDwarfDoc 构造/析构

CDwarfDoc::CDwarfDoc()
	: DBModule(0)
{
	// TODO: 在此添加一次性构造代码

}

CDwarfDoc::~CDwarfDoc()
{
}

BOOL CDwarfDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

// CDwarfDoc 序列化

void CDwarfDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CDwarfDoc 诊断

#ifdef _DEBUG
void CDwarfDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDwarfDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDwarfDoc 命令
