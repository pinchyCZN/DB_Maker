int fill_ledger_config(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="config";
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

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	int count=1;
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("name","'Name%i'",count); //12,50
		ADDVALUE("address1","'address1_%i'",count); //12,30
		ADDVALUE("address2","'address2_%i'",count); //12,30
		ADDVALUE("city","'city_%i'",count); //12,20
		ADDVALUE("state","'state_%i'",count); //12,20
		ADDVALUE("country","'country_%i'",count); //12,20
		ADDVALUE("zip","'zip_%i'",count); //12,15
		ADDVALUE("system","2",count); //4,10
		ADDVALUE("dodaac","'"_DODAAC_"'",count,count+1); //12,6
		ADDVALUE("service_index","2",count); //4,10
		ADDVALUE("freeint1","%i",count); //4,10
		ADDVALUE("freeint2","%i",count); //4,10
		ADDVALUE("freetext1","'free1'",count); //12,50
		ADDVALUE("freetext2","'free2'",count); //12,50
		ADDVALUE("db_version","'2.2.0.7'",count); //12,50
		ADDVALUE("warning0","'its a warning yo dawg'",count); //-1,1073741823
		ADDVALUE("warning1","''",count); //12,255
		ADDVALUE("warning2","''",count); //12,255

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
