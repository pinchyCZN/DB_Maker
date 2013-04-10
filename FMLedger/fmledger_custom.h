int fill_ledger_customdata(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="CustomData";
	int	field_count=0;

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-24*2,0,0); //roll back few days
	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));

	int cust_data[16]={1,2,1001,1002,1003,1004,1005,1006,1008,1021,1022,1023,1024,1025,0};
	for(int count=0;count<100;count++){
		CString hash;
		if(cust_data[count]==0)
			break;
		hash.Format("%08X%08X%08X",0xFFFF^cust_data[count],0xFFFF^(cust_data[count]==1?1:0),0xFFFF^0x270F016C);

		ADDVALUE("CustomId","%i",cust_data[count]); //4,10
		ADDVALUE("Code","'%s'",hash); //12,255
		ADDVALUE("CreateDate","'%s %s'",date,time); //11,19
		ADDVALUE("Operator","1",count); //4,10
		ADDVALUE("Data","%i",cust_data[count]==1?1:0); //4,10

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}

int fill_ledger_customerrefdata(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="CustomerRefData";
	int	field_count=0;

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	int count=0;
	SKIP_VALUE("LOCAL_RECORD_ID","",count); //4,10
	ADDVALUE("CUST_DODAAC","'"_DODAAC_"'",count,count+1); //12,6
	ADDVALUE("SIG_CODE","'A'",count); //12,1
	ADDVALUE("FUND_CODE","'A'",count); //12,2
	ADDVALUE("CUST_SUP_DODAAC","'A'",count); //12,6
	ADDVALUE("SUB_ACCOUNT","'A'",count); //12,12

	if(dbm.execute_sql_insert(&db)==FALSE){
		db.Close();return FALSE;
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}