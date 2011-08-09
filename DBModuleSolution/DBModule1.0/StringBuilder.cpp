#include "StringBuilder.h"

bool CStringBuilder::Append(const tstring& str)
{

}

bool CStringBuilder::Append(LPCTSTR);
bool CStringBuilder::Append(char);

int CStringBuilder::Trim();
int CStringBuilder::Trim(int trimCharCount, ...);

int CStringBuilder::TrimLeft();
int CStringBuilder::TrimLeft(int trimCharCount, ...);

int CStringBuilder::TrimRight();
int CStringBuilder::TrimRight(int trimCharCount, ...);

int CStringBuilder::Replace(const tstring& ori, const tstring& cur);

size_t CStringBuilder::Length();
tstring CStringBuilder::ToString();
