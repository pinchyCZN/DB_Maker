int fill_unitconfig(CString dbname)  //FMU stuff
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="UnitConfig";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}
	int max_site=20;
	if(get_ini_value(dbm.table_name,"max_site",&max_site))
		cout<<"max_site="<<max_site<<endl;

	int max_unit=1;
	if(get_ini_value(dbm.table_name,"max_unit",&max_unit))
		cout<<"max_unit="<<max_unit<<endl;

	int fmu_name_limit=9999;
	get_ini_value(dbm.table_name,"fmu_name_limit",&fmu_name_limit);
	cout<<"fmu_name_limit:"<<fmu_name_limit<<endl;

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-72,0,0); //roll back few days

	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));

	int count=0;
	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int total_count=1;
	for(count=1;count<=max_site;count++)
	{
		int unit;
		for(unit=0;unit<max_unit;unit++){
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		increment_time(&systime,0,0,1);

		ADDVALUE("SITEID","'A%03i'",count); //12,50
		ADDVALUE("UNITNUM","'%i'",unit); //5,5
		ADDVALUE("UNITNAME","'fmu%04i'",count<=fmu_name_limit?count:fmu_name_limit); //12,50
		ADDVALUE("PUMP","'%i'",total_count++); //5,5
		ADDVALUE("VALIDKEY","'30'",count); //5,5
		ADDVALUE("MESSAGEDUR","'4'",count); //5,5
		ADDVALUE("DECDIGITS","'2'",count); //5,5
		ADDVALUE("MODBEGHOUR","'0'",count); //12,5
		ADDVALUE("MODENDHOUR","'24'",count); //12,5
		ADDVALUE("ZEROQUANT","'50'",count); //5,5
		ADDVALUE("PUMPTIMEOUT","'10'",count); //5,5
		ADDVALUE("JDHOUR","'0'",count); //5,5
		ADDVALUE("UPDATETIME","'%s %s'",date,time); //11,19
		ADDVALUE("LOADTIME","'%s %s'",date,time); //11,19
		ADDVALUE("TOTADIGITS_L","'8'",count); //5,5
		ADDVALUE("TOTADIGITS_R","'3'",count); //5,5
		ADDVALUE("FMUFLASHVER","'FUELMASTER FMU2500+'",count); //12,50
		ADDVALUE("FREE1","'free1'",count); //12,15
		ADDVALUE("FREE2","'free2'",count); //12,15
		ADDVALUE("FREE3","'free3'",count); //12,15
		ADDVALUE("CONFIGVERSION","'0'",count); //5,5
		ADDVALUE("DOWNLOADTIME","'%s %s'",date,time); //11,19
		ADDVALUE("VERIFYTIME","'%s %s'",date,time); //11,19
		ADDVALUE("VERIFYERROR","'0'",count); //5,5
		ADDVALUE("VERIFIEDPRICE","0",-count%2); //-7,1
		ADDVALUE("eprom_version","'ver%i'",count); //12,50
		ADDVALUE("last_flash","'%s %s'",date,time); //11,19

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}
		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
		}
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}

int fill_pumpconfig(CString dbname)  //FMU stuff
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="PumpConfig";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}
	int max_site=20;
	if(get_ini_value("UnitConfig","max_site",&max_site))
		cout<<"max_site="<<max_site<<endl;

	int max_unit=1;
	if(get_ini_value("UnitConfig","max_unit",&max_unit))
		cout<<"max_unit="<<max_unit<<endl;
	max_site*=max_unit;

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";


	int total_count=1;
	for(int fmu=1;fmu<=max_site;fmu++)
	{
		for(int count=1;count<=8;count++)
		{
			ADDVALUE("PUMP","'%i'",fmu); //5,5
			ADDVALUE("PUMPNUM","'%i'",count); //5,5
			ADDVALUE("HOSENUM","'%i'",count); //5,5
			ADDVALUE("GRADE","'%i'",total_count); //5,5
			total_count++;
			ADDVALUE("ELECTRONIC","0",count); //-7,1
			ADDVALUE("DIVRATIO","'10'",count); //5,5
			ADDVALUE("NPT","'30'",count); //5,5
			ADDVALUE("PFTO","'10'",count); //4,10
			ADDVALUE("PHL","'3'",count); //5,5
			ADDVALUE("PIE","'0'",count); //5,5

			if(dbm.execute_sql_insert(&db)==FALSE){
				db.Close();return FALSE;
			}
			if(GetTickCount() > (elapsed+250)){
				printf("%i %i      \r",fmu,count);
				elapsed=GetTickCount();
			}
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}


int fill_gradeconfig(CString dbname)  //FMU stuff
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="GradeConfig";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	int max_site=20;
	if(get_ini_value("UnitConfig","max_site",&max_site))
		cout<<"max_site="<<max_site<<endl;

	int max_unit=1;
	if(get_ini_value("UnitConfig","max_unit",&max_unit))
		cout<<"max_unit="<<max_unit<<endl;
	max_site*=max_unit;

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int total_count=1;
	for(int fmu=0;fmu<max_site;fmu++)
	{
		for(int count=1;count<=8;count++)
		{
			ADDVALUE("GRADE","'%i'",total_count); //5,5
			ADDVALUE("GRADENUM","'1'",count); //5,5
			ADDVALUE("TANK","'%i'",count); //5,5
			ADDVALUE("PIE","'1'",count); //5,5
			ADDVALUE("PLEVEL","'0'",count); //5,5
			ADDVALUE("AIMNOZZLE","'%i'",70+(count%10)); //5,5

			total_count++;

			if(dbm.execute_sql_insert(&db)==FALSE){
				db.Close();return FALSE;
			}
			if(GetTickCount() > (elapsed+250)){
				printf("%i %i      \r",fmu,count);
				elapsed=GetTickCount();
			}
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}