#include "OS.h"
#include "Windows.h"
#include <iostream>
#include <string>

std::ostream& operator<<(std::ostream& os, std::wstring str) {
	for (int i = 0; i < str.length(); i++) {
		wchar_t wc = str[i];
		char c = unsigned(wc) < 127 ? wc : '?';
		os << c;
	}
	return os;
}

void getCurrentDirectory() {
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring path(buffer);
	std::cout << path;
	std::cout << "\n";
}

