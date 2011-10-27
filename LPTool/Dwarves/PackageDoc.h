
// PackageDoc.h : CPackageDoc 类的接口
//


#pragma once
#include <Module\DBModule.h>


class CPackageDoc : public CDocument
{
public:
	CPackageDoc();

protected: // 仅从序列化创建
	
	DECLARE_DYNCREATE(CPackageDoc)

// 属性
public:

// 操作
public:

	virtual void SetDBModule(NSDBModule::CDBModule* pModule)
	{
		if(DBModule)
		{
			delete DBModule;
		}
		DBModule = pModule;
	}

	virtual NSDBModule::CDBModule* GetDBModule()
	{
		return DBModule;
	}

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CPackageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	NSDBModule::CDBModule* DBModule;

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


