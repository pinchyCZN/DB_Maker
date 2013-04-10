int fill_utility(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="Utility";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"WHERE UTILITYID<>'00000001'"))
	{
		db.Close();return FALSE;
	}

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-24*2,0,0); //roll back few days

	DWORD elapsed=0;
	int int_key=0;
	get_table_index(&db,"Config","VEHIINTKEY",&int_key);

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(int count=2;count<100;count++)
	{
		char keytype[4]={'S','M','T','A'};

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("UTILITYID","'%08i'",count); //12,8
		ADDVALUE("DESCRIPT","'descript%04i'",count); //12,40
		ADDVALUE("VEHIYEAR","'%04i'",count+2000); //5,5
		ADDVALUE("REISSUE","'%i'",count%10); //5,5
		ADDVALUE("DATEREISS","'%s %s'",date,time); //11,19
		ADDVALUE("ENCODED","%i",-count%2); //-7,1
		ADDVALUE("DATEENCOD","'%s %s'",date,time); //11,19
		ADDVALUE("AUTH","%i",-count%2); //-7,1
		ADDVALUE("DATEAUTH","'%s %s'",date,time); //11,19
		ADDVALUE("KEYTYPE","'%c'",keytype[(count/10)%sizeof(keytype)]); //12,2
		ADDVALUE("INTKEY","'%09i'",int_key++); //12,9
		ADDVALUE("OLDINTKEY","'%09i'",0); //12,9
		ADDVALUE("DEPTID","'%09i'",count%1000); //12,9
		ADDVALUE("CUSTID","'%09i'",count); //12,9
		ADDVALUE("COMMENT","'comment%04i'",count); //12,40
		ADDVALUE("TAXABLE","%i",-count%2); //-7,1
		ADDVALUE("KO_PIN","%i",-count%2); //-7,1
		ADDVALUE("KO_USERID","%i",-count%2); //-7,1
		ADDVALUE("KO_RECEIPT","%i",-count%2); //-7,1
		ADDVALUE("SITEID","'A%03i'",count%100); //12,4
		ADDVALUE("SITENAME","'sitename%04i'",count); //12,40
		ADDVALUE("MISC1","'misc1%04i'",count); //12,15
		ADDVALUE("MISC2","'misc2%04i'",count); //12,15
		ADDVALUE("MISC3","'misc3%04i'",count); //12,15
		ADDVALUE("MISC4","'misc4%04i'",count); //12,15
		ADDVALUE("MISC5","'misc5%04i'",count); //12,15
		ADDVALUE("FREE1","'free1%04i'",count); //12,15
		ADDVALUE("FREE2","'free2%04i'",count); //12,15
		ADDVALUE("FREE3","'free3%04i'",count); //12,15
		ADDVALUE("PROD_A","'%i'",count%10); //5,5
		ADDVALUE("PROD_B","'%i'",count%10); //5,5
		ADDVALUE("PROD_C","'%i'",count%10); //5,5
		ADDVALUE("PROD_D","'%i'",count%10); //5,5
		ADDVALUE("PROD_E","'%i'",count%10); //5,5
		ADDVALUE("PROD_F","'%i'",count%10); //5,5
		ADDVALUE("PROD_G","'%i'",count%10); //5,5
		ADDVALUE("PROD_H","'%i'",count%10); //5,5
		ADDVALUE("DPA_A","'%i'",count*10%10); //5,5
		ADDVALUE("DPA_B","'%i'",count*10%10); //5,5
		ADDVALUE("DPA_C","'%i'",count*10%10); //5,5
		ADDVALUE("DPA_D","'%i'",count*10%10); //5,5
		ADDVALUE("DPA_E","'%i'",count*10%10); //5,5
		ADDVALUE("DPA_F","'%i'",count*10%10); //5,5
		ADDVALUE("DPA_G","'%i'",count*10%10); //5,5
		ADDVALUE("DPA_H","'%i'",count*10%10); //5,5
		ADDVALUE("TRXLIM_A","'%i'",count*10%10); //5,5
		ADDVALUE("TRXLIM_B","'%i'",count*10%10); //5,5
		ADDVALUE("TRXLIM_C","'%i'",count*10%10); //5,5
		ADDVALUE("TRXLIM_D","'%i'",count*10%10); //5,5
		ADDVALUE("TRXLIM_E","'%i'",count*10%10); //5,5
		ADDVALUE("TRXLIM_F","'%i'",count*10%10); //5,5
		ADDVALUE("TRXLIM_G","'%i'",count*10%10); //5,5
		ADDVALUE("TRXLIM_H","'%i'",count*10%10); //5,5
		ADDVALUE("PRICELEV_A","'%i'",count%10); //12,2
		ADDVALUE("PRICELEV_B","'%i'",count%10); //12,2
		ADDVALUE("PRICELEV_C","'%i'",count%10); //12,2
		ADDVALUE("PRICELEV_D","'%i'",count%10); //12,2
		ADDVALUE("PRICELEV_E","'%i'",count%10); //12,2
		ADDVALUE("PRICELEV_F","'%i'",count%10); //12,2
		ADDVALUE("PRICELEV_G","'%i'",count%10); //12,2
		ADDVALUE("PRICELEV_H","'%i'",count%10); //12,2
		ADDVALUE("MATCH_A","%i",-count%2); //-7,1
		ADDVALUE("MATCH_B","%i",-count%2); //-7,1
		ADDVALUE("MATCH_C","%i",-count%2); //-7,1
		ADDVALUE("MATCH_D","%i",-count%2); //-7,1
		ADDVALUE("MATCH_E","%i",-count%2); //-7,1
		ADDVALUE("MATCH_F","%i",-count%2); //-7,1
		ADDVALUE("MATCH_G","%i",-count%2); //-7,1
		ADDVALUE("MATCH_H","%i",-count%2); //-7,1
		ADDVALUE("SERVDATE","'%s %s'",date,time); //11,19
		ADDVALUE("USAGE","'use%04i'",count%100); //12,10
		ADDVALUE("AGENCY","'acy%02i'",count%100); //12,5
		ADDVALUE("ORG","'%i'",count%100); //12,3
		ADDVALUE("KO_FREE1","%i",-count%2); //-7,1
		ADDVALUE("KO_FREE2","%i",-count%2); //-7,1
		ADDVALUE("KO_FREE3","%i",-count%2); //-7,1
		ADDVALUE("ENCODING","%i",-count%2); //-7,1
		SKIP_VALUE("AUTOINCREMENT","''",count); //4,10

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}

		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
		}
	}
	char val[20];
	sprintf(val,"%09d",int_key);
	set_table_index(&db,"Config","VEHIINTKEY","",val);

	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}