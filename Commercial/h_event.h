int fill_hevent(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="H_EVENT";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	dbm.open_db(&dbname);
	dbm.get_fields_array();

	if(!delete_table_records(&dbm.db,dbm.table_name,"")){
		dbm.db.Close();return FALSE;
	}

	char time[20],date[20];
	SYSTEMTIME systime;
//	GetSystemTime(&systime);
	set_system_time(&systime,2010,1,1,3,0,0);
//	increment_time(&systime,-72,0,0); //roll back few days

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

	int shift_seq=1;
	for(count=1;count<=max_count*2;count++)
	{
		ADDVALUE("STORENUM","%i",store_num); //8,15
		ADDVALUE("TICK_DATETIME","'%s %s'",date,time); //11,26
		ADDVALUE("REGNUM","%i",1); //-6,3
		ADDVALUE("TICKET","%i",count); //8,15
		ADDVALUE("SHIFT_SEQ","%I64u",count&1?(__int64)shift_seq++:(__int64)SHIFT9); //8,15
		ADDVALUE("ELSDATE","%s","null"); //9,10
		ADDVALUE("CASHIER","%i",count); //8,15
		ADDVALUE("SOURCE_DEV","%i",count); //5,5
		ADDVALUE("SOURCE_UNT","%i",count); //5,5
		ADDVALUE("CUSTOMER","%i",count); //2,20
		ADDVALUE("TRANS_TYPE","'%i'",count); //12,8
		ADDVALUE("ITEMNUM","'%i'",count); //12,20
		ADDVALUE("MODIFIER","%i",count); //5,5
		ADDVALUE("COUNT","%i",count); //2,9
		ADDVALUE("AMOUNT","%i",count); //3,12
		ADDVALUE("FP","%i",count); //5,5
		ADDVALUE("FUEL_AMT","%i",count); //3,12
		ADDVALUE("FUEL_VOL","%i",count); //3,16
		ADDVALUE("DESCRIPTION","'%i'",count); //12,40
		ADDVALUE("COPIED","'%i'",0); //12,1
		ADDVALUE("EVENT_ID","%i",count); //4,10
		ADDVALUE("LOCATION","'%i'",count); //12,6
		ADDVALUE("CURR_CODE","'%i'",0); //12,3
		ADDVALUE("EXCH_RATE","%i",0); //2,6
		ADDVALUE("POS_TYPE","%i",0); //2,1

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


