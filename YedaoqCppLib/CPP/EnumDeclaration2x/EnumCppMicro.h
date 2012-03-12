#undef BEGINENUM
#undef ENDENUM
#undef ENUMITEM0
#undef ENUMITEM1
#undef ENUMITEM2
#undef ENUMITEM3
#undef ENUMTEMPLATENAME

#define ENUMTEMPLATENAME(name,base,flag) CEnumContainer<Enum##name,base,flag>

#define BEGINENUM(name,base,flag) Enum##name::constructor Enum##name::constructor_(Enum##name::Items_); Enum##name::constructor::constructor(ENUMTEMPLATENAME(name,base,flag)::collection& items){	base idx = 0;

#define ENUMITEM0(name)				items.Add(make_enumitem(idx++, TEXT(#name), TEXT("")));
#define ENUMITEM1(name, desc)		items.Add(make_enumitem(idx++, TEXT(#name), TEXT(#desc)));
#define ENUMITEM2(name, val)		idx = val; items.Add(make_enumitem(idx++, TEXT(#name), TEXT("")));
#define ENUMITEM3(name, val, desc)	idx = val; items.Add(make_enumitem(idx++, TEXT(#name), TEXT(#desc)));

#define ENDENUM						}