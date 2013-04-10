int fill_tankconfig(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="TankConfig";

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
	increment_time(&systime,-24*7,0,0); //roll back few days

	DWORD elapsed=0;
	cout<<"Writing to "<< dbm.table_name << " Table\n";

	SYSTEMTIME originaltime=systime;
	for(int site=1;site<=20;site++)
	{
		systime=originaltime; //roll back to orignal time for each code
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,10,0);
		for(int count=1;count<=20;count++) //cant have tank # of 0
		{
	
			ADDVALUE("TANK","'%i'",count); //5,5
			ADDVALUE("SITEID","'A%03i'",site); //12,50
			ADDVALUE("PRODUCT","'%i'",((count-1)%99)+1); //5,5
			ADDVALUE("PRICE","'-1'",count); //5,5
			ADDVALUE("COMBOTANK","'-1'",count); //5,5
			ADDVALUE("BASECOST","'0'",count); //8,15
			ADDVALUE("VENDOR","'vendor%i'",(site*100)+count); //12,50
			ADDVALUE("TANKPRICING","0",count); //-7,1
			ADDVALUE("QUANTITY","'%i'",count*10); //8,15
			ADDVALUE("CAPACITY","'%i'",count*100); //8,15
			ADDVALUE("REORDER","'%i'",count*100/2); //8,15
			ADDVALUE("MAIN_INV","%i",-count%2); //-7,1
			ADDVALUE("LAST_DROP","'%i'",count*10); //8,15
			ADDVALUE("DROP_TIME","'%s %s'",date,time); //11,19
			ADDVALUE("STICK_QTY","'%i'",count*10); //8,15
			ADDVALUE("STICK_TIME","'%s %s'",date,time); //11,19
			ADDVALUE("TMU_QTY","'%i'",count*10); //8,15
			ADDVALUE("TMU_TIME","'%s %s'",date,time); //11,19
			ADDVALUE("LAST_ADJ","'%i'",count*2); //8,15
			ADDVALUE("ADJ_TIME","'%s %s'",date,time); //11,19
			ADDVALUE("LAST_DNLD","'%i'",count*2); //8,15
			ADDVALUE("DNLD_TIME","'%s %s'",date,time); //11,19
			ADDVALUE("LAST_XFER","'%i'",count*2); //8,15
			ADDVALUE("XFER_TIME","'%s %s'",date,time); //11,19
			ADDVALUE("FREE1","''",count); //12,15
			ADDVALUE("FREE2","''",count); //12,15
			ADDVALUE("FREE3","''",count); //12,15

			if(dbm.execute_sql_insert(&db)==FALSE){
				db.Close();return FALSE;
			}
			if(GetTickCount() > (elapsed+250)){
				printf("%i %i       \r",site,count);
				elapsed=GetTickCount();
			}
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}