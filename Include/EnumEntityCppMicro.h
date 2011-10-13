#undef BEGINENUM
#undef ENDENUM
#undef ENUMITEM0
#undef ENUMITEM1
#undef ENUMITEM2
#undef ENUMITEM3

#define ENUMNAME(name) Enum##name

#define BEGINENUM(name,comment) \
	const ENUMENTITY<ENUMNAME(name)>& EnumEntityOf##name(){  \
	static ENUMENTITY<ENUMNAME(name)> entity; 

#define ENDENUM return entity; }

#define ENUMITEM0(name) entity.Add(ENUMITEM(name, TEXT(#name), TEXT("")));
#define ENUMITEM1(name, desc) entity.Add(ENUMITEM(name, TEXT(#name), TEXT(""))); 
#define ENUMITEM2(name, val) entity.Add(ENUMITEM(name, TEXT(#name), TEXT(""))); 
#define ENUMITEM3(name, val, desc) entity.Add(ENUMITEM(name, TEXT(#name), TEXT(#desc)));
