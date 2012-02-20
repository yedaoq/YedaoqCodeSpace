// Tree.h: interface for the CTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREE_H__A1BBFD69_5A1F_4F60_94B4_AD858C524A00__INCLUDED_)
#define AFX_TREE_H__A1BBFD69_5A1F_4F60_94B4_AD858C524A00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct TreeNode;
typedef TreeNode* LPTreeNode;

struct TreeNode
{
	LPTreeNode	pParent;
	LPTreeNode	pChild;
	LPTreeNode	pBrother;
	void*		pData;
};

class CTree  
{
public:
	CTree();
	virtual ~CTree();

private:
	LPTreeNode pRoot;
};

#endif // !defined(AFX_TREE_H__A1BBFD69_5A1F_4F60_94B4_AD858C524A00__INCLUDED_)
