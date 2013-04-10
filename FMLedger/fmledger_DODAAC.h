int fill_ledger_dodaac(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="DODAAC";
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
	for(int count=1;count<20;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("DODAAC","'"_DODAAC_"'",count%100,(count+1)%100); //12,50
		ADDVALUE("DESCRIPTION","'descrip%i'",count); //12,255
		ADDVALUE("Freetext1","'free1_%i'",count); //12,50
		ADDVALUE("Freetext2","%i",count); //4,10
		ADDVALUE("IS_DELETED","0",count); //-7,1
		SKIP_VALUE("LOCAL_RECORD_ID","",count); //4,10

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

int fill_ledger_dodaac_lock(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="dodaac_org_lock";
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

	int max_count=100;
	if(get_ini_value(dbm.table_name,"max_count",&max_count))
		cout<<"max_count="<<max_count<<endl;

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	for(int count=1;count<=max_count;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		SKIP_VALUE("list_index","",count); //4,10
		ADDVALUE("dodaac","'%06i'",count*100); //12,50
		ADDVALUE("orgcode","'ORG%i'",count%10); //12,50
		ADDVALUE("user_id","%i",count); //4,10
		ADDVALUE("lock_date","'%s %s'",date,time); //11,19
		ADDVALUE("comment","'locked on %s %s'",date,time); //12,255

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
