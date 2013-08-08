int fill_posplu(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="posplu";

	CString str;
	get_ini_str("posplu","database",&str);
	if(!str.IsEmpty())
		dbname=str;

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	dbm.open_db(&dbname);
	dbm.get_fields_array();

	if(!delete_table_records(&dbm.db,dbm.table_name,"")){
		dbm.db.Close();return FALSE;
	}

	char time[20],date[20];
	SYSTEMTIME systime;
//	GetSystemTime(&systime);
	set_system_time(&systime,2010,1,1,0,0,0);
//	increment_time(&systime,-72,0,0); //roll back few days

	//2008-09-29 08:41:35.000
	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));

	int count=0;
	DWORD elapsed=0;

//	int int_key=0;
//	get_table_index(&db,"Config","VEHIINTKEY",&int_key);
	
	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_count=5;
	get_ini_value(dbm.table_name,"count",&max_count);

	for(count=0;count<max_count;count++)
	{
		char types[3]={'P','D','S'};

		ADDVALUE("board","%i",count); //2,3
		ADDVALUE("desc","'board %i %c'",count,types[count%(sizeof(types))]); //1,25
		ADDVALUE("btn_height","67",count); //2,4
		ADDVALUE("btn_width","76",count); //2,4
		ADDVALUE("rows","6",count); //2,3
		ADDVALUE("cols","6",count); //2,3
		ADDVALUE("notebook","0",count); //-7,1
		ADDVALUE("type","'%c'",types[count%(sizeof(types))]); //1,1
		ADDVALUE("ondemand","0",count); //-7,1
		ADDVALUE("security","0",count); //2,2
		ADDVALUE("inactive","0",count); //-7,1
		ADDVALUE("col_labels","0",count); //-7,1
		ADDVALUE("icon","' '",count); //1,40
		ADDVALUE("next_board","0",count); //2,3
		ADDVALUE("num_reqd","0",count); //2,3

		if(dbm.execute_sql_insert()==FALSE){
			dbm.db.Close();return FALSE;
		}
	}
	dbm.close();
	return TRUE;
}

