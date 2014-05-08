int fill_htender(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="H_TENDER";

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

	int shift_seq=1;
	for(count=1;count<=max_count*2;count++)
	{
		ADDVALUE("STORENUM","",count); //8,15
		ADDVALUE("tick_datetime","''",count); //11,26
		ADDVALUE("REGNUM","",count); //-6,3
		ADDVALUE("TICKET","",count); //8,15
		ADDVALUE("ORDINAL","",count); //5,5
		ADDVALUE("ELSDATE","",count); //9,10
		ADDVALUE("TENDER_ID","''",count); //12,8
		ADDVALUE("SUB_ID","''",count); //12,16
		ADDVALUE("AMOUNT","",count); //3,12
		ADDVALUE("TENDERED","",count); //3,12
		ADDVALUE("FEE","",count); //3,12
		ADDVALUE("ACCT_TYPE","''",count); //12,4
		ADDVALUE("ACCOUNT","''",count); //12,40
		ADDVALUE("TRANS_ID","''",count); //12,20
		ADDVALUE("REF","''",count); //12,20
		ADDVALUE("AUTH_CODE","''",count); //12,20
		ADDVALUE("EXP_DATE","",count); //9,10
		ADDVALUE("INTERFACED","''",count); //12,10
		ADDVALUE("BATCH","",count); //5,5
		ADDVALUE("ticket_id","",count); //4,10
		ADDVALUE("SURCHARGE","",count); //2,8
		ADDVALUE("FMT_ACCT","''",count); //12,20
		ADDVALUE("IV","''",count); //12,32
		ADDVALUE("SIG_STATUS","''",count); //12,1
		if(dbm.execute_sql_insert()==FALSE){
			dbm.close();return FALSE;
		}
	}
	dbm.close();
	return TRUE;
}

