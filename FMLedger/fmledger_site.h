int fill_ledger_site(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="site";
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

	int max_site=100;
	if(get_ini_value(dbm.table_name,"max_site",&max_site))
		cout<<"max_site="<<max_site<<endl;

	CString ipaddr="172.16.40.99";
	if(get_ini_str(dbm.table_name,"ipaddr",&ipaddr))
		cout<<"ipaddr="<<(LPCSTR)ipaddr<<endl;

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	for(int count=1;count<=max_site;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("siteid","'%06i'",count); //12,15
		ADDVALUE("description","'descrip_%i'",count); //12,50
		ADDVALUE("fmu","'%09i'",count); //12,50
		ADDVALUE("phone","'phone%i'",count); //12,50
		ADDVALUE("contact","'contact%i'",count); //12,50
		ADDVALUE("address1","'addr1_%i'",count); //12,50
		ADDVALUE("address2","'addr2_%i'",count); //12,50
		ADDVALUE("city","'city_%i'",count); //12,50
		ADDVALUE("state","'state_%i'",count); //12,50
		ADDVALUE("zip","'zip_%i'",count); //12,50
		ADDVALUE("country","'country_%i'",count); //12,50
		ADDVALUE("base","'base_%i'",count); //12,50
		ADDVALUE("dodaac","'"_DODAAC_"'",count%10,(count+1)%10); //12,6
		ADDVALUE("connect_method","%i",(count/10)%2); //4,10
		ADDVALUE("vir_ip","'%s'",ipaddr); //12,50
		ADDVALUE("comport","%i",count%4); //4,10
		ADDVALUE("email","'email_%i'",count); //12,50
		ADDVALUE("fax","'fax_%i'",count); //12,50
		ADDVALUE("comment","'comment_%i'",count); //12,50
		ADDVALUE("download","'%s %s'",date,time); //11,19
		ADDVALUE("freeint1","%i",count); //4,10
		ADDVALUE("freeint2","%i",count); //4,10
		ADDVALUE("freetext1","''",count); //12,50
		ADDVALUE("freetext2","''",count); //12,50
		ADDVALUE("freetext3","''",count); //12,50
		ADDVALUE("buad_rate","%i",2400*(1<<(count%3))); //4,10
		ADDVALUE("fmu_type","%i",count%100); //4,10
		ADDVALUE("send_rules","%i",count); //4,10
		ADDVALUE("send_vil_lock","%i",count); //4,10
		ADDVALUE("send_dodaac_lock","%i",count); //4,10
		ADDVALUE("send_org_lock","%i",count); //4,10
		ADDVALUE("LOCAL_RECORD_ID","%i",count); //4,10

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
int fill_ledger_unitconfig(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="unit_config";
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

	int max_unit=9;
	if(get_ini_value(dbm.table_name,"max_unit",&max_unit))
		cout<<"max_unit="<<max_unit<<endl;

	int max_site=100;
	if(get_ini_value(dbm.table_name,"max_site",&max_site))
		cout<<"max_site="<<max_site<<endl;

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	for(int count=1;count<=max_site;count++)
	{
		for(int unit=0;unit<max_unit;unit++){
			GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
			GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
			increment_time(&systime,0,0,1);

			ADDVALUE("siteid","'%06i'",count); //12,15
			ADDVALUE("unit","%i",unit); //4,10
			ADDVALUE("unit_name","'unit_%i'",unit); //12,17
			ADDVALUE("dodaac","'"_DODAAC_"'",count,count+1); //12,6
			ADDVALUE("ric","'US%c'",'A'+count%26); //12,3
			ADDVALUE("system_des","1",count); //4,10
			ADDVALUE("max_zero_trans","%i",5+unit%9); //4,10
			ADDVALUE("rollover","%i",unit%4); //4,10
			ADDVALUE("vk_timer","%i",20+unit%9); //4,10
			ADDVALUE("mess_dur","%i",5+unit%9); //4,10
			ADDVALUE("modem_begin","%i",rand()%6); //4,10
			ADDVALUE("modem_end","%i",19+(rand()%6)); //4,10
			ADDVALUE("user_options","%i",100+count%29); //4,10
			ADDVALUE("system_options","%i",100+count%61); //4,10
			ADDVALUE("tmu_options","%i",100+count%20); //4,10
			ADDVALUE("rec_options","%i",100+count%48); //4,10
			ADDVALUE("eprom_version","'ver_%i'",count); //12,50
			ADDVALUE("last_change","'%s %s'",date,time); //11,19
			ADDVALUE("last_modified","'%s %s'",date,time); //11,19
			ADDVALUE("last_flash","'%s %s'",date,time); //11,19
			ADDVALUE("freetext1","'free1_%i'",count); //12,50
			ADDVALUE("freetext2","'free2_%i'",count); //12,50
			ADDVALUE("freeint1","%i",count); //4,10
			ADDVALUE("freeint2","%i",count); //4,10
			ADDVALUE("LOCAL_RECORD_ID","%i",count); //4,10

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
int fill_ledger_hoses(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="hoses";
	int	field_count=0;

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}
	int max_site=100;
	if(get_ini_value("site","max_site",&max_site))
		cout<<"max_site="<<max_site<<endl;

	int max_tank=1;
	if(get_ini_value("tank","max_tank",&max_tank))
		cout<<"max_tank="<<max_tank<<endl;

	int max_hose=8;
	if(get_ini_value(dbm.table_name,"max_hose",&max_hose))
		cout<<"max_hose="<<max_hose<<endl;

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	for(int site=1;site<=max_site;site++)
	{
		for(int hose=1;hose<=max_hose;hose++){
			int count=site;
			ADDVALUE("siteid","'%06i'",site); //12,15
			ADDVALUE("unit","0",count); //4,10
			ADDVALUE("tank","%i",((hose-1)%max_tank)+1); //4,10
			ADDVALUE("hose","%i",hose); //4,10
			ADDVALUE("pump","'%c'",'A'+((hose-1)%8)); //12,50
			ADDVALUE("ui","%i",count%6); //4,10
			ADDVALUE("upper_divide","100",count); //4,10
			ADDVALUE("lower_divide","1",count); //4,10
			ADDVALUE("no_pulse","20",count); //4,10
			ADDVALUE("pump_finish","10",count); //4,10
			ADDVALUE("phandle","1",count); //4,10
			ADDVALUE("product","0",count); //4,10
			ADDVALUE("freeint1","0",count); //4,10
			ADDVALUE("freeint2","0",count); //4,10
			ADDVALUE("freetext1","'free1_%i'",count); //12,50
			ADDVALUE("freetext2","'free2_%i'",count); //12,50
			SKIP_VALUE("LOCAL_RECORD_ID","%i",count); //4,10

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
int fill_ledger_totalizer(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="totalizer";
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
		increment_time(&systime,1,0,1);

		ADDVALUE("siteid","'%06i'",count%10); //12,15
		ADDVALUE("unit","%i",count%9); //4,10
		ADDVALUE("downloadtime","'%s %s'",date,time); //11,19
		ADDVALUE("pump","%i",(count%8)+1); //4,10
		ADDVALUE("totalizer","%i",count); //8,15
		ADDVALUE("comment","'comment_%i'",count); //12,50
		ADDVALUE("freeint1","%i",count); //4,10
		ADDVALUE("freetext1","'free_%i'",count); //12,50

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

int fill_ledger_inventory_history(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="inventory_history";
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

		ADDVALUE("siteid","'%06i'",count%50); //12,15
		ADDVALUE("tank","%i",count%10); //4,10
		ADDVALUE("transtime","'%s %s'",date,time); //11,19
		ADDVALUE("invcode","%i",count%50); //4,10
		ADDVALUE("comment","'comment_%i'",count); //12,150
		ADDVALUE("qty","%i",count%300); //8,15
		ADDVALUE("fuelqty","%i",count%300); //8,15
		ADDVALUE("initinventory","%i",count%300); //8,15
		ADDVALUE("resultantinventory","%i",count%300); //8,15
		ADDVALUE("unit","%i",count%100); //4,10
		ADDVALUE("totalizer","%i",count%100); //4,10
		ADDVALUE("hose","%i",(count%8)+1); //4,10
		ADDVALUE("freeint1","%i",count%100); //4,10
		ADDVALUE("freetext1","'free_%i'",count); //12,50
		ADDVALUE("grade","'GRD%03i'",count%100); //12,50
		ADDVALUE("fuel_volume","%i",count); //8,15
		ADDVALUE("fuel_height","%i",count); //8,15
		ADDVALUE("temperature","%i",count); //8,15
		ADDVALUE("water_volume","%i",count); //8,15
		ADDVALUE("water_height","%i",count); //8,15
		ADDVALUE("ullage","%i",count); //8,15
		ADDVALUE("status","'status_%i'",count); //12,50
		ADDVALUE("ui","'ui_%i'",count); //12,50
		ADDVALUE("linear_ui","'lui_%i'",count); //12,50
		ADDVALUE("FML_RECORD_ID","'rec_id_%i'",count); //12,50
		ADDVALUE("temperature_ui","'tui_%i'",count); //12,50
		SKIP_VALUE("INV_HISTORY_RECORD_ID","%i",count); //4,10

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

int fill_ledger_delivery(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="delivery";
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
		ADDVALUE("start_date","'%s %s'",date,time); //11,19

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("end_date","'%s %s'",date,time); //11,19
		ADDVALUE("start_gallons","%i",count%50); //8,15
		ADDVALUE("end_gallons","%i",(count+1)%51); //8,15
		ADDVALUE("gallons","%i",count); //8,15
		ADDVALUE("start_tcgallons","%i",count); //8,15
		ADDVALUE("end_tcgallons","%i",count+1); //8,15
		ADDVALUE("tc_gallons","%i",count); //8,15
		ADDVALUE("start_water","%i",count); //8,15
		ADDVALUE("end_water","%i",count+1); //8,15
		ADDVALUE("start_temp","%i",count); //8,15
		ADDVALUE("end_temp","%i",count+1); //4,10
		ADDVALUE("start_height","%i",count); //8,15
		ADDVALUE("end_height","%i",count+1); //8,15
		ADDVALUE("archive","%i",count%2); //4,10
		ADDVALUE("freeint1","%i",count); //4,10
		ADDVALUE("freeint2","%i",count); //4,10
		ADDVALUE("freetext1","'free1_%i'",count); //12,50
		ADDVALUE("freetext2","'free2_%i'",count); //12,50
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
