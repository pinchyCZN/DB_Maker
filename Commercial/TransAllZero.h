int fill_transallzero(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="TransAllZero";

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

	int count=0;
	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	
	for(count=0;count<100;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);
	
		ADDVALUE("TC","'00'",count); //12,2
		ADDVALUE("ODOMETER","'%i'",count%100); //4,10
		ADDVALUE("ODOM_FLAG","'0'",count); //12,2
		ADDVALUE("USERID","'%09i'",count%100); //12,9
		ADDVALUE("HOSE","'0'",count); //5,5
		ADDVALUE("PRODUCT","'%i'",(count%50)+1); //5,5
		ADDVALUE("LQUANTITY","'%i'",count%20); //12,5
		ADDVALUE("INTKEY","'%i'",count%100); //12,9
		ADDVALUE("JDATE","'0'",count); //4,10
		ADDVALUE("MTIME","'0'",count); //12,8
		ADDVALUE("ORGAN","'0'",count); //12,3
		ADDVALUE("AGENCY","'123'",count); //12,7
		ADDVALUE("DNLDTIME","'%s %s'",date,time); //11,19
		ADDVALUE("TRAN_NO","'0'",count); //5,5
		ADDVALUE("FILL","'0'",count); //12,2
		ADDVALUE("QUANTITY","'0'",count); //8,15
		ADDVALUE("TRANTIME","'%s %s'",date,time); //11,19
		ADDVALUE("SITEID","'A%03i'",count%100); //12,4
		ADDVALUE("MOBILESITEID","'%04i'",count%1000); //12,4
		ADDVALUE("TOTALCOST","'0'",count); //8,15
		ADDVALUE("TANK","'0'",count); //5,5
		ADDVALUE("VEHICLEID","'%08i'",count%100); //12,8
		ADDVALUE("DIV_R","'0'",count); //5,5
		ADDVALUE("UNITCOST","'%i'",count%10); //8,15
		ADDVALUE("ONLINETYPE","'0'",count); //5,5
		ADDVALUE("FREE1","'0'",count); //12,15
		ADDVALUE("FREE2","'0'",count); //12,15
		ADDVALUE("FREE3","'0'",count); //12,15
		ADDVALUE("FREE4","'0'",count); //12,15
		ADDVALUE("FINCODE","'%i'",count%5); //5,5
		ADDVALUE("CCARDID","'0'",count); //12,20
		ADDVALUE("TRANNUM","'%i'",count%9999); //4,10
		ADDVALUE("INTKEY2","'0'",count); //12,9
		ADDVALUE("CCARDTYPE","'0'",count); //4,10
		ADDVALUE("PPCBALANCE","'0'",count); //8,15
		ADDVALUE("PPCCREDIT","'0'",count); //8,15
		ADDVALUE("CCTRANID","'0'",count); //12,9
		ADDVALUE("PRICELEVEL","'0'",count); //5,5
		ADDVALUE("CUSTOMDATA1","'0'",count); //12,255
		ADDVALUE("CUSTOMDATA2","'0'",count); //12,255
		ADDVALUE("AIMSTAT","'0'",count); //12,15
		ADDVALUE("EXCEPTION","0",count); //-7,1
		ADDVALUE("TRANTYPE","'2500PLUS'",count); //12,8
		ADDVALUE("COSTINGCODE","'VEHI'",count); //12,4
		ADDVALUE("PPCSTAT","'0'",count); //12,4
		ADDVALUE("SITECOUNTER","'0'",count); //4,10
		ADDVALUE("CUSTID","'0'",count); //12,9
		ADDVALUE("FMUTRANTYPE","'0'",count); //5,5
		ADDVALUE("TRANINITIATOR","'0'",count); //5,5
		ADDVALUE("TRANINITIATORMAPPED","'1'",count); //5,5
		ADDVALUE("CHRONOMETER1","'0'",count); //4,10
		ADDVALUE("CHRONOMETER2","'0'",count); //4,10
		ADDVALUE("TAILNUMBER","'0'",count); //12,8
		ADDVALUE("AIM2ID","'0'",count); //12,12
		ADDVALUE("CHRONOMETER3","'0'",count); //4,10
		ADDVALUE("Encrypt_KeyID","'0'",count); //5,5
		ADDVALUE("PABPCompliant","'1'",count); //4,10
		ADDVALUE("ADDITIVE","'0'",count); //5,5
		ADDVALUE("ALTERNATE","'0'",count); //4,10
		ADDVALUE("GRADENUM","'0'",count); //5,5

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

int fill_transzero(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="TransZero";

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

	int count=0;
	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	
	for(count=0;count<100;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);
		
		ADDVALUE("TC","'00'",count); //12,2
		ADDVALUE("ODOMETER","'%i'",count%100); //4,10
		ADDVALUE("ODOM_FLAG","'0'",count); //12,2
		ADDVALUE("USERID","'%09i'",count%100); //12,9
		ADDVALUE("HOSE","'0'",count); //5,5
		ADDVALUE("PRODUCT","'%i'",(count%50)+1); //5,5
		ADDVALUE("LQUANTITY","'%i'",count%20); //12,5
		ADDVALUE("INTKEY","'%i'",count%100); //12,9
		ADDVALUE("JDATE","'0'",count); //4,10
		ADDVALUE("MTIME","'0'",count); //12,8
		ADDVALUE("ORGAN","'0'",count); //12,3
		ADDVALUE("AGENCY","'123'",count); //12,7
		ADDVALUE("DNLDTIME","'%s %s'",date,time); //11,19
		ADDVALUE("TRAN_NO","'0'",count); //5,5
		ADDVALUE("FILL","'0'",count); //12,2
		ADDVALUE("QUANTITY","'0'",count); //8,15
		ADDVALUE("TRANTIME","'%s %s'",date,time); //11,19
		ADDVALUE("SITEID","'A%03i'",count%100); //12,4
		ADDVALUE("MOBILESITEID","'%04i'",count%1000); //12,4
		ADDVALUE("TOTALCOST","'0'",count); //8,15
		ADDVALUE("TANK","'0'",count); //5,5
		ADDVALUE("VEHICLEID","'%08i'",count%100); //12,8
		ADDVALUE("DIV_R","'0'",count); //5,5
		ADDVALUE("UNITCOST","'%i'",count%10); //8,15
		ADDVALUE("ONLINETYPE","'0'",count); //5,5
		ADDVALUE("FREE1","'0'",count); //12,15
		ADDVALUE("FREE2","'0'",count); //12,15
		ADDVALUE("FREE3","'0'",count); //12,15
		ADDVALUE("FREE4","'0'",count); //12,15
		ADDVALUE("FINCODE","'%i'",count%5); //5,5
		ADDVALUE("CCARDID","'0'",count); //12,20
		ADDVALUE("TRANNUM","'%i'",count%9999); //4,10
		ADDVALUE("INTKEY2","'0'",count); //12,9
		ADDVALUE("CCARDTYPE","'0'",count); //4,10
		ADDVALUE("PPCBALANCE","'0'",count); //8,15
		ADDVALUE("PPCCREDIT","'0'",count); //8,15
		ADDVALUE("CCTRANID","'0'",count); //12,9
		ADDVALUE("PRICELEVEL","'0'",count); //5,5
		ADDVALUE("CUSTOMDATA1","'0'",count); //12,255
		ADDVALUE("CUSTOMDATA2","'0'",count); //12,255
		ADDVALUE("AIMSTAT","'0'",count); //12,15
		ADDVALUE("EXCEPTION","0",count); //-7,1
		ADDVALUE("TRANTYPE","'2500PLUS'",count); //12,8
		ADDVALUE("COSTINGCODE","'VEHI'",count); //12,4
		ADDVALUE("PPCSTAT","'0'",count); //12,4
		ADDVALUE("SITECOUNTER","'0'",count); //4,10
		ADDVALUE("CUSTID","'0'",count); //12,9
		ADDVALUE("FMUTRANTYPE","'0'",count); //5,5
		ADDVALUE("TRANINITIATOR","'0'",count); //5,5
		ADDVALUE("TRANINITIATORMAPPED","'1'",count); //5,5
		ADDVALUE("CHRONOMETER1","'0'",count); //4,10
		ADDVALUE("CHRONOMETER2","'0'",count); //4,10
		ADDVALUE("TAILNUMBER","'0'",count); //12,8
		ADDVALUE("AIM2ID","'0'",count); //12,12
		ADDVALUE("CHRONOMETER3","'0'",count); //4,10
		ADDVALUE("Encrypt_KeyID","'0'",count); //5,5
		ADDVALUE("PABPCompliant","'1'",count); //4,10
		ADDVALUE("ADDITIVE","'0'",count); //5,5
		ADDVALUE("ALTERNATE","'0'",count); //4,10
		ADDVALUE("GRADENUM","'0'",count); //5,5
				
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