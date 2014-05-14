int fill_herror(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="H_ERROR";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	dbm.open_db(&dbname);
	dbm.get_fields_array();

	if(!delete_table_records(&dbm.db,dbm.table_name,"")){
		dbm.db.Close();return FALSE;
	}

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	int timeshift=-3;
	//set_system_time(&systime,2010,1,1,3,0,0);
	get_ini_value(dbm.table_name,"timeshift",&timeshift);
	increment_time(&systime,timeshift*24,0,0); //roll back few days

	//2008-09-29 08:41:35.000
	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));

	int count=0;
	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_count=500;
	get_ini_value(dbm.table_name,"count",&max_count);
	cout<<"max count "<<max_count<<endl;

	for(count=1;count<=max_count;count++)
	{

		ADDVALUE("STORENUM","%i",count); //8,15
		ADDVALUE("tick_datetime","'%s %s'",date,time); //11,26
		ADDVALUE("REGNUM","%i",count%10); //-6,3
		ADDVALUE("ELSDATE","'%s'",date); //9,10
		ADDVALUE("USER_ID","%i",count%100); //8,15
		ADDVALUE("SENDER","'send%i'",count); //12,120
		ADDVALUE("CLASS","'%i'",count); //12,20
		ADDVALUE("VERSION","'%i'",count); //12,8
		ADDVALUE("APP_NAME","'%i'",count); //12,20
		ADDVALUE("TITLE","'%i'",count); //12,15
		ADDVALUE("DESCRIPTION","'%i'",count); //12,80
		ADDVALUE("FILENAME","'%i'",count); //12,80
		ADDVALUE("SYSTEMINFO","'%i'",count); //12,80
		ADDVALUE("EXENAME","'%i'",count); //12,80
		ADDVALUE("TRANS_TYPE","'%i'",count); //12,4
		ADDVALUE("SOURCE_DEV","%i",count%100); //5,5
		ADDVALUE("SOURCE_UNT","%i",count%100); //5,5
		ADDVALUE("LIB_NAME","'%i'",count); //12,20
		ADDVALUE("LIB_VERS","'%i'",count); //12,20
		ADDVALUE("DISK_SPACE","%i",count%100); //8,15
		ADDVALUE("FREE_MEM","%i",count); //8,15
		ADDVALUE("FREE_RES","%i",count); //8,15
		ADDVALUE("NET_NAME","'%i'",count); //12,20
		ADDVALUE("USER_NAME","'%i'",count); //12,20
		ADDVALUE("OS_NAME","'%i'",count); //12,20
		ADDVALUE("OS_VERSION","'%i'",count); //12,20
		ADDVALUE("CO_NAME","'%i'",count); //12,20
		ADDVALUE("SERIAL_NO","'%i'",count); //12,30
		ADDVALUE("PROCESSOR","'%i'",count); //12,20
		ADDVALUE("ERROR_ID","%i",count); //4,10
		ADDVALUE("logfile","'%i'",count); //12,40
		ADDVALUE("logrecord","'%i'",count); //12,40
		if(dbm.execute_sql_insert()==FALSE){
			dbm.close();return FALSE;
		}
		if(!(count&1))
		{
			increment_time(&systime,24,0,0);
			GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
			GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));
		}
	}
	dbm.close();
	return TRUE;
}

