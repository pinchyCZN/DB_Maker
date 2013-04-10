char *vehicle_gradesFIELDS[]={"vehicleid","grade","limit","freetext1","freetext2","freetext3","freeint1","freeint2","freeint3",0};
TABLE_PARAMS vehicle_grades_params={"vehicle_grades",vehicle_gradesFIELDS};

int fill_ledger_vehicle_grades(CString dbname, TABLE_PARAMS params)
{
	CDatabase db;
	CString SqlString;

	FILE *flog=0;

	open_database(&db,&dbname,g_dbpassword);

//	flog=fopen("log.txt","w");
//	get_fields(&db,params.name);
//	return 0;

	SqlString.Format("DELETE * FROM %s;",params.name);
	cout<<"deleting " << params.name <<"\n";
	db.ExecuteSQL(SqlString);

	cout<<"Writing to "<< params.name << " Table\n";

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-24*2,0,0); //roll back few days
	GetTimeFormat(LOCALE_USER_DEFAULT,NULL,&systime,"hh':'mm':'ss tt",time,20);
	GetDateFormat(LOCALE_USER_DEFAULT,NULL,&systime,"M'/'d'/'yyyy",date,20);

	CRecordset rec(&db);
	SqlString.Format("SELECT vehicleid FROM vehicle;");
	rec.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);

	int grade_count=0;
	int count=0;
	do
	{
		CString veh="";
		rec.GetFieldValue("vehicleid",veh);
		if(strlen(veh)==0)
		{
			cout<<"error setting grade of vehicle\n";
			break;
		}
		for(int grade=0;grade<8;grade++)
		{
			sql_insert_preamble(&db,&SqlString,&params);
		
			ADDVALUE(veh,"'%s'"); //vehicleid ,12,8
			ADDVALUE(1+(grade_count%163),", '%i'"); //grade ,4,10
			ADDVALUE(count%100,", '%i'"); //limit ,4,10
			ADDVALUE(count,", 'free1_%i'"); //freetext1 ,12,50
			ADDVALUE(count,", 'free2_%i'"); //freetext2 ,12,50
			ADDVALUE(count,", 'free3_%i'"); //freetext3 ,12,50
			ADDVALUE(count%10,", '%i'"); //freeint1 ,4,10
			ADDVALUE(count%10,", '%i'"); //freeint2 ,4,10
			ADDVALUE(count%10,", '%i'"); //freeint3 ,4,10

			sql_post_insertexecute(&db,&SqlString,flog);

			grade_count+=(rand()%15)+1;
			if(grade_count>163)
				grade_count=1;
		}
		rec.MoveNext();
		count++;
	}
	while(!rec.IsEOF());
	rec.Close();

	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}


char *vehicleFIELDS[]={"vehicleid","lockout","description","vehicle_year","cic","org","dodaac","signal","fund","vims","job","tid","misc","encoded","dateencoded","expire","expiration_date","trans_limit","fueling_limit","range","comment","reissue_count","reissue_date","free1","free2","free3","int1","subaccount","create_date",0};
TABLE_PARAMS vehicle_params={"vehicle",vehicleFIELDS};

int fill_ledger_vehicle(CString dbname, TABLE_PARAMS params)
{
	CDatabase db;
	CString SqlString;

	FILE *flog=0;

	open_database(&db,&dbname,g_dbpassword);

//	flog=fopen("log.txt","w");
//	get_fields(&db,params.name);
//	return 0;

	SqlString.Format("DELETE * FROM %s;",params.name);
	cout<<"deleting " << params.name <<"\n";
	db.ExecuteSQL(SqlString);

	cout<<"Writing to "<< params.name << " Table\n";

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-24*2,0,0); //roll back few days
	GetTimeFormat(LOCALE_USER_DEFAULT,NULL,&systime,"hh':'mm':'ss tt",time,20);
	GetDateFormat(LOCALE_USER_DEFAULT,NULL,&systime,"M'/'d'/'yyyy",date,20);

	char signalcode[]={'A','B','C','J','K','L'};
	char alphanumeric[26+10+1]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};

	for(int count=0;count<100;count++)
	{
		sql_insert_preamble(&db,&SqlString,&params);
	
		ADDVALUE(count,"'%08i'"); //vehicleid ,12,8
		ADDVALUE(count%16,", '%i'"); //lockout ,4,10
		ADDVALUE(count,", 'description%i'"); //description ,12,100
		ADDVALUE(2000+count,", '%04i'"); //vehicle_year ,4,10
		ADDVALUE('A'+count%26,", 'A%i%c'",count%10); //cic ,12,3
		ADDVALUE(count%10,", 'org%i'"); //org ,12,4
		ADDVALUE((count+1)%100,", '" _DODAAC_ "'",count%100); //dodaac ,12,6
		ADDVALUE(signalcode[count%6],", '%c'"); //signal ,12,1
		ADDVALUE(alphanumeric[(count+8)%36],", '%c%c'",alphanumeric[count%36]); //fund ,12,2
		ADDVALUE(count%10,", '%i'"); //vims ,12,1
		ADDVALUE(count%10,", '%i'"); //job ,12,6
		ADDVALUE("AGE",", '%s'"); //tid ,12,15
		ADDVALUE(count%100,", 'misc%02i'"); //misc ,12,6
		ADDVALUE(-count%2,", '%i'"); //encoded ,-7,1
		ADDVALUE(time,", '%s %s'",date); //dateencoded ,11,19
		ADDVALUE(-(count/10)%2,", '%i'"); //expire ,-7,1
		ADDVALUE(time,", '%s %s'",date); //expiration_date ,11,19
		ADDVALUE(10+count%100,", '%i'"); //trans_limit ,4,10
		ADDVALUE(10+count%100,", '%i'"); //fueling_limit ,4,10
		ADDVALUE(100+count%100,", '%i'"); //range ,4,10
		ADDVALUE(count,", 'comment%i'"); //comment ,12,50
		ADDVALUE(count%100,", '%i'"); //reissue_count ,4,10
		ADDVALUE(time,", '%s %s'",date); //reissue_date ,11,19
		ADDVALUE(count,", 'free1_%i'"); //free1 ,12,50
		ADDVALUE(count,", 'free2_%i'"); //free2 ,12,50
		ADDVALUE(count,", 'free3_%i'"); //free3 ,12,50
		ADDVALUE(count%100,", '%i'"); //int1 ,4,10
		ADDVALUE(count,", 'subact%i'"); //subaccount ,12,50
		ADDVALUE(time,", '%s %s'",date); //create_date ,11,19

		sql_post_insertexecute(&db,&SqlString,flog);
	}

	increment_time(&systime,24*365*10,0,0);
	GetTimeFormat(LOCALE_USER_DEFAULT,NULL,&systime,"hh':'mm':'ss tt",time,20);
	GetDateFormat(LOCALE_USER_DEFAULT,NULL,&systime,"M'/'d'/'yyyy",date,20);

	for(count=0;count<50;count++)
	{
		sql_insert_preamble(&db,&SqlString,&params);
		CString veh="";
		for(int i=0;i<8;i++)
		{
				veh+=alphanumeric[rand()%37];
		}
		ADDVALUE(veh,"'%s'"); //vehicleid ,12,8
		ADDVALUE(count%16,", '%i'"); //lockout ,4,10
		ADDVALUE(count,", 'description%i'"); //description ,12,100
		ADDVALUE(2000+count,", '%04i'"); //vehicle_year ,4,10
		ADDVALUE('A'+count%26,", 'A%i%c'",count%10); //cic ,12,3
		ADDVALUE(count%10,", 'org%i'"); //org ,12,4
		ADDVALUE((count+1)%100,", '" _DODAAC_ "'",count%100); //dodaac ,12,6
		ADDVALUE(signalcode[count%6],", '%c'"); //signal ,12,1
		ADDVALUE(alphanumeric[(count+8)%36],", '%c%c'",alphanumeric[count%36]); //fund ,12,2
		ADDVALUE(count%10,", '%i'"); //vims ,12,1
		ADDVALUE(count%10,", '%i'"); //job ,12,6
		ADDVALUE("AGE",", '%s'"); //tid ,12,15
		ADDVALUE(count%100,", 'misc%02i'"); //misc ,12,6
		ADDVALUE(-count%2,", '%i'"); //encoded ,-7,1
		ADDVALUE(time,", '%s %s'",date); //dateencoded ,11,19
		ADDVALUE(-count%2,", '%i'"); //expire ,-7,1
		ADDVALUE(time,", '%s %s'",date); //expiration_date ,11,19
		ADDVALUE(10+count%100,", '%i'"); //trans_limit ,4,10
		ADDVALUE(10+count%100,", '%i'"); //fueling_limit ,4,10
		ADDVALUE(100+count%100,", '%i'"); //range ,4,10
		ADDVALUE(count,", 'comment%i'"); //comment ,12,50
		ADDVALUE(count%100,", '%i'"); //reissue_count ,4,10
		ADDVALUE(time,", '%s %s'",date); //reissue_date ,11,19
		ADDVALUE(count,", 'free1_%i'"); //free1 ,12,50
		ADDVALUE(count,", 'free2_%i'"); //free2 ,12,50
		ADDVALUE(count,", 'free3_%i'"); //free3 ,12,50
		ADDVALUE(count%100,", '%i'"); //int1 ,4,10
		ADDVALUE(count,", 'subact%i'"); //subaccount ,12,50
		ADDVALUE(time,", '%s %s'",date); //create_date ,11,19

		sql_post_insertexecute(&db,&SqlString,flog);
	}
	cout<<"done\n---\n";
	db.Close();


/************************************************************/
	fill_ledger_vehicle_grades(dbname,vehicle_grades_params);
/************************************************************/


	return 0;
}


