// XMLite.h: interface for the XMLite class.
//
// XMLite : XML Lite Parser Library
// by bro ( Cho,Kyung Min: bro@shinbiro.com ) 2002-10-30
// Microsoft MVP (Visual C++) bro@msmvp.com
// 
// History.
// 2002-10-29 : First Coded. Parsing XMLElelement and Attributes.
//              get xml parsed string ( looks good )
// 2002-10-30 : Get Node Functions, error handling ( not completed )
// 2002-12-06 : Helper Funtion string to long
// 2002-12-12 : Entity Helper Support
// 2003-04-08 : Close, 
// 2003-07-23 : add property escape_value. (now no escape on default)
// 2003-10-24 : bugfix) attribute parsing <tag a='1' \r\n/> is now ok
// 2004-03-05 : add branch copy functions
// 2004-06-14 : add _tcseistr/_tcsenistr/_tcsenicmp functions
// 2004-06-14 : now support, XML Document and PI, Comment, CDATA node
// 2004-06-15 : add GetText()/ Find() functions
// 2004-06-15 : add force_parse : now can parse HTML (not-welformed xml)
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLITE_H__786258A5_8360_4AE4_BDAF_2A52F8E1B877__INCLUDED_)
#define AFX_XMLITE_H__786258A5_8360_4AE4_BDAF_2A52F8E1B877__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <deque>
#include <string>
#include "tstring.h"
#include <tchar.h>

struct _tagXMLAttr;
typedef _tagXMLAttr XAttr, *LPXAttr;
typedef std::vector<LPXAttr> XAttrs;

struct _tagXMLNode;
typedef _tagXMLNode XNode, *LPXNode;
typedef std::vector<LPXNode> XNodes, *LPXNodes;

struct _tagXMLDocument;
typedef struct _tagXMLDocument XDoc, *LPXDoc;

// Entity Encode/Decode Support
typedef struct _tagXmlEntity
{
	tchar	entity;					// entity ( & " ' < > )
	tchar	ref[10];					// entity reference ( &amp; &quot; etc )
	int		ref_len;					// entity reference length
}XENTITY,*LPXENTITY;

typedef struct _tagXMLEntitys : public std::vector<XENTITY>
{
	LPXENTITY	GetEntity( int entity );
	LPXENTITY	GetEntity( const tchar* entity );	
	int			GetEntityCount( const tchar* str );

	int			Ref2Entity( const tchar* estr, tchar* str, int strlen );
	int			Entity2Ref( const tchar* str, tchar* estr, int estrlen );

	tstring		Ref2Entity( const tchar* estr );
	tstring		Entity2Ref( const tchar* str );	

	bool		Entity2Ref(tstring& tar, const tstring& val);
	bool		Ref2Entity(tstring& tar, const tstring& val);

	_tagXMLEntitys(){};
	_tagXMLEntitys( LPXENTITY entities, int count );
}XENTITYS,*LPXENTITYS;

extern XENTITYS entityDefault;

tstring XRef2Entity( const tchar* estr );
tstring XEntity2Ref( const tchar* str );	

typedef enum 
{
	PIE_PARSE_WELFORMED	= 0,
	PIE_ALONE_NOT_CLOSED,
	PIE_NOT_CLOSED,
	PIE_NOT_NESTED,
	PIE_ATTR_NO_VALUE
}PCODE;

// Parse info.
typedef struct _tagParseInfo
{
	bool		trim_value;			// [set] do trim when parse?
	bool		entity_value;		// [set] do convert from reference to entity? ( &lt; -> < )
	LPXENTITYS	entitys;			// [set] entity table for entity decode
	tchar		escape_value;		// [set] escape value (default '\\')
	bool		force_parse;		// [set] force parse even if xml is not welformed

	tchar*		xml;				// [get] xml source
	bool		erorr_occur;		// [get] is occurance of error?
	tchar*		error_pointer;		// [get] error position of xml source
	PCODE		error_code;			// [get] error code
	tstring		error_string;		// [get] error string

	LPXDoc		doc;
	_tagParseInfo() { trim_value = false; entity_value = true; force_parse = false; entitys = &entityDefault; xml = 0; erorr_occur = false; error_pointer = 0; error_code = PIE_PARSE_WELFORMED; escape_value = '\\'; }
}PARSEINFO,*LPPARSEINFO;
extern PARSEINFO piDefault;

// display optional environment
typedef struct _tagDispOption
{
	bool newline;			// newline when new tag
	bool reference_value;	// do convert from entity to reference ( < -> &lt; )
	char value_quotation_mark;	// val="" (default value quotation mark "
	LPXENTITYS	entitys;	// entity table for entity encode

	int tab_base;			// internal usage
	_tagDispOption() { newline = true; reference_value = true; entitys = &entityDefault; tab_base = 0; value_quotation_mark = '"'; }
}DISP_OPT, *LPDISP_OPT;
extern DISP_OPT optDefault;

// XAttr : Attribute Implementation
typedef struct _tagXMLAttr
{
	tstring			name;
	tstring			value;
	_tagXMLNode*	parent;

	bool GetXML(tstring& tar, LPDISP_OPT opt /*= &optDefault*/ );

}XAttr, *LPXAttr;

typedef enum
{
	XNODE_ELEMENT,				// general node '<element>...</element>' or <element/>
	XNODE_PI,					// <?xml version="1.0" ?>
	XNODE_COMMENT,				// <!-- comment -->
	XNODE_CDATA,				// <![CDATA[ cdata ]]>
	XNODE_DOC,					// internal virtual root
}NODETEXTYPE;

// XMLNode structure
typedef struct _tagXMLNode
{
	// name and value
	tstring name;
	tstring	value;

	// internal variables
	LPXNode	parent;		// parent node
	XNodes	childs;		// child node
	XAttrs	attrs;		// attributes
	NODETEXTYPE type;		// node type 
	LPXDoc	doc;		// document

	// Load/Save XML
	tchar*	Load( const tchar* pszXml, LPPARSEINFO pi = &piDefault );

	bool GetXML(tstring& tar, LPDISP_OPT opt = &optDefault );
	bool GetText(tstring& tar, LPDISP_OPT opt = &optDefault );

	tstring GetXML( LPDISP_OPT opt = &optDefault );
	tstring GetText( LPDISP_OPT opt = &optDefault );

	// internal load functions
	tchar*	LoadAttributes( const tchar* pszAttrs, LPPARSEINFO pi = &piDefault );
	tchar*	LoadAttributes( const tchar* pszAttrs, const tchar* pszEnd, LPPARSEINFO pi = &piDefault );
	tchar*	LoadProcessingInstrunction( const tchar* pszXml, LPPARSEINFO pi = &piDefault );
	tchar*	LoadComment( const tchar* pszXml, LPPARSEINFO pi = &piDefault ); 
	tchar*	LoadCDATA( const tchar* pszXml, LPPARSEINFO pi = &piDefault ); 

	// in own attribute list
	LPXAttr	GetAttr( const tchar* attrname ); 
	const tchar*	GetAttrValue( const tchar* attrname ); 
	XAttrs	GetAttrs( const tchar* name ); 

	// in one level child nodes
	LPXNode	GetChild( const tchar* name ); 
	const tchar*	GetChildValue( const tchar* name ); 
	tstring	GetChildText( const tchar* name, LPDISP_OPT opt = &optDefault );
	XNodes	GetChilds( const tchar* name ); 
	XNodes	GetChilds(); 

	LPXAttr GetChildAttr( const tchar* name, const tchar* attrname );
	const tchar* GetChildAttrValue( const tchar* name, const tchar* attrname );
	
	// search node
	LPXNode	Find( const tchar* name );

	// modify DOM 
	int		GetChildCount();
	LPXNode GetChild( int i );
	XNodes::iterator GetChildIterator( LPXNode node );
	LPXNode CreateNode( const tchar* name = 0, const tchar* value = 0 );
	LPXNode	AppendChild( const tchar* name = 0, const tchar* value = 0 );
	LPXNode	AppendChild( LPXNode node );
	bool	RemoveChild( LPXNode node );
	LPXNode DetachChild( LPXNode node );

	// node/branch copy
	void	CopyNode( LPXNode node );
	void	CopyBranch( LPXNode branch );
	void	_CopyBranch( LPXNode node );
	LPXNode	AppendChildBranch( LPXNode node );

	// modify attribute
	LPXAttr GetAttr( int i );
	XAttrs::iterator GetAttrIterator( LPXAttr node );
	LPXAttr CreateAttr( const tchar* anem = 0, const tchar* value = 0 );
	LPXAttr AppendAttr( const tchar* name = 0, const tchar* value = 0 );
	LPXAttr	AppendAttr( LPXAttr attr );
	bool	RemoveAttr( LPXAttr attr );
	LPXAttr DetachAttr( LPXAttr attr );

	// operator overloads
	LPXNode operator [] ( int i ) { return GetChild(i); }
	XNode& operator = ( XNode& node ) { CopyBranch(&node); return *this; }

	_tagXMLNode() { parent = 0; doc = 0; type = XNODE_ELEMENT; }
	~_tagXMLNode();

	void Close();
}XNode, *LPXNode;

// XMLDocument structure
typedef struct _tagXMLDocument : public XNode
{
	PARSEINFO	parse_info;

	_tagXMLDocument() { parent = 0; doc = this; type = XNODE_DOC; }
	
	tchar*	Load( const tchar* pszXml, LPPARSEINFO pi = 0 );
	LPXNode	GetRoot();

}XDoc, *LPXDoc;

// Helper Funtion
long XStr2Int( const tchar* str, long default_value = 0 );
bool XIsEmptyString( const tchar* str );

#endif // !defined(AFX_XMLITE_H__786258A5_8360_4AE4_BDAF_2A52F8E1B877__INCLUDED_)
