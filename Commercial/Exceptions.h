int fill_Exceptions(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="Exceptions";

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

//	int int_key=0;
//	get_table_index(&db,"Config","VEHIINTKEY",&int_key);
	
	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_count=50;
	get_ini_value(dbm.table_name,"count",&max_count);
	cout<<"max count "<<max_count<<endl;

	for(count=1;count<=max_count;count++)
	{
		ADDVALUE("ID","%i",count); //4,10
		ADDVALUE("DATETIME","'%s %s'",date,time); //11,26
		ADDVALUE("STORENUM","%i",count%100); //8,15
		ADDVALUE("REGNUM","%i",count%10); //-6,3
		ADDVALUE("CASHIER","%i",count%10); //8,15
		ADDVALUE("SHIFT_SEQ","%i",count); //8,15
		ADDVALUE("TICKET_ID","%i",count); //4,10
		ADDVALUE("DESCRIPTION","'exc%i'",count); //12,256
		ADDVALUE("TRANSID","'trans%i'",count); //12,256
		ADDVALUE("FILENAME","'fn%i'",count); //12,30
		if(dbm.execute_sql_insert()==FALSE){
			dbm.close();return FALSE;
		}
	}
	dbm.close();
	return TRUE;
}

