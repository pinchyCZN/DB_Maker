int fill_htender(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="H_TENDER";

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
			ADDVALUE("TICKET","%i",count%1000); //8,15
			ADDVALUE("ORDINAL","%i",count%100); //5,5
			//ADDVALUE("ELSDATE","'%s'",tickdatetime); //9,10
			ADDVALUE("ELSDATE","null",tickdatetime); //9,10
			ADDVALUE("TENDER_ID","'%i'",count%100); //12,8
			ADDVALUE("SUB_ID","'%i'",count%100); //12,16
			ADDVALUE("AMOUNT","%i",count%1000); //3,12
			ADDVALUE("TENDERED","%i",count%1000); //3,12
			ADDVALUE("FEE","%i",count%10); //3,12
			ADDVALUE("ACCT_TYPE","'%i'",count%10); //12,4
			ADDVALUE("ACCOUNT","'acct%i'",count); //12,40
			ADDVALUE("TRANS_ID","'trans%i'",count); //12,20
			ADDVALUE("REF","'ref%i'",count); //12,20
			ADDVALUE("AUTH_CODE","'%i'",count); //12,20
			ADDVALUE("EXP_DATE","'%s'",tickdatetime); //9,10
			ADDVALUE("INTERFACED","'%i'",count%100); //12,10
			ADDVALUE("BATCH","%i",count); //5,5
			ADDVALUE("ticket_id","%s",ticketid); //4,10
			ADDVALUE("SURCHARGE","%i",count%1000); //2,8
			ADDVALUE("FMT_ACCT","'%i'",count%1000); //12,20
			ADDVALUE("IV","'%i'",count); //12,32
			ADDVALUE("SIG_STATUS","'%i'",count%10); //12,1
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

