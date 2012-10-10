/* ___________________________________________________________________________
@ 
@ file - YedaoqCodeSpace\YedaoqCppLib\Win32\TimerEx.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-8-27
@ info -
@     
/* ___________________________________________________________________________*/

#ifndef TimerEx_h__
#define TimerEx_h__

#include <atlstdthunk.h>

class CTimerEx
{
public:
	enum { FLAG_STARTUP, FLAG_TIMERDOT = 1, FLAG_STOP = 2 };
	typedef bool (CALLBACK* callback_t)(CTimerEx* timer, int flag);

public:
	CTimerEx(UINT interval_miniseconds, UINT max_dot_times, callback_t callback = 0, void* user_data = 0, bool start_immediate = true) 
		: callback_(callback), user_data_(user_data), timer_id_(0), dot_times_(0)
	{
		if(start_immediate)
			start(interval_miniseconds, max_dot_times);

		thunk_.Init(*static_cast<DWORD_PTR>( static_cast<void*>(&CTimerEx::timer_work)), this);
	}
	
	~CTimerEx(void)
	{
		stop();
	}

	void stop()
	{
		if(timer_id_)
		{
			KillTimer(NULL, timer_id_);
			timer_id_ = 0;
			if(callback_)
				callback_(this, FLAG_STOP);
		}
	}

	void start(UINT interval_miniseconds, UINT max_dot_times)
	{
		if(timer_id_)
			stop();

		interval_miniseconds_ = interval_miniseconds;
		max_dot_times_ = max_dot_times;
		dot_times_ = 0;

		if(callback_)
		{
			if(!callback_(this, FLAG_STARTUP))
				return;
		}

		SetTimer(NULL, 0, interval_miniseconds, (TIMERPROC)thunk_.GetCodeAddress());
	}

	UINT interval_miniseconds() const { return interval_miniseconds_; }
	UINT max_dot_times() const { return max_dot_times_; }
	callback_t callback() const { return callback_; }
	void* user_data() const { return user_data_; }
	UINT_PTR timer_id() const { return timer_id_; }
	UINT dot_times() const { return dot_times_; }
	bool running() const { return 0 != timer_id_; }
	
	void set_callback(callback_t callback, void* user_data) { callback_ = callback; }
	void set_user_data(void*  user_data) { user_data_ = user_data; }
	void set_max_dot_times(UINT val) 
	{ 
		max_dot_times_ = val; 
		if(max_dot_times_ <= dot_times_)
			stop();
	}

protected:
	VOID CALLBACK timer_work(HWND, UINT, UINT_PTR, DWORD)
	{
		++dot_times_;
		
		if(!callback_(this, FLAG_TIMERDOT))
			stop();
		else if(dot_times_ >= interval_miniseconds_)
			stop();
	}

protected:
	UINT				interval_miniseconds_;
	UINT				max_dot_times_;
	callback_t			callback_;
	void*				user_data_;
	UINT_PTR			timer_id_;	
	UINT				dot_times_;
	ATL::CStdCallThunk	thunk_;
};

#endif // TimerEx_h__
