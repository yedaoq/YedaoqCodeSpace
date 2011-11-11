
// DwarfDoc.h : CDwarfDoc 类的接口
//


#pragma once
#include <Module\DBModule.h>


class CDwarfDoc : public CDocument
{
public:
	CDwarfDoc();

protected: // 仅从序列化创建
	
	DECLARE_DYNCREATE(CDwarfDoc)

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
	virtual ~CDwarfDoc();
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


