#define TESTHOST
#define WinMainCRTStartup real_entry
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define memset my_memset
#define memcpy my_memcpy
#include "sddark.c"
#undef memset
#undef memcpy
static unsigned char *img; static struct { char pad[0x10]; void *base; } fakepeb;
NTSTATUS NtQueryInformationProcess(HANDLE h, int c, void *p, DWORD n, DWORD *r){ PBI *b=p; b->peb=&fakepeb; return 0; }
BOOL ReadProcessMemory(HANDLE h,const void*a,void*b,SIZE_T n,SIZE_T*g){ memcpy(b,a,n); if(g)*g=n; return 1; }
BOOL WriteProcessMemory(HANDLE h,void*a,const void*b,SIZE_T n,SIZE_T*g){ memcpy(a,b,n); if(g)*g=n; return 1; }
BOOL VirtualProtectEx(HANDLE h,void*a,SIZE_T n,DWORD p,DWORD*o){ *o=2; return 1; }
void *VirtualAlloc(void*a,SIZE_T n,DWORD t,DWORD p){ return calloc(1,n); }
BOOL FlushInstructionCache(HANDLE h,const void*a,SIZE_T n){ return 1; }
BOOL VirtualFree(void*a,SIZE_T n,DWORD t){ free(a); return 1; }
int main(int argc,char**argv){
  FILE*f=fopen(argv[1],"rb"); fseek(f,0,2); long sz=ftell(f); fseek(f,0,0); unsigned char*d=malloc(sz); fread(d,1,sz,f); fclose(f);
  unsigned nt=*(unsigned*)(d+0x3c); unsigned soi=*(unsigned*)(d+nt+24+56), soh=*(unsigned*)(d+nt+24+60);
  img=calloc(1,soi); memcpy(img,d,soh);
  unsigned ns=*(unsigned short*)(d+nt+6), opt=*(unsigned short*)(d+nt+20); unsigned char*s=d+nt+24+opt;
  for(unsigned i=0;i<ns;i++,s+=40){ unsigned va=*(unsigned*)(s+12), rs=*(unsigned*)(s+16), ro=*(unsigned*)(s+20); memcpy(img+va,d+ro,rs); }
  fakepeb.base=img;
  g_theme=atoi(argv[3]);
  if(argc>4){ static const unsigned char pals[2][7][3]={{{0x99,0xEB,0xFF},{0x4C,0xC2,0xFF},{0x00,0x91,0xF8},{0x00,0x78,0xD4},{0x00,0x67,0xC0},{0x00,0x3E,0x92},{0x00,0x1A,0x68}},
     {{0xFF,0xB3,0xD9},{0xFF,0x8C,0xC6},{0xF2,0x5C,0xAB},{0xE3,0x00,0x8C},{0xB8,0x00,0x71},{0x8A,0x00,0x55},{0x5C,0x00,0x38}}};
     int p=atoi(argv[4]); g_accent=1; for(int i=0;i<7;i++){ g_pal[i][0]=pals[p][i][0]; g_pal[i][1]=pals[p][i][1]; g_pal[i][2]=pals[p][i][2]; g_pal[i][3]=0; } }
  int r=patch(0);
  FILE*o=fopen(argv[2],"wb"); fwrite(img,1,soi,o); fclose(o);
  printf("patch result %d\n",r); return 0;
}
