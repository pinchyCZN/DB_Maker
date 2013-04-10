#include <windows.h>
#include <commctrl.h>
#include <stdio.h>


BYTE swizdata[]={
0x86,0x00,0x00,0x00,0xFB,0x00,0x00,0x00,0xEC,0x00,0x00,0x00,0x37,0x00,0x00,0x00,
0x5D,0x00,0x00,0x00,0x44,0x00,0x00,0x00,0x9C,0x00,0x00,0x00,0xFA,0x00,0x00,0x00,
0xC6,0x00,0x00,0x00,0x5E,0x00,0x00,0x00,0x28,0x00,0x00,0x00,0xE6,0x00,0x00,0x00,
0x13,0x00,0x00,0x00,0xB6,0x00,0x00,0x00,0x8A,0x00,0x00,0x00,0x60,0x00,0x00,0x00,
0x54,0x00,0x00,0x00,0x94,0x00,0x00,0x00,0x7B,0x00,0x00,0x00,0x36,0x00,0x00,0x00,
0xF5,0x00,0x00,0x00,0x72,0x00,0x00,0x00,0xDF,0x00,0x00,0x00,0xB1,0x00,0x00,0x00,
0x77,0x00,0x00,0x00,0xF4,0x00,0x00,0x00,0x13,0x00,0x00,0x00,0x43,0x00,0x00,0x00,
0xCF,0x00,0x00,0x00,0xAF,0x00,0x00,0x00,0xB1,0x00,0x00,0x00,0x33,0x00,0x00,0x00,
0x34,0x00,0x00,0x00,0x61,0x00,0x00,0x00,0x79,0x00,0x00,0x00,0x5B,0x00,0x00,0x00,
0x92,0x00,0x00,0x00,0xB5,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0x2A,0x00,0x00,0x00
};
int check_mdb_type(BYTE *buf)
{
	if(strcmp(buf+4,"Standard Jet DB")==0){
		if(buf[0x14]==1)
			return 4;
		else if(buf[0x14]==0)
			return 3;
		else
			return -2;
	}
	return -1;
}
int check_mdb_header(BYTE *response,BYTE *buf)
{
	int type;
	type=check_mdb_type(buf);
	if(type>0){
		response[0x12A]=type;
		return 1;
	}
	else{
		response[0x12A]=0;
		sprintf(response+0x2A,"Invalid or unsupported Jet/Access Database format.");
		return 0;
	}
}
int swizzle_1(int a1, int a2, int a3, int a4, int a5, int a6)
{
  unsigned __int8 v6; // cl@1
  char v7; // bl@1
  int result; // eax@4
  int v9; // eax@10
  signed int v10; // edx@10
  signed int v11; // esi@10
  int v12; // eax@14
  int v13; // eax@18
  int v14; // eax@22
  signed int v15; // [sp+10h] [bp-4h]@15
  signed int v16; // [sp+Ch] [bp-8h]@19
  signed int v17; // [sp+8h] [bp-Ch]@23

  a2 ^= 0x80u;
  a3 ^= 0x6Cu;
  a4 ^= 0x3Eu;
  v6 = (BYTE)a5 ^ 0x60;
  v7 = (BYTE)a6 ^ 0x66;
  if ( ((BYTE)a6 ^ 0x66) == 127 )
  {
    result = v6 < 0xD0u || v6 >= 0xE0u;
  }
  else
  {
    if ( v7 == -1 )
    {
      if ( v6 < 0xF4u )
        result = 0;
      else
        result = -1;
    }
    else
    {
      v10 = v6 + ((unsigned __int8)v7 % 128 % 127 << 8);
      v11 = v10 - (v10 - v10 % 16 - 16);
      v9 = (v10 - 48) >> 4;
      if ( v9 < 0 )
      {
        if ( v9 < 0 )
          v11 >>= (char)-(BYTE)v9;
      }
      else
      {
        v11 <<= (char)v9;
      }
      v12 = (v10 - 176) >> 4;
      if ( v12 < 0 )
      {
        if ( v12 < 0 )
          v15 = (signed int)(unsigned __int8)a4 >> -(BYTE)v12;
      }
      else
      {
        v15 = (unsigned __int8)a4 << v12;
      }
      v13 = (v10 - 304) >> 4;
      if ( v13 < 0 )
      {
        if ( v13 < 0 )
          v16 = (signed int)(unsigned __int8)a3 >> -(BYTE)v13;
      }
      else
      {
        v16 = (unsigned __int8)a3 << v13;
      }
      v14 = (v10 - 432) >> 4;
      if ( v14 < 0 )
      {
        if ( v14 < 0 )
          v17 = (signed int)(unsigned __int8)a2 >> -(BYTE)v14;
      }
      else
      {
        v17 = (unsigned __int8)a2 << v14;
      }
      result = v17 + v16 + v11 + v15;
      if ( v7 == 128 )
        result = -result;
    }
  }
  return result;
}
int swizzle_2(int a1,int seed)
{
	int result; // eax@1
	BYTE *v3; // edx@1
	int v4; // ecx@1
	int v5; // esi@2
	int v6; // ebx@1
	BYTE v7[4]; // [sp+8h] [bp-4h]@4
	int i;
	for(i=0;i<4;i++)
		v7[i]=seed>>(8*i);
	v6 = a1;
	memset(a1 + 299, 0, 40);
	result = 0;
	v4 = v6 + 299;
	v3 = &swizdata;
	while ( (char)result < 40 )
	{
		v5 = (char)result & 0x80000003;
		if ( v5 < 0 )
			v5 = ((v5 - 1) | 0xFFFFFFFC) + 1;
		*(BYTE *)v4 = *(BYTE *)v3 ^ v7[v5&3];
		++result;
		++v4;
		v3 = (char *)v3 + 4;
	}
	return result;
}

int decrypt(LPWSTR lpWideCharStr, int a2)
{
  int v2; // edi@1
  LPWSTR v3; // esi@1
  int result; // al@2
  int v5; // eax@3
  signed int v6; // edx@3
  int v7; // ecx@3
  int v8; // eax@4
  char v9; // bl@4
  int v10; // eax@6
  int v11; // ecx@6
  int v12; // edi@6
  int v13; // ST14_4@4
  int v14; // ST10_4@4
  int v15; // ecx@4
  int v16; // eax@4
  BYTE MultiByteStr[20]; // [sp+Ch] [bp-14h]@9

  v2 = a2;
  v3 = lpWideCharStr;
  if ( (unsigned __int8)check_mdb_header(lpWideCharStr, a2) )
  {
    v5 = memset(v3, 0, 42);
    v6 = *((BYTE *)v3 + 298);
    if ( v6 < 4 )
    {
      v8 = 0;
      v9 = 20;
    }
    else
    {
      LOBYTE(v7) = *(BYTE *)(v2 + 121);
      v13 = v7;
      LOBYTE(v5) = *(BYTE *)(v2 + 120);
      v14 = v5;
      LOBYTE(v6) = *(BYTE *)(v2 + 119);
      LOBYTE(v15) = *(BYTE *)(v2 + 118);
      LOBYTE(v16) = *(BYTE *)(v2 + 117);
      v8 = swizzle_1(v3, v16, v15, v6, v14, v13);
      v9 = 40;
    }
    swizzle_2(v3, v8);
    v11 = 0;
    v12 = v2 + 66;
    v10 = (int)((char *)v3 + 299);
    if ( v9 > 0 )
    {
      do
      {
        *(BYTE *)(v10 - 299) = *(BYTE *)v10 ^ *(BYTE *)v12;
        ++v11;
        ++v10;
        ++v12;
      }
      while ( v9 > (BYTE)v11 );
    }
    if ( (signed int)*((BYTE *)v3 + 298) < 4 )
    {
      memset(MultiByteStr, 0, 20);
      strncpy(MultiByteStr, v3, 20);
      memset(v3, 0, 42);
      mbstowcs(v3, MultiByteStr, 20);
    }
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

int get_password(char *fname,char *p,int plen,int copy_to_clip)
{
	FILE *f;
	BYTE buffer[0x80];
	BYTE pwdata[0x200];
	int result=FALSE;
	f=fopen(fname,"rb");
	if(f!=0){
		memset(buffer,0,sizeof(buffer));
		fread(buffer,1,0x80,f);
		fclose(f);
		memset(pwdata,0,sizeof(pwdata));
		if(decrypt(pwdata,buffer)){
			memset(p,0,plen);
			if(_mbslen(pwdata)==0)
				printf("no password DB type=%i\n",pwdata[0x12A]);
			else{
				wcstombs(p,(const SHORT*)pwdata,plen-1);
				printf("DB type=%i\npassword=[%s]\n",pwdata[0x12A],p);
				if(copy_to_clip && text_to_clip(p))
					printf("password copied to windows clipboard\n");
			}
			result=TRUE;
		}
		else
			printf("some error decrypting:%s\n",pwdata+0x2A);

	}
	else
		printf("cant open file %s\n",fname);
	return result;
}

int text_to_clip(BYTE *str)
{
	int len;
	HANDLE h;
	LPSTR lpszData;
	int result=FALSE;
	if(OpenClipboard(NULL)!=0){
		EmptyClipboard();
		len=strlen(str);
		h=GlobalAlloc(GMEM_ZEROINIT, len+1);
		if(h!=NULL){
			lpszData=(LPSTR)GlobalLock(h);
			memcpy(lpszData, str, len+1);
			GlobalUnlock(h);
			SetClipboardData(CF_TEXT, h);
			result=TRUE;
		}
		CloseClipboard();
	}
	return result;
}

//***********************************************************************//
UINT CALLBACK OFNHookProc(HWND hDlg,UINT Msg,WPARAM wParam,LPARAM lParam)
{

	HWND hWnd;
	RECT rect;
	static int init_size=TRUE,init_details=TRUE;
	static int scroll_pos=0;
	static int last_selection=0;

	switch(Msg)
	{
	case WM_INITDIALOG:
		init_details=TRUE;
		SetForegroundWindow(hDlg);
		return 0; //0=dialog process msg, nonzero=ignore
	case WM_NOTIFY:
        PostMessage( hDlg, WM_APP + 1, 0, 0 ); 
		return 0;
	case WM_APP + 1:
		{ 
			int i;
			HWND const dlg      = GetParent( hDlg ); 
			HWND defView  = GetDlgItem( dlg, 0x0461 ); 
			HWND list = GetDlgItem(defView,1);
			if(init_details)
			{
				SendMessage( defView, WM_COMMAND, 28716, 0 ); //details view
				ListView_EnsureVisible(list,last_selection,FALSE);
				ListView_SetItemState(list,last_selection,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
				SetFocus(list);
				init_details=FALSE;
			}
			if(ListView_GetItemCount(list)>0)
			{
				ListView_SetColumnWidth(list,0,LVSCW_AUTOSIZE);
				ListView_SetColumnWidth(list,1,LVSCW_AUTOSIZE);
				ListView_SetColumnWidth(list,2,LVSCW_AUTOSIZE);
				ListView_SetColumnWidth(list,3,LVSCW_AUTOSIZE);
			}
			i=ListView_GetNextItem(list,-1,LVNI_SELECTED);
			if(i>=0)
				last_selection=i;

			hWnd=GetDesktopWindow();
			if(init_size && GetWindowRect(hWnd, &rect)) //only do at start , later resizing operations remain
			{
				SetWindowPos(dlg,HWND_TOP,0,0,(int)(rect.right*.75),(int)(rect.bottom*.75),NULL);
				init_size=FALSE;
			}
		} 
		return TRUE;
	case WM_DESTROY:
		return 0;
		break;
	default:  
		return 0;
	}

}
int OpenFileR(char *title)
{
	static TCHAR szFilter[] = TEXT ("mdb Files\0*.mdb;\0\0") ;
	static TCHAR szFileName [MAX_PATH],  startpath[MAX_PATH];
	static TCHAR szTitleName [MAX_PATH];
	static OPENFILENAME ofn ;
	memset(&ofn,0,sizeof (OPENFILENAME));
	memset(szFileName,0,sizeof(szFileName));

	ofn.lStructSize       = sizeof (OPENFILENAME) ;
	ofn.lpstrFilter       = szFilter ;
	ofn.lpstrFile         = szFileName ;          // Set in Open and Close functions
	ofn.nMaxFile          = MAX_PATH ;
	ofn.lpstrFileTitle    = szTitleName ;          // Set in Open and Close functions
	ofn.nMaxFileTitle     = MAX_PATH ;
	ofn.lpfnHook		  = OFNHookProc;
	ofn.Flags			  = OFN_ENABLEHOOK|OFN_EXPLORER|OFN_ENABLESIZING;
	if(title!=0)
		ofn.lpstrTitle=title;

	strcpy(startpath,".");
	ofn.lpstrInitialDir   = startpath;


	if(GetOpenFileName (&ofn)!=0)
		return ofn.lpstrFile;
	return 0;
}
int custom_sleep(int milliseconds)
{
	int i,index=0;
	char wait[4]={'|','/','-','\\'};
	for(i=0;i<milliseconds;i+=50)
	{
		Sleep(50);
		if(kbhit()!=0)
			return -1;
		if((i%150)==0)
			printf("%c\r",wait[(index++)%4]);
	}
	return 0;
}
#ifndef NODECRYPTMAIN
int main(int argc,char **argv)
{
	BYTE password[255];
	char *fname;
	if(argc<=1){
		fname=OpenFileR("select password protected MDB file");
		if(fname==0){
			printf("no file selected\n");
			custom_sleep(1200);
			return FALSE;
		}
		get_password(fname,password,sizeof(password),TRUE);
		custom_sleep(1200);
	}
	else{
		get_password(argv[1],password,sizeof(password),TRUE);
		custom_sleep(500);
	}
	printf("\n");
	return 0;
}
#endif