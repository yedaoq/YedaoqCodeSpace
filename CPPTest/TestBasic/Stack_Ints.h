// Stack_Ints.h: interface for the Stack_Ints class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STACK_INTS_H__FF76F7A4_AC04_437E_970A_B38EEA9B7365__INCLUDED_)
#define AFX_STACK_INTS_H__FF76F7A4_AC04_437E_970A_B38EEA9B7365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


using namespace std;

class Stack_Ints_Exception : CException
{
	//...
};

class Stack_Ints_Node
{
public :
	Stack_Ints_Node() : pNext(0), iValue(0) {}
	Stack_Ints_Node(int v) : pNext(0), iValue(v) {}
	Stack_Ints_Node(const Stack_Ints_Node& next, int v) : pNext(next), iValue(v) {}

public :
	Stack_Ints_Node* pNext;
	int iValue;
};

class Stack_Ints_Node_List
{
public:
	Stack_Ints_Node_List() : last(&head) {}
	Stack_Ints_Node_List(const Stack_Ints_Node *node);
	~Stack_Ints_Node_List();

	bool Add(const Stack_Ints_Node* node);
	Stack_Ints_Node* Release();
	
	Stack_Ints_Node head;
	Stack_Ints_Node* last;

private:
	Stack_Ints_Node_List operator =(const Stack_Ints_Node_List&) {}
	Stack_Ints_Node(const Stack_Ints_Node_List&) {}
};

class Stack_Ints  
{
public:
	Stack_Ints() {};
	Stack_Ints(const Stack_Ints&);
	virtual ~Stack_Ints();

	void push(int v);
	void pop();

	int  top() const;
	bool isempty() const;

private:
	Stack_Ints_Node* lstcpy() const;

	void dispose();
	void listDestroy();
	

private:
	Stack_Ints_Node listHead;
	Mutex mutex;
};

#endif // !defined(AFX_STACK_INTS_H__FF76F7A4_AC04_437E_970A_B38EEA9B7365__INCLUDED_)
