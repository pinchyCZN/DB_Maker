#include "stdafx.h"
#include "afxdb.h"
#include "DB_MAKER.h"
#include <conio.h>
#include <assert.h>
using namespace std;


int DUMP_TABLE_FIELDS=FALSE;
int VERIFY_ALL_FIELDS=FALSE;
int LOG_SQL_INSERT=FALSE;
int LOG_SQL_EXCEPTION=FALSE;
int DISABLE_INI_CHECK=FALSE;
int CREATE_NEW_INI_FILE=FALSE;
#define __INIFILE__ "DBMAKER.ini"
#define __NEWINI__ "NEWDBMAKER.ini"
#define SHIFT9 999999999999

char *g_dbpassword=0;
char *start_path=0;
extern "C" int get_password(char *fname,char *p,int plen,int copy_to_clip);

class DBMaker
{
public:
	CDatabase db;
	CString SQLFields;
	CString SQLParams;
	CString SQLValues;
	char *table_name;
	int	field_count;
	int field_counter;
	int insert_count;
	DWORD elapsed_time;
	~DBMaker(){
	};
	DBMaker(){
		table_name="";
		insert_count=field_counter=field_count=0;
		SQLFields=SQLParams=SQLValues="";
		elapsed_time=0;
	};
	int open_db(CString *dbname);
	int close(){
		db.Close();
		if(insert_count>0)
			printf("executed %i inserts, ",insert_count);
		printf("done.\n\n");
		return TRUE;
	};
	int execute_sql_insert();
	int get_fields_array();
	int check_field_exist(char *field)
	{
		CString fname;
		CString tmp;
		fname.Format("[%s]",field);
		fname.MakeUpper();
		tmp=SQLFields;
		tmp.MakeUpper();
		if(tmp.Find(fname)==-1)
			return FALSE;
		else
			return TRUE;

	};
	int add_param(char *fieldname){
		if(field_counter!=0)
			SQLParams+=",";
		SQLParams+="[";
		SQLParams+=fieldname;
		SQLParams+="]";
		return TRUE;
	};
	int ADDVALUE(char *fieldname,char *fmt,...){
		va_list ap;
		if(check_field_exist(fieldname))
		{
			add_param(fieldname);
			CString tstr;
			va_start(ap, fmt);
			tstr.FormatV(fmt,ap);
			va_end(ap);
			if(field_counter!=0)
				SQLValues+=",";
			SQLValues+=tstr;
			field_counter++;
			return TRUE;
		}
		else if(VERIFY_ALL_FIELDS){
			add_param(fieldname);
			field_counter++;
		}
		return FALSE;
	};
	int SKIP_VALUE(char *fieldname,char *fmt,...){
		if(VERIFY_ALL_FIELDS){
			add_param(fieldname);
			field_counter++;
		}
		return TRUE;
	};
};
int DBMaker::execute_sql_insert()
{
	if(CREATE_NEW_INI_FILE){
		printf("(creating new ini file,no SQL insert done)\n");
		return FALSE;
	}
	if(VERIFY_ALL_FIELDS){
		if(field_count!=field_counter)
		{
			CString str;
			FILE *flog;
			flog=fopen("log.txt","a");
			printf("*****All fields not accounted for!\n");
			printf("field_count=%i\n",field_count);
			printf("field_counter=%i\n",field_counter);
			str.Format("%s fc=%i,fcounter=%i\n",table_name,field_count,field_counter);
			fputs(str,flog);
			str.Format("%s\n",SQLFields);
			fputs(str,flog);
			str.Format("%s\n\n",SQLParams);
			fputs(str,flog);
			fclose(flog);
			return FALSE;
		}
		else
		{
			printf("++all fields accounted for\n");
			return FALSE;
		}
	}
	else{
		if(!db.IsOpen()){
			printf("error:database is not open\n");
			return FALSE;
		}
		CString SqlString;
		SqlString.Format("INSERT INTO [%s] (%s) VALUES(%s);\n",table_name,SQLParams,SQLValues);
		field_counter=0; //reset these for next insert
		SQLParams="";
		SQLValues="";
		TRY{
			if(LOG_SQL_INSERT){
				FILE *flog;
				flog=fopen("log_sql_insert.txt","a");
				fputs(SqlString,flog);
				fclose(flog);
			}
			db.ExecuteSQL(SqlString);
			insert_count++;
			if(GetTickCount() > (elapsed_time+250)){
				printf("%i       \r",insert_count);
				elapsed_time=GetTickCount();
			}
			return TRUE;
		}
		CATCH(CDBException, e){
			if(LOG_SQL_EXCEPTION){
				FILE *flog;
				flog=fopen("log.txt","a");
				fputs(SqlString,flog);
				fclose(flog);
			}
			CString error = ">ERROR: " + e->m_strError;
			cout<< (LPCSTR)error;
			if(strstr(e->m_strError,"Feature is not available")!=0)
				return TRUE;
			else
				return FALSE;
		}
		END_CATCH
	}
}
int DBMaker::get_fields_array()
{
	CString SqlString;
	int i;

	if(!db.IsOpen()){
		printf("error:database is not open\n");
		return 0;
	}
	CRecordset rec1( &db );

	SqlString = 
	"SELECT * FROM [";
	SqlString+=table_name;
	SqlString+="]; ";
	TRY{
		rec1.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);
	}
	CATCH(CDBException, e){
		CString error = ">ERROR: " + e->m_strError;
		cout<< (LPCSTR)error;
		rec1.Close();
		return 0;
	}
	END_CATCH
	field_count=rec1.GetODBCFieldCount();

	SQLFields="";

	for(i=0;i<field_count;i++) //output field names 
	{
		CODBCFieldInfo finfo;
		rec1.GetODBCFieldInfo((short)i,finfo);

		SQLFields+="[";
		SQLFields+=finfo.m_strName;
		SQLFields+="]";
		SQLFields+=",";
	}
	rec1.Close();
	return field_count;
}

#define ADDVALUE dbm.ADDVALUE
#define SKIP_VALUE dbm.SKIP_VALUE


int key_ctrl=FALSE;
int key_shift=FALSE;
int extended_key=FALSE;
int getkey()
{
	int i=0;
	key_ctrl=FALSE;
	key_shift=FALSE;
	extended_key=FALSE;
	i=getch();
	if (GetKeyState(VK_SHIFT) < 0)
		key_shift=TRUE;
	if(GetKeyState(VK_CONTROL) < 0)
		key_ctrl=TRUE;
	if(i==0 || i==0xE0)
	{
		i=getch();
		extended_key=TRUE;
	}
	return i&0xFF;

}
int getkey2()
{
	int i=0;
	key_ctrl=FALSE;
	key_shift=FALSE;
	extended_key=FALSE;
	if(kbhit())
	{
		i=getch();
		if(i==0 || i==0xE0)
		{
			i=getch();
			extended_key=TRUE;
		}
		if (GetKeyState(VK_SHIFT) < 0)
			key_shift=TRUE;
		if(GetKeyState(VK_CONTROL) < 0)
			key_ctrl=TRUE;

	}
	return i&0xFF;

}
int custom_sleep(int milliseconds)
{
	int i;
	for(i=0;i<milliseconds;i+=50)
	{
		Sleep(50);
		if(getkey2()!=0)
			return -1;
	}
	return 0;
}
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



static TCHAR szTitleName [MAX_PATH];
char * OpenFileR(char *title)
{
	static TCHAR szFilter[] = TEXT ("mdb Files\0*.mdb;\0\0") ;
	static TCHAR szFileName [MAX_PATH],  startpath[MAX_PATH];
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

	GetOpenFileName (&ofn) ;
	return ofn.lpstrFile;
}

int OpenDB(CString *dbfile) 
{
	*dbfile=OpenFileR("Open database");
	if(dbfile->IsEmpty())
		return FALSE;
	return TRUE;
}
int set_system_time(SYSTEMTIME *time,int year,int month,int day,int hour,int min,int sec)
{
	memset(time,0,sizeof(SYSTEMTIME));
	GetSystemTime(time);
	time->wDayOfWeek=0;
	time->wMilliseconds=0;
	if(year!=0)
		time->wYear=year;
	if(month!=0)
		time->wMonth=month;
	if(day!=0)
		time->wDay=day;
	time->wHour=hour;
	time->wMinute=min;
	time->wSecond=sec;
	return 0;
}
int increment_time(SYSTEMTIME *time, int hour, int min, int sec)
{
	FILETIME ft;               
	SystemTimeToFileTime(time,&ft);
	LARGE_INTEGER li = {ft.dwLowDateTime,ft.dwHighDateTime};
	li.QuadPart += (LONGLONG)1000*1000*10 * (LONGLONG)sec; //*10nano2micro *1000micro2mili *1000mili2seconds
	li.QuadPart += (LONGLONG)60*1000*1000*10 * (LONGLONG)min; //*10nano2micro *1000micro2mili *1000mili2seconds *60minutes
	li.QuadPart += (LONGLONG)60*60*1000*1000*10 * (LONGLONG)hour; //*10nano2micro *1000micro2mili *1000mili2seconds *60mins*60secs
	ft.dwHighDateTime = li.HighPart;
	ft.dwLowDateTime = li.LowPart;
	FileTimeToSystemTime(&ft, time);
	return TRUE;
}
int get_field_count(CDatabase *db,char *table)
{
	CString SqlString;
	int rec_count;

	CRecordset rec( db );

	SqlString = 
	"SELECT TOP 1 * FROM [";
	SqlString+=table;
	SqlString+="]; ";

	rec.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);
	rec_count=rec.GetODBCFieldCount();
	rec.Close();
	if(rec_count<=0)
		cout<<"error getting rec count\n";
	return rec_count;

}
int get_fields(CDatabase *db,char *table)
{
	CString str,SqlString;
	FILE *flog=0;
	int i;

	CRecordset rec1( db );

	flog=fopen("table_fields.txt","a");
	str.Format("\n\n%s\n",table);
	fputs(str,flog);

	cout<<"getting "<<table<<" fields, logging to table_fields.txt\n";
	SqlString = 
	"SELECT * FROM ";
	SqlString+=table;
	//SqlString+="";

	rec1.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);
	for(i=0;i<rec1.GetODBCFieldCount();i++) //output string code
	{
		CODBCFieldInfo finfo;
		rec1.GetODBCFieldInfo((short)i,finfo);

		str.Format("ADDVALUE");
		fputs(str,flog);
		if(finfo.m_nSQLType>=11) //11,12 use single quotes
			str.Format("(\"%s\",\"''\",count); //%i,%i\n",finfo.m_strName,finfo.m_nSQLType,finfo.m_nPrecision);
		else
			str.Format("(\"%s\",\"\",count); //%i,%i\n",finfo.m_strName,finfo.m_nSQLType,finfo.m_nPrecision);
		fputs(str,flog);
		fflush(flog);
	}
	if(flog!=0)
		fclose(flog);

	rec1.Close();
	return 0;
}

int check_table_exist(CDatabase *db,char *table)
{
	CString SqlString;

	TRY{
		SqlString = 
			"SELECT TOP 1 * FROM [";
		SqlString+=table;
		SqlString+="]; ";
		db->ExecuteSQL(SqlString);
	}
	CATCH(CDBException, e){
		return FALSE;
	}
	END_CATCH

	return TRUE;
}
int check_field_exist(CString *SQLFields,char *field)
{
	CString fname;
	CString tmp;
	fname.Format("[%s]",field);
	fname.MakeUpper();
	tmp=*SQLFields;
	tmp.MakeUpper();
	if(tmp.Find(fname)==-1)
		return FALSE;
	else
		return TRUE;
}

int open_database(CDatabase *db,CString *dbname)
{
	int type=0;
	CString connect;
	CString tstr;
	tstr=*dbname;
	tstr.MakeLower();
	if(tstr.Find(".db")>=0)
		type=0;
	else
		type=1;
	switch(type)
	{
	default:
	case 0: //DB fox pro
		//UID=dba;PWD=********;DBN=Journal;DBF=C:\Journal Manager\Journal.db;ASTOP=YES;DSN=Journal;INT=NO;DBG=YES;LOG=C:\Journal Manager\log.txt;DMRF=NO;LINKS=SharedMemory;COMP=NO
			connect.Format("ODBC;UID=dba;PWD=sql;DBN=Journal;DBF=C:\\Journal Manager\\Journal.db;ASTOP=YES;DSN=Journal;INT=NO;DBG=YES;DMRF=NO;LINKS=SharedMemory;COMP=NO",*dbname);
//			connect.Format("ODBC;Driver={Microsoft Visual FoxPro Driver};SourceType=DBC;DSN='';DBQ=%s",*dbname);
		break;
	case 1: //SQL
		//connect.Format("ODBC;DRIVER={SQL Server};%s;Integrated Security=SSPI",*dbname);
		connect.Format("ODBC;%s",*dbname);
		printf("%s\n",connect);
		break;
	}
	int success=0;
	TRY{
		success=db->Open(NULL,FALSE,FALSE,connect,FALSE);
	}
	CATCH(CDBException, e){
		CString error = ">ERROR: " + e->m_strError;
		cout<< (LPCSTR)error;
	}
	END_CATCH
	return success!=0?TRUE:FALSE;
}
int DBMaker::open_db(CString *dbname)
{
	int result=open_database(&db,dbname);
	if(result)
		return TRUE;
	else{
		cout<<"cant open database "<<dbname<<endl;
		exit(0);
	}
	return FALSE;		
}
int get_table_index(CDatabase *db,char *table_name,char *field_name,int *index)
{
	int result=FALSE;
	CString SQLstr;
	if(!db->IsOpen())
		return result;
	SQLstr.Format("SELECT TOP 1 ([%s]) FROM [%s];",field_name,table_name);
	CRecordset rec(db);

	TRY{
		rec.Open(CRecordset::snapshot,SQLstr,CRecordset::readOnly);
		CString str;
		if(!rec.IsEOF())
			rec.GetFieldValue(field_name,str);
		*index=atoi(str);
		result=TRUE;
	}
	CATCH(CDBException, e){
		CString error = ">ERROR: " + e->m_strError;
		cout<< (LPCSTR)error;
	}
	END_CATCH
	if(rec.IsOpen())
		rec.Close();
	return result;
}
int set_table_index(CDatabase *db,char *table_name,char *field_name,char *where,char *value)
{
	CString SQLstr;
	if(!db->IsOpen())
		return FALSE;
	SQLstr.Format("UPDATE [%s] SET [%s]='%s' %s;",table_name,field_name,value,where);
	TRY{
		db->ExecuteSQL(SQLstr);
		return TRUE;
	}
	CATCH(CDBException, e){
		if(LOG_SQL_EXCEPTION){
		FILE *flog;
		flog=fopen("log.txt","a");
		fputs(SQLstr,flog);
		fclose(flog);
		}
		CString error = ">ERROR: " + e->m_strError;
		cout<< (LPCSTR)error;
		return FALSE;
	}
	END_CATCH
}
int delete_table_records(CDatabase *db,char *table_name,char *where_statement)
{
	if(CREATE_NEW_INI_FILE){
		printf("(no deleting done, new ini being created)\n");
		return TRUE;
	}
	if(VERIFY_ALL_FIELDS){
		printf("(no deleting done, verifying all fields)\n");
		return TRUE;
	}
	if(DUMP_TABLE_FIELDS)
	{
		get_fields(db,table_name);
		return FALSE;
	}
	else
	{
		CString SqlString;
		TRY{
			SqlString.Format("DELETE FROM [%s] %s;",table_name,where_statement);
			printf("deleting from [%s]\n",table_name);
			db->SetQueryTimeout(60*5);
			db->ExecuteSQL(SqlString);
		}
		CATCH(CDBException, e){
			printf(">error:unable to delete [%s], table might not exist\n",table_name);
			return FALSE;
		}
		END_CATCH
	}
	return TRUE;
}
char *create_uuid()
{
	static char str[80];
	UUID uuid;
	RPC_STATUS result=UuidCreate(&uuid);
	str[0]=0;
	if(result==RPC_S_OK){
		unsigned char *p=0;
		UuidToString(&uuid,&p);
		if(p!=0){
			strncpy(str,(const char*)p,sizeof(str));
			str[sizeof(str)-1]=0;
			strupr(str);
			RpcStringFree(&p);
		}
	}
	return str;
}
int get_string_ini_status(char *str,char *key,int *found_key,int *commented)
{
	char *a,*b;
	int klen=strlen(key);
	char tmp[256]={0};
	_snprintf(tmp,sizeof(tmp),"[%s]",key);
	tmp[sizeof(tmp)-1]=0;
		
	if((b=strstr(str,tmp))!=0)
	{
		*found_key=TRUE;
		*commented=FALSE;
		if((a=strstr(str,";"))!=0) 
			if(a<b)
				*commented=TRUE;
		if((a=strstr(str,"//"))!=0)
			if(a<b)
				*commented=TRUE;
		return TRUE;
	}
	else 
		return FALSE;
}
int get_ini_path(char *path)
{
	char drive[_MAX_DRIVE],dir[_MAX_DIR];
	char full[_MAX_DIR];
	_splitpath(start_path,drive,dir,NULL,NULL);
	sprintf(full,"%s%s",drive,dir);
	strcpy(path,full);
	return 0;
}
int add_key_new_ini(char *key)
{
	char str[80];
	char fname[MAX_PATH];
	FILE *f;
	get_ini_path(fname);
	strcat(fname,__NEWINI__);
	f=fopen(fname,"a");
	if(f!=0){
		_snprintf(str,sizeof(str),"[%s]\n",key);
		fputs(str,f);
		fclose(f);
	}
	return TRUE;
}
int check_ini_file(char *key)
{
	FILE *f=0;
	char str[80];
	char fname[MAX_PATH];
	int err=0;
	int disable=FALSE;
	if(CREATE_NEW_INI_FILE) return add_key_new_ini(key);
	get_ini_path(fname);
	strcat(fname,__INIFILE__);

	f=fopen(fname,"rt");
	if(f==0)
		return FALSE; //default to false if no INI found
	int found_key=FALSE;
	int commented=FALSE;
	while((found_key==FALSE) || (commented==TRUE))
	{
		memset(str,0,sizeof(str));
		if(fgets(str,sizeof(str)-1,f)!=0)
		{
			if(strncmp(str,"break",5)==0)
				disable=TRUE;
			if(strncmp(str,"resume",6)==0)
				disable=FALSE;
			if(!disable)
				get_string_ini_status(str,key,&found_key,&commented);
			if(found_key && (!commented))
				break;
		}
		else
			break;
	}
	fclose(f);
	if(!found_key)
		printf("key not found %s\n",key);

	return found_key && (!commented);
}
int write_newini_str(char *section,char *key,CString *str)
{
	char fname[MAX_PATH];
	get_ini_path(fname);
	strcat(fname,__NEWINI__);
	WritePrivateProfileString(section,key,str->GetBuffer(str->GetLength()),fname);
	str->ReleaseBuffer();
	return TRUE;
}
int get_ini_str(char *section,char *key,CString *str)
{
	char fstr[1024];
	char fname[MAX_PATH];
	DWORD err;
	if(CREATE_NEW_INI_FILE) return write_newini_str(section,key,str);
	get_ini_path(fname);
	strcat(fname,__INIFILE__);

	memset(fstr,0,sizeof(fstr));
	err=GetPrivateProfileString(section,key,"",fstr,sizeof(fstr)-1,fname);
	if(err==0)
		return FALSE;
	str->Format("%s",fstr);
	return TRUE;
}
int write_newini_val(char *section,char *key,int *val)
{
	char fname[MAX_PATH];
	char str[80];
	get_ini_path(fname);
	strcat(fname,__NEWINI__);
	_snprintf(str,sizeof(str),"%u",val[0]);
	WritePrivateProfileString(section,key,str,fname);
	return TRUE;
}
int get_ini_value(char *section,char *key,int *val)
{
	char fname[MAX_PATH];
	DWORD err;
	if(CREATE_NEW_INI_FILE) return write_newini_val(section,key,val);
	get_ini_path(fname);
	strcat(fname,__INIFILE__);
	//printf("%s\n",fname);
	err=GetPrivateProfileInt(section,key,0xFFFFFFFF,fname);
	if(err==0xFFFFFFFF)
		return FALSE;
	*val=err;
	return TRUE;
}
int write_newini_systime(char *section,char *key)
{
	char fname[MAX_PATH];
	get_ini_path(fname);
	strcat(fname,__NEWINI__);
	WritePrivateProfileString(section,key,"2000/1/1 00:00:00",fname);
	return TRUE;
}
int get_ini_systime(char *section,char *key,SYSTEMTIME *systime)
{
	char fstr[255],*s;
	char fname[MAX_PATH];
	int year,mon,day;
	int hour,min,sec;
	DWORD err;
	if(CREATE_NEW_INI_FILE) return write_newini_systime(section,key);
	get_ini_path(fname);
	strcat(fname,__INIFILE__);

	err=GetPrivateProfileString(section,key,"",fstr,sizeof(fstr)-1,fname);
	if(err==0)
		return FALSE;

	err=sscanf(fstr,"%d/%d/%d",&year,&mon,&day);
	if(err==3)
	{
		systime->wYear=year;
		systime->wMonth=mon;
		systime->wDay=day;
	}
	s=strstr(fstr,":");
	if(s!=0)
	{
		s-=2;
		err=sscanf(s,"%d:%d:%d",&hour,&min,&sec);
		if(err==3)
		{
			systime->wHour=hour;
			systime->wMinute=min;
			systime->wSecond=sec;
		}
	}
	return TRUE;
}
int check_ini_globalvars()
{
	if(CREATE_NEW_INI_FILE){
		FILE *f;
		char fname[MAX_PATH];
		get_ini_path(fname);
		strcat(fname,__NEWINI__);
		f=fopen(fname,"a");
		if(f!=0){
			fprintf(f,";[LOG_SQL_INSERT]\n;[LOG_SQL_EXCEPTION]\n;[VERIFY_ALL_FIELDS]\n;[DUMP_TABLE_FIELDS]\n; VMN-VEHICLE\n; COM-VEHICLE,USER,EITHER,BOTH\n; VVI-USER\n; OTR-VEHICLE\n; KEYLESS-VEHICLE\n;break,resume\n");
			fclose(f);
		}
		return TRUE;
	}
	if(check_ini_file("LOG_SQL_EXCEPTION"))
	{
		cout<<"loggin SQL exceptions\n";
		LOG_SQL_EXCEPTION=TRUE;
	}
	if(check_ini_file("LOG_SQL_INSERT"))
	{
		cout<<"loggin SQL insert statements\n";
		LOG_SQL_INSERT=TRUE;
	}
	if(check_ini_file("DUMP_TABLE_FIELDS"))
	{
		cout<<"DUMP_TABLE_FIELDS enabled\n";
		DUMP_TABLE_FIELDS=TRUE;
	}
	if(check_ini_file("VERIFY_ALL_FIELDS"))
	{
		cout<<"VERIFY_ALL_FIELDS enabled\n";
		VERIFY_ALL_FIELDS=TRUE;
	}
	return TRUE;
}
int ini_exist()
{
	FILE *f=0;
	char fname[MAX_PATH];
	if(CREATE_NEW_INI_FILE)return TRUE;
	get_ini_path(fname);
	strcat(fname,__INIFILE__);
	f=fopen(fname,"rt");
	if(f==0)
		return FALSE;
	else
		fclose(f);
	return TRUE;
}
void wait_exit()
{
	DWORD time=GetTickCount();
	while((GetTickCount()-time)<30000)
	{
		if(getkey2()>0)
			return;
		Sleep(100);
	}
}
int store_num=104;
#include "Commercial\h_shift.h"
#include "Commercial\h_event.h"
#include "Commercial\h_acct.h"
#include "Commercial\h_ticket.h"
#include "Commercial\DAY_CLOSE_CUTOFF.h"

#include "OASIS\MOPMAP.h"
#include "OASIS\pb_combo.h"
#include "OASIS\pb_combo_old.h"
#include "OASIS\pb_conv.h"
#include "OASIS\pb_item.h"
#include "OASIS\pb_pgrp.h"
#include "OASIS\pb_pos.h"
#include "OASIS\pb_retl.h"
#include "OASIS\pb_upc.h"
#include "OASIS\pbchglog.h"
#include "OASIS\taxmap.h"
#include "OASIS\pb_mixm.h"

#include "DPALM\posplu.h"
#include "DPALM\posplux.h"

int parse_args(int argc,TCHAR *argv[],CString *dbname)
{
	int i;
	char tstr[255];
	char server[255],dbase[255];
	int serverfound=0,dbfound=0;
	for(i=1;i<argc;i++)
	{
		sprintf(tstr,argv[i]);
		strupr(tstr);
		if(strstr(tstr,"SERVER=")!=0)
		{
			sprintf(server,"%s",argv[i]+sizeof("SERVER="));
			serverfound=TRUE;
		}
		else if(strstr(tstr,"DATABASE=")!=0)
		{
			sprintf(dbase,"%s",argv[i]+sizeof("DATABASE="));
			dbfound=TRUE;
		}

	}
	if(dbfound && serverfound)
	{
		dbname->Format("SERVER=%s;DATABASE=%s",server,dbase);
		return TRUE;
	}
	else if(strstr(tstr,".MDB")!=0)
	{
		dbname->Format("%s",argv[1]);
		return TRUE;
	}

	return FALSE;
}

int usage()
{
	printf("no arguments will open file requester for access DB\n"
		"Access:dbmaker c:\\temp\\fuelmaster.mdb\n"
		"       dbmaker c:\\temp\\fmledger.mdb\n\n"
		"SQL:   dbmaker SERVER=PC1_XYZ;DATABASE=fmdb\n"
		"       dbmaker SERVER=PC1_XYZ;DATABASE=fmdb;UID=user;PWD=pass\n\n"
		"-liltim [count] create SQL script for TIM\n"
		"-newini create " __NEWINI__ " to replace " __INIFILE__ "\n"
		"-newdodini create " __NEWINI__ " for fmledger to replace " __INIFILE__ "\n"
		"-? help\n");
	return 0;
}
int main(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)){
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		return -1;
	}
	else
	{
		char mpath[MAX_PATH];
		GetCurrentDirectory(sizeof(mpath)-1,mpath);
		strcat(mpath,"\\");
		start_path=mpath;
#ifdef _DEBUG
//		start_path="c:\\temp\\";
#endif
		printf("start path=%s\n",start_path);
		CString db1;
		int i;
		for(i=1;i<argc;i++){
			if(strcmp(argv[i],"-?")==0){
				usage();
				return 0;
			}
			else if(strcmp(argv[i],"-newini")==0){
				printf("creation of " __NEWINI__ " enabled\n");
				CREATE_NEW_INI_FILE=TRUE;
				DeleteFile(__NEWINI__);
				db1.Empty();
				break;
			}
			else{
				db1+=argv[i]; //whatevers left is gonna be DB string
				printf("DBstring=%s\n",db1);
				break;
			}
		}
		if(!ini_exist() && !CREATE_NEW_INI_FILE){
			printf("cant find " __INIFILE__ " file.\n"
				"use -newini or -newdodini argument to create new one,\n"
				"make your changes then rename it to " __INIFILE__ " and start again\n\n");
			usage();
			printf("\npress any key to continue\n");
			wait_exit();
			return -1;
		}
		if(argc<=1 || db1.IsEmpty() && !CREATE_NEW_INI_FILE){
			get_ini_str("DATABASES","DATABASE",&db1);
			/*
			printf("no database given,opening file requester\n");
			if(OpenDB(&db1)==FALSE){
				cout<<"no file selected\n";
				custom_sleep(500);
				return 0;
			}else{
				cout<<"selected DB:"<<(LPCSTR)db1<<"\n";
			}
			*/
			if(db1.IsEmpty()){
				usage();
				exit(0);
			}
			else{
				printf("using database %s\n",db1);
			}
		}
		check_ini_globalvars();
/*
		char str[255];
		_splitpath(db1,NULL,NULL,str,NULL);
		strlwr(str);

		g_dbpassword=0;

		if(get_db_version(&db1)==FALSE){
			cout<<"Get DB version failed\n";
			return -2;
		}
*/
		if(VERIFY_ALL_FIELDS) 
			DeleteFile("log.txt"); 
		
		if(db1.Find("FoxPro")>=0){
			
			fill_mopmap(db1);
			fill_pb_combo(db1);
			fill_pb_combo_old(db1);
			fill_pb_conv(db1);
			fill_pb_item(db1);
			fill_pb_mixm(db1);
			fill_pb_pgrp(db1);
			fill_pb_pos(db1);
			fill_pb_retl(db1);
			fill_pb_upc(db1);
			fill_pbchglog(db1);
			fill_taxmap(db1);
			
			fill_posplu(db1);
			fill_posplux(db1);
		}
		else{
			fill_hshift(db1);
			fill_hevent(db1);
			fill_hacct(db1);
			fill_hticket(db1);
			fill_DAY_CLOSE_CUTOFF(db1);
		}

		cout << "\nfinished\n";
#ifndef _DEBUG
		if(argc==1)
#endif
		{
			cout << "press any key\n";
			wait_exit();
		}

	}
	return 0;
}


