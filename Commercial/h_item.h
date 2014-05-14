int fill_hitem(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="H_ITEM";

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
			CString ticketid="0";
			rec.GetFieldValue("STORENUM",storenum);
			rec.GetFieldValue("TICK_DATETIME",tickdatetime);
			rec.GetFieldValue("TICKET_ID",ticketid);
			ADDVALUE("STORENUM","%s",storenum); //8,15
			ADDVALUE("TICK_DATETIME","'%s'",tickdatetime); //11,26
			ADDVALUE("REGNUM","%i",count%10); //-6,3
			ADDVALUE("TICKET","%i",count); //8,15
			ADDVALUE("ORDINAL","%i",count%100); //5,5
			ADDVALUE("ELSDATE","'%s'",tickdatetime); //9,10
			ADDVALUE("ITEM_TYPE","'%i'",count%10); //12,1
			ADDVALUE("COMBO_NUM","%i",count%100); //2,5
			ADDVALUE("ITEMNUM","'item%i'",count); //12,20
			ADDVALUE("DESCRIPTION","'desc%i'",count); //12,16
			ADDVALUE("MODIFIER","%i",count%2); //5,5
			ADDVALUE("PRICE","%i",count%1000); //3,12
			ADDVALUE("AMOUNT","%i",count%1000); //3,12
			ADDVALUE("QTY","%i",count%100); //3,12
			ADDVALUE("TAX_TABLE","%i",count%10); //2,3
			ADDVALUE("FOOD_STAMP","'fs%i'",count); //12,10
			ADDVALUE("DISCOUNT","'%i'",count%1000); //12,10
			ADDVALUE("AGE","%i",count%100); //5,5
			ADDVALUE("DEPARTMENT","%i",count%100); //5,5
			ADDVALUE("TICKET_ID","%s",ticketid); //4,10
			ADDVALUE("PROMO_CODE","%i",count%1000); //2,5
			ADDVALUE("KV_SENT","'%i'",count%1000); //12,10
			ADDVALUE("PARENT_ORD","%i",count%1000); //5,5
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

