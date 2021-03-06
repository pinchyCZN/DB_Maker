int fill_pb_combo(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="pb_combo";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	dbm.open_db(&dbname);
	dbm.get_fields_array();

	if(!delete_table_records(&dbm.db,dbm.table_name,"")){
		dbm.db.Close();return FALSE;
	}

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
//	set_system_time(&systime,2010,1,1,0,0,0);
	increment_time(&systime,-72,0,0); //roll back few days

	//2008-09-29 08:41:35.000
	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));

	int count=0;
	DWORD elapsed=0;

//	int int_key=0;
//	get_table_index(&db,"Config","VEHIINTKEY",&int_key);
	
	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_count=50;
	get_ini_value(dbm.table_name,"count",&max_count);

	int i,combos=5,item=0;
	if(combos<=0)
		combos=2;
	for(i=0;i<max_count*combos;i++)
	{
		int retail=(count/combos)+1;
		int upsell=retail+1;
		char fmt[20]={0};
		count=i;

		if((count%combos)==0){
			retail=0;
			upsell=0;
			if(item>0)
				item--;
			_snprintf(fmt,sizeof(fmt),"%s","''");
		}
		else{
			_snprintf(fmt,sizeof(fmt),"%s","'%012i'");
			item++;
			item=item%50;
		}

		ADDVALUE("combo_num","%i",count/combos); //2,5
		ADDVALUE("desc","'combo%i'",count/combos); //1,16
		ADDVALUE("item_num",fmt,item); //1,20
		ADDVALUE("retail","%i",retail); //2,8
		ADDVALUE("upsell","%i",upsell); //2,5



		if(dbm.execute_sql_insert()==FALSE){
			dbm.db.Close();
			return FALSE;
		}
	}
	dbm.close();
	return TRUE;
}

