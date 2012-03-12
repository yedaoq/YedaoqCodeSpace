#undef BEGINENUM
#undef ENDENUM
#undef ENUMITEM0
#undef ENUMITEM1
#undef ENUMITEM2
#undef ENUMITEM3
#undef ENUMTEMPLATENAME

#define ENUMTEMPLATENAME(name,base,flag) CEnumContainer<Enum##name,base,flag>

#define BEGINENUM(name,base,flag) void Enum##name::Initialize(){	base idx = 0;

#define ENUMITEM0(name)				Items_.Add(make_enumitem(idx++, TEXT(#name), TEXT("")));
#define ENUMITEM1(name, desc)		Items_.Add(make_enumitem(idx++, TEXT(#name), TEXT(#desc)));
#define ENUMITEM2(name, val)		idx = val; Items_.Add(make_enumitem(idx++, TEXT(#name), TEXT("")));
#define ENUMITEM3(name, val, desc)	idx = val; Items_.Add(make_enumitem(idx++, TEXT(#name), TEXT(#desc)));

#define ENDENUM						}