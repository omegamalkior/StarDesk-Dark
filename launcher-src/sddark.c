/* StarDesk Dark - starts StarDesk with a dark color palette.
 * It launches bin\StarDesk.exe paused, swaps the 255-entry color table in
 * memory (no files are changed), then lets StarDesk run normally. */
#include "palette.h"
#include "strpatch.h"
#include "accent.h"
#include "codepatch.h"

typedef unsigned short WCHAR;
typedef unsigned int DWORD;
typedef int BOOL;
typedef void *HANDLE;
typedef unsigned long long SIZE_T;
typedef int NTSTATUS;
#ifndef TESTHOST
#define WINAPI __stdcall
#define IMP __declspec(dllimport)
#else
#define WINAPI
#define IMP
#endif
#define NULL ((void *)0)

typedef struct { DWORD cb; WCHAR *r, *desk, *title; DWORD x, y, xs, ys, xc, yc, fill, flags; unsigned short show, cb2; void *r2; HANDLE i, o, e; } STARTUPINFOW;
typedef struct { HANDLE hProcess, hThread; DWORD pid, tid; } PROCESS_INFORMATION;
typedef struct { NTSTATUS st; void *peb; SIZE_T affinity; int prio; SIZE_T pid, ppid; } PBI;
typedef struct { DWORD dwSize, cntUsage, pid; SIZE_T heapid; DWORD modid, threads, ppid; int pri; DWORD flags; WCHAR exe[260]; } PROCESSENTRY32W;

IMP WCHAR *WINAPI GetCommandLineW(void);
IMP DWORD WINAPI ExpandEnvironmentStringsW(const WCHAR *, WCHAR *, DWORD);
IMP DWORD WINAPI GetFileAttributesW(const WCHAR *);
IMP BOOL WINAPI CreateProcessW(const WCHAR *, WCHAR *, void *, void *, BOOL, DWORD, void *, const WCHAR *, STARTUPINFOW *, PROCESS_INFORMATION *);
IMP BOOL WINAPI ReadProcessMemory(HANDLE, const void *, void *, SIZE_T, SIZE_T *);
IMP BOOL WINAPI WriteProcessMemory(HANDLE, void *, const void *, SIZE_T, SIZE_T *);
IMP BOOL WINAPI VirtualProtectEx(HANDLE, void *, SIZE_T, DWORD, DWORD *);
IMP void *WINAPI VirtualAlloc(void *, SIZE_T, DWORD, DWORD);
IMP BOOL WINAPI VirtualFree(void *, SIZE_T, DWORD);
IMP DWORD WINAPI ResumeThread(HANDLE);
IMP BOOL WINAPI CloseHandle(HANDLE);
IMP void WINAPI ExitProcess(unsigned);
IMP HANDLE WINAPI CreateToolhelp32Snapshot(DWORD, DWORD);
IMP BOOL WINAPI Process32FirstW(HANDLE, PROCESSENTRY32W *);
IMP BOOL WINAPI Process32NextW(HANDLE, PROCESSENTRY32W *);
IMP HANDLE WINAPI OpenProcess(DWORD, BOOL, DWORD);
IMP BOOL WINAPI QueryFullProcessImageNameW(HANDLE, DWORD, WCHAR *, DWORD *);
IMP BOOL WINAPI TerminateProcess(HANDLE, unsigned);
IMP DWORD WINAPI WaitForSingleObject(HANDLE, DWORD);
IMP void WINAPI Sleep(DWORD);
IMP NTSTATUS WINAPI NtQueryInformationProcess(HANDLE, int, void *, DWORD, DWORD *);
IMP int WINAPI MessageBoxW(void *, const WCHAR *, const WCHAR *, unsigned);
typedef int (WINAPI *WNDENUMPROC_)(void *, long long);
IMP int WINAPI EnumWindows(WNDENUMPROC_, long long);
IMP DWORD WINAPI GetWindowThreadProcessId(void *, DWORD *);
IMP int WINAPI GetClassNameW(void *, WCHAR *, int);
IMP int WINAPI PostMessageW(void *, unsigned, unsigned long long, long long);
IMP int WINAPI IsWindowVisible(void *);
IMP int WINAPI GetWindowRect(void *, int *);
IMP BOOL WINAPI WaitForDebugEvent(void *, DWORD);
IMP BOOL WINAPI ContinueDebugEvent(DWORD, DWORD, DWORD);
IMP BOOL WINAPI DebugActiveProcessStop(DWORD);
IMP BOOL WINAPI DebugSetProcessKillOnExit(BOOL);
IMP DWORD WINAPI GetTickCount(void);
IMP void *WINAPI LoadLibraryW(const WCHAR *);
IMP void *WINAPI GetProcAddress(void *, const char *);
IMP void *WINAPI GetModuleHandleW(const WCHAR *);
IMP DWORD WINAPI GetModuleFileNameW(void *, WCHAR *, DWORD);
IMP HANDLE WINAPI CreateEventW(void *, BOOL, BOOL, const WCHAR *);
IMP HANDLE WINAPI OpenEventW(DWORD, BOOL, const WCHAR *);
IMP BOOL WINAPI DuplicateHandle(HANDLE, HANDLE, HANDLE, HANDLE *, DWORD, BOOL, DWORD);
IMP void *WINAPI LoadIconW(void *, const WCHAR *);
IMP BOOL WINAPI FlushInstructionCache(HANDLE, const void *, SIZE_T);
IMP BOOL WINAPI SetEnvironmentVariableW(const WCHAR *, const WCHAR *);
IMP HANDLE WINAPI CreateFileW(const WCHAR *, DWORD, DWORD, void *, DWORD, DWORD, HANDLE);
IMP DWORD WINAPI GetFileSize(HANDLE, DWORD *);
IMP BOOL WINAPI ReadFile(HANDLE, void *, DWORD, DWORD *, void *);
IMP BOOL WINAPI WriteFile(HANDLE, const void *, DWORD, DWORD *, void *);
IMP HANDLE WINAPI CreateMutexW(void *, BOOL, const WCHAR *);
IMP DWORD WINAPI GetLastError(void);
IMP BOOL WINAPI CopyFileW(const WCHAR *, const WCHAR *, BOOL);
IMP HANDLE WINAPI FindFirstChangeNotificationW(const WCHAR *, BOOL, DWORD);
IMP BOOL WINAPI FindNextChangeNotification(HANDLE);
IMP BOOL WINAPI FindCloseChangeNotification(HANDLE);
IMP BOOL WINAPI GetFileAttributesExW(const WCHAR *, int, void *);
IMP BOOL WINAPI CreateDirectoryW(const WCHAR *, void *);
IMP HANDLE WINAPI FindFirstFileW(const WCHAR *, void *);
IMP BOOL WINAPI FindNextFileW(HANDLE, void *);
IMP BOOL WINAPI FindClose(HANDLE);
IMP BOOL WINAPI DeleteFileW(const WCHAR *);
IMP int WINAPI RegGetValueW(void *, const WCHAR *, const WCHAR *, DWORD, DWORD *, void *, DWORD *);
IMP int WINAPI RegSetKeyValueW(void *, const WCHAR *, const WCHAR *, DWORD, const void *, DWORD);

void *memset(void *d, int c, SIZE_T n) { unsigned char *p = d; while (n--) *p++ = (unsigned char)c; return d; }
void *memcpy(void *d, const void *s, SIZE_T n) { unsigned char *p = d; const unsigned char *q = s; while (n--) *p++ = *q++; return d; }

static const WCHAR kTitle[] = L"StarDesk Dark";
static WCHAR g_exe[600], g_dir[600], g_cmd[4096];

static WCHAR lower(WCHAR c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }
static int wlen(const WCHAR *s) { int n = 0; while (s[n]) n++; return n; }
static int wieq(const WCHAR *a, const WCHAR *b) { while (*a && lower(*a) == lower(*b)) a++, b++; return lower(*a) == lower(*b); }
static void wcat(WCHAR *d, const WCHAR *s) { d += wlen(d); while ((*d++ = *s++)) {} }

static int g_theme;   /* 0 = Dark, 1 = AMOLED (the dark table in use) */
static int g_light;   /* 1 = "Match Windows" chose StarDesk's own light look: no patching */
static int g_accent;  /* 1 = use the Windows accent colour instead of the built-in cyan/blue */
static unsigned char g_pal[8][4];   /* Windows accent palette: Light3, Light2, Light1, Base, Dark1, Dark2, Dark3 */

#ifndef BUILD_ID
#define BUILD_ID L"dev"
#endif
/* Name of the marker left attached to a StarDesk process this build has themed. It includes the
   process id, the theme and this build's fingerprint, so a newer build or another theme restarts it. */
static void marker_name(WCHAR *out, DWORD pid, int any) {
    out[0] = 0; wcat(out, L"Local\\StarDeskDark-");
    WCHAR num[16]; int n = 0; do { num[n++] = (WCHAR)('0' + pid % 10); pid /= 10; } while (pid);
    WCHAR *p = out + wlen(out); while (n) *p++ = num[--n]; *p = 0;
    if (any) { wcat(out, L"-any"); return; }
    wcat(out, g_theme ? L"-amoled-" : L"-dark-");
    if (g_accent) {   /* the accent colour is part of the look: a new Windows accent means a restart */
        static const WCHAR hx[] = L"0123456789abcdef"; WCHAR a[8]; int k = 0;
        for (int i = 0; i < 3; i++) { a[k++] = hx[g_pal[3][i] >> 4]; a[k++] = hx[g_pal[3][i] & 15]; }
        a[k++] = '-'; a[k] = 0; wcat(out, a);
    }
    wcat(out, BUILD_ID);
}
static int has_marker(DWORD pid, int any) {
    WCHAR name[160]; marker_name(name, pid, any);
    HANDLE e = OpenEventW(0x00100000 /*SYNCHRONIZE*/, 0, name);
    if (!e) return 0;
    CloseHandle(e);
    return 1;
}
/* Leave the marker inside the StarDesk process, so it lives exactly as long as that StarDesk. */
static void leave_marker(HANDLE hp, DWORD pid) {
    for (int any = 0; any < 2; any++) {
        WCHAR name[160]; marker_name(name, pid, any);
        HANDLE e = CreateEventW(NULL, 1, 0, name), dup = 0;
        if (!e) continue;
        DuplicateHandle((HANDLE)-1, e, hp, &dup, 0, 0, 2 /*DUPLICATE_SAME_ACCESS*/);
        CloseHandle(e);
    }
}

/* Running copies of the StarDesk window app (bin\StarDesk.exe only):
   returns 0 = none running, 2 = already in the chosen theme (leave it), 1 = was light, now closed. */
static int g_force_restart;
static int close_running(void) {
    int light = 0, themed = 0;
    for (int pass = 0; pass < 2; pass++) {
        HANDLE snap = CreateToolhelp32Snapshot(2 /*TH32CS_SNAPPROCESS*/, 0);
        if (snap == (HANDLE)-1) return 0;
        PROCESSENTRY32W pe; memset(&pe, 0, sizeof pe); pe.dwSize = sizeof pe;
        for (BOOL ok = Process32FirstW(snap, &pe); ok; ok = Process32NextW(snap, &pe)) {
            if (!wieq(pe.exe, L"StarDesk.exe")) continue;
            HANDLE h = OpenProcess(0x1000 | 0x0400 | 0x0010 | 0x0001 | 0x00100000, 0, pe.pid);
            if (!h) continue;
            WCHAR path[600]; DWORD n = 600;
            if (QueryFullProcessImageNameW(h, 0, path, &n) && wieq(path, g_exe)) {
                /* "light" here means "not in the look we want": in light mode that's any themed copy */
                if (pass == 0) { int ok = !g_force_restart && (g_light ? !has_marker(pe.pid, 1) : has_marker(pe.pid, 0)); if (ok) themed = 1; else light = 1; }
                else { TerminateProcess(h, 0); WaitForSingleObject(h, 5000); }
            }
            CloseHandle(h);
        }
        CloseHandle(snap);
        if (pass == 0) {
            if (!light && !themed) return 0;
            if (!light) return 2;
        }
    }
    Sleep(800);
    return 1;
}

/* Undo the 'being debugged' marks so the program starts exactly as normal. */
static void clear_debug_flags(HANDLE hp) {
    PBI pbi; DWORD rl; SIZE_T n; unsigned char z = 0; DWORD gf = 0;
    if (NtQueryInformationProcess(hp, 0, &pbi, sizeof pbi, &rl) < 0) return;
    WriteProcessMemory(hp, (char *)pbi.peb + 2, &z, 1, &n);
    if (ReadProcessMemory(hp, (char *)pbi.peb + 0xBC, &gf, 4, &n)) { gf &= ~0x70u; WriteProcessMemory(hp, (char *)pbi.peb + 0xBC, &gf, 4, &n); }
}

/* Find the color table and the built-in artwork/styles inside the paused StarDesk
   and swap in the dark versions. */
static unsigned short g_pstart[65536];
static unsigned char g_pcnt[65536];

/* Qt's Windows plugin (qwindows.dll) decides whether to use its dark palette from Windows' "app mode"
   setting. StarDesk Dark is always dark, so it gives Qt its own copy of that plugin (in %LOCALAPPDATA%\StarDesk Dark\platforms,
   StarDesk's files are left alone) whose check always says "dark". With QT_QPA_PLATFORM=windows:darkmode=2 Qt then gives plain,
   un-styled widgets - text boxes and the like - a dark background instead of white. */
static const unsigned char kQwinOld[25] = {0x74,0x0f,0x83,0x7c,0x24,0x68,0x00,0x75,0x08,0xb0,0x01,0x48,0x83,0xc4,0x50,0x5b,0xc3,0x32,0xc0,0x48,0x83,0xc4,0x50,0x5b,0xc3};
typedef struct { const unsigned char *pat; unsigned len; int nfix; unsigned char off[3], val[3]; } QwFix;
static const unsigned char kQwinPal[25] = {0x48,0x8d,0x4d,0xb0,0xff,0x15,0xaa,0x83,0x0c,0x00,0xba,0x02,0x00,0x00,0x00,0x48,0x8d,0x4d,0xc8,0xff,0x15,0x1b,0x85,0x0c,0x00};
static const unsigned char kQwinDark[20] = {0x48,0x8d,0x4d,0xb0,0xff,0x15,0x26,0x83,0x0c,0x00,0xba,0x80,0x80,0x80,0x00,0x48,0x8d,0x4d,0xe8,0xff};
static const unsigned char kQwinWin[36] = {0x41,0xb8,0x01,0x00,0x00,0x00,0x48,0x8d,0x55,0xc8,0x48,0x8d,0x4d,0xb0,0xff,0x15,0xe1,0x81,0x0c,0x00,0xba,0x05,0x00,0x00,0x00,0x48,0x8b,0xcb,0x4c,0x8b,0xc8,0x44,0x8d,0x42,0x05,0xff};
/* window background brush: GetSysColorBrush(COLOR_WINDOW) (white) -> COLOR_WINDOWTEXT (black). That brush is
   what Windows shows in a new window before StarDesk has drawn its first frame - the white flash on opening. */
static const unsigned char kQwinBrush[17] = {0xff,0x15,0xd4,0x67,0x0e,0x00,0xb9,0x05,0x00,0x00,0x00,0xff,0x15,0xf1,0x85,0x0e,0x00};
static const QwFix kQwinFix[4] = {
    { kQwinPal, 25, 1, {11}, {0x13} },                      /* Qt::black -> Qt::transparent (Base, Button) */
    { kQwinDark, 20, 3, {11, 12, 13}, {0x32, 0x32, 0x32} }, /* Dark #808080 -> #323232 */
    { kQwinWin, 36, 1, {9}, {0xe8} },                       /* Window uses that #323232 colour */
    { kQwinBrush, 17, 1, {7}, {0x08} },                     /* new windows start black, not white */
};
/* every StarDesk window: after Qt marks it dark (DWMWA_USE_IMMERSIVE_DARK_MODE), also give it StarDesk's
   title-bar colour (DWMWA_CAPTION_COLOR). Otherwise, with "Show accent colour on title bars" on, Windows paints its
   accent colour into the see-through title strip of the remote-session window. */
static const unsigned char kQwinCapOld[42] = {0x41,0x8d,0x51,0x10,0xff,0x15,0x43,0x18,0x0f,0x00,0x85,0xc0,0x79,0x4e,0x41,0xb9,0x04,0x00,0x00,0x00,0x4c,0x8d,0x44,0x24,0x58,0x48,0x8b,0xcb,0x41,0x8d,0x51,0x0f,0xff,0x15,0x27,0x18,0x0f,0x00,0x85,0xc0,0x79,0x32};
static const unsigned char kQwinCapNew[32] = {0xc7,0x44,0x24,0x58,0x32,0x32,0x32,0x00,0x6a,0x04,0x41,0x59,0x4c,0x8d,0x44,0x24,0x58,0x48,0x8b,0xcb,0x6a,0x23,0x5a,0xff,0x15,0x26,0x18,0x0f,0x00,0xeb,0x33,0x90};   /* bytes 10..41; COLORREF at +4 */
/* Qt's dark palette "Light" role is a fixed #C0C0C0; the remote-session window paints its tab strip with it
   (the light grey bar). Give it StarDesk's title-bar colour instead (Dark #323232, AMOLED black). */
static const unsigned char kQwinLight[19] = {0xff,0x15,0x68,0x83,0x0c,0x00,0xba,0xc0,0xc0,0xc0,0x00,0x48,0x8d,0x4d,0xd8,0xff,0x15,0x81,0x82};   /* imm32 colour at +7 */
static const unsigned char kQwinLight2[16] = {0x48,0x8b,0xc8,0xba,0xc0,0xc0,0xc0,0x00,0x8b,0x00,0x89,0x45,0xe7,0xf2,0x0f,0x10};   /* the same #C0C0C0 in the colour picker Qt uses for dark mode; imm32 at +4 */
static WCHAR g_qwdir[600];
static unsigned char *read_all(const WCHAR *p, DWORD *len) {
    HANDLE f = CreateFileW(p, 0x80000000 /*GENERIC_READ*/, 1 | 2, NULL, 3 /*OPEN_EXISTING*/, 0, NULL);
    if (f == (HANDLE)-1) return NULL;
    DWORD hi = 0, n = GetFileSize(f, &hi), got = 0;
    unsigned char *b = (n && n < (64u << 20) && !hi) ? VirtualAlloc(NULL, n, 0x3000, 4) : NULL;
    if (b && (!ReadFile(f, b, n, &got, NULL) || got != n)) { VirtualFree(b, 0, 0x8000); b = NULL; }
    CloseHandle(f); *len = n; return b;
}
static int prepare_qwindows(const WCHAR *rootdir) {
    WCHAR src[600]; src[0] = 0; wcat(src, rootdir); wcat(src, L"\\bin\\plugins\\platforms\\qwindows.dll");
    ExpandEnvironmentStringsW(L"%LOCALAPPDATA%\\StarDesk Dark", g_qwdir, 560);
    CreateDirectoryW(g_qwdir, NULL); wcat(g_qwdir, L"\\platforms"); CreateDirectoryW(g_qwdir, NULL);
    WCHAR dst[700]; dst[0] = 0; wcat(dst, g_qwdir); wcat(dst, L"\\qwindows.dll");
    DWORD n = 0, m = 0; unsigned char *b = read_all(src, &n); if (!b) return 0;
    int found = 0;
    for (DWORD i = 0; i + 25 <= n; i++) {
        if (b[i] != 0x74 || b[i + 1] != 0x0f) continue;
        int q = 2; while (q < 25 && b[i + q] == kQwinOld[q]) q++;
        if (q < 25) continue;
        b[i + 17] = 0xb0; b[i + 18] = 0x01; found = 1; break;
    }
    /* Qt's dark palette: plain labels and boxes (Base) see-through instead of a black slab; window backgrounds
       (Window) the same dark grey as StarDesk's title bar instead of black; Qt's "Dark" shade to that grey too */
    for (int k = 0; found && k < 4; k++) {
        const unsigned char *pat = kQwinFix[k].pat; unsigned len = kQwinFix[k].len;
        for (DWORD i = 0; i + len <= n; i++) {
            if (b[i] != pat[0] || b[i + 1] != pat[1]) continue;
            unsigned q = 2; while (q < len && b[i + q] == pat[q]) q++;
            if (q < len) continue;
            for (int j = 0; j < kQwinFix[k].nfix; j++) b[i + kQwinFix[k].off[j]] = kQwinFix[k].val[j];
            break;
        }
    }
    for (DWORD i = 0; found && i + 42 <= n; i++) {
        if (b[i] != kQwinCapOld[0] || b[i + 1] != kQwinCapOld[1]) continue;
        unsigned q = 2; while (q < 42 && b[i + q] == kQwinCapOld[q]) q++;
        if (q < 42) continue;
        for (int j = 0; j < 32; j++) b[i + 10 + j] = kQwinCapNew[j];
        DWORD col = g_theme ? 0x000000 : 0x323232;          /* AMOLED black, Dark #323232 */
        b[i + 14] = (unsigned char)col; b[i + 15] = (unsigned char)(col >> 8); b[i + 16] = (unsigned char)(col >> 16); b[i + 17] = 0;
        break;
    }
    for (DWORD i = 0; found && i + sizeof kQwinLight <= n; i++) {
        if (b[i] != kQwinLight[0] || b[i + 1] != kQwinLight[1]) continue;
        unsigned q = 2; while (q < sizeof kQwinLight && b[i + q] == kQwinLight[q]) q++;
        if (q < sizeof kQwinLight) continue;
        DWORD col = g_theme ? 0x000000 : 0x323232;
        b[i + 7] = (unsigned char)col; b[i + 8] = (unsigned char)(col >> 8); b[i + 9] = (unsigned char)(col >> 16);
        break;
    }
    for (DWORD i = 0; found && i + sizeof kQwinLight2 <= n; i++) {
        if (b[i] != kQwinLight2[0] || b[i + 1] != kQwinLight2[1]) continue;
        unsigned q = 2; while (q < sizeof kQwinLight2 && b[i + q] == kQwinLight2[q]) q++;
        if (q < sizeof kQwinLight2) continue;
        DWORD col = g_theme ? 0x000000 : 0x323232;
        b[i + 4] = (unsigned char)col; b[i + 5] = (unsigned char)(col >> 8); b[i + 6] = (unsigned char)(col >> 16);
        break;
    }
    int ok = 0;
    if (found) {
        unsigned char *c = read_all(dst, &m);
        if (c && m == n) { ok = 1; for (DWORD i = 0; i < n; i++) if (c[i] != b[i]) { ok = 0; break; } }
        if (c) VirtualFree(c, 0, 0x8000);
        /* the old copy can still be loaded by a StarDesk that is just closing: retry for a few seconds */
        for (int tries = 0; !ok && tries < 20; tries++) {
            if (tries) Sleep(250);
            HANDLE f = CreateFileW(dst, 0x40000000 /*GENERIC_WRITE*/, 0, NULL, 2 /*CREATE_ALWAYS*/, 0, NULL);
            if (f != (HANDLE)-1) { DWORD w = 0; ok = WriteFile(f, b, n, &w, NULL) && w == n; CloseHandle(f); }
        }
    }
    VirtualFree(b, 0, 0x8000);
    return ok;
}

/* After an update restart StarDesk comes back hidden in the tray. Bring its window up the way a click on its
   tray icon does: post the tray icon's own "selected" notification to the icon's message window. */
static DWORD g_client_pid;
static void *g_tray; static int g_visible;
static int WINAPI find_client_windows(void *h, long long lp) {
    DWORD pid = 0; GetWindowThreadProcessId(h, &pid);
    if (pid != g_client_pid) return 1;
    WCHAR cls[64]; int n = GetClassNameW(h, cls, 64);
    if (n >= 15 && cls[0] == 'Q' && cls[1] == 'T' && cls[2] == 'r' && cls[3] == 'a' && cls[4] == 'y') g_tray = h;   /* QTrayIconMessageWindowClass */
    else if (IsWindowVisible(h)) { int r[4]; if (GetWindowRect(h, r) && r[2] - r[0] > 400 && r[3] - r[1] > 300) g_visible = 1; }
    return 1;
}
static void show_client(void) {
    if (!g_client_pid) return;
    for (int t = 0; t < 40; t++) {                 /* wait (up to 20 s) for StarDesk to put its icon in the tray */
        g_tray = NULL; g_visible = 0; EnumWindows(find_client_windows, 0);
        if (g_visible) return;
        if (g_tray) break;
        Sleep(500);
    }
    if (!g_tray) return;
    Sleep(2000);                                   /* let it finish starting up */
    g_tray = NULL; g_visible = 0; EnumWindows(find_client_windows, 0);
    if (g_visible || !g_tray) return;
    PostMessageW(g_tray, 0x8000 + 101 /* WM_APP+101: Qt's tray notification */, 0, 0x0400 /* NIN_SELECT: a click */);
    Sleep(1500);
    g_tray = NULL; g_visible = 0; EnumWindows(find_client_windows, 0);
    if (g_visible || !g_tray) return;
    PostMessageW(g_tray, 0x8000 + 101, 0, 0x0203 /* WM_LBUTTONDBLCLK */);
}

static void flush(HANDLE hp, unsigned char *rbase, unsigned char *buf, SIZE_T s, SIZE_T e) {
    DWORD old; SIZE_T n;
    if (!VirtualProtectEx(hp, rbase + s, e - s, 0x04, &old) && !VirtualProtectEx(hp, rbase + s, e - s, 0x08, &old)) return;
    WriteProcessMemory(hp, rbase + s, buf + s, e - s, &n);
    VirtualProtectEx(hp, rbase + s, e - s, old, &old);
}

/* Accent colours. The palette comes from Windows (Settings > Personalization > Colors). */
static unsigned lum(const unsigned char *c) { return (2126u * kLin[c[0]] + 7152u * kLin[c[1]] + 722u * kLin[c[2]]) / 10000u; }
static const unsigned char *accent_shade(int code) {
    if (code < 10) return g_pal[code == 7 ? 6 : code];
    /* buttons with white text: the lightest shade white still reads on (4.5:1 means luminance <= 0.1833) */
    int i = 2; while (i < 6 && lum(g_pal[i]) > 183333u) i++;
    i += code - 10; if (i > 6) i = 6;
    return g_pal[i];
}
static unsigned short g_afirst[NPAT]; static unsigned char g_acnt[NPAT];
static void accent_bytes(unsigned char *dst, int j) {
    if (!g_accent) return;
    for (unsigned f = g_afirst[j]; f < (unsigned)g_afirst[j] + g_acnt[j]; f++) {
        const AccentFix *x = &kAccentFix[f];
        if (x->theme != g_theme) continue;
        const unsigned char *c = accent_shade(x->shade);
        unsigned char *h = dst + x->off; int upper = 0;
        for (int q = 0; q < 6; q++) if (h[q] >= 'A' && h[q] <= 'F') upper = 1;
        const char *hx = upper ? "0123456789ABCDEF" : "0123456789abcdef";
        for (int q = 0; q < 3; q++) { h[2 * q] = hx[c[q] >> 4]; h[2 * q + 1] = hx[c[q] & 15]; }
    }
}

static int patch(HANDLE hp) {
    PBI pbi; DWORD rl; unsigned char *base = 0; SIZE_T got;
    if (NtQueryInformationProcess(hp, 0, &pbi, sizeof pbi, &rl) < 0) return 1;
    if (!ReadProcessMemory(hp, (char *)pbi.peb + 0x10, &base, 8, &got)) return 2;
    unsigned char hdr[4096];
    if (!ReadProcessMemory(hp, base, hdr, sizeof hdr, &got)) return 3;
    DWORD nt = *(DWORD *)(hdr + 0x3c);
    if (nt > 3000) return 4;
    unsigned nsec = *(unsigned short *)(hdr + nt + 6), opt = *(unsigned short *)(hdr + nt + 20);
    unsigned char *sec = hdr + nt + 24 + opt;
    for (int f = NACCFIX - 1; f >= 0; f--) { g_afirst[kAccentFix[f].pat] = (unsigned short)f; g_acnt[kAccentFix[f].pat]++; }
    for (int i = 0; i < NPAT; i++) {
        unsigned k = kPatOld[i][0] | (kPatOld[i][1] << 8);
        if (!g_pcnt[k]) g_pstart[k] = (unsigned short)i;
        g_pcnt[k]++;
    }
    const SIZE_T CH = 8u << 20, OV = MAXPAT > TOKEN_COUNT * 4 ? MAXPAT : TOKEN_COUNT * 4, TB = TOKEN_COUNT * 4;
    unsigned char *buf = VirtualAlloc(NULL, CH + OV, 0x3000, 4);
    if (!buf) return 5;
    int table = 8;
    for (unsigned s = 0; s < nsec && s < 40; s++, sec += 40) {
        int is_rsrc = sec[0] == '.' && sec[1] == 'r' && sec[2] == 's';
        int is_data = sec[0] == '.' && sec[1] == 'd' && sec[2] == 'a' && sec[3] == 't' && sec[4] == 'a' && !sec[5];
        /* .rdata, .rsrc, and .data (Qt string literals: UTF-16 stylesheets of the newer screens) */
        if (!(is_data || (sec[0] == '.' && sec[1] == 'r' && (sec[2] == 'd' || is_rsrc)))) continue;
        DWORD vsize = *(DWORD *)(sec + 8), va = *(DWORD *)(sec + 12);
        for (SIZE_T off = 0; off < vsize; off += CH) {
            SIZE_T want = vsize - off; if (want > CH + OV) want = CH + OV;
            if (!ReadProcessMemory(hp, base + va + off, buf, want, &got)) continue;
            unsigned char *rb = base + va + off;
            SIZE_T limit = (got == CH + OV) ? CH : got;
            /* 1. the 255-color table */
            if (table != 0 && !is_rsrc && !is_data)
            for (SIZE_T i = 0; i + TB <= got && i < limit; i += 4) {
                const unsigned int *t = (const unsigned int *)(buf + i);
                if (t[0] != kLight[0] || t[1] != kLight[1] || t[2] != kLight[2] || t[3] != kLight[3] ||
                    t[4] != kLight[4] || t[5] != kLight[5] || t[6] != kLight[6] || t[7] != kLight[7]) continue;
                unsigned int nw[TOKEN_COUNT]; int match = 0;
                for (int k = 0; k < TOKEN_COUNT; k++) {
                    if (t[k] == kLight[k]) { nw[k] = g_theme ? kAmoled[k] : kDark[k]; match++; } else nw[k] = t[k];
                    int sh = g_theme ? kAccentShadeAmoled[k] : kAccentShadeDark[k];
                    if (g_accent && sh >= 0 && t[k] == kLight[k]) {
                        const unsigned char *c = accent_shade(sh);
                        nw[k] = (nw[k] & 0xFF000000u) | ((unsigned)c[0] << 16) | ((unsigned)c[1] << 8) | c[2];
                    }
                }
                if (match < TOKEN_COUNT * 3 / 4) continue;
                memcpy(buf + i, nw, TB);
                flush(hp, rb, buf, i, i + TB);
                table = 0;
                break;
            }
            /* 2. icons, pictures and hard-coded style colors */
            SIZE_T ps = 0, pe = 0; int pending = 0;
            for (SIZE_T i = 0; i + 2 <= got && i < limit; i++) {
                unsigned k = buf[i] | (buf[i + 1] << 8);
                unsigned c = g_pcnt[k];
                if (!c) continue;
                for (unsigned j = g_pstart[k]; j < g_pstart[k] + c; j++) {
                    unsigned L = kPatLen[j];
                    if (i + L > got) continue;
                    const unsigned char *o = kPatOld[j];
                    unsigned q = 2; while (q < L && buf[i + q] == o[q]) q++;
                    if (q < L) continue;
                    memcpy(buf + i, g_theme ? kPatNewA[j] : kPatNew[j], L);
                    accent_bytes(buf + i, j);
                    if (pending && (i > pe + 4096 || i + L - ps > 65536)) { flush(hp, rb, buf, ps, pe); pending = 0; }
                    if (!pending) { ps = i; pending = 1; }
                    pe = i + L;
                    i += L - 1;
                    break;
                }
            }
            if (pending) flush(hp, rb, buf, ps, pe);
        }
    }
    /* 3. a few instructions in the code: which colour the remote-session tabs are painted with */
    sec = hdr + nt + 24 + opt;
    for (unsigned s = 0; s < nsec && s < 40; s++, sec += 40) {
        if (!(sec[0] == '.' && sec[1] == 't' && sec[2] == 'e' && sec[3] == 'x' && sec[4] == 't')) continue;
        DWORD vsize = *(DWORD *)(sec + 8), va = *(DWORD *)(sec + 12);
        for (SIZE_T off = 0; off < vsize; off += CH) {
            SIZE_T want = vsize - off; if (want > CH + CODELEN) want = CH + CODELEN;
            if (!ReadProcessMemory(hp, base + va + off, buf, want, &got)) continue;
            SIZE_T limit = (got == CH + CODELEN) ? CH : got;
            for (SIZE_T i = 0; i + CODELEN <= got && i < limit; i++) {
                for (int j = 0; j < NCODE; j++) {
                    if (buf[i] != kCodeOld[j][0] || buf[i + 1] != kCodeOld[j][1]) continue;
                    int L = kCodeLen[j], q = 2; while (q < L && buf[i + q] == kCodeOld[j][q]) q++;
                    if (q < L) continue;
                    unsigned char *at = base + va + off + i; DWORD old; SIZE_T n;
                    if (VirtualProtectEx(hp, at, L, 0x40 /*PAGE_EXECUTE_READWRITE*/, &old)) {
                        WriteProcessMemory(hp, at, g_theme ? kCodeNewA[j] : kCodeNew[j], L, &n);
                        VirtualProtectEx(hp, at, L, old, &old);
                        FlushInstructionCache(hp, at, L);
                    }
                }
            }
        }
    }
    VirtualFree(buf, 0, 0x8000);
    return table;
}

#define HKCU ((void *)(long long)(int)0x80000001)
static const WCHAR kKey[] = L"Software\\StarDesk Dark";
static DWORD read_dword(const WCHAR *key, const WCHAR *name, DWORD def) {
    DWORD v = 0, sz = 4;
    return RegGetValueW(HKCU, key, name, 0x10 /*RRF_RT_REG_DWORD*/, NULL, &v, &sz) == 0 ? v : def;
}
/* 0 = Dark, 1 = AMOLED, 2 = Match Windows */
static int read_theme(void) { DWORD v = read_dword(kKey, L"Theme", 0); return v <= 2 ? (int)v : 0; }
static void save_dword(const WCHAR *name, DWORD v) { RegSetKeyValueW(HKCU, kKey, name, 4 /*REG_DWORD*/, &v, 4); }
static void save_theme(int t) { save_dword(L"Theme", (DWORD)t); }
/* Is Windows set to dark mode for apps? (Settings > Personalization > Colors > Choose your mode) */
static int windows_is_dark(void) {
    return read_dword(L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", L"AppsUseLightTheme", 1) == 0;
}
static int read_accent_palette(void) {
    DWORD sz = sizeof g_pal;
    if (RegGetValueW(HKCU, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Accent", L"AccentPalette",
                     0x8 /*RRF_RT_REG_BINARY*/, NULL, g_pal, &sz) != 0 || sz < 28) return 0;
    return 1;
}

/* Task dialog structures (Windows packs these to 1 byte) */
#pragma pack(push, 1)
typedef struct { int id; const WCHAR *text; } TDBUTTON;
typedef struct {
    DWORD cbSize; void *hwndParent; void *hInstance; int dwFlags; int dwCommonButtons;
    const WCHAR *title; void *mainIcon; const WCHAR *mainInstruction; const WCHAR *content;
    DWORD cButtons; const TDBUTTON *buttons; int defButton;
    DWORD cRadio; const TDBUTTON *radios; int defRadio;
    const WCHAR *verification, *expanded, *expandedCtl, *collapsedCtl; void *footerIcon; const WCHAR *footer;
    void *callback; long long cbData; DWORD cxWidth;
} TDCONFIG;
#pragma pack(pop)
typedef int (WINAPI *TaskDialogIndirectFn)(const TDCONFIG *, int *, int *, BOOL *);

/* The settings window. Returns 1 if the user applied a theme, 0 if closed. */
static int settings_dialog(void) {
    int cur = read_theme(), btn = 0, radio = 100 + cur; BOOL acc = read_dword(kKey, L"UseAccent", 0) != 0;
    void *cc = LoadLibraryW(L"comctl32.dll");
    TaskDialogIndirectFn td = cc ? (TaskDialogIndirectFn)GetProcAddress(cc, "TaskDialogIndirect") : 0;
    if (td) {
        static const TDBUTTON radios[] = {
            { 100, L"Dark  (MuMuPlayer style: neutral grays, cyan accents)" },
            { 101, L"AMOLED  (pure black, neutral grays, no blue tint)" },
            { 102, L"Match Windows  (Dark when Windows is in dark mode, StarDesk's normal look when it's light)" } };
        static const TDBUTTON buttons[] = { { 200, L"Apply" } };
        TDCONFIG c; memset(&c, 0, sizeof c);
        c.cbSize = sizeof c; c.hInstance = GetModuleHandleW(NULL);
        c.dwFlags = 0x2 /*USE_HICON_MAIN*/ | 0x8 /*ALLOW_CANCEL (X)*/ | 0x1000000 /*SIZE_TO_CONTENT*/ | (acc ? 0x100 /*VERIFICATION_FLAG_CHECKED*/ : 0);
        c.verification = L"Use my Windows accent colour (instead of cyan / blue)";
        c.dwCommonButtons = 0x8 /*Cancel*/;
        c.title = L"StarDesk Dark Settings";
        c.mainIcon = LoadIconW(c.hInstance, (const WCHAR *)1);
        c.mainInstruction = L"Theme";
        c.content = L"Choose how StarDesk looks when you open it with StarDesk Dark. All text is tuned to meet the WCAG AA contrast guidelines.\n\nApply restarts StarDesk with the theme you picked. Any remote session open in StarDesk will be closed.";
        c.cButtons = 1; c.buttons = buttons; c.defButton = 200;
        c.cRadio = 3; c.radios = radios; c.defRadio = 100 + cur;
        if (td(&c, &btn, &radio, &acc) != 0 || btn != 200) return 0;
        save_theme(radio >= 100 && radio <= 102 ? radio - 100 : 0);
        save_dword(L"UseAccent", acc ? 1 : 0);
        return 1;
    }
    /* fallback if the modern dialog isn't available */
    int r = MessageBoxW(NULL, L"Use the AMOLED theme (pure black)?\n\nYes = AMOLED\nNo = Dark\nCancel = keep the current theme",
                        L"StarDesk Dark Settings", 0x3 | 0x20 | 0x40000);
    if (r == 6) { save_theme(1); return 1; }
    if (r == 7) { save_theme(0); return 1; }
    return 0;
}

/* ---- Update watcher --------------------------------------------------------------------------
   A tiny background helper (a copy of this launcher, run from %LOCALAPPDATA%\StarDesk Dark) that waits
   for StarDeskDark.exe to be replaced by a newer version. When that happens and StarDesk is open, it
   runs the new launcher once, which restarts StarDesk with the new look - no clicks needed. It uses no
   CPU while waiting (Windows wakes it only when a file in the StarDesk folder changes). */
static WCHAR g_self[600];
typedef struct { DWORD attr, c[2], a[2], w[2], hi, lo; } FILEINFO;   /* WIN32_FILE_ATTRIBUTE_DATA */
static unsigned long long write_time(const WCHAR *p) {
    FILEINFO fi; if (!GetFileAttributesExW(p, 0, &fi)) return 0;
    return ((unsigned long long)fi.w[1] << 32 | fi.w[0]) ^ ((unsigned long long)fi.lo << 1);
}
static int client_running(void) {
    HANDLE snap = CreateToolhelp32Snapshot(2, 0); int found = 0;
    if (snap == (HANDLE)-1) return 0;
    PROCESSENTRY32W pe; memset(&pe, 0, sizeof pe); pe.dwSize = sizeof pe;
    for (BOOL ok = Process32FirstW(snap, &pe); ok && !found; ok = Process32NextW(snap, &pe)) {
        if (!wieq(pe.exe, L"StarDesk.exe")) continue;
        HANDLE h = OpenProcess(0x1000, 0, pe.pid);
        if (!h) continue;
        WCHAR path[600]; DWORD n = 600;
        if (QueryFullProcessImageNameW(h, 0, path, &n) && wieq(path, g_exe)) found = 1;
        CloseHandle(h);
    }
    CloseHandle(snap);
    return found;
}
static void watch_for_updates(const WCHAR *launcher) {
    HANDLE mx = CreateMutexW(NULL, 1, L"Local\\StarDeskDarkWatcher");
    if (!mx || GetLastError() == 183 /*ALREADY_EXISTS*/) ExitProcess(0);
    WCHAR dir[600]; dir[0] = 0; wcat(dir, launcher);
    int n = wlen(dir); while (n && dir[n - 1] != '\\') n--; dir[n ? n - 1 : 0] = 0;
    unsigned long long t = write_time(launcher);
    HANDLE ch = FindFirstChangeNotificationW(dir, 0, 0x10 /*LAST_WRITE*/ | 0x8 /*SIZE*/ | 0x1 /*FILE_NAME*/);
    if (ch == (HANDLE)-1) ExitProcess(0);
    for (;;) {
        if (WaitForSingleObject(ch, 0xFFFFFFFF) != 0) break;
        Sleep(3000);                                  /* let the copy finish */
        unsigned long long t2 = write_time(launcher);
        if (t2 && t2 != t) {
            FindCloseChangeNotification(ch); CloseHandle(mx);   /* the new launcher starts a fresh watcher */
            if (client_running()) {
                STARTUPINFOW si; PROCESS_INFORMATION pi;
                memset(&si, 0, sizeof si); si.cb = sizeof si; memset(&pi, 0, sizeof pi);
                WCHAR cmd[700]; cmd[0] = 0; wcat(cmd, L"\""); wcat(cmd, launcher); wcat(cmd, L"\" --updated");
                if (CreateProcessW(launcher, cmd, NULL, NULL, 0, 0, NULL, dir, &si, &pi)) { CloseHandle(pi.hThread); CloseHandle(pi.hProcess); }
            } else {
                STARTUPINFOW si; PROCESS_INFORMATION pi;    /* StarDesk closed: just refresh the watcher */
                memset(&si, 0, sizeof si); si.cb = sizeof si; memset(&pi, 0, sizeof pi);
                WCHAR cmd[700]; cmd[0] = 0; wcat(cmd, L"\""); wcat(cmd, launcher); wcat(cmd, L"\" --startwatcher");
                if (CreateProcessW(launcher, cmd, NULL, NULL, 0, 0, NULL, dir, &si, &pi)) { CloseHandle(pi.hThread); CloseHandle(pi.hProcess); }
            }
            ExitProcess(0);
        }
        FindNextChangeNotification(ch);
    }
    ExitProcess(0);
}
/* Every build leaves its own watcher-<build>.exe copy (about 6 MB) in %LOCALAPPDATA%\\StarDesk Dark.
   Delete the copies of older builds so they don't pile up. A copy that is still running is locked by
   Windows, so DeleteFileW just fails for it and it is removed on a later start. */
static void remove_old_watchers(void) {
    WCHAR dir[600], pat[640]; ExpandEnvironmentStringsW(L"%LOCALAPPDATA%\\StarDesk Dark\\", dir, 560);
    pat[0] = 0; wcat(pat, dir); wcat(pat, L"watcher-*.exe");
    WCHAR cur[64]; cur[0] = 0; wcat(cur, L"watcher-"); wcat(cur, BUILD_ID); wcat(cur, L".exe");
    struct { DWORD attr; DWORD t[6]; DWORD sizeHigh, sizeLow, r0, r1; WCHAR name[260]; WCHAR alt[14]; DWORD pad[4]; } fd;
    HANDLE h = FindFirstFileW(pat, &fd);
    if (h == (HANDLE)-1) return;
    do {
        if (wieq(fd.name, cur) || wlen(fd.name) > 200) continue;
        WCHAR p[900]; p[0] = 0; wcat(p, dir); wcat(p, fd.name);
        DeleteFileW(p);
    } while (FindNextFileW(h, &fd));
    FindClose(h);
}
/* Make sure the watcher for this exact build is running (copy of us in %LOCALAPPDATA%). */
static void ensure_watcher(void) {
    remove_old_watchers();
    HANDLE mx = CreateMutexW(NULL, 0, L"Local\\StarDeskDarkWatcher");
    int exists = mx && GetLastError() == 183;
    if (mx) CloseHandle(mx);
    if (exists) return;
    WCHAR dst[600]; ExpandEnvironmentStringsW(L"%LOCALAPPDATA%\\StarDesk Dark", dst, 560);
    CreateDirectoryW(dst, NULL);
    wcat(dst, L"\\watcher-"); wcat(dst, BUILD_ID); wcat(dst, L".exe");
    if (GetFileAttributesW(dst) == (DWORD)-1 && !CopyFileW(g_self, dst, 0)) return;
    STARTUPINFOW si; PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof si); si.cb = sizeof si; memset(&pi, 0, sizeof pi);
    WCHAR cmd[1400]; cmd[0] = 0; wcat(cmd, L"\""); wcat(cmd, dst); wcat(cmd, L"\" --watch \""); wcat(cmd, g_self); wcat(cmd, L"\"");
    if (CreateProcessW(dst, cmd, NULL, NULL, 0, 0x08000000 /*CREATE_NO_WINDOW*/, NULL, NULL, &si, &pi)) { CloseHandle(pi.hThread); CloseHandle(pi.hProcess); }
}

void WinMainCRTStartup(void) {
    /* Where StarDesk lives */
    /* StarDesk is normally right next to this launcher (installed into the StarDesk folder);
       otherwise fall back to the standard install location */
    {
        DWORD n = GetModuleFileNameW(NULL, g_dir, 560);
        while (n && g_dir[n - 1] != '\\') n--;
        g_dir[n] = 0; wcat(g_dir, L"bin");
        WCHAR probe[600]; probe[0] = 0; wcat(probe, g_dir); wcat(probe, L"\\StarDesk.exe");
        if (!n || GetFileAttributesW(probe) == (DWORD)-1) {
            ExpandEnvironmentStringsW(L"%ProgramFiles%\\StarDesk\\bin", g_dir, 600);
            if (GetFileAttributesW(g_dir) == (DWORD)-1) { g_dir[0] = 0; wcat(g_dir, L"C:\\Program Files\\StarDesk\\bin"); }
        }
    }
    g_exe[0] = 0; wcat(g_exe, g_dir); wcat(g_exe, L"\\StarDesk.exe");
    if (GetFileAttributesW(g_exe) == (DWORD)-1) {
        MessageBoxW(NULL, L"Couldn't find StarDesk. Put StarDeskDark.exe in the StarDesk folder (next to StarDesk.exe).", kTitle, 0x10);
        ExitProcess(1);
    }

    /* Pass our own arguments through (for example --autostart) */
    WCHAR *cl = GetCommandLineW(); int autostart = 0;
    if (*cl == '"') { cl++; while (*cl && *cl != '"') cl++; if (*cl) cl++; }
    else while (*cl && *cl != ' ' && *cl != '\t') cl++;
    while (*cl == ' ' || *cl == '\t') cl++;
    if (cl[0] == '-' && cl[1] == '-' && cl[2] == 'w' && cl[3] == 'a' && cl[4] == 't' && cl[5] == 'c' && cl[6] == 'h') {
        WCHAR *p = cl + 7; while (*p == ' ') p++;
        if (*p == '"') p++;
        WCHAR launcher[600]; int k = 0; while (*p && *p != '"' && k < 599) launcher[k++] = *p++; launcher[k] = 0;
        watch_for_updates(launcher);
    }
    GetModuleFileNameW(NULL, g_self, 599);
    if (wieq(cl, L"--startwatcher")) { ensure_watcher(); ExitProcess(0); }
    int updated = 0;
    if (wieq(cl, L"--updated")) { updated = 1; g_force_restart = 1; cl += wlen(cl); }
    ensure_watcher();
    if (wieq(cl, L"--autostart")) { autostart = 1; cl += wlen(cl); }
    if (wieq(cl, L"--settings")) {
        cl += wlen(cl);
        if (!settings_dialog()) ExitProcess(0);
    }
    {
        int t = read_theme();
        if (t == 2) { g_theme = 0; g_light = !windows_is_dark(); } else g_theme = t;
        g_accent = read_dword(kKey, L"UseAccent", 0) && read_accent_palette();
    }
    if (autostart) Sleep(3000);   /* let StarDesk's own login start happen first */

    /* StarDesk's own helpers sometimes start the window program again by themselves (light) right
       after it is closed, or StarDesk's launcher just shows an existing copy instead of starting a new
       one. Either way nothing got themed - so close whatever is running and try again (up to 3 times). */
    static WCHAR root[600], rootdir[600];
    rootdir[0] = 0; wcat(rootdir, g_dir); rootdir[wlen(rootdir) - 4] = 0;   /* strip "\bin" */
    root[0] = 0; wcat(root, rootdir); wcat(root, L"\\StarDesk.exe");
    g_cmd[0] = 0; wcat(g_cmd, L"\""); wcat(g_cmd, root); wcat(g_cmd, L"\"");
    if (*cl) { wcat(g_cmd, L" "); wcat(g_cmd, cl); }
    int r = -1;
    for (int attempt = 0; attempt < 3 && r != 0; attempt++) {
        if (attempt) Sleep(1500);
        int running = close_running();
        STARTUPINFOW si; PROCESS_INFORMATION pi;
        memset(&si, 0, sizeof si); si.cb = sizeof si; memset(&pi, 0, sizeof pi);
        if (running == 2 || g_light) {
            /* already in the right look, or Windows is light: start / bring up StarDesk normally */
            if (CreateProcessW(root, g_cmd, NULL, NULL, 0, 0, NULL, rootdir, &si, &pi)) { CloseHandle(pi.hThread); CloseHandle(pi.hProcess); }
            ExitProcess(0);
        }
        int qwin = prepare_qwindows(rootdir);
        if (qwin) {
            SetEnvironmentVariableW(L"QT_QPA_PLATFORM_PLUGIN_PATH", g_qwdir);
            SetEnvironmentVariableW(L"QT_QPA_PLATFORM", L"windows:darkmode=2");
        }
        /* StarDesk's web pages (Feedback, message centre, cloud devices, purchase history) run in WebView2:
           ask its Chromium engine for dark mode - pages that support it switch to their own dark style,
           the rest get Chromium's automatic dark rendering */
        SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS", L"--force-dark-mode --enable-features=WebContentsForceDark");
        if (!CreateProcessW(root, g_cmd, NULL, NULL, 0, 0x1 /*DEBUG_PROCESS*/, NULL, rootdir, &si, &pi)) {
            MessageBoxW(NULL, L"Couldn't start StarDesk.", kTitle, 0x10);
            ExitProcess(1);
        }
        if (qwin) { SetEnvironmentVariableW(L"QT_QPA_PLATFORM_PLUGIN_PATH", NULL); SetEnvironmentVariableW(L"QT_QPA_PLATFORM", NULL); }
        SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS", NULL);
        DebugSetProcessKillOnExit(0);
        DWORD t0 = GetTickCount(); int launcher_alive = 1;
        static unsigned char ev[256] __attribute__((aligned(16)));
        while (launcher_alive && GetTickCount() - t0 < 30000) {
            if (!WaitForDebugEvent(ev, 500)) continue;
            DWORD code = *(DWORD *)ev, pid = *(DWORD *)(ev + 4), tid = *(DWORD *)(ev + 8);
            DWORD cont = 0x00010002; /* DBG_CONTINUE */
            int detach = 0;
            if (code == 3) {                       /* a process was created */
                HANDLE hf = *(HANDLE *)(ev + 16), hp = *(HANDLE *)(ev + 24);
                if (hf) CloseHandle(hf);
                clear_debug_flags(hp);
                if (pid != pi.pid) {
                    WCHAR path[600]; DWORD n = 600;
                    if (QueryFullProcessImageNameW(hp, 0, path, &n) && wieq(path, g_exe)) { r = patch(hp); if (r == 0) { leave_marker(hp, pid); g_client_pid = pid; } }
                    detach = 1;
                }
            } else if (code == 6) {                /* DLL loaded */
                HANDLE hf = *(HANDLE *)(ev + 16); if (hf) CloseHandle(hf);
            } else if (code == 1) {                /* exception */
                DWORD ex = *(DWORD *)(ev + 16);
                if (ex != 0x80000003u && ex != 0x4000001Fu) cont = 0x80010001; /* not ours: pass it on */
            } else if (code == 5 && pid == pi.pid) launcher_alive = 0;
            ContinueDebugEvent(pid, tid, cont);
            if (detach) DebugActiveProcessStop(pid);
            if (r >= 0) break;                     /* window program handled: let go of the launcher */
        }
        if (launcher_alive) DebugActiveProcessStop(pi.pid);
        CloseHandle(pi.hThread); CloseHandle(pi.hProcess);
        if (r > 0) break;                          /* themed program found but colours not recognised: retrying won't help */
    }
    if (r == 0 && updated) {
        /* restarted after an update: StarDesk comes back hidden in the tray, so show the window again
           (starting StarDesk's launcher while it runs just brings its window to the front) */
        show_client();
    }
    if (r != 0 && !autostart && !updated)
        MessageBoxW(NULL, r < 0 ? L"StarDesk started, but dark mode couldn't be applied this time (StarDesk's window program didn't start the usual way)."
                                : L"StarDesk started, but dark mode couldn't be applied this time.\n\nA StarDesk update may have changed its colors; the dark mode needs updating for the new version.",
                    kTitle, 0x30);
    ExitProcess(0);
}
