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
	~DBMaker(){
	};
	DBMaker(){
		table_name="";
		field_counter=field_count=0;
		SQLFields=SQLParams=SQLValues="";
	};
	int execute_sql_insert(CDatabase *db);
	int get_fields_array(CDatabase *db);
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
int DBMaker::execute_sql_insert(CDatabase *db)
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
		if(!db->IsOpen()){
			printf("error:database is not open\n");
			return FALSE;
		}
		CString SqlString;
		SqlString.Format("INSERT INTO [%s] (%s) VALUES(%s);\n",table_name,SQLParams,SQLValues);
		field_counter=0; //reset these for next insert
		SQLParams="";
		SQLValues="";
		TRY{
			db->ExecuteSQL(SqlString);
			if(LOG_SQL_INSERT){
				FILE *flog;
				flog=fopen("log_sql_insert.txt","a");
				fputs(SqlString,flog);
				fclose(flog);
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
			return FALSE;
		}
		END_CATCH
	}
}
int DBMaker::get_fields_array(CDatabase *db)
{
	CString SqlString;
	int i;

	if(!db->IsOpen()){
		printf("error:database is not open\n");
		return 0;
	}
	CRecordset rec1( db );

	SqlString = 
	"SELECT TOP 1 * FROM [";
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
	"SELECT TOP 1 * FROM [";
	SqlString+=table;
	SqlString+="]; ";

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
	if(tstr.Find(".mdb")>=0)
		type=0;
	else
		type=1;
	switch(type)
	{
	default:
	case 0: //ACCESS
		if(g_dbpassword!=0)
			connect.Format("ODBC;DRIVER={MICROSOFT ACCESS DRIVER (*.mdb)};DSN='';DBQ=%s;PWD=%s",*dbname,g_dbpassword);
		else
			connect.Format("ODBC;DRIVER={MICROSOFT ACCESS DRIVER (*.mdb)};DSN='';DBQ=%s",*dbname);
		break;
	case 1: //SQL
		if(tstr.Find("server=")<0){
			printf("cant find 'server=' in connection string\n");
			return FALSE;
		}
		//connect.Format("ODBC;DRIVER={SQLServer};SERVER=%s;DATABASE=%s;Trusted_Connection=yes",*dbname);
		connect.Format("ODBC;DRIVER={SQL Server};%s;Integrated Security=SSPI",*dbname);
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
int get_db_version(CString *dbname)
{
	CDatabase db;
	CString SqlString;

	if(open_database(&db,dbname)==0)
	{
		cout<<"failed to open database\n";
		return FALSE;
	}
	CRecordset rec( &db );
	TRY{
		SqlString = "SELECT [ThisDatabaseVersion],[DateWasCreated] FROM [DBVersionStamp] ;";
		rec.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);
		cout<<"database version ";
		while(!rec.IsEOF())
		{
			CString result;
			rec.GetFieldValue("ThisDatabaseVersion",result);
			cout<<(LPCTSTR)result << "      ";
			rec.GetFieldValue("DateWasCreated",result);
			cout<<(LPCTSTR)result;
			rec.MoveNext();
			cout<<"\n";
		}
	}CATCH(CDBException, e){
		cout<<"failed to get DB version\n";
	}END_CATCH
	if(rec.IsOpen())
		rec.Close();
	db.Close();
	return TRUE;
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
int get_string_ini_status(char *str,char *key,int *found_key,int *commented)
{
	char *a,*b;
	int klen=strlen(key);
		
	if((b=strstr(str,key))!=0)
	{
		*found_key=TRUE;
		if((b!=str) && (b[-1]=='[')) //check for brackets []
		{
			if((b[klen]&0xFF)!=']')//double check entire str for partial key match
				*found_key=FALSE;
		}
		else if( !((b[klen]&0xFF)<=' ')) //double check entire str for partial key match
				*found_key=FALSE;

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
	char fstr[255];
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
#include "Commercial\AIM.h"
#include "Commercial\Config.h"
#include "Commercial\CREDITCARD.h"
#include "Commercial\CustomData.h"
#include "Commercial\Customer.h"
#include "Commercial\InventoryRec.h"
#include "Commercial\MainTrans.h"
#include "Commercial\OBD_TABLES.h"
#include "Commercial\ProductConfig.h"
#include "Commercial\ReportConfig.h"
#include "Commercial\Security_Password.h"
#include "Commercial\Site.h"
#include "Commercial\TankConfig.h"
#include "Commercial\User.h"
#include "Commercial\Utility.h"
#include "Commercial\Vehicle.h"
#include "Commercial\TransAllZero.h"
#include "Commercial\UnitConfig.h"


#define _DODAAC_ "F%02iZ%02i"
#include "FMLedger\fmledger_Additive.h"
#include "FMLedger\fmledger_DODAAC.h"
#include "FMLedger\fmledger_Link_DoDAAC_NSN.h"
#include "FMLedger\fmledger_Config.h"
#include "FMLedger\fmledger_custom.h"
#include "FMLedger\fmledger_secu.h"
#include "FMLedger\fmledger_tmu.h"
#include "FMLedger\fmledger_veh.h"
#include "FMLedger\fmledger_site.h"
#include "FMLedger\fmledger_tank.h"
#include "FMLedger\fmledger_tim_trans.h"
#include "FMLedger\fmledger_transactions.h"
#include "FMLedger\fmledger_messages.h"
#include "FMLedger\fmledger_credit.h"




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
			else if(strcmp(argv[i],"-newdodini")==0){
				printf("creation of " __NEWINI__ " enabled\n");
				CREATE_NEW_INI_FILE=TRUE;
				DeleteFile(__NEWINI__);
				goto newdodini;
				break;
			}
			else if(strcmp(argv[i],"-liltim")==0){
				int count=100;
				if(argc>(i+1))
					count=atoi(argv[2]);
				fill_liltimtrans(db1,count);
				return 0;
			}
			else{
				db1+=argv[i]; //whatevers left is gonna be DB string
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
			printf("no database given,opening file requester\n");
			if(OpenDB(&db1)==FALSE){
				cout<<"no file selected\n";
				custom_sleep(500);
				return 0;
			}else{
				cout<<"selected DB:"<<(LPCSTR)db1<<"\n";
			}
		}
		check_ini_globalvars();

		char str[255];
		_splitpath(db1,NULL,NULL,str,NULL);
		strlwr(str);
		if(strstr(str,"fmledger")!=0){
			char fname[MAX_PATH];
			char pass[80];
			strcpy(fname,db1);
			if(get_password(fname,pass,sizeof(pass),FALSE)){
				g_dbpassword=pass;
newdodini:
				fill_ledger_additive(db1);
				fill_ledger_dodaac(db1);
				fill_ledger_dodaac_lock(db1);
				fill_ledger_link_dodaac_nsn(db1);
				fill_ledger_config(db1);
				fill_ledger_security(db1);
				fill_ledger_site(db1);
				fill_ledger_unitconfig(db1);
				fill_ledger_veh(db1);
				fill_ledger_veh_grades(db1);
				fill_ledger_tank(db1);
				fill_ledger_hoses(db1);

				fill_ledger_customdata(db1);
				fill_ledger_customerrefdata(db1);
				fill_ledger_carddef(db1);
				fill_ledger_carddeffield(db1);
				fill_ledger_carddefprefix(db1);

				fill_ledger_pdatrans(db1);
				fill_ledger_transsig(db1);

				fill_ledger_transactions(db1);
				fill_ledger_messages(db1);
				fill_ledger_totalizer(db1);
				fill_ledger_inventory_history(db1);
				fill_ledger_delivery(db1);

				fill_ledger_tmulv(db1);
				fill_ledger_tmualarm(db1);
				fill_ledger_tmucsld(db1);
				fill_ledger_tmuinventory(db1);
				fill_ledger_tmuleak(db1);
				fill_ledger_tmushiftinv(db1);

				fill_ledger_mobilevil(db1);
				fill_ledger_configvil(db1);
				fill_ledger_vil_lock(db1);
				fill_ledger_cc_lock(db1);
				fill_ledger_rules(db1);
			}
			if(argc==1){
				cout << "press any key\n";
				wait_exit();
			}
			return 0;
		}
		else
			g_dbpassword=0;
/*
		if(get_db_version(&db1)==FALSE){
			cout<<"Get DB version failed\n";
			return -2;
		}
*/
		if(VERIFY_ALL_FIELDS) 
			DeleteFile("log.txt"); 
		
		/* COMMERCIAL STUFF */
		fill_config(db1);

		fill_aimprogramhistory(db1);
		fill_aimlocklist(db1);


		fill_ppchistory(db1);
		fill_prepaidcard(db1);
		fill_cardowner(db1);
		fill_discountcard(db1);
		fill_localauthlock(db1);
		fill_sitelalock(db1);
		fill_localauthaccounts(db1);
		fill_sitelaaccnt(db1);

		fill_customer(db1);

		fill_inventoryrec(db1);

		fill_maintrans(db1);
		fill_exporttrans(db1);

		fill_obdrecords(db1);
		fill_obdstat(db1);

		fill_reportconfig(db1);

		fill_user(db1);
		fill_userlocklist(db1);
		fill_vehicle(db1);
		fill_vehiclelocklist(db1);

		fill_transallmsg(db1);
		fill_pieduptrans(db1);

		fill_site(db1);

		fill_tankconfig(db1);

		fill_productconfig(db1);
		fill_priceconfig(db1);

		fill_unitconfig(db1);
		fill_pumpconfig(db1);
		fill_gradeconfig(db1);

		fill_customdata(db1);

		fill_utility(db1);

		fill_securitypassword(db1);

		fill_transallzero(db1);
		fill_transzero(db1);

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

