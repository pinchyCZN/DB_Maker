int fill_hacct(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="H_ACCT";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	dbm.open_db(&dbname);
	dbm.get_fields_array();

	if(!delete_table_records(&dbm.db,dbm.table_name,"")){
		dbm.db.Close();return FALSE;
	}

	char time[20],date[20];
	SYSTEMTIME systime;
	set_system_time(&systime,2010,1,1,3,0,0);
	//increment_time(&systime,-72,0,0); //roll back few days

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

	int shift_seq=1;
	for(count=1;count<=max_count*2;count++)
	{
		ADDVALUE("tick_datetime","'%s %s'",date,time); //11,26
		ADDVALUE("SHIFT_SEQ","%I64u",count&1?(__int64)shift_seq++:(__int64)SHIFT9); //8,15
		ADDVALUE("ID","%i",count); //8,15
		ADDVALUE("ELSDATE","%s","null"); //9,10
		ADDVALUE("ACCT_AREA","'%i'",count); //12,8
		ADDVALUE("ACCT_TYPE","'%i'",count); //12,4
		ADDVALUE("AMOUNT","%i",count); //3,12
		ADDVALUE("DONE","%i",1); //-7,1
		ADDVALUE("STORENUM","%i",store_num); //8,15
		ADDVALUE("REGNUM","%i",1); //-6,3
		ADDVALUE("acct_id","%i",count); //4,10
		ADDVALUE("LOCATION","'%i'",1); //12,6
		ADDVALUE("CURR_CODE","'%i'",1); //12,3
		ADDVALUE("EXCH_RATE","%i",0); //2,6
		ADDVALUE("POS_TYPE","%i",1); //2,1

		if(dbm.execute_sql_insert()==FALSE){
			dbm.close();return FALSE;
		}
		if(!(count&1))
		{
			increment_time(&systime,6,0,0);
			GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
			GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));
		}
	}
	dbm.close();
	return TRUE;
}

