int fill_hshift(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="H_SHIFT";

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
		ADDVALUE("CASHIER","%i",1); //8,15
		ADDVALUE("start_datetime","'%s %s'",date,time); //11,26
		ADDVALUE("ELSDATE","%s","null"); //9,10
		ADDVALUE("stop_datetime","'%s %s'",date,time); //11,26
		ADDVALUE("SHIFT_SEQ","%i",count); //8,15
		ADDVALUE("REGNUM","%i",1); //-6,3
		ADDVALUE("DRAWER","%i",1); //-6,3
		ADDVALUE("SOURCE","%i",1); //5,5
		ADDVALUE("NRGT","%i",1); //3,12
		ADDVALUE("TOTAL_OVRR","%i",0); //3,12
		ADDVALUE("TOTAL_SLS","%i",0); //3,12
		ADDVALUE("OVER_SHORT","%i",0); //3,12
		ADDVALUE("RECEIPT","%i",1); //8,15
		ADDVALUE("DROPPED","%i",0); //8,15
		ADDVALUE("LOAN","%i",0); //8,15
		ADDVALUE("shift_id","%i",count); //4,10
		ADDVALUE("shift","%i",1); //4,10
		ADDVALUE("POS_TYPE","%i",1); //2,1

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}
		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}

