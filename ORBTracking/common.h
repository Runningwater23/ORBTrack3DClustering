#pragma once
#include <windows.h> 
#include <wchar.h> 
class Common
{
public:
	//�� ���ֽ�char* ת��Ϊ ���ֽ� wchar*
	static wchar_t* AnsiToUnicode(const char* szStr);
	//��TCHARתΪchar   
	//*tchar��TCHAR����ָ�룬*_char��char����ָ��   
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
//��TCHARתΪchar   
//*tchar��TCHAR����ָ�룬*_char��char����ָ��   
inline void Common::TcharToChar(const TCHAR * tchar, char * _char)
{
	int iLength;
	//��ȡ�ֽڳ���   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//��tcharֵ����_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}


