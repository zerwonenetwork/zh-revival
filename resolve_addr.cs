using System;
using System.Runtime.InteropServices;

public static class SymResolver {
    [DllImport("dbghelp.dll", SetLastError=true, CharSet=CharSet.Unicode)]
    public static extern bool SymInitialize(IntPtr hProcess, string UserSearchPath, bool fInvadeProcess);

    [DllImport("dbghelp.dll", SetLastError=true, CharSet=CharSet.Unicode)]
    public static extern ulong SymLoadModuleExW(IntPtr hProcess, IntPtr hFile, string ImageName, string ModuleName, ulong BaseOfDll, uint DllSize, IntPtr Data, uint Flags);

    [DllImport("dbghelp.dll", SetLastError=true)]
    public static extern bool SymFromAddr(IntPtr hProcess, ulong Address, out ulong Displacement, IntPtr Symbol);

    [DllImport("dbghelp.dll", SetLastError=true)]
    public static extern bool SymGetLineFromAddr64(IntPtr hProcess, ulong dwAddr, out uint pdwDisplacement, ref IMAGEHLP_LINE64 Line64);

    [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Ansi)]
    public struct SYMBOL_INFO {
        public uint SizeOfStruct;
        public uint TypeIndex;
        public ulong Reserved1;
        public ulong Reserved2;
        public uint Index;
        public uint Size;
        public ulong ModBase;
        public uint Flags;
        public ulong Value;
        public ulong Address;
        public uint Register;
        public uint Scope;
        public uint Tag;
        public uint NameLen;
        public uint MaxNameLen;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 1024)]
        public string Name;
    }

    [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Ansi)]
    public struct IMAGEHLP_LINE64 {
        public uint SizeOfStruct;
        public IntPtr Key;
        public uint LineNumber;
        [MarshalAs(UnmanagedType.LPStr)]
        public string FileName;
        public ulong Address;
    }
}
