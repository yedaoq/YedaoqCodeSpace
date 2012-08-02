/* ___________________________________________________________________________
@ 
@ file - e:\YunleDev\YunTimer\YunTimer\WindowExitUtil.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-7-19
@ info -
@     
/* ___________________________________________________________________________*/

#ifndef WindowExitUtil_h__
#define WindowExitUtil_h__

#include <boost\static_assert.hpp>

class WindowExitUtil
{
public:
	// the follow definition used for set parameter of process shutdown
	enum EnumAppExitPriority : unsigned char { Last, AfterNormal, Normal, BeforeNormal, First };	
	enum EnumAppExitFlag : unsigned short { None, Noretry = SHUTDOWN_NORETRY };

	struct AppExitParam
	{
		union
		{
			struct
			{
				EnumAppExitPriority Priority;
				unsigned char		Order;		// the litter, the later app shutdown
			};
			unsigned short			nonamed;
		};

		EnumAppExitFlag				Flag;

		AppExitParam(EnumAppExitPriority priority = Normal, unsigned char order = 0x80, EnumAppExitFlag flag = None)
			: Priority(priority), Order(order), Flag(flag)
		{}

		AppExitParam(EnumAppExitFlag flag)
			: Priority(Normal), Order(0x80), Flag(flag)
		{}

	protected:
		AppExitParam(DWORD level, DWORD flag)
			: nonamed(WORD(level), Flag(WORD(flag))
		{}

		friend class WindowExitUtil;
	};

	// the follow definition used for set parameter of system shutdown
	enum EnumSysExitAction : unsigned int { Logoff = EWX_LOGOFF, Shutdown = EWX_SHUTDOWN, Reboot = EWX_REBOOT, Poweroff = EWX_POWEROFF, RestartApp = EWX_RESTARTAPPS };
	enum EnumSysExitFlag : unsigned int { Force = EWX_FORCE, ForceIfHung = EWX_FORCEIFHUNG };
	
	enum EnumSysExitIssuer : unsigned char { Other, Hardware, OS, Software, Application, SystemFail, PowerFail, LegacyAPI };
	
	enum EnumSysExitReason : unsigned short 
	{ 
		OtherReason, Maintenance, Installation, Upgrade, Reconfig, Hung, Unstable, Disk, Processor, NetworkCard, 
		PowerSupply, CordUnplugged, Environment, HardwareDriver, OtherDriver, BlueScreen, ServicePack, Hotfix,
		Securityfix, Security, NetworkConnectivity, WMI, ServicePackUninstall, HotfixUninstall, SecurityfixUninstall, 
		MMC, SystemRestore, Termsrv, Promotion, Demotion	
	};

	BOOST_STATIC_ASSERT(SHTDN_REASON_MINOR_PROCESSOR == EnumSysExitReason::Processor 
		&& SHTDN_REASON_MINOR_HOTFIX == EnumSysExitReason::Hotfix
		&& SHTDN_REASON_MINOR_SECURITYFIX_UNINSTALL == EnumSysExitReason::SecurityfixUninstall
		&& SHTDN_REASON_MINOR_DC_DEMOTION == EnumSysExitReason::Demotion);

	enum EnumSysExitReasonType : unsigned char { PreDefine, UserDefine = 4, Planned = 8 };

	struct SysExitRecord
	{
		union
		{
			struct  
			{
				EnumSysExitReasonType	ReasonType;
				union{
					EnumSysExitIssuer	Issuer;
					unsigned char		CustomIssuer;
				};
				union
				{
					EnumSysExitReason	Reason;
					unsigned short		CustomIssuer;
				};
				
			};
			DWORD nonamed;
		};

		SysExitRecord(EnumSysExitReasonType reason_type = PreDefine, EnumSysExitIssuer issuer = Application, EnumSysExitReason reason = OtherReason)
			: ReasonType(reason_type), Issuer(issuer), Reason(reason)
		{}

		SysExitRecord(unsigned char issuer, unsigned short reason)
			: ReasonType(UserDefine), Issuer(issuer), Reason(reason)
		{}
	};
 };

public:

	static bool GetPrivilege4Shutdown()
	{
		HANDLE hToken; 
		TOKEN_PRIVILEGES tkp; 
		bool ret = false;

		do 
		{
			// Get a token for this process. 
			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) break;

			// Get the LUID for the shutdown privilege. 
			if(!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) break; 

			tkp.PrivilegeCount = 1;  // one privilege to set    
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

			// Get the shutdown privilege for this process. 
			if(!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0)) break; 

			ret = true;

		} while (false);
		
		return ret;
	}

	static AppExitParam GetCurrentProcessShutdownParam()
	{
		DWORD level = 0x280, flag = 0;
		::GetProcessShutdownParameters(&level, &flag);
		return AppExitParam(level, flag);
	}

	static bool SetCurrentProcessShutdownParam(AppExitParam param)
	{
		return 0 != ::SetProcessShutdownParameters(param.nonamed, param.Flag);
	}

	static int SetCurrentProcessShutdownParam(EnumAppExitPriority priority, unsigned char order_range_head, unsigned char order_range_tail, EnumAppExitFlag flag = None)
	{
		unsigned int head = order_range_head + ((unsigned int)priority << 8);
		unsigned int tail = order_range_tail + ((unsigned int)priority << 8);
		int step = (head > tail) ? -1 : 1;
		tail += step;
		for(unsigned int i = head; i != tail; i += step)
		{
			if(::SetProcessShutdownParameters(i, flag)) return i - (priority << 8);
		}

		return -1;
	}

	static bool ExitWindow(EnumSysExitAction action, EnumSysExitFlag flag, SysExitRecord record)
	{
		return 0 != ::ExitWindowsEx(action | flag, record.nonamed);
	}
};

#endif // WindowExitUtil_h__
