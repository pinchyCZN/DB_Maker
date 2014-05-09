int fill_hdisc(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="H_DISC";

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

//	int int_key=0;
//	get_table_index(&db,"Config","VEHIINTKEY",&int_key);
	
	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_count=500;
	get_ini_value(dbm.table_name,"count",&max_count);
	cout<<"max count "<<max_count<<endl;

	int shift_seq=1;
	CRecordset rec(&dbm.db);
	if(get_records(&rec,"SELECT STORENUM,TICK_DATETIME,TICKET_ID FROM H_TICKET ORDER BY TICK_DATETIME")){
		for(count=1;count<=max_count;count++)
		{
			if(rec.IsEOF())
				break;
			CString storenum="1";
			CString tickdatetime="2010-01-01";
			CString ticketid="1";
			rec.GetFieldValue("STORENUM",storenum);
			rec.GetFieldValue("TICK_DATETIME",tickdatetime);
			rec.GetFieldValue("TICKET_ID",ticketid);
			ADDVALUE("STORENUM","%s",storenum); //8,15
			ADDVALUE("SHIFT_SEQ","%i",shift_seq); //4,10
			ADDVALUE("TICKET","%i",count); //4,10
			ADDVALUE("REGNUM","%i",count%10); //4,10
			ADDVALUE("TICK_DATETIME","'%s'",tickdatetime); //11,26
			ADDVALUE("ID","%i",count%10); //4,10
			ADDVALUE("TYPE","'%i'",count%10); //12,1
			ADDVALUE("DESCRIPTION","'desc%i'",count); //12,80
			ADDVALUE("TOTALAMT","%i",count%1000); //3,12
			ADDVALUE("ORDINAL","%i",count%1000); //5,5
			ADDVALUE("TICKET_ID","%s",ticketid); //4,10
			ADDVALUE("MAXGAL","%i",count%1000); //2,6
			ADDVALUE("DISCAMT","%i",count%1000); //2,8
			if(dbm.execute_sql_insert()==FALSE){
				break;
			}
			shift_seq++;
			rec.MoveNext();
		}
		if(rec.IsOpen())
			rec.Close();
	}
	dbm.close();
	return TRUE;
}

