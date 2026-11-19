#!/bin/sh
# Full rebuild: palettes -> contrast fixes -> patterns -> headers -> harness test -> Windows exe
set -e
cd "$(dirname "$0")"
python3 mumu.py >/dev/null; python3 amoled.py >/dev/null; python3 contrast.py >/dev/null
python3 mumu_str.py >/dev/null; WITH_Z=1 python3 gen_headers.py | tail -1; python3 codepatch.py
for l in kernel32 user32 ntdll advapi32; do llvm-dlltool -m i386:x86-64 -d $l.def -l $l.lib; done
BID=$(cat palette.h strpatch.h accent.h codepatch.h sddark.c | sha256sum | cut -c1-12)
clang --target=x86_64-pc-windows-msvc -O2 -ffreestanding -fno-builtin -mno-stack-arg-probe -fms-extensions -fshort-wchar -DBUILD_ID="L\"$BID\"" -c sddark.c -o sddark.obj
lld-link /subsystem:windows /entry:WinMainCRTStartup /nodefaultlib /manifest:embed "/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'" /out:StarDeskDark.exe sddark.obj res.res kernel32.lib user32.lib ntdll.lib advapi32.lib
cp StarDeskDark.exe "StarDesk Dark.exe"; echo "BUILD_ID $BID"
