int fill_htax(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="H_TAX";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	dbm.open_db(&dbname);
	dbm.get_fields_array();

	if(!delete_table_records(&dbm.db,dbm.table_name,"")){
		dbm.db.Close();return FALSE;
	}

	int count=0;
	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_count=500;
	get_ini_value(dbm.table_name,"count",&max_count);
	cout<<"max count "<<max_count<<endl;

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
			ADDVALUE("tick_datetime","'%s'",tickdatetime); //11,26
			ADDVALUE("REGNUM","%i",count%10); //-6,3
			ADDVALUE("TICKET","%i",count); //8,15
			ADDVALUE("ORD_ASSOC","%i",count%100); //5,5
			ADDVALUE("ELSDATE","'%s'",tickdatetime); //9,10
			ADDVALUE("TAX_TABLE","%i",count%100); //-6,3
			ADDVALUE("TAX","%i",count%100); //2,14
			ADDVALUE("TAX_TOTAL","%i",count%100); //2,14
			ADDVALUE("FS_TAXOFF","%i",count%100); //3,12
			ADDVALUE("FS_TAX_TOT","%i",count%100); //2,14
			ADDVALUE("WC_TAX_TOT","%i",count%100); //2,14
			ADDVALUE("ticket_id","%s",ticketid); //4,10
			if(dbm.execute_sql_insert()==FALSE){
				break;
			}
			rec.MoveNext();
		}
		if(rec.IsOpen())
			rec.Close();
	}
	dbm.close();
	return TRUE;
}

