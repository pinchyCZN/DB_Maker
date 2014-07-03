int fill_DAY_CLOSE_CUTOFF(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="DAY_CLOSE_CUTOFF";

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

	int max_count=400;
	get_ini_value(dbm.table_name,"count",&max_count);
	cout<<"max count "<<max_count<<endl;

	for(count=1;count<=max_count;count++)
	{
		ADDVALUE("ELSDATE","'%s'",date); //9,10 //cannot be null
		ADDVALUE("FIRST_SHIFT_START","'%s %s'",date,time); //11,26
		ADDVALUE("FIRST_SHIFT_SEQ","%i",1); //8,15
		ADDVALUE("FIRST_SHIFT_REGNUM","%i",1); //-6,3
		ADDVALUE("LAST_SHIFT_END","'%s %s'",date,time); //11,26
		ADDVALUE("LAST_SHIFT_SEQ","%i",1); //8,15
		ADDVALUE("LAST_SHIFT_REGNUM","%i",1); //-6,3
		ADDVALUE("CUTOFF","'%s %s'",date,time); //11,26
		ADDVALUE("TOTAL_SALES","%i",1); //3,14
		ADDVALUE("FINALIZED","'%s %s'",date,time); //11,26


		if(dbm.execute_sql_insert()==FALSE){
			dbm.close();return FALSE;
		}
		increment_time(&systime,24,0,0);
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));
	}
	dbm.close();
	return TRUE;
}

