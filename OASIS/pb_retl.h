int fill_pb_retl(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="pb_retl";

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

	int max_count=50;
	get_ini_value(dbm.table_name,"count",&max_count);

	for(count=0;count<max_count;count++)
	{
		ADDVALUE("profile","0",count); //2,4
		ADDVALUE("item_num","%05i",count); //2,12
		ADDVALUE("date","{d'%s'}",date); //9,10
		ADDVALUE("retail","%i",(count+10)%100); //2,8
		ADDVALUE("promo_code","0",count); //2,5
		ADDVALUE("print","1",count); //-7,1
		ADDVALUE("processed","%i",(count/3)&1); //-7,1
		ADDVALUE("mixmatchtb","%i",count%20); //2,4
		ADDVALUE("c_print","'T'",count); //1,1
		ADDVALUE("printshelf","0",count); //-7,1


		if(FALSE){
			increment_time(&systime,-6,0,0);
			GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
			GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));

		}

		if(dbm.execute_sql_insert()==FALSE){
			dbm.db.Close();return FALSE;
		}
	}
	dbm.close();
	return TRUE;
}

