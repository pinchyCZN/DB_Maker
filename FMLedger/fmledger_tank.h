int fill_ledger_tank(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="tank";
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
	if(get_ini_value("site","max_site",&max_site))
		cout<<"max_site="<<max_site<<endl;

	int max_tank=1;
	if(get_ini_value(dbm.table_name,"max_tank",&max_tank))
		cout<<"max_tank="<<max_tank<<endl;

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	for(int count=1;count<=max_site;count++)
	{
		for(int tank=1;tank<=max_tank;tank++){
			GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
			GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
			increment_time(&systime,0,0,1);

			CString str;
			CRecordset rec(&db);
			str.Format("SELECT [NSN] FROM [NSN] WHERE [PRODUCT_CODE]=(SELECT [GRADE] FROM [GRADE] WHERE [FMU_INDEX]=%i);",tank);
			TRY{
				if(db.IsOpen())rec.Open(CRecordset::snapshot,str,CRecordset::readOnly);
				str.Empty();
				if(rec.IsOpen())rec.GetFieldValue("NSN",str);
				rec.Close();
			}CATCH(CDBException, e){
				printf("error\n");
				str.Format("nsn_%i",count);
				if(rec.IsOpen())rec.Close();
			}END_CATCH

			ADDVALUE("siteid","'%06i'",count); //12,15
			ADDVALUE("tank","%i",tank); //4,10
			ADDVALUE("grade","%i",tank); //4,10
			ADDVALUE("product","%i",tank); //5,5
			ADDVALUE("ui","%i",count%6); //4,10
			ADDVALUE("vendor","'vendor_%i'",count); //12,50
			ADDVALUE("reorder","%i",count); //8,15
			ADDVALUE("capacity","%i",count); //8,15
			ADDVALUE("inventory","%i",count); //8,15
			ADDVALUE("stick","%i",count); //8,15
			ADDVALUE("stickdate","'%s %s'",date,time); //11,19
			ADDVALUE("adjustment","%i",count); //8,15
			ADDVALUE("adjustdate","'%s %s'",date,time); //11,19
			ADDVALUE("delivery","%i",count); //8,15
			ADDVALUE("deliverydate","'%s %s'",date,time); //11,19
			ADDVALUE("tmu","%i",count%100); //8,15
			ADDVALUE("tmudate","'%s %s'",date,time); //11,19
			ADDVALUE("transfer","%i",count); //8,15
			ADDVALUE("transferdate","'%s %s'",date,time); //11,19
			ADDVALUE("freeint1","%i",count); //4,10
			ADDVALUE("freeint2","%i",count); //4,10
			ADDVALUE("freeint3","%i",count); //4,10
			ADDVALUE("freetext1","'free1_%i'",count); //12,50
			ADDVALUE("freetext2","'free2_%i'",count); //12,50
			ADDVALUE("freetext3","'free3_%i'",count); //12,50
			ADDVALUE("lines","%i",count); //4,10
			ADDVALUE("NSN","'%s'",str); //12,50
			ADDVALUE("fuel_volume","%i",count); //8,15
			ADDVALUE("fuel_height","%i",count); //8,15
			ADDVALUE("water_volume","%i",count); //8,15
			ADDVALUE("water_height","%i",count); //8,15
			ADDVALUE("ullage","%i",count); //8,15
			ADDVALUE("temperature","%i",count); //8,15
			ADDVALUE("volume_ui","'vui_%i'",count); //12,50
			ADDVALUE("level_ui","'lui_%i'",count); //12,50
			ADDVALUE("temperature_ui","'tui_%i'",count); //12,50
			ADDVALUE("tank_bottom","%i",count); //8,15
			ADDVALUE("tank_top","%i",count); //8,15
			ADDVALUE("structural_loss","%i",count); //8,15
			ADDVALUE("construction","'constr_%i'",count); //12,50
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