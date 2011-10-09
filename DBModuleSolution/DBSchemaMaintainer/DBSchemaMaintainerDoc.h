
// DBSchemaMaintainerDoc.h : CDBSchemaMaintainerDoc 类的接口
//

#pragma once
#include <set>
#include "CDBSchemaTableItem.h"
#include "Module\DBModule.h"

class CDBSchemaMaintainerDoc : public CDocument
{
protected: // 仅从序列化创建
	CDBSchemaMaintainerDoc();
	DECLARE_DYNCREATE(CDBSchemaMaintainerDoc)

// 属性
public:
	virtual CDBModule& GetDBModule() { return DBModule_; }
	virtual CString& GetFile() { return File_; }

// 操作
public:
	virtual int	LoadDBSchema(IDBDataAdapter* dbAdapter, IDBFactory* dbFactory, IDBNameMapping* dbNameMapping);
	virtual int LoadBuildInSchema(LPCTSTR lpszSchemaFile);
	virtual int SaveBuildInSchema();

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CDBSchemaMaintainerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	NSDBModule::CDBModule	DBModule_;
	CString					File_;

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnOpenDB();
};


