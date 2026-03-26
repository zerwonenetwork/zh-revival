#include <windows.h>
#include <dia2.h>
#include <diacreate.h>
#include <atlbase.h>
#include <iostream>
#pragma comment(lib, "diaguids.lib")

int wmain(int argc, wchar_t** argv) {
    if (argc < 3) return 2;
    const wchar_t* pdb = argv[1];
    DWORD rva = wcstoul(argv[2], nullptr, 16);
    HRESULT hr = CoInitialize(nullptr);
    CComPtr<IDiaDataSource> source;
    hr = NoRegCoCreate(L"C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\DIA SDK\\bin\\msdia140.dll", __uuidof(DiaSource), __uuidof(IDiaDataSource), (void**)&source);
    if (FAILED(hr)) { std::wcerr << L"NoRegCoCreate failed: " << std::hex << hr << std::endl; return 3; }
    hr = source->loadDataFromPdb(pdb);
    if (FAILED(hr)) { std::wcerr << L"loadDataFromPdb failed: " << std::hex << hr << std::endl; return 4; }
    CComPtr<IDiaSession> session;
    hr = source->openSession(&session);
    if (FAILED(hr)) { std::wcerr << L"openSession failed: " << std::hex << hr << std::endl; return 5; }
    CComPtr<IDiaSymbol> symbol;
    hr = session->findSymbolByRVA(rva, SymTagFunction, &symbol);
    if (FAILED(hr) || !symbol) { std::wcerr << L"findSymbolByRVA failed: " << std::hex << hr << std::endl; return 6; }
    BSTR name = nullptr;
    symbol->get_name(&name);
    DWORD symRva = 0; ULONGLONG len = 0;
    symbol->get_relativeVirtualAddress(&symRva);
    symbol->get_length(&len);
    std::wcout << L"FUNCTION=" << (name ? name : L"<null>") << L" RVA=0x" << std::hex << symRva << L" LEN=0x" << len << std::endl;
    if (name) SysFreeString(name);

    CComPtr<IDiaEnumLineNumbers> lines;
    hr = session->findLinesByRVA(rva, 1, &lines);
    if (SUCCEEDED(hr) && lines) {
        CComPtr<IDiaLineNumber> line;
        ULONG fetched = 0;
        if (SUCCEEDED(lines->Next(1, &line, &fetched)) && fetched == 1) {
            DWORD lineNum = 0; line->get_lineNumber(&lineNum);
            CComPtr<IDiaSourceFile> src;
            line->get_sourceFile(&src);
            BSTR file = nullptr;
            if (src) src->get_fileName(&file);
            std::wcout << L"LINE=" << (file ? file : L"<null>") << L":" << std::dec << lineNum << std::endl;
            if (file) SysFreeString(file);
        }
    }
    return 0;
}
