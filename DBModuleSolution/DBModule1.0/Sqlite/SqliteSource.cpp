#include "SqliteSource.h"

using namespace NSDBModule;

#define PathProperty TEXT("FilePath")

using namespace NSDBModule;

CSqliteConnection::CSqliteConnection()
	: Path_(PathProperty)
{}

CSqliteConnection::CSqliteConnection(const tstring& file)
	: Path_(PathProperty, file)
{}

IDBConnection::PropertyEnumerator* CSqliteConnection::EnumPropertyName()
{
	return new_iterator_enumerator(&Path_, &Path_ + 1);
}

const tstring& CSqliteConnection::GetProperty(const tstring& name)
{
	if(name != PathProperty)
	{
		_ASSERT(false);
		throw std::exception();
	}

	return Path_.Value;
}

bool CSqliteConnection::SetProperty(const tstring& name, const tstring& val)
{
	if(name != PathProperty)
	{
		_ASSERT(false);
		throw std::exception();
	}

	Path_.Value = val;
	return true;
}

int	CSqliteConnection::Open()
{
	
}
int	CSqliteConnection::Close()
{

}

tstring	CSqliteConnection::ToString()
{
	return Path_.Value;
}

IDBConnection* CSqliteSourceManager::PromptSelectDBSource(IDBConnection* pDefault)
{	
	TCHAR pszFilter = TEXT("Sqlite Database File(*.db)|*.db|All Files(*.*)|*.*|");
	TCHAR pszFileName = NULL;
	if(pDefault)
	{
		pszFileName = pDefault->GetProperty(PathProperty);
	}

	CFileDialog dlg(TRUE, NULL, pszFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, pszFilter);

	if(dlg.DoModal() != IDOK)
	{
		return 0;
	}

	CSqliteConnection* pConn = new CSqliteConnection(dlg.GetPathName());
	return pConn;
}

IDBConnection* CSqliteSourceManager::PromptCreateDBSource(IDBConnection* pDefault)
{
	return 0;
}

IDBConnection* CSqliteSourceManager::ParseDBConnection(const tchar* pcConnStr)
{
	if(!pcConnStr)
	{
		_ASSERT(false);
		return 0;
	}

	if(0 == _access(pcConnStr))
	{
		return new CSqliteConnection(pcConnStr);
	}
	else
	{
		return 0;
	}
}