#undef BEGINENUM
#undef ENDENUM
#undef ENUMITEM0
#undef ENUMITEM1
#undef ENUMITEM2
#undef ENUMITEM3
#undef ENUMTEMPLATENAME

#define ENUMTEMPLATENAME(name,base,flag) CEnumContainer<Enum##name,base,flag>

#define BEGINENUM(name,base,flag) void Enum##name::Initialize(){ items_creator_.do_nothing();	base idx = 0;

#define ENUMITEM0(name)				Items_.push_back(nsYedaoqEnum::make_enumitem(idx++, TEXT(#name), TEXT("")));
#define ENUMITEM1(name, desc)		Items_.push_back(nsYedaoqEnum::make_enumitem(idx++, TEXT(#name), TEXT(#desc)));
#define ENUMITEM2(name, val)		idx = val; Items_.push_back(nsYedaoqEnum::make_enumitem(idx++, TEXT(#name), TEXT("")));
#define ENUMITEM3(name, val, desc)	idx = val; Items_.push_back(nsYedaoqEnum::make_enumitem(idx++, TEXT(#name), TEXT(#desc)));

#define ENDENUM						}