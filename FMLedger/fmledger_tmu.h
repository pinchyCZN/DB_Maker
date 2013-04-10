int fill_ledger_tmualarm(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="tmu_alarm";
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
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("siteid","'%06i'",count); //12,15
		ADDVALUE("tank","%i",count%100); //4,10
		ADDVALUE("transtime","'%s %s'",date,time); //11,19
		ADDVALUE("alarm_index","%i",count%30); //4,10
		ADDVALUE("description","'description_%i'",count); //12,50
		ADDVALUE("archive","%i",count%100); //4,10
		ADDVALUE("free_int1","%i",count%1000); //4,10
		ADDVALUE("free_int2","%i",count%1000); //4,10
		ADDVALUE("free_text1","'free_%i'",count%1000); //12,50
		ADDVALUE("free_text2","'free_%i'",count%1000); //12,50

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
int fill_ledger_tmucsld(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="tmu_csld";
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
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("siteid","'%06i'",count); //12,15
		ADDVALUE("tank","%i",count%100); //4,10
		ADDVALUE("csld_index","%i",count%15); //4,10
		ADDVALUE("description","'descrip_%i'",count); //12,50
		ADDVALUE("transtime","'%s %s'",date,time); //11,19
		ADDVALUE("archive","%i",count%100); //4,10
		ADDVALUE("free_int1","%i",count); //4,10
		ADDVALUE("free_int2","%i",count); //4,10
		ADDVALUE("free_text1","'free_%i'",count%1000); //12,50
		ADDVALUE("free_text2","'free_%i'",count%1000); //12,50

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
int fill_ledger_tmuinventory(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="tmu_inventory";
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
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("siteid","'%06i'",count); //12,15
		ADDVALUE("tank","%i",count%100); //4,10
		ADDVALUE("downloadtime","'%s %s'",date,time); //11,19
		ADDVALUE("transtime","'%s %s'",date,time); //11,19
		ADDVALUE("product","%i",count%100); //4,10
		ADDVALUE("volume","%i",count%100); //8,15
		ADDVALUE("tc_volume","%i",count%100); //8,15
		ADDVALUE("ullage","%i",count%100); //8,15
		ADDVALUE("height","%i",count); //8,15
		ADDVALUE("water","%i",count); //8,15
		ADDVALUE("temp","%i",count); //8,15
		ADDVALUE("water_volume","%i",count); //8,15
		ADDVALUE("archive","%i",count); //4,10
		ADDVALUE("free_int1","%i",count); //4,10
		ADDVALUE("free_int2","%i",count); //4,10
		ADDVALUE("free_text1","'free_%i'",count); //12,50
		ADDVALUE("free_text2","'free_%i'",count); //12,50
		ADDVALUE("ui","'ui_%i'",count); //12,50
		ADDVALUE("linear_ui","'lui_%i'",count); //12,50
		ADDVALUE("temperature_ui","'tui_%i'",count); //12,50

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

int fill_ledger_tmuleak(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="tmu_leak";
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
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("siteid","'%06i'",count); //12,15
		ADDVALUE("tank","%i",count%100); //4,10
		ADDVALUE("downloadtime","'%s %s'",date,time); //11,19
		ADDVALUE("product","%i",count%100); //4,10
		ADDVALUE("start_time","'%s %s'",date,time); //11,19
		ADDVALUE("duration","%i",count); //8,15
		ADDVALUE("start_temp","%i",count); //8,15
		ADDVALUE("start_volume","%i",count); //8,15
		ADDVALUE("end_temp","%i",count); //8,15
		ADDVALUE("leak_rate","%i",count); //8,15
		ADDVALUE("hourly_changes","'hr_ch_%i'",count); //12,50
		ADDVALUE("archive","%i",count); //4,10
		ADDVALUE("free_int1","%i",count); //4,10
		ADDVALUE("free_int2","%i",count); //4,10
		ADDVALUE("free_text1","'free1_%i'",count); //12,50
		ADDVALUE("free_text2","'free2_%i'",count); //12,50
		ADDVALUE("ui","'ui_%i'",count); //12,50
		ADDVALUE("linear_ui","'lui_%i'",count); //12,50
		ADDVALUE("temperature_ui","'tui_%i'",count); //12,50

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
int fill_ledger_tmulv(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="tmu_liquid_vapor";
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
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("alarm_type","%i",(count%10)); //4,10
		ADDVALUE("siteid","'%06i'",count); //12,15
		ADDVALUE("tank","%i",count); //4,10
		ADDVALUE("transtime","'%s %s'",date,time); //11,19
		ADDVALUE("sensor_index","%i",count%100); //4,10
		ADDVALUE("description","'description_%i'",count); //12,50
		ADDVALUE("archive","%i",count%100); //4,10
		ADDVALUE("free_int1","%i",count%50); //4,10
		ADDVALUE("free_int2","%i",count%50); //4,10
		ADDVALUE("free_text1","'free1_%i'",count); //12,50
		ADDVALUE("free_text2","'free2_%i'",count); //12,50

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
int fill_ledger_tmushiftinv(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="tmu_shift_inventory";
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
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("siteid","'%06i'",count); //12,15
		ADDVALUE("tank","%i",count); //4,10
		ADDVALUE("downloadtime","'%s %s'",date,time); //11,19
		ADDVALUE("transtime","'%s %s'",date,time); //11,19
		ADDVALUE("shift","%i",count); //4,10
		ADDVALUE("product","%i",count%100); //4,10
		ADDVALUE("start_volume","%i",count); //8,15
		ADDVALUE("start_tc_volume","%i",count); //8,15
		ADDVALUE("start_ullage","%i",count); //8,15
		ADDVALUE("start_height","%i",count); //8,15
		ADDVALUE("start_water","%i",count); //8,15
		ADDVALUE("start_temp","%i",count); //8,15
		ADDVALUE("end_volume","%i",count); //8,15
		ADDVALUE("end_tc_volume","%i",count); //8,15
		ADDVALUE("end_ullage","%i",count); //8,15
		ADDVALUE("end_height","%i",count); //8,15
		ADDVALUE("end_water","%i",count); //8,15
		ADDVALUE("end_temp","%i",count); //8,15
		ADDVALUE("total","%i",count); //8,15
		ADDVALUE("archive","%i",count); //4,10
		ADDVALUE("free_int1","%i",count); //4,10
		ADDVALUE("free_int2","%i",count); //4,10
		ADDVALUE("free_text1","'free1_%i'",count); //12,50
		ADDVALUE("free_text2","'free2_%i'",count); //12,50
		ADDVALUE("ui","'ui_%i'",count); //12,50
		ADDVALUE("linear_ui","'lui_%i'",count); //12,50
		ADDVALUE("temperature_ui","'tui_%i'",count); //12,50

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