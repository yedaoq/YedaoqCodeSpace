#pragma once

#include "detours.h"
#include <list>
#include "boost\assert.hpp"
#include "boost\static_assert.hpp"

#ifndef interface
#define interface struct
#endif

#ifndef SINGLELOOP
#define SINGLELOOP for(bool __anonym = true;  __anonym; __anonym = false)
#endif

#ifndef STDENUM
#define STDENUM(containertype, container, iter) for ( containertype::iterator iter = container.begin(); iter != container.end(); ++iter )
#endif

#ifdef _X86_
	#pragma comment(lib, "detours_X86.lib")
#endif
#ifdef _X64_
	#pragma comment(lib, "detours_X64.lib")
#endif
#ifdef _IA64_
	#pragma comment(lib, "detours_IA64.lib")
#endif
#ifdef _ARM_
	#pragma comment(lib, "detours_ARM.lib")
#endif

namespace nsYedaoqDetourPlus
{

interface IDetourHookItem
{
	typedef void* pvoid_t;

	virtual ~IDetourHookItem() = 0 {}

	virtual bool hook(bool transaction) = 0;
	virtual bool unhook(bool transaction) = 0;

	virtual pvoid_t& primitive_void() = 0;
	virtual pvoid_t& substitute_void() = 0;

	virtual bool hooking() const = 0;
	virtual void hooking(bool val) = 0;
};

class CDetourHookGroup
{
	typedef std::list<IDetourHookItem*>	DetourHookItemList;

public:
	bool hook()
	{
		bool bSuccessed = false;
		SINGLELOOP
		{
			if(NO_ERROR != DetourUpdateThread(GetCurrentThread())) break;
			if(NO_ERROR != DetourTransactionBegin()) break;	

			STDENUM(DetourHookItemList, DetourHookItems_, iter)
			{
				if(NO_ERROR != DetourAttach(&(*iter)->primitive_void(),(*iter)->substitute_void())) break;
			}

			if(NO_ERROR != DetourTransactionCommit()) break;
			hooking_ = bSuccessed = true;
		}

		if(!bSuccessed)
		{
			DetourTransactionAbort();
		}
		else
		{
			STDENUM(DetourHookItemList, DetourHookItems_, iter)
			{
				(*iter)->hooking(true);
			}
		}

		return bSuccessed;
	}

	bool unhook()
	{
		bool bSuccessed = false;
		SINGLELOOP
		{
			if(NO_ERROR != DetourUpdateThread(GetCurrentThread())) break;
			if(NO_ERROR != DetourTransactionBegin()) break;

			STDENUM(DetourHookItemList, DetourHookItems_, iter)
			{
				if(NO_ERROR != DetourDetach(&(*iter)->primitive_void(),(*iter)->substitute_void())) break;
			}

			if(NO_ERROR != DetourTransactionCommit()) break;
			bSuccessed = true;
			hooking_ = false;
		}

		if(!bSuccessed)
		{
			DetourTransactionAbort();
		}
		else
		{
			STDENUM(DetourHookItemList, DetourHookItems_, iter)
			{
				(*iter)->hooking(false);
			}
		}

		return bSuccessed;
	}

	bool Register(IDetourHookItem* item)
	{
		DetourHookItems_.push_back(item);
		return true;
	}

	bool Unregister(IDetourHookItem* item)
	{
		STDENUM(DetourHookItemList, DetourHookItems_, iter)
		{
			if(item == *iter)
			{
				DetourHookItems_.erase(iter);
				return true;
			}
		}
		return false;
	}

	bool hooking() const
	{
		BOOST_ASSERT(!DetourHookItems_.empty());
		return (*DetourHookItems_.begin())->hooking();
	}

	int count() const { return DetourHookItems_.size(); }

protected:
	DetourHookItemList	DetourHookItems_;
	bool				hooking_;
};

struct DetourHookItemBase : public IDetourHookItem
{
	DetourHookItemBase(pvoid_t api, pvoid_t substitute, CDetourHookGroup* group)
		: hooking_(false), substitute_(substitute), primitive_(api)
	{
		if(group) group->Register(this);
	}

	virtual ~DetourHookItemBase() { BOOST_ASSERT(!hooking()); }

	virtual bool hook(bool transaction)
	{
		bool bSuccessed = false;
		SINGLELOOP
		{
			if(transaction && NO_ERROR != DetourUpdateThread(GetCurrentThread())) break;
			if(transaction && NO_ERROR != DetourTransactionBegin()) break;
			if(NO_ERROR != DetourAttach(&primitive_void(), substitute_void())) break;
			if(transaction && NO_ERROR != DetourTransactionCommit()) break;
			hooking_ = bSuccessed = true;
		}

		if(!bSuccessed && transaction) DetourTransactionAbort();
		return bSuccessed;
	}

	virtual bool unhook(bool transaction)
	{
		bool bSuccessed = false;
		SINGLELOOP
		{
			if(transaction && NO_ERROR != DetourUpdateThread(GetCurrentThread())) break;
			if(transaction && NO_ERROR != DetourTransactionBegin()) break;
			if(NO_ERROR != DetourDetach(&primitive_void(), substitute_void())) break;
			if(transaction && NO_ERROR != DetourTransactionCommit()) break;
			bSuccessed = true;
			hooking_ = false;
		}

		if(!bSuccessed && transaction) DetourTransactionAbort();
		return bSuccessed;
	}

	virtual pvoid_t&	primitive_void() { return primitive_; }
	virtual pvoid_t&	substitute_void() { return substitute_; }
	virtual bool		hooking() const { return hooking_; }
	virtual void		hooking(bool val) { hooking_ = val; }

protected:
	bool	hooking_;
	pvoid_t	substitute_;
	pvoid_t	primitive_;
};

template<typename proc_t>
struct DetourHookItem : public DetourHookItemBase
{
	BOOST_STATIC_ASSERT(sizeof(proc_t) == sizeof(pvoid_t));

	DetourHookItem(proc_t api, proc_t substitute, CDetourHookGroup* group)
		: DetourHookItemBase(api, substitute, group)
	{}

	proc_t&	primitive() { return reinterpret_cast<proc_t&>(primitive_); }
	proc_t&	substitute() { return reinterpret_cast<proc_t&>(substitute_); }
	proc_t const& primitive() const { return reinterpret_cast<proc_t const&>(primitive_); }
	proc_t const& substitute() const { return reinterpret_cast<proc_t const&>(substitute_); }
};

template<typename proc_t>
DetourHookItem<proc_t> make_detourhookitem(proc_t api, proc_t substitute)
{
	return DetourHookItem<proc_t>(api, substitute);
}

struct scoped_grouphook
{
	scoped_grouphook(CDetourHookGroup& group)
		: group_(&group)
	{
		group_->hook();
	}

	~scoped_grouphook() { group_->unhook(); }

protected:
	CDetourHookGroup* group_;
};

}
