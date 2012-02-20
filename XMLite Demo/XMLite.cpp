// XMLite.cpp: implementation of the XMLite class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "XMLite.h"
#include <boost\algorithm\string\trim.hpp>
#include "boost\format.hpp"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

static const tchar chXMLTagOpen		= '<';
static const tchar chXMLTagClose	= '>';
static const tchar chXMLTagPre	= '/';
static const tchar chXMLEscape = '\\';	// for value field escape

static const tchar szXMLPIOpen[] = TEXT("<?");
static const tchar szXMLPIClose[] = TEXT("?>");
static const tchar szXMLCommentOpen[] = TEXT("<!--");
static const tchar szXMLCommentClose[] = TEXT("-->");
static const tchar szXMLCDATAOpen[] = TEXT("<![CDATA[");
static const tchar szXMLCDATAClose[] = TEXT("]]>");

static const XENTITY x_EntityTable[] = {
		{ '&', TEXT("&amp;"), 5 } ,
		{ '\"', TEXT("&quot;"), 6 } ,
		{ '\'', TEXT("&apos;"), 6 } ,
		{ '<', TEXT("&lt;"), 4 } ,
		{ '>', TEXT("&gt;"), 4 } 
	};

PARSEINFO piDefault;
DISP_OPT optDefault;
XENTITYS entityDefault((LPXENTITY)x_EntityTable, sizeof(x_EntityTable)/sizeof(x_EntityTable[0]) );
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//========================================================
// Name   : _tcschrs
// Desc   : same with _tcspbrk 
// Param  :
// Return :
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
tchar* _tcschrs( const tchar* psz, const tchar* pszchs )
{
	while( psz && *psz )
	{
		if( _tcschr( pszchs, *psz ) )
			return (tchar*)psz;
		psz++;
	}
	return NULL;
}

//========================================================
// Name   : _tcsskip
// Desc   : skip space
// Param  : 
// Return : skiped string
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
tchar* _tcsskip( const tchar* psz )
{
	//while( psz && *psz == ' ' && *psz == 13 && *psz == 10 ) psz++;
	while( psz && isspace(*psz) ) psz++;
		
	return (tchar*)psz;
}

//========================================================
// Name   : _tcsechr
// Desc   : similar with _tcschr with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
tchar* _tcsechr( const tchar* psz, int ch, int escape )
{
	tchar* pch = (tchar*)psz;

	while( pch && *pch )
	{
		if( escape != 0 && *pch == escape )
			pch++;
		else
		if( *pch == ch ) 
			return (tchar*)pch;
		pch++;
	}
	return pch;
}

//========================================================
// Name   : _tcselen
// Desc   : similar with _tcslen with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
int _tcselen( int escape, tchar* srt, tchar* end = NULL ) 
{
	int len = 0;
	tchar* pch = srt;
	if( end==NULL ) end = (tchar*)sizeof(long);
	tchar* prev_escape = NULL;
	while( pch && *pch && pch<end )
	{
		if( escape != 0 && *pch == escape && prev_escape == NULL )
			prev_escape = pch;
		else
		{
			prev_escape = NULL;
			len++;
		}
		pch++;
	}
	return len;
}

//========================================================
// Name   : _tcsecpy
// Desc   : similar with _tcscpy with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
void _tcsecpy( tchar* psz, int escape, tchar* src, tchar* end = NULL )
{
	tchar* pch = src;
	if( end==NULL ) end = (tchar*)sizeof(long);
	tchar* prev_escape = NULL;
	while( pch && *pch && pch<end )
	{
		if( escape != 0 && *pch == escape && prev_escape == NULL )
			prev_escape = pch;
		else
		{
			prev_escape = NULL;
			*psz++ = *pch;
		}

		pch++;
	}

	*psz = '\0';
}

//========================================================
// Name   : _tcsecpy
// Desc   : similar with _tcscpy with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
void _tcsecpy( tstring& psz, int escape, const tchar* src, tchar* end = NULL )
{
	const tchar* pch = src;
	if( end==NULL ) end = (tchar*)sizeof(long);
	const tchar* prev_escape = NULL;
	while( pch && *pch && pch<end )
	{
		if( escape != 0 && *pch == escape && prev_escape == NULL )
			prev_escape = pch;
		else
		{
			prev_escape = NULL;
			psz.append(1, *pch);
		}

		pch++;
	}

	psz.append(1, '\0');
}

//========================================================
// Name   : _tcsepbrk
// Desc   : similar with _tcspbrk with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
tchar* _tcsepbrk( const tchar* psz, const tchar* chset, int escape )
{
	tchar* pch = (tchar*)psz;
	tchar* prev_escape = NULL;
	while( pch && *pch )
	{
		if( escape != 0 && *pch == escape && prev_escape == NULL )
			prev_escape = pch;
		else
		{
			prev_escape = NULL;
			if( _tcschr( chset, *pch ) )
				return (tchar*)pch;		
		}
		pch++;
	}
	return pch;
}

//========================================================
// Name   : _tcsenicmp
// Desc   : similar with _tcsnicmp with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
int _tcsenicmp( const tchar* psz, const tchar* str, int len, int escape )
{
	tchar* pch = (tchar*)psz;
	tchar* prev_escape = NULL;
	tchar* des = (tchar*)str;
	int i = 0;
	
	while( pch && *pch && i < len )
	{
		if( escape != 0 && *pch == escape && prev_escape == NULL )
			prev_escape = pch;
		else
		{
			prev_escape = NULL;
			if( tolower(*pch) != tolower(des[i]) )
				break;
			i++;
		}
		pch ++;
	}
	
	// find
	if( i == len )
		return 0;
	if( psz[i] > des[i] )
		return 1;
	return -1;
}

//========================================================
// Name   : _tcsenistr
// Desc   : similar with _tcsistr with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
tchar* _tcsenistr( const tchar* psz, const tchar* str, int len, int escape )
{
	tchar* pch = (tchar*)psz;
	tchar* prev_escape = NULL;
	tchar* des = (tchar*)str;
	int i = 0;
	
	while( pch && *pch )
	{
		if( escape != 0 && *pch == escape && prev_escape == NULL )
			prev_escape = pch;
		else
		{
			prev_escape = NULL;
			if( _tcsenicmp( pch, str, len, escape ) == 0 )
				return (tchar*)pch;
		}
		pch++;
	}
	return pch;
}

//========================================================
// Name   : _tcseistr
// Desc   : similar with _tcsistr with escape process
// Param  : escape - will be escape character
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
tchar* _tcseistr( const tchar* psz, const tchar* str, int escape )
{
	int len = _tcslen( str );
	return _tcsenistr( psz, str, len, escape );
}

//========================================================
// Name   : _SetString
// Desc   : put string of (psz~end) on ps string
// Param  : trim - will be trim?
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
void _SetString( tchar* psz, tchar* end, tstring& ps, bool trim = false, int escape = 0 )
{
	//trim
	if( trim )
	{
		while( psz && psz < end && _istspace(*psz) ) psz++;
		while( (end-1) && psz < (end-1) && _istspace(*(end-1)) ) end--;
	}
	int len = end - psz;
	if( len <= 0 ) return;
	if( escape )
	{
		_tcsecpy( ps, escape, psz, end );
	}
	else
	{
		ps.assign(psz);
		if(ps.length() > len)
		{
			ps = ps.substr(tstring::size_type(0), len);
		}
	}
}

_tagXMLNode::~_tagXMLNode()
{
	Close();
}

void _tagXMLNode::Close()
{
	for( int i = 0 ; i < childs.size(); i ++)
	{
		LPXNode p = childs[i];
		if( p )
		{
			delete p; childs[i] = NULL;
		}
	}
	if(!childs.empty())	childs.clear();
	
	for(int i = 0 ; i < attrs.size(); i ++)
	{
		LPXAttr p = attrs[i];
		if( p )
		{
			delete p; attrs[i] = NULL;
		}
	}
	
	if(!attrs.empty()) attrs.clear();
}
	
// attr1="value1" attr2='value2' attr3=value3 />
//                                            ^- return pointer
//========================================================
// Name   : LoadAttributes
// Desc   : loading attribute plain xml text
// Param  : pszAttrs - xml of attributes
//          pi = parser information
// Return : advanced string pointer. (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
tchar* _tagXMLNode::LoadAttributes( const tchar* pszAttrs , LPPARSEINFO pi /*= &piDefault*/)
{
	tchar* xml = (tchar*)pszAttrs;

	while( xml && *xml )
	{
		if( xml = _tcsskip( xml ) )
		{
			// close tag
			if( *xml == chXMLTagClose || *xml == chXMLTagPre )
				// wel-formed tag
				return xml;

			// XML Attr Name
			tchar* pEnd = _tcspbrk( xml, TEXT(" =") );
			if( pEnd == NULL ) 
			{
				// error
				if( pi->erorr_occur == false ) 
				{
					pi->erorr_occur = true;
					pi->error_pointer = xml;
					pi->error_code = PIE_ATTR_NO_VALUE;
					pi->error_string = (tboostformat(TEXT("<%s> attribute has error "))%name).str();
					//pi->error_string.append()
				}
				return NULL;
			}
			
			LPXAttr attr = new XAttr;
			attr->parent = this;

			// XML Attr Name
			_SetString( xml, pEnd, attr->name );
			
			// add new attribute
			attrs.push_back( attr );
			xml = pEnd;
			
			// XML Attr Value
			if( xml = _tcsskip( xml ) )
			{
				//if( xml = _tcschr( xml, '=' ) )
				if( *xml == '=' )
				{
					if( xml = _tcsskip( ++xml ) )
					{
						// if " or '
						// or none quote
						int quote = *xml;
						if( quote == '"' || quote == '\'' )
							pEnd = _tcsechr( ++xml, quote, chXMLEscape );
						else
						{
							//attr= value> 
							// none quote mode
							//pEnd = _tcsechr( xml, ' ', '\\' );
							pEnd = _tcsepbrk( xml, TEXT(" >"), chXMLEscape );
						}

						bool trim = pi->trim_value;
						tchar escape = pi->escape_value;
						//_SetString( xml, pEnd, &attr->value, trim, chXMLEscape );	
						_SetString( xml, pEnd, attr->value, trim, escape );
						xml = pEnd;
						// ATTRVALUE 
						if( pi->entity_value && pi->entitys )
							attr->value = pi->entitys->Ref2Entity(attr->value.c_str() );

						if( quote == '"' || quote == '\'' )
							xml++;
					}
				}
			}
		}
	}

	// not wel-formed tag
	return NULL;
}

// attr1="value1" attr2='value2' attr3=value3 />
//                                            ^- return pointer
//========================================================
// Name   : LoadAttributes
// Desc   : loading attribute plain xml text
// Param  : pszAttrs - xml of attributes
//          pszEnd - last string
//          pi = parser information
// Return : advanced string pointer. (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2004-06-14
//========================================================
tchar* _tagXMLNode::LoadAttributes( const tchar* pszAttrs, const tchar* pszEnd, LPPARSEINFO pi /*= &piDefault*/ )
{
	tchar* xml = (tchar*)pszAttrs;

	while( xml && *xml )
	{
		if( xml = _tcsskip( xml ) )
		{
			// close tag
			if( xml >= pszEnd )
				// wel-formed tag
				return xml;

			// XML Attr Name
			tchar* pEnd = _tcspbrk( xml, TEXT(" =") );
			if( pEnd == NULL ) 
			{
				// error
				if( pi->erorr_occur == false ) 
				{
					pi->erorr_occur = true;
					pi->error_pointer = xml;
					pi->error_code = PIE_ATTR_NO_VALUE;
					pi->error_string = (tboostformat(TEXT("<%s> attribute has error "))%name).str();
					//pi->error_string.Format( TEXT("<%s> attribute has error "), name );
				}
				return NULL;
			}
			
			LPXAttr attr = new XAttr;
			attr->parent = this;

			// XML Attr Name
			_SetString( xml, pEnd, attr->name );
			
			// add new attribute
			attrs.push_back( attr );
			xml = pEnd;
			
			// XML Attr Value
			if( xml = _tcsskip( xml ) )
			{
				//if( xml = _tcschr( xml, '=' ) )
				if( *xml == '=' )
				{
					if( xml = _tcsskip( ++xml ) )
					{
						// if " or '
						// or none quote
						int quote = *xml;
						if( quote == '"' || quote == '\'' )
							pEnd = _tcsechr( ++xml, quote, chXMLEscape );
						else
						{
							//attr= value> 
							// none quote mode
							//pEnd = _tcsechr( xml, ' ', '\\' );
							pEnd = _tcsepbrk( xml, TEXT(" >"), chXMLEscape );
						}

						bool trim = pi->trim_value;
						tchar escape = pi->escape_value;
						//_SetString( xml, pEnd, &attr->value, trim, chXMLEscape );	
						_SetString( xml, pEnd, attr->value, trim, escape );
						xml = pEnd;
						// ATTRVALUE 
						if( pi->entity_value && pi->entitys )
							attr->value = pi->entitys->Ref2Entity(attr->value.c_str() );

						if( quote == '"' || quote == '\'' )
							xml++;
					}
				}
			}
		}
	}

	// not wel-formed tag
	return NULL;
}

// <?xml version="1.0"?>
//                      ^- return pointer
//========================================================
// Name   : LoadProcessingInstrunction
// Desc   : loading processing instruction
// Param  : pszXml - PI string
//          pi - parser information
// Return : advanced string pointer. (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2004-06-14
//========================================================
tchar* _tagXMLNode::LoadProcessingInstrunction( const tchar* pszXml, LPPARSEINFO pi /*= &piDefault*/ )
{
	// find the end of pi
	tchar* end = _tcsenistr( pszXml, szXMLPIClose, sizeof(szXMLPIClose)-1, pi ? pi->escape_value : 0 );
	if( end == NULL )
		return NULL;

	// process pi
	if( doc )
	{
		tchar* xml = (tchar*)pszXml;

		LPXNode node = new XNode;
		node->parent = this;
		node->doc = doc;
		node->type = XNODE_PI;
		
		xml += sizeof(szXMLPIOpen)-1;
		tchar* pTagEnd = _tcspbrk( xml, TEXT(" ?>") );
		_SetString( xml, pTagEnd, node->name );
		xml = pTagEnd;
		
		node->LoadAttributes( xml, end, pi );

		doc->childs.push_back( node );
	}

	end += sizeof(szXMLPIClose)-1;
	return end;
}

// <!-- comment -->
//                 ^- return pointer
//========================================================
// Name   : LoadComment
// Desc   : loading comment
// Param  : pszXml - comment string
//          pi - parser information
// Return : advanced string pointer. (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2004-06-14
//========================================================
tchar* _tagXMLNode::LoadComment( const tchar* pszXml, LPPARSEINFO pi /*= &piDefault*/ )
{
	// find the end of comment
	tchar* end = _tcsenistr( pszXml, szXMLCommentClose, sizeof(szXMLCommentClose)-1, pi ? pi->escape_value : 0 );
	if( end == NULL )
		return NULL;

	// process comment
	LPXNode par = parent;
	if( parent == NULL && doc )
		par = (LPXNode)&doc;
	if( par )
	{
		tchar* xml = (tchar*)pszXml;
		xml += sizeof(szXMLCommentOpen)-1;
		
		LPXNode node = new XNode;
		node->parent = this;
		node->doc = doc;
		node->type = XNODE_COMMENT;
		node->name = TEXT("#COMMENT");
		_SetString( xml, end, node->value, false );

		par->childs.push_back( node );
	}

	end += sizeof(szXMLCommentClose)-1;
	return end;
}

// <![CDATA[ cdata ]]>
//                    ^- return pointer
//========================================================
// Name   : LoadCDATA
// Desc   : loading CDATA
// Param  : pszXml - CDATA string
//          pi - parser information
// Return : advanced string pointer. (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2004-06-14
//========================================================
tchar* _tagXMLNode::LoadCDATA( const tchar* pszXml, LPPARSEINFO pi /*= &piDefault*/ )
{
	// find the end of CDATA
	tchar* end = _tcsenistr( pszXml, szXMLCDATAClose, sizeof(szXMLCDATAClose)-1, pi ? pi->escape_value : 0 );
	if( end == NULL )
		return NULL;

	// process CDATA
	LPXNode par = parent;
	if( parent == NULL && doc )
		par = (LPXNode)&doc;
	if( par )
	{
		tchar* xml = (tchar*)pszXml;
		xml += sizeof(szXMLCDATAOpen)-1;
		
		LPXNode node = new XNode;
		node->parent = this;
		node->doc = doc;
		node->type = XNODE_CDATA;
		node->name = TEXT("#CDATA");
		_SetString( xml, end, node->value, false );

		par->childs.push_back( node );
	}

	end += sizeof(szXMLCDATAClose)-1;
	return end;
}

//========================================================
// Name   : LoadOtherNodes
// Desc   : internal function for loading PI/CDATA/Comment
// Param  : node - current xml node
//          pbRet - error occur
//          pszXml - CDATA string
//          pi - parser information
// Return : advanced string pointer. (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2004-06-14
//========================================================
tchar* LoadOtherNodes( LPXNode node, bool* pbRet, const tchar* pszXml, LPPARSEINFO pi /*= &piDefault*/ )
{
	tchar* xml = (tchar*)pszXml;
	bool do_other_type = true;
	*pbRet = false;

	while( xml && do_other_type )
	{
		do_other_type = false;

		xml = _tcsskip( xml );
		tchar* prev = xml;
		// is PI( Processing Instruction ) Node?
		if( _tcsnicmp( xml, szXMLPIOpen, sizeof(szXMLPIOpen)-1 ) == 0 )
		{
			// processing instrunction parse
			// return pointer is next node of pi
			xml = node->LoadProcessingInstrunction( xml, pi );
			//if( xml == NULL )
			//	return NULL;
			// restart xml parse
		}

		if( xml != prev )
			do_other_type = true;
		xml = _tcsskip( xml );
		prev = xml;

		// is comment Node?
		if( _tcsnicmp( xml, szXMLCommentOpen, sizeof(szXMLCommentOpen)-1 ) == 0 )
		{
			// processing comment parse
			// return pointer is next node of comment
			xml = node->LoadComment( xml, pi );
			// comment node is terminal node
			if( node->parent && node->parent->type != XNODE_DOC 
				&& xml != prev )
			{
				*pbRet = true;
				return xml;
			}
			// restart xml parse when this node is root doc node
		}

		if( xml != prev )
			do_other_type = true;

		xml = _tcsskip( xml );
		prev = xml;
		// is CDATA Node?
		if( _tcsnicmp( xml, szXMLCDATAOpen, sizeof(szXMLCDATAOpen)-1 ) == 0 )
		{
			// processing CDATA parse
			// return pointer is next node of CDATA
			xml = node->LoadCDATA( xml, pi );
			// CDATA node is terminal node
			if( node->parent && node->parent->type != XNODE_DOC 
				&& xml != prev )
			{
				*pbRet = true;
				return xml;
			}
			// restart xml parse when this node is root doc node
		}

		if( xml != prev )
			do_other_type = true;
	}

	return xml;
}

// <TAG attr1="value1" attr2='value2' attr3=value3 >
// </TAG>
// or
// <TAG />
//        ^- return pointer
//========================================================
// Name   : Load
// Desc   : load xml plain text
// Param  : pszXml - plain xml text
//          pi = parser information
// Return : advanced string pointer  (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
tchar* _tagXMLNode::Load( const tchar* pszXml, LPPARSEINFO pi /*= &piDefault*/ )
{
	// Close it
	Close();

	tchar* xml = (tchar*)pszXml;

	xml = _tcschr( xml, chXMLTagOpen );
	if( xml == NULL )
		return NULL;

	// Close Tag
	if( *(xml+1) == chXMLTagPre ) // </Close
		return xml;

	// Load Other Node before <Tag>(pi, comment, CDATA etc)
	bool bRet = false;
	tchar* ret = NULL;
	ret = LoadOtherNodes( this, &bRet, xml, pi );
	if( ret != NULL ) 
		xml = ret;
	if( bRet ) 
		return xml;

	// XML Node Tag Name Open
	xml++;
	tchar* pTagEnd = _tcspbrk( xml, TEXT(" />\t\r\n") );
	_SetString( xml, pTagEnd, name );
	xml = pTagEnd;
	// Generate XML Attributte List
	if( xml = LoadAttributes( xml, pi ) )
	{
		// alone tag <TAG ... />
		if( *xml == chXMLTagPre )
		{
			xml++;
			if( *xml == chXMLTagClose )
				// wel-formed tag
				return ++xml;
			else
			{
				// error: <TAG ... / >
				if( pi->erorr_occur == false ) 
				{
					pi->erorr_occur = true;
					pi->error_pointer = xml;
					pi->error_code = PIE_ALONE_NOT_CLOSED;
					pi->error_string = TEXT("Element must be closed.");
				}
				// not wel-formed tag
				return NULL;
			}
		}
		else
		// open/close tag <TAG ..> ... </TAG>
		//                        ^- current pointer
		{
			// if text value is not exist, then assign value
			//if( this->value.IsEmpty() || this->value == TEXT("") )
			if(XIsEmptyString( value.c_str()))
			{
				// Text Value 
				tchar* pEnd = _tcsechr( ++xml, chXMLTagOpen, chXMLEscape );
				if( pEnd == NULL ) 
				{
					if( pi->erorr_occur == false ) 
					{
						pi->erorr_occur = true;
						pi->error_pointer = xml;
						pi->error_code = PIE_NOT_CLOSED;
						pi->error_string = (tboostformat(TEXT("%s must be closed with </%s>"))%name%name).str();
					}
					// error cos not exist CloseTag </TAG>
					return NULL;
				}
				
				bool trim = pi->trim_value;
				tchar escape = pi->escape_value;
				//_SetString( xml, pEnd, &value, trim, chXMLEscape );
				_SetString( xml, pEnd, value, trim, escape );

				xml = pEnd;
				// TEXTVALUE reference
				if( pi->entity_value && pi->entitys )
					value = pi->entitys->Ref2Entity(value.c_str() );
			}

			// generate child nodes
			while( xml && *xml )
			{
				LPXNode node = new XNode;
				node->parent = this;
				node->doc = doc;
				node->type = type;
				
				xml = node->Load( xml,pi );
				if( node->name.empty() == false )
				{
					childs.push_back( node );

				}
				else
				{
					delete node;
				}

				// open/close tag <TAG ..> ... </TAG>
				//                             ^- current pointer
				// CloseTag case
				if( xml && *xml && *(xml+1) && *xml == chXMLTagOpen && *(xml+1) == chXMLTagPre )
				{
					// </Close>
					xml+=2; // C
					
					if( xml = _tcsskip( xml ) )
					{
						tstring closename;
						tchar* pEnd = _tcspbrk( xml, TEXT(" >") );
						if( pEnd == NULL ) 
						{
							if( pi->erorr_occur == false ) 
							{
								pi->erorr_occur = true;
								pi->error_pointer = xml;
								pi->error_code = PIE_NOT_CLOSED;
								pi->error_string = (tboostformat(TEXT("it must be closed with </%s>"))%name).str();
							}
							// error
							return NULL;
						}
						_SetString( xml, pEnd, closename );
						if( closename == this->name )
						{
							// wel-formed open/close
							xml = pEnd+1;
							// return '>' or ' ' after pointer
							return xml;
						}
						else
						{
							xml = pEnd+1;
							// 2004.6.15 - example <B> alone tag
							// now it can parse with attribute 'force_arse'
							if( pi->force_parse == false )
							{
								// not welformed open/close
								if( pi->erorr_occur == false ) 
								{
									pi->erorr_occur = true;
									pi->error_pointer = xml;
									pi->error_code = PIE_NOT_NESTED;
									pi->error_string = (tboostformat(TEXT("'<%s> ... </%s>' is not wel-formed."))%name%closename).str();
								}
								return NULL;
							}
						}
					}
				}
				else	// Alone child Tag Loaded
						// else 해야하는지 말아야하는지 의심간다.
				{
					
					//if( xml && this->value.IsEmpty() && *xml !=chXMLTagOpen )
					if( xml && XIsEmptyString(value.c_str()) && *xml !=chXMLTagOpen )
					{
						// Text Value 
						tchar* pEnd = _tcsechr( xml, chXMLTagOpen, chXMLEscape );
						if( pEnd == NULL ) 
						{
							// error cos not exist CloseTag </TAG>
							if( pi->erorr_occur == false )  
							{
								pi->erorr_occur = true;
								pi->error_pointer = xml;
								pi->error_code = PIE_NOT_CLOSED;
								pi->error_string = (tboostformat(TEXT("it must be closed with </%s>"))%name).str();
							}
							return NULL;
						}
						
						bool trim = pi->trim_value;
						tchar escape = pi->escape_value;
						//_SetString( xml, pEnd, &value, trim, chXMLEscape );
						_SetString( xml, pEnd, value, trim, escape );

						xml = pEnd;
						//TEXTVALUE
						if( pi->entity_value && pi->entitys )
							value = pi->entitys->Ref2Entity(value.c_str() );
					}
				}
			}
		}
	}

	return xml;
}

// <?xml version='1.0'?>
// <TAG attr1="value1" attr2='value2' attr3=value3 >
// </TAG>
// or
// <TAG />
//        ^- return pointer
//========================================================
// Name   : Load
// Desc   : load xml plain text for xml document
// Param  : pszXml - plain xml text
//          pi = parser information
// Return : advanced string pointer  (error return NULL)
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
tchar* _tagXMLDocument::Load( const tchar* pszXml, LPPARSEINFO pi /*= NULL*/ )
{
	LPXNode node = new XNode;
	node->parent = (LPXNode)this;
	node->type = XNODE_ELEMENT;
	node->doc = this;
	tchar* end;
	
	if( pi == NULL )
		pi = &parse_info;

	if( (end = node->Load( pszXml, pi )) == NULL )
	{
		delete node;
		return NULL;
	}

	childs.push_back( node );

	// Load Other Node after </Tag>(pi, comment, CDATA etc)
	tchar* ret;
	bool bRet = false;
	ret = LoadOtherNodes( node, &bRet, end, pi );
	if( ret != NULL ) 
		end = ret;

	return end;
}

LPXNode	_tagXMLDocument::GetRoot()
{
	XNodes::iterator it = childs.begin();
	for( ; it != childs.end() ; ++(it) )
	{
		LPXNode node = *it;
		if( node->type == XNODE_ELEMENT )
			return node;
	}
	return NULL;
}

//========================================================
// Name   : GetXML
// Desc   : convert plain xml text from parsed xml attirbute
// Param  :
// Return : converted plain string
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
bool _tagXMLAttr::GetXML(tstring& tar, LPDISP_OPT opt /*= &optDefault*/ )
{
	tar.append(name).push_back('=');
	tar.push_back(opt->value_quotation_mark);

	if(opt->reference_value&&opt->entitys)
	{
		opt->entitys->Entity2Ref(tar, value);
	}
	else
	{
		tar.append(value);
	}

	tar.push_back(opt->value_quotation_mark);
	tar.push_back(' ');
	
	return true;
}

//========================================================
// Name   : GetXML
// Desc   : convert plain xml text from parsed xml node
// Param  :
// Return : converted plain string
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
bool _tagXMLNode::GetXML(tstring& tar, LPDISP_OPT opt /*= &optDefault*/ )
{
	// tab
	if( opt && opt->newline )
	{
		tar.append(TEXT("\r\n"));
		tar.append(opt->tab_base, '\t');
	}

	if(type == XNODE_ELEMENT)
	{
		// <TAG
		tar.push_back('<');
		tar.append(name);

		// <TAG Attr1="Val1" 
		if(!attrs.empty())
		{
			tar.push_back(' ');
			for( int i = 0 ; i < attrs.size(); i++ )
			{
				attrs[i]->GetXML(tar, opt);
			}
		}

		if( childs.empty() && value.empty())
		{
			// <TAG Attr1="Val1"/> alone tag 
			tar.append(TEXT("/>"));	
		}
		else
		{
			// <TAG Attr1="Val1"> and get child
			tar.push_back('>');
			if(!childs.empty() )
			{
				++opt->tab_base;
				for( int i = 0 ; i < childs.size(); i++ )
					childs[i]->GetXML(tar, opt);
				--opt->tab_base;
			}			

			// Text Value
			if(!value.empty())
			{
				if(opt && opt->newline && !childs.empty())
				{
					tar.append(TEXT("\r\n"));
					tar.append(opt->tab_base + 1, '\t');
				}
				if(opt->reference_value&&opt->entitys)
				{
					opt->entitys->Entity2Ref(tar, value);
				}
				else
				{
					tar.append(value);
				}
			}

			// </TAG> CloseTag
			if( opt && opt->newline && !childs.empty() )
			{
				tar.append(TEXT("\r\n")).append(opt->tab_base, '\t');
			}
			tar.append(TEXT("</")).append(name).push_back('>');
		}
	}
	else if( type == XNODE_DOC )
	{
		for( int i = 0 ; i < childs.size(); i++ )
			childs[i]->GetXML(tar, opt );
		return true;
	}
	else if( type == XNODE_PI )
	{
		// <?TAG
		tar.append(szXMLPIOpen).append(name);
		// <?TAG Attr1="Val1" 
		if(!attrs.empty())
		{
			tar.append(1, ' ');
			for( int i = 0 ; i < attrs.size(); ++i )
			{
				attrs[i]->GetXML(tar, opt);
			}
		}
		//?>
		tar.append(szXMLPIClose);	
		return true;
	}
	else if( type == XNODE_COMMENT )
	{
		tar.append(szXMLCommentOpen).append(value).append(szXMLCommentClose);
		return true;
	}
	else if( type == XNODE_CDATA )
	{
		tar.append(szXMLCDATAOpen).append(value).append(szXMLCDATAClose);
		return true;
	}

	return true;
}

tstring _tagXMLNode::GetXML( LPDISP_OPT opt /*= &optDefault*/ )
{
	tstring ret;
	GetXML(ret, opt);
	return ret;
}

//========================================================
// 함수명 : GetText
// 설  명 : 노드 하나를 텍스트 문자열로 반환
// 인  자 :
// 리턴값 : 변환된 문자열
//--------------------------------------------------------
// 작성자   작성일                 작성이유
// 조경민   2004-06-15
//========================================================
bool _tagXMLNode::GetText(tstring& tar, LPDISP_OPT opt /*= &optDefault*/ )
{
	if( type == XNODE_ELEMENT )
	{
		// childs text
		for( int i = 0 ; i < childs.size(); i++ )
			childs[i]->GetText(tar, opt);

		// Text Value
		if(!value.empty())
		{
			if(opt->reference_value&&opt->entitys)
			{
				opt->entitys->Entity2Ref(tar, value);
			}
			else
			{
				tar.append(value);
			}
		}
	}
	else if( type == XNODE_DOC )
	{
		for( int i = 0 ; i < childs.size(); i++ )
			childs[i]->GetText(tar, opt );
	}
	else if( type == XNODE_PI )
	{
		// no text
	}
	else if( type == XNODE_COMMENT )
	{
		// no text
	}
	else if( type == XNODE_CDATA )
	{
		tar.append(value);
	}

	return true;
}

tstring _tagXMLNode::GetText( LPDISP_OPT opt /*= &optDefault*/ )
{
	tstring ret;
	GetText(ret, opt);
	return ret;
}

//========================================================
// Name   : GetAttr
// Desc   : get attribute with attribute name
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXAttr	_tagXMLNode::GetAttr( const tchar* attrname )
{
	for( int i = 0 ; i < attrs.size(); i++ )
	{
		LPXAttr attr = attrs[i];
		if( attr )
		{
			if( attr->name == attrname )
				return attr;
		}
	}
	return NULL;
}

//========================================================
// Name   : GetAttrs
// Desc   : find attributes with attribute name, return its list
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
XAttrs _tagXMLNode::GetAttrs( const tchar* name )
{
	XAttrs attrs;
	for( int i = 0 ; i < attrs.size(); i++ )
	{
		LPXAttr attr = attrs[i];
		if( attr )
		{
			if( attr->name == name )
				attrs.push_back( attr );
		}
	}
	return attrs;
}

//========================================================
// Name   : GetAttrValue
// Desc   : get attribute with attribute name, return its value
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
const tchar* _tagXMLNode::GetAttrValue( const tchar* attrname )
{
	LPXAttr attr = GetAttr( attrname );
	return attr ? attr->value.c_str() : NULL;
}

XNodes _tagXMLNode::GetChilds()
{
	return childs;
}

//========================================================
// Name   : GetChilds
// Desc   : Find childs with name and return childs list
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
XNodes _tagXMLNode::GetChilds( const tchar* name )
{
	XNodes nodes;
	for( int i = 0 ; i < childs.size(); i++ )
	{
		LPXNode node = childs[i];
		if( node )
		{
			if( node->name == name )
				nodes.push_back( node );
		}
	}
	return nodes;	
}

//========================================================
// Name   : GetChild
// Desc   : get child node with index
// Param  :
// Return : NULL return if no child.
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode _tagXMLNode::GetChild( int i )
{
	if( i >= 0 && i < childs.size() )
		return childs[i];
	return NULL;
}

//========================================================
// Name   : GetChildCount
// Desc   : get child node count
// Param  :
// Return : 0 return if no child
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-12-26
//========================================================
int	_tagXMLNode::GetChildCount()
{
	return childs.size();
}

//========================================================
// Name   : GetChild
// Desc   : Find child with name and return child
// Param  :
// Return : NULL return if no child.
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode	_tagXMLNode::GetChild( const tchar* name )
{
	for( int i = 0 ; i < childs.size(); i++ )
	{
		LPXNode node = childs[i];
		if( node )
		{
			if( node->name == name )
				return node;
		}
	}
	return NULL;
}

//========================================================
// Name   : GetChildValue
// Desc   : Find child with name and return child's value
// Param  :
// Return : NULL return if no child.
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
const tchar*	_tagXMLNode::GetChildValue( const tchar* name )
{
	LPXNode node = GetChild( name );
	return (node != NULL)? node->value.c_str() : NULL;
}

tstring	_tagXMLNode::GetChildText( const tchar* name, LPDISP_OPT opt /*= &optDefault*/ )
{
	tstring ret;
	LPXNode node = GetChild(name);
	if(node) node->GetText(ret, opt);
	return ret;
}

LPXAttr _tagXMLNode::GetChildAttr( const tchar* name, const tchar* attrname )
{
	LPXNode node = GetChild(name);
	return node ? node->GetAttr(attrname) : NULL;
}

const tchar* _tagXMLNode::GetChildAttrValue( const tchar* name, const tchar* attrname )
{
	LPXAttr attr = GetChildAttr( name, attrname );
	return attr ? attr->value.c_str() : NULL;
}

//========================================================
// Name   : Find
// Desc   : find node with tag name from it's all childs
// Param  :
// Return : NULL return if no found node.
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode	_tagXMLNode::Find( const tchar* name )
{
	XNodes::iterator it = childs.begin();
	for( ; it != childs.end(); ++(it))
	{
		LPXNode child = *it;
		if( child->name == name )
			return child;

		XNodes::iterator it = child->childs.begin();
		for( ; it != child->childs.end(); ++(it))
		{
			LPXNode find = child->Find( name );
			if( find != NULL )
				return find;
		}
	}

	return NULL;
}

//========================================================
// Name   : GetChildIterator
// Desc   : get child nodes iterator
// Param  :
// Return : NULL return if no childs.
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
XNodes::iterator _tagXMLNode::GetChildIterator( LPXNode node )
{
	XNodes::iterator it = childs.begin();
	for( ; it != childs.end() ; ++(it) )
	{
		if( *it == node )
			return it;
	}
	return XNodes::iterator();
}

//========================================================
// Name   : AppendChild
// Desc   : add node
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode	_tagXMLNode::AppendChild( const tchar* name /*= NULL*/, const tchar* value /*= NULL*/ )
{
	return AppendChild( CreateNode( name, value ) );
}

//========================================================
// Name   : AppendChild
// Desc   : add node
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode _tagXMLNode::AppendChild( LPXNode node )
{
	//ASSERT(node != this);
	if(node == this)
	{
		return NULL;
	}

	node->parent = this;
	node->doc = doc;
	childs.push_back( node );
	return node;
}

//========================================================
// Name   : RemoveChild
// Desc   : detach node and delete object
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
bool _tagXMLNode::RemoveChild( LPXNode node )
{
	XNodes::iterator it = GetChildIterator( node );
	if( *it )
	{
		delete *it;
		childs.erase( it );
		return true;
	}
	return false;
}

//========================================================
// Name   : GetAttr
// Desc   : get attribute with index in attribute list
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXAttr _tagXMLNode::GetAttr( int i )
{
	if( i >= 0 && i < attrs.size() )
		return attrs[i];
	return NULL;
}

//========================================================
// Name   : GetAttrIterator
// Desc   : get attribute iterator
// Param  : 
// Return : std::vector<LPXAttr>::iterator
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
XAttrs::iterator _tagXMLNode::GetAttrIterator( LPXAttr attr )
{
	XAttrs::iterator it = attrs.begin();
	for( ; it != attrs.end() ; ++(it) )
	{
		if( *it == attr )
			return it;
	}
	return XAttrs::iterator();
}

//========================================================
// Name   : AppendAttr
// Desc   : add attribute
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXAttr _tagXMLNode::AppendAttr( LPXAttr attr )
{
	attr->parent = this;
	attrs.push_back( attr );
	return attr;
}

//========================================================
// Name   : RemoveAttr
// Desc   : detach attribute and delete object
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
bool _tagXMLNode::RemoveAttr( LPXAttr attr )
{
	XAttrs::iterator it = GetAttrIterator( attr );
	if( it != attrs.end() )
	{
		delete *it;
		attrs.erase( it );
		return true;
	}
	return false;
}

//========================================================
// Name   : CreateNode
// Desc   : Create node object and return it
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode _tagXMLNode::CreateNode( const tchar* name /*= NULL*/, const tchar* value /*= NULL*/ )
{
	LPXNode node = new XNode;
	node->name = name;
	node->value = value;
	return node;
}

//========================================================
// Name   : CreateAttr
// Desc   : create Attribute object and return it
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXAttr _tagXMLNode::CreateAttr( const tchar* name /*= NULL*/, const tchar* value /*= NULL*/ )
{
	LPXAttr attr = new XAttr;
	attr->name = name;
	attr->value = value;
	return attr;
}

//========================================================
// Name   : AppendAttr
// Desc   : add attribute
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXAttr _tagXMLNode::AppendAttr( const tchar* name /*= NULL*/, const tchar* value /*= NULL*/ )
{
	return AppendAttr( CreateAttr( name, value ) );
}

//========================================================
// Name   : DetachChild
// Desc   : no delete object, just detach in list
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode _tagXMLNode::DetachChild( LPXNode node )
{
	XNodes::iterator it = GetChildIterator( node );
	if( it != childs.end() )
	{
		childs.erase( it );
		return node;
	}
	return NULL;
}

//========================================================
// Name   : DetachAttr
// Desc   : no delete object, just detach in list
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXAttr _tagXMLNode::DetachAttr( LPXAttr attr )
{
	XAttrs::iterator it = GetAttrIterator( attr );
	if( it != attrs.end() )
	{
		attrs.erase( it );
		return attr;
	}
	return NULL;
}

//========================================================
// Name   : CopyNode
// Desc   : copy current level node with own attributes
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
void _tagXMLNode::CopyNode( LPXNode node )
{
	Close();

	doc = node->doc;
	parent = node->parent;
	name = node->name;
	value = node->value;
	type = node->type;

	// copy attributes
	for( int i = 0 ; i < node->attrs.size(); i++)
	{
		LPXAttr attr = node->attrs[i];
		if( attr )
			AppendAttr( attr->name.c_str(), attr->value.c_str() );
	}
}

//========================================================
// Name   : _CopyBranch
// Desc   : recursive internal copy branch 
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
void _tagXMLNode::_CopyBranch( LPXNode node )
{
	CopyNode( node );

	for( int i = 0 ; i < node->childs.size(); i++)
	{
		LPXNode child = node->childs[i];
		if( child )
		{
			LPXNode mychild = new XNode;
			mychild->CopyNode( child );
			AppendChild( mychild );

			mychild->_CopyBranch( child );
		}
	}
}

//========================================================
// Name   : AppendChildBranch
// Desc   : add child branch ( deep-copy )
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
LPXNode	_tagXMLNode::AppendChildBranch( LPXNode node )
{
	LPXNode child = new XNode;
	child->CopyBranch( node );

	return AppendChild( child );
}

//========================================================
// Name   : CopyBranch
// Desc   : copy branch ( deep-copy )
// Param  :
// Return : 
//--------------------------------------------------------
// Coder    Date                      Desc
// bro      2002-10-29
//========================================================
void _tagXMLNode::CopyBranch( LPXNode branch )
{
	Close();
	
	_CopyBranch( branch );
}


_tagXMLEntitys::_tagXMLEntitys( LPXENTITY entities, int count )
{
	for( int i = 0; i < count; i++)
		push_back( entities[i] );
}

LPXENTITY _tagXMLEntitys::GetEntity( int entity )
{
	for( int i = 0 ; i < size(); i ++ )
	{
		if( at(i).entity == entity )
			return LPXENTITY(&at(i));
	}
	return NULL;
}

LPXENTITY _tagXMLEntitys::GetEntity(const tchar* entity )
{
	for( int i = 0 ; i < size(); i ++ )
	{
		const tchar* ref = at(i).ref;
		const tchar* ps = entity;
		while( ref && *ref )
			if( *ref++ != *ps++ )
				break;
		if( ref && !*ref )	// found!
			return LPXENTITY(&at(i));
	}
	return NULL;
}

int _tagXMLEntitys::GetEntityCount( const tchar* str )
{
	int nCount = 0;
	tchar* ps = (tchar*)str;
	while( ps && *ps )
		if( GetEntity( *ps++ ) ) nCount ++;
	return nCount;
}

int _tagXMLEntitys::Ref2Entity( const tchar* estr, tchar* str, int strlen )
{
	tchar* pes = (tchar*)estr;
	tchar* ps = str;
	tchar* ps_end = ps+strlen;
	while( pes && *pes && ps < ps_end )
	{
		LPXENTITY ent = GetEntity( pes );
		if( ent )
		{
			// copy entity meanning char
			*ps = ent->entity;
			pes += ent->ref_len;
		}
		else
			*ps = *pes++;	// default character copy
		ps++;
	}
	*ps = '\0';
	
	// total copied characters
	return ps-str;	
}

bool _tagXMLEntitys::Ref2Entity( tstring& tar, const tstring& val )
{
	const tchar* ps = val.c_str();
	const tchar* ps_end = ps+val.length();
	while(ps < ps_end )
	{
		LPXENTITY ent = GetEntity(ps);
		if(ent)
		{
			tar.push_back(ent->entity);
			ps += ent->ref_len;
		}
		else
			tar.push_back(*ps++);
	}

	// total copied characters
	return true;	
}

bool _tagXMLEntitys::Entity2Ref( tstring& tar, const tstring& val )
{
	const tchar* pszVal = val.c_str();
	const tchar* pszTmp = pszVal;
	while(*pszTmp)
	{
		LPXENTITY ent = GetEntity(pszTmp);
		if(ent)
		{
			tar.append(ent->ref);
		}
		else
		{
			tar.push_back(*pszTmp);
		}
		++pszTmp;
	}
	return true;
}


int _tagXMLEntitys::Entity2Ref( const tchar* str, tchar* estr, int estrlen )
{
	tchar* ps = (tchar*)str;
	tchar* pes = (tchar*)estr;
	tchar* pes_end = pes+estrlen;
	while( ps && *ps && pes < pes_end )
	{
		LPXENTITY ent = GetEntity( *ps );
		if( ent )
		{
			// copy entity string
			tchar* ref = (tchar*)ent->ref;
			while( ref && *ref )
				*pes++ = *ref++;
		}
		else
			*pes++ = *ps;	// default character copy
		ps++;
	}
	*pes = '\0';
	
	// total copied characters
	return pes-estr;
}

tstring _tagXMLEntitys::Ref2Entity( const tchar* estr )
{
	tstring ret;
	if( estr )
	{
		int		len = _tcslen(estr);
		tchar*	buf = new tchar[len+1];
		Ref2Entity( estr, buf, len );
		ret.assign(buf);
		delete[] buf;
	}
	return ret;
}

tstring _tagXMLEntitys::Entity2Ref( const tchar* str )
{	
	tstring ret;
	if(str)
	{
		int nEntityCount = GetEntityCount(str);
		if( nEntityCount > 0 )
		{
			int len = _tcslen(str) + nEntityCount*10 ;
			tchar* buf = new tchar[len];
			Entity2Ref( str, buf, len );
			ret.assign(buf);
			delete[] buf;
		}	
		else
		{
			ret.assign(str);
		}
	}
	return ret;
}

tstring XRef2Entity( const tchar* estr )
{
	return entityDefault.Ref2Entity( estr );
}

tstring XEntity2Ref( const tchar* str )
{
	return entityDefault.Entity2Ref( str );
}

// Helper Funtion
long XStr2Int( const tchar* str, long default_value)
{
	return ( str && *str ) ? _ttol(str) : default_value;
}

bool XIsEmptyString( const tchar* str )
{
	tstring s(str);
	boost::trim(s);
	return ( s.empty() || s == TEXT("") );
}