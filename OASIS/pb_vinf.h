int fill_pb_vinf(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="pb_vinf";

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

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_count=50;
	get_ini_value(dbm.table_name,"count",&max_count);

	int i;
	for(i=0;i<max_count;i++)
	{
		count=i;

		ADDVALUE("item_num","%i",count); //2,12
		ADDVALUE("vendor","%i",count); //2,9
		ADDVALUE("vnditem","'%i'",count); //1,12
		ADDVALUE("sequence","%i",count); //2,8
		ADDVALUE("link_typ","'0'",count); //1,1

		if(dbm.execute_sql_insert()==FALSE){
			dbm.db.Close();
			return FALSE;
		}
	}
	dbm.close();
	return TRUE;
}

