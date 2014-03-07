int fill_pb_pgrp(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="pb_pgrp";

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
		int age=18;
		int val=(count/2)%3;
		if(val==0)
			age=21;
		else if(val==1)
			age=19;
		else
			age=18;

		ADDVALUE("group","%i",count); //2,4
		ADDVALUE("desc","'pgrp_desc%i'",count); //1,40
		ADDVALUE("prepared","%i",count%1); //-7,1
		ADDVALUE("tax_status","'%03i'",100+count%7); //1,3
		ADDVALUE("fs_status","'%s'",(count%2)?"Y":"N"); //1,1
		ADDVALUE("category","'%03i'",count); //1,3
		ADDVALUE("department","%i",count); //2,4
		ADDVALUE("round_type","''",count); //1,2
		ADDVALUE("nacs_cat","%05i",count); //2,6
		ADDVALUE("age","%i",age); //2,2
		ADDVALUE("discountbl","%i",count%1); //-7,1
		ADDVALUE("ord_assort","%i",count%1); //2,3
		ADDVALUE("prod_code","'%03i'",count); //1,5
		ADDVALUE("kv_type","''",count); //1,1
		ADDVALUE("kv_route","%i",count%1); //2,1
		ADDVALUE("ord_group","%i",count%1); //2,4
		ADDVALUE("type","'N'",count); //1,1
		ADDVALUE("wic_status","'N'",count); //1,1
		ADDVALUE("c_discount","'F'",count); //1,1
		ADDVALUE("cashonly","%i",(count/4)&1); //-7,1
		ADDVALUE("restr_grp","''",count); //1,8
		ADDVALUE("c_cashonly","'F'",count); //1,1



		if(dbm.execute_sql_insert()==FALSE){
			dbm.db.Close();return FALSE;
		}
	}
	dbm.close();
	return TRUE;
}

