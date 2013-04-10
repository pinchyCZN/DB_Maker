int fill_ledger_messages(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="messages";
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
	for(int count=0;count<150;count++)
	{

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("downloadtime","'%s %s'",date,time); //11,19
		ADDVALUE("messageid","%i",count%30); //4,10
		ADDVALUE("comment","'comment_%i'",count); //12,50
		ADDVALUE("siteid","'%06i'",count); //12,15
		ADDVALUE("transtime","'%s %s'",date,time); //11,19
		ADDVALUE("poweroff_time","'%s %s'",date,time); //11,19
		ADDVALUE("vehicleid","'%08i'",count); //12,8
		ADDVALUE("dodaac","'%06i'",count); //12,6
		ADDVALUE("org","'org%c'",'A'+count%26); //12,4
		ADDVALUE("signal_vims","'%c'",'A'+count%26); //12,1
		ADDVALUE("fund_sysd","'F%c'",'A'+count%26); //12,2
		ADDVALUE("cic","'cic'",count); //12,3
		ADDVALUE("job","'job%03i'",count%100); //12,6
		ADDVALUE("tid","'tid_%i'",count%100); //12,15
		ADDVALUE("misc","'MIS%03i'",count%100); //12,6
		ADDVALUE("method","'method_%i'",count); //12,50
		ADDVALUE("hose","%i",(count%8)+1); //4,10
		ADDVALUE("mobile","%i",count%100); //4,10
		ADDVALUE("truck_number","'truck_%i'",count); //12,50
		ADDVALUE("rollover","%i",count%10); //4,10
		ADDVALUE("end_rollover","%i",count%10); //4,10
		ADDVALUE("unit","%i",count%10); //4,10
		ADDVALUE("archive","%i",count%10); //4,10
		ADDVALUE("lockout","%i",count%10); //4,10
		ADDVALUE("freeint2","%i",count%10); //4,10
		ADDVALUE("freetext1","'free1_%i'",count); //12,50
		ADDVALUE("freetext2","'free2_%i'",count); //12,50
		ADDVALUE("cc_number","'cc%08i'",count); //12,50
		ADDVALUE("cc_ext_number","'%05i'",count%100); //12,5

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