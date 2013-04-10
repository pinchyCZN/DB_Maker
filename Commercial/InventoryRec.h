char *inventorycodes[]={
"00","Delivery",
"01","Adjustments",
"02","Downloaded Transactions From FMU",
"03","Stick Reading",
"04","TMU Quantity",
"05","Stick Auto Adjustment",
"09","Transfer From",
"10","Transfer To",
"11","TMU Auto Adjustment",
"13","FMU Totalizer",
"D0","Deleted Delivery",
"D1","Deleted Adjustments",
"D3","Deleted Stick Reading",
"D5","Deleted Stick Auto Adjustment",
"D9","Deleted Transfer From",
"DA","Deleted Transfer To",
"DB","Deleted TMU Auto Adjustment",
"DD","Deleted Duplicates",
"DF","Deleted Fuel Drop",
"DN","Deleted FMU Negative Adjustment",
"DP","Deleted FMU Positive Adjustment",
"DS","Deleted FMU Stick Reading",
"DX","Deleted Transaction",
"F0","Fuel Drop",
"F1","FMU Positive Adjustment",
"F2","FMU Negative Adjustment",
"F3","FMU Stick Reading"
};
//char *inventorycodes[]={"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","AR","RT","F0","F1","F2","F3","F4","F5","TX","TT","D0","D3","D1","DA","D9","D5","DB","DX"};
int fill_inventoryrec(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="InventoryRec";

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

	int rdays=7;
	get_ini_value(dbm.table_name,"rollback",&rdays);

	increment_time(&systime,-24*rdays,0,0); //roll back few days

	int count=0;
	DWORD elapsed=0;
	cout<<"Writing to "<< dbm.table_name << " Table\n";

//	SYSTEMTIME originaltime=systime;
	int hour=0,min=1,sec=0;
	if(get_ini_value(dbm.table_name,"hour",&hour)||
		get_ini_value(dbm.table_name,"sec",&sec) ||
		get_ini_value(dbm.table_name,"min",&min))
		printf("increment hour=%i,min=%i,sec=%i\n",hour,min,sec);

	int max_count=20;
	if(get_ini_value(dbm.table_name,"count",&max_count))
		cout<<"count="<<max_count<<endl;

	CString exclude_ic;
	if(get_ini_str(dbm.table_name,"exclude",&exclude_ic))
		cout<<"excluding certain inventory codes:\n"<<(LPCTSTR)exclude_ic<<endl;

	for(count=0;count<max_count;count++)
	{
		//systime=originaltime; //roll back to orignal time
		int siteid=1;
		int tank=1;
		for(int icodes=0;icodes<sizeof(inventorycodes)/sizeof(char*)/2;icodes++)
		{
			if(exclude_ic.Find(inventorycodes[icodes*2])!=-1)
				continue;

			GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
			GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
			increment_time(&systime,hour,min,sec);

			ADDVALUE("INVCODE","'%s'",inventorycodes[icodes*2]); //12,2
			ADDVALUE("SITEID","'A%03i'",(count+1)%1000); //12,4
			ADDVALUE("TANK","'%i'",count%20); //5,5
			ADDVALUE("SITEID2","'A%03i'",(count+2)%1000); //12,4
			ADDVALUE("TANK2","'%i'",count%20); //5,5
			ADDVALUE("INV_TIME","'%s %s'",date,time); //11,19
			ADDVALUE("QUANTITY","'%f'",(icodes+1)+.25); //8,15
			ADDVALUE("QUANTITY2","'%i'",icodes*2); //8,15
			ADDVALUE("RESULTINV","'%i'",count*10); //8,15
			ADDVALUE("COMMENT","'%s'",inventorycodes[(icodes*2)+1]); //12,255
			ADDVALUE("FREE1","''",count); //12,25
			ADDVALUE("FREE2","''",count); //12,25
			ADDVALUE("FREE3","''",count); //12,25
			ADDVALUE("UNITCOST","'%i'",count*2); //8,15
			ADDVALUE("TOTALCOST","'%i'",count*10); //8,15
			ADDVALUE("ISSUEUNIT","'%i'",count%30); //4,10
			ADDVALUE("INTKEY","''",count); //12,9
			ADDVALUE("KEYID","''",count); //12,8
			ADDVALUE("OPNUM","'0'",count); //4,10
			ADDVALUE("FMUTRANTYPE","'0'",count); //5,5
			ADDVALUE("TRANINITIATOR","'0'",count); //5,5
			ADDVALUE("TRANINITIATORMAPPED","'0'",count); //5,5
			ADDVALUE("GRADENUM","'0'",count); //5,5
			ADDVALUE("ENTRY_TIME","'%s %s'",date,time); //11,19
			SKIP_VALUE("AUTONUM","''",count); //4,10
			//184 version DB
			ADDVALUE("GRADE","'0'",count); //5,5
			ADDVALUE("MAXTANKVOLUME","'0'",count); //8,15
			ADDVALUE("CURRENTTANKVOLUME","'0'",count); //8,15
			ADDVALUE("ULLAGE","'0'",count); //8,15
			ADDVALUE("WATER","'0'",count); //8,15

			if(dbm.execute_sql_insert(&db)==FALSE){
				db.Close();return FALSE;
			}
			if(GetTickCount() > (elapsed+250)){
				printf("%i %i       \r",icodes,count);
				elapsed=GetTickCount();
			}
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}