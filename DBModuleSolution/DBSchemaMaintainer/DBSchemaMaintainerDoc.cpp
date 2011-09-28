
// DBSchemaMaintainerDoc.cpp : CDBSchemaMaintainerDoc 类的实现
//

#include "stdafx.h"
#include "DBSchemaMaintainer.h"

#include "DBSchemaMaintainerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDBSchemaMaintainerDoc

IMPLEMENT_DYNCREATE(CDBSchemaMaintainerDoc, CDocument)

BEGIN_MESSAGE_MAP(CDBSchemaMaintainerDoc, CDocument)
END_MESSAGE_MAP()


// CDBSchemaMaintainerDoc 构造/析构

CDBSchemaMaintainerDoc::CDBSchemaMaintainerDoc()
{
	// TODO: 在此添加一次性构造代码

}

CDBSchemaMaintainerDoc::~CDBSchemaMaintainerDoc()
{
}

BOOL CDBSchemaMaintainerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CDBSchemaMaintainerDoc 序列化

void CDBSchemaMaintainerDoc::Serialize(CArchive& ar)
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


// CDBSchemaMaintainerDoc 诊断

#ifdef _DEBUG
void CDBSchemaMaintainerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDBSchemaMaintainerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDBSchemaMaintainerDoc 命令
