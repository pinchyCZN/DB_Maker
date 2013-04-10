int fill_user(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="User";

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
	increment_time(&systime,-72,0,0); //roll back few days

	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));

	int user=0;
	DWORD elapsed=0;

	int int_key=0;
	get_table_index(&db,"Config","USERINTKEY",&int_key);

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_users=50;
	get_ini_value(dbm.table_name,"count",&max_users);
	cout<<"user count "<<max_users<<endl;
	for(user=1;user<=max_users;user++) //cant have users of 00000000
	{
		int count;
		count=user;
		if(user==max_users)
			ADDVALUE("USERID","'USER%05i'",count); //12,9
		else
			ADDVALUE("USERID","'%09i'",count); //12,9
		ADDVALUE("LNAME","'userL%i'",count); //12,20
		ADDVALUE("FNAME","'userF%i'",count); //12,20
		ADDVALUE("MI","'%c'",'A'+(count%26)); //12,2
		ADDVALUE("CUSTID","'%09i'",count%100); //12,9
		ADDVALUE("SSN","'%i'",count%1000000); //12,11
		ADDVALUE("REISSUE","'%i'",count%2); //5,5
		ADDVALUE("DATEREISS","'%s %s'",date,time); //11,19
		ADDVALUE("EMPID","'%i'",count%1000); //12,11
		ADDVALUE("ENCODED","%i",0); //-7,1
		ADDVALUE("DATEENCOD","'%s %s'",date,time); //11,19
		ADDVALUE("AUTH","%i",-count%2); //-7,1
		ADDVALUE("DATEAUTH","'%s %s'",date,time); //11,19
		ADDVALUE("INTKEY","'%09i'",int_key++); //12,9
		ADDVALUE("OLDINTKEY","'%09i'",0); //12,9
		ADDVALUE("DEPTID","'dept%i'",count%99); //12,6
		ADDVALUE("COMMENT","'comment%i'",count); //12,40
		ADDVALUE("EMAIL","'email%i'",count); //12,50
		ADDVALUE("PHONENO","'phn%i'",count); //12,20
		ADDVALUE("FAXNO","'fax%i'",count); //12,20
		ADDVALUE("KEYTYPE","''",count); //12,2
		ADDVALUE("KO_ODOM","0",count); //-7,1
		ADDVALUE("KO_PIN","0",count); //-7,1
		ADDVALUE("KO_VEHIID","0",count); //-7,1
		ADDVALUE("KO_RECEIPT","0",count); //-7,1
		ADDVALUE("KO_FREE1","0",count); //-7,1
		ADDVALUE("KO_FREE2","0",count); //-7,1
		ADDVALUE("KO_FREE3","0",count); //-7,1
		ADDVALUE("AGENCY","'agcy%i'",count%1000); //12,7
		ADDVALUE("FREE1","'%i'",count%100); //12,15
		ADDVALUE("FREE2","''",count); //12,15
		ADDVALUE("FREE3","''",count); //12,15
		ADDVALUE("PROD_A","'%i'",count%100); //5,5
		ADDVALUE("PROD_B","'%i'",(count%8)+1); //5,5
		ADDVALUE("PROD_C","'0'",count); //5,5
		ADDVALUE("PROD_D","'0'",count); //5,5
		ADDVALUE("PROD_E","'0'",count); //5,5
		ADDVALUE("PROD_F","'0'",count); //5,5
		ADDVALUE("PROD_G","'0'",count); //5,5
		ADDVALUE("PROD_H","'0'",count); //5,5
		ADDVALUE("DPA_A","'%i'",count%100); //5,5
		ADDVALUE("DPA_B","'%i'",(count%8)+1); //5,5
		ADDVALUE("DPA_C","'0'",count); //5,5
		ADDVALUE("DPA_D","'0'",count); //5,5
		ADDVALUE("DPA_E","'0'",count); //5,5
		ADDVALUE("DPA_F","'0'",count); //5,5
		ADDVALUE("DPA_G","'0'",count); //5,5
		ADDVALUE("DPA_H","'0'",count); //5,5
		ADDVALUE("TRXLIM_A","'%i'",count%100); //5,5
		ADDVALUE("TRXLIM_B","'%i'",(count%8)+1); //5,5
		ADDVALUE("TRXLIM_C","'0'",count); //5,5
		ADDVALUE("TRXLIM_D","'0'",count); //5,5
		ADDVALUE("TRXLIM_E","'0'",count); //5,5
		ADDVALUE("TRXLIM_F","'0'",count); //5,5
		ADDVALUE("TRXLIM_G","'0'",count); //5,5
		ADDVALUE("TRXLIM_H","'0'",count); //5,5
		ADDVALUE("PRICELEV_A","'%i'",(count%8)+1); //12,2
		ADDVALUE("PRICELEV_B","'%i'",(count%8)+1); //12,2
		ADDVALUE("PRICELEV_C","'0'",count); //12,2
		ADDVALUE("PRICELEV_D","'0'",count); //12,2
		ADDVALUE("PRICELEV_E","'0'",count); //12,2
		ADDVALUE("PRICELEV_F","'0'",count); //12,2
		ADDVALUE("PRICELEV_G","'0'",count); //12,2
		ADDVALUE("PRICELEV_H","'0'",count); //12,2
		ADDVALUE("MATCH_A","%i",-count%2); //-7,1
		ADDVALUE("MATCH_B","%i",-count%2); //-7,1
		ADDVALUE("MATCH_C","0",count); //-7,1
		ADDVALUE("MATCH_D","0",count); //-7,1
		ADDVALUE("MATCH_E","0",count); //-7,1
		ADDVALUE("MATCH_F","0",count); //-7,1
		ADDVALUE("MATCH_G","0",count); //-7,1
		ADDVALUE("MATCH_H","0",count); //-7,1
		ADDVALUE("ENCODING","%i",-count%2); //-7,1
		ADDVALUE("KEYPADENABLE","%i",-count%2); //-7,1
		SKIP_VALUE("AUTOINCREMENT","''",count); //4,10

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}
		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",user);
			elapsed=GetTickCount();
		}
	}
	char val[20];
	sprintf(val,"%09d",int_key);
	set_table_index(&db,"Config","USERINTKEY","",val);

	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}

int fill_userlocklist(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="UserLockList";

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
	increment_time(&systime,-72,0,0); //roll back few days

	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));

	int user=0;
	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_users=50/2;
	get_ini_value(dbm.table_name,"count",&max_users);
	cout<<"user count "<<max_users<<endl;
	for(user=0;user<max_users;user++) //cant have users of 00000000
	{
		int count;
		count=user;

		ADDVALUE("INTKEY","'%09i'",count*2); //12,9
		ADDVALUE("LOCKDATE","'%s %s'",date,time); //11,19
		ADDVALUE("CODE","'%i'",(count%3)+1); //5,5
		ADDVALUE("FREE1","'F1'",count); //12,15
		ADDVALUE("FREE2","'F2'",count); //12,15
		ADDVALUE("FREE3","'F3'",count); //12,15

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}
		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",user);
			elapsed=GetTickCount();
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}
