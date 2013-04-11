int fill_hticket(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="H_TICKET";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	char time[20],date[20];
	SYSTEMTIME systime;
	set_system_time(&systime,2010,1,1,6,0,0);
	//increment_time(&systime,-72,0,0); //roll back few days

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
	cout<<"aim prog count "<<max_count<<endl;

	for(count=1;count<=max_count;count++)
	{
		ADDVALUE("STORENUM","%i",1); //8,15
		ADDVALUE("TICK_DATETIME","'%s %s'",date,time); //11,26
		ADDVALUE("REGNUM","%i",1); //-6,3
		ADDVALUE("TICKET","%i",count); //8,15
		ADDVALUE("SHIFT_SEQ","%i",count); //8,15
		ADDVALUE("ELSDATE","%s","null"); //9,10
		ADDVALUE("CASHIER","%i",1); //8,15
		ADDVALUE("SOURCE_DEV","%i",1); //5,5
		ADDVALUE("SOURCE_UNT","%i",1); //5,5
		ADDVALUE("CUSTOMER","%i",1); //2,20
		ADDVALUE("CUST_DEMO","'%i'",0); //12,6
		ADDVALUE("CUST_DATE","%i",0); //9,10
		ADDVALUE("TRANS_TYPE","'%i'",1); //12,1
		ADDVALUE("CUST_TYPE","'%i'",0); //12,3
		ADDVALUE("TOTAL_AMT","%i",0); //3,12
		ADDVALUE("TOTAL_TAX","%i",0); //3,12
		ADDVALUE("FUEL_AMT","%i",0); //3,12
		ADDVALUE("FUEL_VOL","%i",0); //3,16
		ADDVALUE("SCAN_COUNT","%i",0); //5,5
		ADDVALUE("PLU_COUNT","%i",0); //5,5
		ADDVALUE("DEPT_COUNT","%i",0); //5,5
		ADDVALUE("NFND_COUNT","%i",0); //5,5
		ADDVALUE("DIRC_COUNT","%i",0); //5,5
		ADDVALUE("TOTAL_FS","%i",0); //3,12
		ADDVALUE("TOTAL_WIC","%i",0); //3,12
		ADDVALUE("TOTAL_DISC","%i",0); //3,12
		ADDVALUE("ELAPSED","%i",0); //5,5
		ADDVALUE("DELAY","%i",0); //8,15
		ADDVALUE("MERCH_CUST","%i",0); //2,5
		ADDVALUE("GAS_CUST","%i",0); //2,5
		ADDVALUE("PRINTED","%i",1); //2,3
		ADDVALUE("DRAWERTIME","%i",1); //2,6
		ADDVALUE("KV_SENT","'%i'",1); //12,10
		ADDVALUE("COPIED","'%i'",1); //12,1
		ADDVALUE("ticket_id","%i",count); //4,10
		ADDVALUE("DOB_TYPE","'%i'",1); //12,2
		ADDVALUE("VOID_COUNT","%i",0); //2,3
		ADDVALUE("POS_TYPE","%i",1); //2,1

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}
		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
		}
	}
	char val[20];

	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}

