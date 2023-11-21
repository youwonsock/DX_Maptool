#pragma once

struct PathString
{
	WCHAR Drive[_MAX_DRIVE];
	WCHAR Dir[_MAX_DIR];
	WCHAR Filename[_MAX_FNAME];
	WCHAR Ext[_MAX_EXT];

	PathString(const wchar_t* fullPath)
	{
		_wsplitpath_s(fullPath, Drive, _MAX_DRIVE
			, Dir, _MAX_DIR
			, Filename, _MAX_FNAME
			, Ext, _MAX_EXT);
	}
};

inline void ShowErrorMessage(HRESULT hr);
inline void ShowErrorMessage(ID3DBlob* errorCode);
inline std::string WStringToString(const std::wstring& wstr);
inline std::wstring StringToWString(const std::string& str);
inline bool StartsWith(const std::wstring& str, const std::wstring& comp);
inline bool StartsWith(const std::string& str, const  std::string comp);