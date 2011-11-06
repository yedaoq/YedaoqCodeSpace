
// DBSchemaMaintainerDoc.cpp : CDBSchemaMaintainerDoc 类的实现
//

#include "stdafx.h"
#include "DBSchemaMaintainer.h"
#include "DBSchemaMaintainerDoc.h"
#include "Module\Schema\BuildInSchemaSerializer.h"
#include "DBCommon\DBNameMappingNone.h"
#include "Module\DBTable.h"
#include "Helper.h"
#include "FileEnumerator.h"
#include "DynamicLinkLibrary.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef NSDBModule::IDBSourceManager* (WINAPI *FunGetSourceManager)();
typedef NSDBModule::IDBNameMapping* (WINAPI *FunGetNameMapping)();

// CDBSchemaMaintainerDoc

IMPLEMENT_DYNCREATE(CDBSchemaMaintainerDoc, CDocument)

BEGIN_MESSAGE_MAP(CDBSchemaMaintainerDoc, CDocument)
	ON_COMMAND(ID_FILE_OPENDB, &CDBSchemaMaintainerDoc::OnOpenDB)
	ON_COMMAND(ID_FILE_OPEN, &CDBSchemaMaintainerDoc::OnOpenFile)
	ON_COMMAND(ID_FILE_SAVE, &CDBSchemaMaintainerDoc::OnFileSave)
END_MESSAGE_MAP()


// CDBSchemaMaintainerDoc 构造/析构

CDBSchemaMaintainerDoc::CDBSchemaMaintainerDoc()
{
	// TODO: 在此添加一次性构造代码
	
}

CDBSchemaMaintainerDoc::~CDBSchemaMaintainerDoc()
{
}

int	CDBSchemaMaintainerDoc::LoadDBSchema(IDBDataAdapter* dbAdapter, IDBFactory* dbFactory, IDBNameMapping* dbNameMapping)
{
	DBModule_.Clear(false);

	if(DBModule_.AttachToDatabase(dbAdapter, dbFactory, dbNameMapping) < 1)
	{
		return 0;
	}

	return 1;
}

int CDBSchemaMaintainerDoc::LoadBuildInSchema(LPCTSTR lpszSchemaFile)
{
	if(!lpszSchemaFile)
	{
		TCHAR pszFilter[] = TEXT("Database Schema File(*.dbschema)|*.dbschema|All Files(*.*)|*.*|");
		CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, pszFilter);
		if(dlg.DoModal() != IDOK)
		{
			return 0;
		}
		ParseProjectInfo(dlg.GetPathName());
	}
	else if(_taccess(lpszSchemaFile, 0) != 0)
	{
		_ASSERT(FALSE);
		return 0;
	}
	else
	{
		ParseProjectInfo(lpszSchemaFile);
	}

	DBModule_.Clear(true);

	CBuildInSchemaSerializer serializer; 
	CBuildInSchemaSerializer::FileRowCollection rows;

	//read file
	CStdioFile file(GetBuildInSchemaDetailFile(), CStdioFile::modeRead);
	CString line;
	while(file.ReadString(line))
	{
		rows.push_back((LPCTSTR)line);
	}
	file.Close();

	// read schema
	DBModule_.Clear(true);
	serializer.Read(make_iterator_enumerator(rows.begin(), rows.end()), DBModule_);

	return 1;
}

int CDBSchemaMaintainerDoc::SaveBuildInSchemaDetail()
{
	CBuildInSchemaSerializer serializer; 
	CBuildInSchemaSerializer::FileRowCollection rows;
	serializer.Write(DBModule_, rows);

	CStdioFile file(GetBuildInSchemaDetailFile(), CStdioFile::modeWrite | CStdioFile::modeCreate);

	for (CBuildInSchemaSerializer::FileRowCollection::iterator iter = rows.begin(); iter != rows.end(); ++iter)
	{
		file.WriteString(iter->c_str());
		file.WriteString(TEXT("\n"));
	}

	file.Flush();
	file.Close();

	return 1;
}

int CDBSchemaMaintainerDoc::SaveBuildInSchemaObjID()
{
	std::auto_ptr<DBTableEnumerator> pTblEnum(GetDBModule().Tables().Enum());
	CStdioFile	file(GetBuildInSchemaObjIDFile(), CStdioFile::modeWrite | CStdioFile::modeCreate);
	CDBTable*	pTbl;
	CString		strTmp;

	file.WriteString(TEXT("#pragma once\n\n"));
	strTmp.Format(TEXT("enum Enum%sTables\n"), ProjectName_); 
	file.WriteString(strTmp);
	file.WriteString(TEXT("{\n"));

	while(pTblEnum->MoveNext(pTbl))
	{
		strTmp.Format(TEXT("\tTBL_%s,\n"), pTbl->GetName().c_str());
		file.WriteString(strTmp);
	}
	file.WriteString(TEXT("};\n\n"));
	
	pTblEnum->Reset();
	while(pTblEnum->MoveNext(pTbl))
	{
		strTmp.Format(TEXT("enum Enum%sColumns\n{\n"), pTbl->GetName().c_str());
		file.WriteString(strTmp);

		std::auto_ptr<DBColumnEnumerator> pColEnum(pTbl->EnumColumn());
		while(pColEnum->MoveNext())
		{
			strTmp.Format(TEXT("\tCOL_%s_%s,\n"), pTbl->GetName().c_str(), pColEnum->Current().Name.c_str());
			file.WriteString(strTmp);
		}

		file.WriteString(TEXT("};\n\n"));
	}

	file.Flush();
	file.Close();

	return 1;
}

int	CDBSchemaMaintainerDoc::UpdateModuleAsBuildin()
{
	std::auto_ptr<DBTableEnumerator> pEnumTbl(GetDBModule().Tables().Enum());

	if(!pEnumTbl.get())
	{
		_ASSERT(FALSE);
		return -1;
	}

	DBTablePtr pTbl;
	while(pEnumTbl->MoveNext(pTbl))
	{
		pTbl->GetSchema().SetFlag(CDBTableSchema::BuildIn, true);

		for(CDBTableSchema::ColumnCollection::iterator iter = pTbl->GetSchema().Columns.begin();
			iter != pTbl->GetSchema().Columns.end();
			++iter)
		{
			iter->SetFlag(EnumDBColumnSchemaFlag::BuildIn, true);
		}
	}

	return 1;
}

int CDBSchemaMaintainerDoc::MergeTable(int iTbl1, int iTbl2)
{
	CDBTableCollection& Tables = GetDBModule().Tables();

	flag_t flag = Tables[iTbl1]->GetSchema().Flag ^ Tables[iTbl2]->GetSchema().Flag;
	
	if(!(flag & CDBTableSchema::BuildIn) || !(flag & CDBTableSchema::DBExist))
	{
		MsgboxPrompt(TEXT("表[ %s ]与[ %s ]不满足合并条件!"), 
			Tables[iTbl1]->GetName().c_str(), 
			Tables[iTbl2]->GetName().c_str());
		
		return -1;
	}

	if(EnumMessageBoxResult::MBROK != MsgboxConfirm(
		TEXT("确定要合并表[ %s ]与[ %s ] ?"), 
		Tables[iTbl1]->GetName().c_str(), 
		Tables[iTbl2]->GetName().c_str())
		)
	{
		return 0;
	}

	return 0;
}

int CDBSchemaMaintainerDoc::MergeColumn(int iTbl, int iCol1, int iCol2)
{
	if(iTbl < 0 || iTbl >= DBModule_.Tables().Count())
	{
		_ASSERT(FALSE);
		return -1;
	}
	
	CDBTableSchema& Tbl = DBModule_.Tables()[iTbl]->GetSchema();
	
	flag_t flag = Tbl[iCol1].Flag ^ Tbl[iCol2].Flag;
	if(!(flag & EnumDBColumnSchemaFlag::BuildIn) || !(flag & EnumDBColumnSchemaFlag::DBExist))
	{
		MsgboxPrompt(TEXT("列[ %s ]与[ %s ]不满足合并条件!"), 
			Tbl[iCol1].Name.c_str(), 
			Tbl[iCol2].Name.c_str());

		return -1;
	}

	if(EnumMessageBoxResult::MBROK != MsgboxConfirm(
		TEXT("确定要合并列[ %s ]与[ %s ] ?"), 
		Tbl[iCol1].Name.c_str(), 
		Tbl[iCol2].Name.c_str()))
	{
		return 0;
	}

	// Update the Buildin Column info
	if(iCol1 >= iCol2)
	{
		_ASSERT(false);
		std::swap(iCol1, iCol2);
	}

	DBColumnSchema& Col1 = Tbl[iCol1];
	DBColumnSchema& Col2 = Tbl[iCol2];

	Col1.SetExternInfo(Col2.DBName, Col2.DBIndex, Col2.DBType, Col2.IsDBExist(), Col2.IsDBPrimaryKey(), Col2.IsDBUnnull());
	Col1.Name = Col2.Name;

	Tbl.RemoveColumn(Col2.Index);

	UpdateAllViews(NULL);

	return 1;
}

bool CDBSchemaMaintainerDoc::FindDBProviderDLL(tstring& filePath)
{
	TCHAR AppPath[MAX_PATH] = {0};
	TCHAR DllPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, AppPath, MAX_PATH);
	PathRemoveFileSpec(AppPath);
	CFileEnumerator EnumFile(AppPath, TEXT("*.dll"));

	while(EnumFile.MoveNext())
	{
		if(PathIsRelative(EnumFile.Current().cFileName))
		{
			StrCpyN(DllPath, AppPath, MAX_PATH - 1);
			PathAppend(DllPath, EnumFile.Current().cFileName);
		}
		else
		{
			StrCpyN(DllPath, EnumFile.Current().cFileName, MAX_PATH - 1);
		}

		CDynamicLinkLibrary dll(DllPath);

		if(dll.GetModule() && dll.GetProcAddress("GetDBSourceManager"))
		{
			filePath = EnumFile.Current().cFileName;
			return true;
		}
		else
		{
			DWORD dwErr = GetLastError();
			TTRACE(TEXT("Error %u\n"), dwErr);
		}
	}

	return false;
}

bool CDBSchemaMaintainerDoc::GetDBProvider(HMODULE hModule, IDBSourceManager** ppSourceManager, IDBNameMapping** ppNameMapping)
{
	if(NULL == hModule)
	{
		_ASSERT(false);
		return false;
	}

	FunGetSourceManager fGetSM = (FunGetSourceManager)::GetProcAddress(hModule, "GetDBSourceManager");
	if(!fGetSM) return false;
	*ppSourceManager = fGetSM();
	if(!*ppSourceManager) return false;

	FunGetNameMapping fGetNM = (FunGetNameMapping)::GetProcAddress(hModule, "GetDBNameMapping");
	if(fGetNM)
	{
		*ppNameMapping = fGetNM();
	}
	if(!*ppNameMapping) *ppNameMapping = new CDBNameMappingNone();

	return *ppNameMapping && *ppSourceManager;
}

int CDBSchemaMaintainerDoc::ParseProjectInfo(LPCTSTR lpszProFile)
{
	ProjectPath_ = ProjectName_ = ProjectExt_ = lpszProFile;

	PathRemoveExtension(ProjectName_.GetBuffer());
	ProjectName_.ReleaseBuffer();
	ProjectExt_ = ProjectExt_.Right(ProjectExt_.GetLength() - ProjectName_.GetLength() - 1);

	PathRemoveFileSpec(ProjectPath_.GetBuffer());
	ProjectPath_.ReleaseBuffer();
	ProjectName_ = ProjectName_.Right(ProjectName_.GetLength() - ProjectPath_.GetLength() - 1);	

	return 1;
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
void CDBSchemaMaintainerDoc::OnOpenDB()
{
	IDBSourceManager*	pSourceManager = 0;
	IDBNameMapping*		pMapping = 0;
	IDBDataAdapter*		pAdapter = 0;
	IDBFactory*			pFactory = 0;
	
	tstring				FileDll;

	if(!DllDBProvider_.GetModule())
	{
		if(!FindDBProviderDLL(FileDll))
		{
			return;
		}
		else
		{
			DllDBProvider_ = FileDll.c_str();
		}
	}

	if(!GetDBProvider(DllDBProvider_.GetModule(), &pSourceManager, &pMapping))
	{
		return;
	}

	IDBConnection* pConn = pSourceManager->PromptSelectDBSource(0);
	if(!pConn)
	{
		return;
	}

	pSourceManager->OpenDBConnection(pConn, &pAdapter, &pFactory);
	LoadDBSchema(pAdapter, pFactory, pMapping);

	UpdateAllViews(NULL);
}

void CDBSchemaMaintainerDoc::OnOpenFile()
{
	LoadBuildInSchema(NULL);
	UpdateAllViews(NULL);
}

void CDBSchemaMaintainerDoc::OnFileSave()
{
	if(ProjectName_.IsEmpty())
	{
		TCHAR pszFilter[] = TEXT("Database Schema File(*.dbschema)|*.dbschema|All Files(*.*)|*.*|");
		CFileDialog dlg(FALSE, TEXT("dbschema"), TEXT("Project1"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, pszFilter);
		if(dlg.DoModal() != IDOK)
		{
			return;
		}
		ParseProjectInfo(dlg.GetPathName());
	}

	UpdateModuleAsBuildin();

	SaveBuildInSchemaDetail();
	SaveBuildInSchemaObjID();
	UpdateAllViews(NULL);
}

