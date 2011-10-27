
// PackageDoc.cpp : CPackageDoc 类的实现
//

#include "stdafx.h"
#include "Dwarves.h"

#include "PackageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPackageDoc

IMPLEMENT_DYNCREATE(CPackageDoc, CDocument)

BEGIN_MESSAGE_MAP(CPackageDoc, CDocument)
END_MESSAGE_MAP()


// CPackageDoc 构造/析构

CPackageDoc::CPackageDoc()
	: DBModule(0)
{
	// TODO: 在此添加一次性构造代码

}

CPackageDoc::~CPackageDoc()
{
}

BOOL CPackageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

// CPackageDoc 序列化

void CPackageDoc::Serialize(CArchive& ar)
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


// CPackageDoc 诊断

#ifdef _DEBUG
void CPackageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPackageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPackageDoc 命令
