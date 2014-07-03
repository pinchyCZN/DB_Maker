int fill_hpout(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="H_POUT";

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

	int shift_seq=1;
	for(count=1;count<=max_count;count++)
	{
		ADDVALUE("STORENUM","1",count); //8,15
		ADDVALUE("tick_datetime","'%s %s'",date,time); //11,26
		ADDVALUE("REGNUM","%i",count%10); //-6,3
		ADDVALUE("SHIFT_SEQ","%I64u",count&1?(__int64)shift_seq:(__int64)SHIFT9); //8,15
		ADDVALUE("ELSDATE","%s","null"); //9,10
		ADDVALUE("DESCRIPTION","'desc%i'",count); //12,16
		ADDVALUE("AMOUNT","%i",count%100); //3,12
		ADDVALUE("TYPE","'%c'",'P'+(count%2)); //12,1
		ADDVALUE("TRAN_CODE","'POUT'",count); //12,4
		ADDVALUE("TEN_TYPE","'CASH'",count); //12,4
		ADDVALUE("ID","0",count); //8,15
		ADDVALUE("ACCOUNT","'%i'",count%100); //12,40
		ADDVALUE("CAP_ACCT","'%i'",count%100); //12,10
		ADDVALUE("ACCT_NUM","'%i'",count%100); //12,20
		ADDVALUE("COPIED","'%c'",count&1?'Y':'N'); //12,1
		ADDVALUE("POS_TYPE","%i",count%10); //2,1
		ADDVALUE("ML_UPDATE","null",count); //11,26
		if(dbm.execute_sql_insert()==FALSE){
			break;
		}
		if(count&1){
			increment_time(&systime,24,0,0);
			GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
			GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));

		}

	}
	dbm.close();
	return TRUE;
}

