// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A5E8B36A_2DA9_4105_80A9_180724B74C1A__INCLUDED_)
#define AFX_STDAFX_H__A5E8B36A_2DA9_4105_80A9_180724B74C1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <stdio.h>

#define ANTIVIRUS  putchar(48);
#pragma comment(linker, "/MERGE:.rdata=.data")//把rdata区段合并到data区段里
#pragma comment(linker, "/MERGE:.text=.data")//把text区段合并到data区段里
#pragma comment(linker, "/MERGE:.reloc=.data")//把reloc区段合并到data区段里
// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A5E8B36A_2DA9_4105_80A9_180724B74C1A__INCLUDED_)
