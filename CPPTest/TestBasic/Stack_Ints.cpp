// Stack_Ints.cpp: implementation of the Stack_Ints class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Stack_Ints.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Stack_Ints_Node_List::Stack_Ints_Node_List(const Stack_Ints_Node *node) : last(&head)
{
	if(0 == node) return;

	Add(node);
}

Stack_Ints_Node_List::~Stack_Ints_Node_List()
{
	Stack_Ints_Node* temp;
	while(0 != (temp = head.pNext))
	{
		head.pNext = head.pNext->pNext;
		delete temp;
	}
}

bool Stack_Ints_Node_List::Add(const Stack_Ints_Node* node)
{
	if(0 == node) return false;

	last->pNext = node;
	
	while(0 != last->pNext)
	{
		last = last->pNext;
	}

	return true;
}

Stack_Ints_Node* Stack_Ints_Node_List::Release()
{
	Stack_Ints_Node* temp = head.pNext;
	head.pNext = 0;
	return temp;
}

Stack_Ints::Stack_Ints(const Stack_Ints& other)
{
	dispose();
	try
	{
		listHead.pNext = lstcpy();
	}
	catch (Stack_Ints_Exception* e)
	{
		listHead.pNext = 0;
	}
	catch (CException* e)
	{
		listHead.pNext = 0;
	}	
}

Stack_Ints::~Stack_Ints()
{
	dispose();
}

Stack_Ints::dispose()
{
	Lock ml(&mutex);
	Stack_Ints_Node_List temp(listHead.pNext);
	listHead.pNext = 0;
}

void Stack_Ints::push(int v) throw(Stack_Ints_Exception)
{
	Lock ml(&mutex);
	try
	{
		Stack_Ints_Node * node = new Stack_Ints_Node(listHead.pNext, v);
		listHead.pNext = node;
	}
	catch (CException* e)
	{
		throw new Stack_Ints_Exception();
	}
}

void Stack_Ints::pop()
{
	Lock ml(&mutex);

	if(0 != listHead.pNext)
	{
		listHead.pNext = listHead.pNext->pNext;
	}
}

int Stack_Ints::top() throw(Stack_Ints_Exception)
{
	Lock ml(&mutex);

	if(0  == listHead.pNext)
	{
		throw new Stack_Ints_Exception();
	}

	return listHead.pNext->iValue;
}

int Stack_Ints::isempty()
{
	return 0 == listHead.pNext;
}

Stack_Ints_Node* Stack_Ints::lstcpy() throw(Stack_Ints_Exception)
{
	Lock ml(&mutex);
	Stack_Ints_Node_List other;
	Stack_Ints_Node *temp = listHead.pNext;
	try
	{
		while(0 != temp)
		{
			other.Add(new Stack_Ints_Node(temp->iValue));
		}
		return other.Release();
	}
	catch (CException* e)
	{
		throw new Stack_Ints_Exception();
	}
	
	return 0;
}