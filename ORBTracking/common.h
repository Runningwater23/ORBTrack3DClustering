#pragma once
#include <windows.h> 
#include <wchar.h> 
class Common
{
public:
	//将 单字节char* 转换为 宽字节 wchar*
	static wchar_t* AnsiToUnicode(const char* szStr);
	//将TCHAR转为char   
	//*tchar是TCHAR类型指针，*_char是char类型指针   
	static void TcharToChar(const TCHAR * tchar, char * _char);
	//

};
inline wchar_t* Common::AnsiToUnicode(const char* szStr)
{
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	return pResult;
}
//将TCHAR转为char   
//*tchar是TCHAR类型指针，*_char是char类型指针   
inline void Common::TcharToChar(const TCHAR * tchar, char * _char)
{
	int iLength;
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}


