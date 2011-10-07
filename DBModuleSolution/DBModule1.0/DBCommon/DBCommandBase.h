#pragma once
#include "DBCommand.h"

namespace NSDBModule
{
	class CDBCommandBase : public IDBCommand
	{
	public:
		CDBCommandBase()
			: Type_(EnumDBCommandType::Sql)
		{}

		CDBCommandBase(const tstring& sql)
			: Text_(sql), Type_(EnumDBCommandType::Sql)
		{}

		CDBCommandBase(const tstring& text, EnumDBCommandType type)
			: Text_(text), Type_(type)
		{}

		virtual const tstring& Text() const { return Text_; }
		virtual const EnumDBCommandType Type() const { return Type_; }
		virtual void SetText(const tstring& text){ Text_ = text; }
		virtual void SetType(EnumDBCommandType type){ Type_ = type; }

		virtual ~CDBCommandBase(){}

	protected:
		tstring				Text_;
		EnumDBCommandType	Type_;
	};
}
