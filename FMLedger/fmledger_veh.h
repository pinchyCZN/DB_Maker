int fill_ledger_veh(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="vehicle";
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
	for(int count=1;count<100;count++)
	{
		char signalcode[6]={'A','B','C','J','K','L'};
		char alphanumeric[26+10+1]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};
		char tid[11]={'0','1','2','3','4','5','6','7','8','9','N'}; //use code

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("vehicleid","'%08i'",count); //12,8
		ADDVALUE("lockout","%i",count/10); //4,10
		ADDVALUE("description","'descrip_%i'",count); //12,100
		ADDVALUE("vehicle_year","%i",1900+count); //4,10
		ADDVALUE("cic","'CI%c'",'A'+count%26); //12,3
		ADDVALUE("org","'ORG%c'",'A'+count%26); //12,4
		ADDVALUE("dodaac","'"_DODAAC_"'",count%100,(count+1)%100); //12,6
		ADDVALUE("signal","'%c'",signalcode[count%6]); //12,1
		ADDVALUE("fund","'%c%c'",alphanumeric[count%36],alphanumeric[(count+8)%36]); //12,2
		ADDVALUE("vims","'%i'",count%10); //12,1
		ADDVALUE("job","'%i'",count%10); //12,6
		ADDVALUE("tid","'%c'",tid[count%11]); //12,15
		ADDVALUE("misc","'MISC%02i'",count%100); //12,6
		ADDVALUE("encoded","%i",-count%2); //-7,1
		ADDVALUE("dateencoded","'%s %s'",date,time); //11,19
		ADDVALUE("expire","%i",-(count/10)%2); //-7,1
		ADDVALUE("expiration_date","'%s %s'",date,time); //11,19
		ADDVALUE("trans_limit","%i",1+count%100); //4,10
		ADDVALUE("fueling_limit","%i",1+count%100); //4,10
		ADDVALUE("range","%i",100+count%100); //4,10
		ADDVALUE("comment","'comment_%i'",count); //12,50
		ADDVALUE("reissue_count","%i",count); //4,10
		ADDVALUE("reissue_date","'%s %s'",date,time); //11,19
		ADDVALUE("free1","'free1_%i'",count); //12,50
		ADDVALUE("free2","'free2_%i'",count); //12,50
		ADDVALUE("free3","'free3_%i'",count); //12,50
		ADDVALUE("int1","%i",count%5); //4,10
		ADDVALUE("subaccount","'suba_%i'",count); //12,50
		ADDVALUE("create_date","'%s %s'",date,time); //11,19

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

int fill_ledger_veh_grades(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="vehicle_grades";
	int	field_count=0;

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	DWORD elapsed=0;
	int count=0;
	int grade_count=0;

	CRecordset rec(&db);
	CString SqlString;
	SqlString.Format("SELECT vehicleid FROM vehicle;");
	if(db.IsOpen())rec.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	while(rec.IsOpen() && !rec.IsEOF())
	{
		CString veh="";
		rec.GetFieldValue("vehicleid",veh);
		if(strlen(veh)==0){
			cout<<"error setting grade of vehicle\n";
			break;
		}
		for(int grade=0;grade<8;grade++){

			ADDVALUE("vehicleid","'%s'",veh); //12,8
			ADDVALUE("grade","%i",1+(grade_count%163)); //4,10
			ADDVALUE("limit","%i",count%100); //4,10
			ADDVALUE("freetext1","'free1_%i'",count); //12,50
			ADDVALUE("freetext2","'free2_%i'",count); //12,50
			ADDVALUE("freetext3","'free3_%i'",count); //12,50
			ADDVALUE("freeint1","%i",count); //4,10
			ADDVALUE("freeint2","%i",count); //4,10
			ADDVALUE("freeint3","%i",count); //4,10

			grade_count+=(rand()%15)+1;
			if(grade_count>163)
				grade_count=1;

			if(dbm.execute_sql_insert(&db)==FALSE){
				db.Close();
				return FALSE;
			}

			if(GetTickCount() > (elapsed+250)){
				printf("%i       \r",count);
				elapsed=GetTickCount();
			}
		}
		rec.MoveNext();
		count++;
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}
int fill_ledger_configvil(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="configvil";
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
	for(int count=0;count<100;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("vilcode","'%08i'",count+1000); //12,8
		ADDVALUE("lockout","%i",count/10); //4,10
		ADDVALUE("encoded","%i",-count%2); //-7,1
		ADDVALUE("dateencoded","'%s %s'",date,time); //11,19
		ADDVALUE("expire","%i",-count%2); //-7,1
		ADDVALUE("expiration_date","'%s %s'",date,time); //11,19
		ADDVALUE("comment","'comment_%i'",count); //12,50
		ADDVALUE("freeint1","%i",count); //4,10
		ADDVALUE("freeint2","%i",count); //4,10
		ADDVALUE("freetext1","'free1_%i'",count); //12,50
		ADDVALUE("freetext2","'free2_%i'",count); //12,50

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

int fill_ledger_mobilevil(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="mobilevil";
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
	for(int count=0;count<100;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("vilcode","'%08i'",count+2000); //12,8
		ADDVALUE("lockout","%i",count/10); //4,10
		ADDVALUE("encoded","%i",-count%2); //-7,1
		ADDVALUE("dateencoded","'%s %s'",date,time); //11,19
		ADDVALUE("expire","%i",-count%2); //-7,1
		ADDVALUE("expiration_date","'%s %s'",date,time); //11,19
		ADDVALUE("comment","'comment_%i'",count); //12,50
		ADDVALUE("freeint1","%i",count); //4,10
		ADDVALUE("freeint2","%i",count); //4,10
		ADDVALUE("freetext1","'free1_%i'",count); //12,50
		ADDVALUE("freetext2","'free2_%i'",count); //12,50

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

int fill_ledger_vil_lock(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="vil_lock";
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

	int max_count=300;
	if(get_ini_value(dbm.table_name,"max_count",&max_count))
		cout<<"max_count="<<max_count<<endl;

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	for(int count=0;count<max_count;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);
		int vils[3]={0,1000,2000};
		//keytype: 0=veh,1=config,2=mobile
		ADDVALUE("vehiclevil","'%08i'",(count<300?((count*2)%200):(count*2))+vils[(count/100)<3?(count/100):2]); //12,8
		ADDVALUE("lockout","%i",((count*2)/10)%20); //4,10
		ADDVALUE("lockoutdate","'%s %s'",date,time); //11,19
		ADDVALUE("keytype","%i",(count/100)%3); //4,10
		ADDVALUE("comment","'comment_%i'",count); //12,50
		ADDVALUE("freeint1","%i",count); //4,10
		ADDVALUE("freeint2","%i",count); //4,10
		ADDVALUE("freetext1","'free1_%i'",count); //12,50
		ADDVALUE("freetext2","'free2_%i'",count); //12,50

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

int fill_ledger_rules(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="rules";
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

	int max_count=100;
	if(get_ini_value(dbm.table_name,"max_count",&max_count))
		cout<<"max_count="<<max_count<<endl;

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	for(int count=0;count<max_count;count++)
	{
		char time2[20],date2[20];
		SYSTEMTIME systime2;

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		systime2=systime;
		if((count/4)&1)
			increment_time(&systime2,24*365,0,0); //end_date in the future
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime2,"hh':'mm':'ss tt",time2,sizeof(time2));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime2,"M'/'d'/'yyyy",date2,sizeof(date2));
		increment_time(&systime,0,0,1);

		char signalcode[]={'A','B','C','J','K','L'};

		SKIP_VALUE("rule_number","%i",count%15); //4,10
		ADDVALUE("rule_type","%i",count%7); //4,10
		ADDVALUE("create_date","'%s %s'",date,time); //11,19
		ADDVALUE("user_id","%i",count%10); //4,10
		ADDVALUE("match_dodaac","'%06i'",count*100); //12,6
		ADDVALUE("match_cic","'CI%c'",'A'+count%26); //12,3
		ADDVALUE("match_signal","'%c'",signalcode[count%6]); //12,1
		ADDVALUE("match_org","'ORG%i'",count%10); //12,4
		ADDVALUE("match_jon","'JON%i'",count%10); //12,6
		ADDVALUE("match_fund","'F%c'",'A'+count%26); //12,2
		ADDVALUE("vehicleid","'%08i'",count*100); //12,8
		ADDVALUE("new_org","'ORG%i'",count%10); //12,4
		ADDVALUE("new_jon","'JON%i'",count%10); //12,6
		ADDVALUE("new_signal","'%c'",signalcode[count%6]); //12,1
		ADDVALUE("new_fund","'F%c'",'A'+count%26); //12,2
		ADDVALUE("tid_code","'TID_%i'",count%100); //12,50
		ADDVALUE("start_date","'%s %s'",date,time); //11,19
		ADDVALUE("end_date","'%s %s'",date2,time2); //11,19
		ADDVALUE("comment","'comment_%i'",count); //12,255
		ADDVALUE("deleted","%i",0); //-7,1
		ADDVALUE("sql_statement","'--SQL BLAH BLAH %i'",count); //12,255
		ADDVALUE("applied","%i",1); //4,10
		ADDVALUE("apply_date","'%s %s'",date,time); //11,19
		ADDVALUE("new_misc","'NMISC_%i'",count); //12,50
		ADDVALUE("match_misc","'MMISC_%i'",count); //12,50
		ADDVALUE("match_tid","'MTID_%i'",count); //12,50
		ADDVALUE("new_dodaac","'N%05i'",count); //12,6
		ADDVALUE("new_cic","'NE%i'",count%10); //12,3
		ADDVALUE("new_grade","'10%i'",count%10); //12,3

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