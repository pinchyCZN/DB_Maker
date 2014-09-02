int fill_inv_item(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="INV_ITEM";

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
	int timeshift=-3;
	//set_system_time(&systime,2010,1,1,3,0,0);
	get_ini_value(dbm.table_name,"timeshift",&timeshift);
	increment_time(&systime,timeshift*24,0,0); //roll back few days

	//2008-09-29 08:41:35.000
	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));

	int count=0;
	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_count=500;
	get_ini_value(dbm.table_name,"count",&max_count);
	cout<<"max count "<<max_count<<endl;

	int shift_seq=1;
	for(count=1;count<=max_count;count++)
	{
		ADDVALUE("DAY_NUM","%i",count%10); //2,2
		ADDVALUE("ELSGROUP","'%i'",count%1000); //12,4
		ADDVALUE("ELSSTORE","'%i'",count%100); //12,5
		ADDVALUE("ELSDATE","'%s'",date); //9,10
		ADDVALUE("ELSTIME","'%i'",count%100); //12,5
		ADDVALUE("CONSOLE_ID","%i",count&1); //2,1
		ADDVALUE("INV_ITEM","'item%i'",count); //12,20
		ADDVALUE("INV_DESC","'desc%i'",count); //12,20
		ADDVALUE("INV_GROUP","%i",count%100); //2,3
		ADDVALUE("INV_RGROUP","%i",count%100); //2,3
		ADDVALUE("ORD_ASSORT","%i",count%100); //2,3
		ADDVALUE("SEQ","%i",count%1000); //2,5
		ADDVALUE("INV_PERIOD","'%i'",count&1); //12,1
		ADDVALUE("BEGIN_INV","%i",count%1000); //2,9
		ADDVALUE("MAN_PURCH","%i",count%1000); //2,9
		ADDVALUE("CURR_PURCH","%i",count%1000); //2,9
		ADDVALUE("EXT_SALES","%i",count%1000); //2,9
		ADDVALUE("MAN_SALES","%i",count%1000); //2,9
		ADDVALUE("CURR_SALES","%i",count%1000); //2,9
		ADDVALUE("SALES_VAR","%i",count%1000); //2,10
		ADDVALUE("CURR_ADJ","%i",count%1000); //2,9
		ADDVALUE("AUDIT_ADJ","%i",count%1000); //2,9
		ADDVALUE("TRANS_IN","%i",count%1000); //2,9
		ADDVALUE("TRANS_OUT","%i",count%1000); //2,9
		ADDVALUE("CURR_INV","%i",count%1000); //2,9
		ADDVALUE("WORK_BUCK1","%i",count%1000); //2,12
		ADDVALUE("WORK_BUCK2","%i",count%1000); //2,12
		ADDVALUE("COUNT_INV","%i",count%1000); //2,12
		ADDVALUE("CNT_DATE",count&1?"%s":"'%s'",count&1?"null":date); //9,10
		ADDVALUE("RAW_WASTE","%i",count%100); //2,8
		ADDVALUE("FIN_WASTE","%i",count%100); //2,8
		ADDVALUE("LST_CNTDTE",count&1?"%s":"'%s'",count&1?"null":date); //9,10
		ADDVALUE("WORK_BUCK3","%i",count%100); //2,12
		ADDVALUE("ITEM_NUM","%i",count%1000); //2,12
		ADDVALUE("WORK_BUCK4","%i",count%1000); //2,12
		ADDVALUE("WORK_BUCK5","%i",count%1000); //2,12
		ADDVALUE("AVE_COST","%i",count%1000); //2,12
		ADDVALUE("TOTAL_COST","%i",count%1000); //2,12
		ADDVALUE("SPOILAGE","%i",count%100); //2,8
		ADDVALUE("STORE_USE","%i",count%100); //2,8
		ADDVALUE("CNT_TIME","'%i'",count%1000); //12,8
		ADDVALUE("CNT_QOH","%i",count%1000); //2,12
		ADDVALUE("LST_CNTTME","'%i'",count%1000); //12,8
		ADDVALUE("DEPARTMENT","'%i'",count%1000); //12,2
		ADDVALUE("INV_COST","%i",count%1000); //2,12
		ADDVALUE("INV_RETL","%i",count%1000); //2,12
		if(dbm.execute_sql_insert()==FALSE){
			dbm.close();return FALSE;
		}
		if(!(count&1))
		{
			increment_time(&systime,24,0,0);
			GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
			GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));
		}
	}
	dbm.close();
	return TRUE;
}

