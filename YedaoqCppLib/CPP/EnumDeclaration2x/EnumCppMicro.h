#undef BEGINENUM
#undef ENDENUM
#undef ENUMITEM0
#undef ENUMITEM1
#undef ENUMITEM2
#undef ENUMITEM3

#define struct void
#define public :Initialize()		/##/

#define BEGINENUM					base idx = 0;

#define ENUMITEM0(name)				Items_.Add(make_enumitem(idx++, TEXT(#name), TEXT("")));
#define ENUMITEM1(name, desc)		Items_.Add(make_enumitem(idx++, TEXT(#name), TEXT("")));
#define ENUMITEM2(name, val)		idx = val; Items_.Add(make_enumitem(idx++, TEXT(#name), TEXT("")));
#define ENUMITEM3(name, val, desc)	idx = val; Items_.Add(make_enumitem(idx++, TEXT(#name), TEXT(#desc)));

#define ENDENUM

#define enum \"