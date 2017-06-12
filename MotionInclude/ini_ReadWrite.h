
/*
+-----------------------------------------------------------------------------+
 GetProfileStringEx: Used to get the key value of ini file. The key value will be returned with CString type
 WriteProfileStringEx: Used to write the key value of ini file. The result will be returned with boolen type
  
+-----------------------------------------------------------------------------+
*/
#ifndef ADV_H
#define ADV_H


#ifdef __cplusplus
extern "C"
{
#endif

BOOL GetProfileStringEx(_TCHAR *charSection, _TCHAR *charEntry,_TCHAR *charEntryValue, DWORD nSize,  _TCHAR *charIniPath);
BOOL  WriteProfileStringEx(_TCHAR  *charSection, _TCHAR  *charEntry, _TCHAR  *charValue,_TCHAR  *charIniPath);


#ifdef __cplusplus
}
#endif

#endif