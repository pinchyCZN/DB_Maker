int fill_ledger_transactions(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="transactions";
	int	field_count=0;

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
	increment_time(&systime,-24*2,0,0); //roll back few days

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	for(int count=0;count<500;count++)
	{
		CString vehid;
		char alphanumeric[26+10+1]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};

		if((count%50)==0)
			vehid.Format("%c%c%06i",'A'+rand()%26,'A'+rand()%26,count%10000);
		else if((count%40)==0)
			vehid.Format("%08i",rand());
		else
			vehid.Format("%08i",count%50);

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("downloadtime","'%s %s'",date,time); //11,19
		ADDVALUE("transtime","'%s %s'",date,time); //11,19
		ADDVALUE("dodaac","'"_DODAAC_"'",count%100,(count+1)%100); //12,6
		ADDVALUE("ui","'ui'",count); //12,2
		ADDVALUE("qty","%i",count); //8,15
		ADDVALUE("price","%i",count%100); //8,15
		ADDVALUE("grade","'%i'",count); //12,3
		ADDVALUE("siteid","'%06i'",(count%50)+1); //12,15
		ADDVALUE("tid","'tid_%i'",count%100); //12,15
		ADDVALUE("vehicleid","'%s'",vehid); //12,8
		ADDVALUE("odometer","%i",count); //4,10
		ADDVALUE("org","'ORG%c'",'A'+count%26); //12,4
		ADDVALUE("signal_vims","'%c'",alphanumeric[count%36]); //12,1
		ADDVALUE("fund_sys","'A%c'",alphanumeric[count%36]); //12,2
		ADDVALUE("pin","'%09i'",count); //12,9
		ADDVALUE("job","'%06i'",count); //12,6
		ADDVALUE("cic","'CI%c'",alphanumeric[count%36]); //12,3
		ADDVALUE("hose","%i",(count%8)+1); //4,10
		ADDVALUE("serial","%i",count%100); //4,10
		ADDVALUE("misc","'MISC%02i'",count%100); //12,6
		ADDVALUE("seller_dodaac","'%06i'",count); //12,6
		ADDVALUE("ric","'%03i'",count%100); //12,3
		ADDVALUE("cc_number","'%012i'",count); //12,50
		ADDVALUE("card_expire_year","%i",2000+count); //4,10
		ADDVALUE("card_expire_month","%i",count%12); //4,10
		ADDVALUE("trans_type","'TYPE_%i'",count%100); //12,15
		ADDVALUE("transcode","%i",count%100); //4,10
		ADDVALUE("rollover","%i",count%100); //4,10
		ADDVALUE("archive","%i",count%100); //4,10
		ADDVALUE("freeint1","%i",count%2); //4,10
		ADDVALUE("freeint2","%i",count%2); //4,10
		ADDVALUE("freetext1","'free1_%i'",count); //12,50
		ADDVALUE("freetext2","'free1_%i'",count); //12,50
		ADDVALUE("freetext3","'free1_%i'",count); //12,50
		ADDVALUE("FML_LOCAL_SALES_ID","'SALES_ID_%i'",count); //12,50
		ADDVALUE("trans_index","%i",count%100); //4,10
		ADDVALUE("subaccount","'sub_%i'",count); //12,50
		ADDVALUE("cc_ext_number","'%05i'",count%1000); //12,5

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}

		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
		}

	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}