int fill_pb_conv(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="pb_conv";

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

	int max_count=50;
	get_ini_value(dbm.table_name,"count",&max_count);

	for(count=0;count<max_count;count++)
	{
		ADDVALUE("group","%i",count); //2,3
		ADDVALUE("desc","'pb_conv_desc%i'",count); //1,40
		ADDVALUE("purch_conv","%i",(count%10)+1); //2,9
		ADDVALUE("inven_code","'%i'",(count%10)+1); //1,3
		ADDVALUE("purch_code","'%i'",(count%10)+1); //1,5
		ADDVALUE("inv_sdesc","'invs%i'",count); //1,8
		ADDVALUE("pur_sdesc","'purs%i'",count); //1,8
		ADDVALUE("inv_ldesc","'invl%i'",count); //1,20
		ADDVALUE("pur_ldesc","'purl%i'",count); //1,20


		if(dbm.execute_sql_insert()==FALSE){
			dbm.db.Close();return FALSE;
		}
	}
	dbm.close();
	return TRUE;
}

