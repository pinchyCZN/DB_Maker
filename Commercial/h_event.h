int fill_hevent(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="H_EVENT";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	char time[20],date[20];
	SYSTEMTIME systime;
//	GetSystemTime(&systime);
	set_system_time(&systime,2010,1,1,6,0,0);
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
	cout<<"aim prog count "<<max_count<<endl;

	for(count=1;count<=max_count;count++)
	{
		ADDVALUE("STORENUM","%i",1); //8,15
		ADDVALUE("TICK_DATETIME","'%s %s'",date,time); //11,26
		ADDVALUE("REGNUM","%i",1); //-6,3
		ADDVALUE("TICKET","%i",count); //8,15
		ADDVALUE("SHIFT_SEQ","%i",count); //8,15
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

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}
		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
		}
	}
	char val[20];

	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}


