int fill_ledger_additive(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="Additive";
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
	for(int count=1;count<10;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("ADDITIVE_ID","%i",count); //4,10
		ADDVALUE("CODE","'c%04i'",count); //12,5
		ADDVALUE("DESC","'desc_%i'",count); //12,20
		ADDVALUE("MAX","%i",count+1000); //8,15
		ADDVALUE("MIN","%i",count); //8,15
		ADDVALUE("MISC1","''",count); //12,50
		ADDVALUE("MISC2","''",count); //12,50
		ADDVALUE("MISC3","''",count); //12,50

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
