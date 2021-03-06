int fill_pbchglog(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="pbchglog";

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

	int max_count=100;
	get_ini_value(dbm.table_name,"count",&max_count);

	//char *change_types[]={"COST","AUTH","RETAIL","DELETE","INVEN"};
	char *change_types[]={"RETAIL","DELETE","MIX"};
	int ctype_size=sizeof(change_types)/sizeof(char *);
	for(count=1;count<=max_count;count++)
	{
		char guid[80]={0};
		int type=count%ctype_size;
		int item=count/10;
		
		create_uuid(guid,sizeof(guid));

		ADDVALUE("changeid","'{%s}'",guid); //1,50
		ADDVALUE("lookupnum","'%012i'",item); //1,20
		ADDVALUE("modifier","%i",0); //2,1
		ADDVALUE("effectived","{d'%s'}",date); //9,10
		ADDVALUE("effectivet","''",0); //1,8
		ADDVALUE("changetype","'%s'",change_types[type]); //1,6
		ADDVALUE("prioramt","%i",item); //2,10
		ADDVALUE("newamt","%i",item+1); //2,10
		ADDVALUE("authorized","%i",1); //-7,1
		ADDVALUE("desc","'chglog%03i'",count); //1,16
		ADDVALUE("item_num","%05i",item); //2,12
		ADDVALUE("vendor","%04i",item); //2,9
		ADDVALUE("vendoritem","'%06i'",item); //1,15
		ADDVALUE("department","%i",1); //2,4
		ADDVALUE("pricegroup","%i",1); //2,4
		ADDVALUE("mixmtchold","%i",item); //2,4
		ADDVALUE("mixmatchtb","%i",item+1); //2,4
		ADDVALUE("prtpricelb","%i",0); //-7,1
		ADDVALUE("prtshelflb","%i",0); //-7,1
		ADDVALUE("onoffpromo","%i",0); //-7,1
		ADDVALUE("futurechg","%i",0); //-7,1
		ADDVALUE("export_id","1",(count%2)+1); //2,3


		if(FALSE){
			increment_time(&systime,-6,0,0);
			GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
			GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));

		}
//			systime=t;

		if(dbm.execute_sql_insert()==FALSE){
			dbm.db.Close();return FALSE;
		}
	}
	dbm.close();
	return TRUE;
}

