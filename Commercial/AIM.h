int fill_aimprogramhistory(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="AIMII_Program_History";

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

	int aim_count=100;
	get_ini_value(dbm.table_name,"count",&aim_count);
	cout<<"aim prog count "<<dec<<aim_count<<endl;

	for(int count=0;count<aim_count;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		SKIP_VALUE("EventId","''",count); //4,10
		ADDVALUE("VehicleId","'%08i'",count); //12,8
		ADDVALUE("AIMId","'%012i'",count); //12,12
//		ADDVALUE("Status","'%i'",count%5); //4,10
		ADDVALUE("Status","'%i'",2); //4,10
		ADDVALUE("SuperId","'%i'",count%100); //4,10
		ADDVALUE("Date","'%s %s'",date,time); //11,19
		ADDVALUE("SiteId","'A%03i'",count%100); //12,4
		ADDVALUE("OperatorBegin","'%08i'",count); //12,8
		ADDVALUE("OperatorComplete","'%08i'",count); //12,8
		ADDVALUE("Comment","'comment%04i'",count); //12,60
//		ADDVALUE("Deleted","%i",-(count/10)%2); //-7,1
		ADDVALUE("Deleted","%i",0); //-7,1

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

int fill_aimlocklist(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="AIMII_Lock_List";

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

	int aim_count=100;
	get_ini_value(dbm.table_name,"count",&aim_count);
	cout<<"aim lock count "<<aim_count<<endl;

	for(int count=0;count<aim_count;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("AIMKEY","'%012I64d'",(__int64)(count+80000000)); //12,12
		ADDVALUE("LOCKDATE","'%s %s'",date,time); //11,19
		ADDVALUE("CODE","'%i'",count%3); //5,5
		ADDVALUE("FREE1","''",count); //12,15
		ADDVALUE("FREE2","''",count); //12,15
		ADDVALUE("FREE3","''",count); //12,15

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
