#ifndef BEGINENUM	
#define BEGINENUM enum _Type : base {	
#endif

#ifndef ENDENUM
#define ENDENUM };									\
	static void Initialize(); operator base() const { return Value_; } bool operator==(base val) const { return Value_ == val;} void operator=(base val) { Value_ = val; }
#endif

#ifndef ENUMITEM0
#define ENUMITEM0(name) name,
#endif

#ifndef ENUMITEM1
#define ENUMITEM1(name, desc) name, 
#endif

#ifndef ENUMITEM2
#define ENUMITEM2(name, val) name = val, 
#endif

#ifndef ENUMITEM3
#define ENUMITEM3(name, val, desc) name = val,
#endif

#define endenum ;operator base() const { return Value_; } bool operator==(base val) const { return Value_ == val;} void operator=(base val) { Value_ = val; }