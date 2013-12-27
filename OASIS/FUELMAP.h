int fill_fuelmap(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="fuelmap";

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
//	set_system_time(&systime,2010,1,1,0,0,0);
	increment_time(&systime,-72,0,0); //roll back few days

	//2008-09-29 08:41:35.000
	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));

	int count=0;
	DWORD elapsed=0;

//	int int_key=0;
//	get_table_index(&db,"Config","VEHIINTKEY",&int_key);
	
	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_count=200;
	get_ini_value(dbm.table_name,"count",&max_count);

	int code=0;
	for(count=0;count<max_count;count++)
	{
		char *fmt="'%03i'";
		if(count>=100)
			fmt="'%i'";
		if(count==100)
			code=0;
		ADDVALUE("nacspcode",fmt,code); //1,3
		ADDVALUE("grade","%i",code); //2,2
		
		code++;

		if(dbm.execute_sql_insert()==FALSE){
			dbm.db.Close();return FALSE;
		}
	}
	dbm.close();
	return TRUE;
}

