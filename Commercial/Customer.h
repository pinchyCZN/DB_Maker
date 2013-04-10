int fill_customer(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="Customer";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	int customer=0;
	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(customer=0;customer<100;customer++)
	{
		int count;

		count=customer;

		ADDVALUE("CUSTID","'%09i'",count); //12,9
		ADDVALUE("CUSTNAME","'cust_%03i'",count); //12,40
		ADDVALUE("ADDRESS1","'addr1_%04i'",count); //12,40
		ADDVALUE("ADDRESS2","'addr2_%04i'",count); //12,40
		ADDVALUE("CITY","'city_%04i'",count); //12,20
		ADDVALUE("STATE","'A%c'",'A'+(count%26)); //12,2
		ADDVALUE("ZIP","'zip%03i'",count%1000); //12,10
		ADDVALUE("PHONE","'ph%09i'",count); //12,25
		ADDVALUE("FAX","'fax%04i'",count); //12,25
		ADDVALUE("EMAIL","'email%04i'",count); //12,50
		ADDVALUE("CONTACT","'contact%04i'",count); //12,30
		ADDVALUE("DEFPRICE","'%i'",(count%7)+1); //12,2
		ADDVALUE("COMMENT","'comment%04i'",count); //12,40
		ADDVALUE("FREE1","'free1%04i'",count); //12,15
		ADDVALUE("FREE2","'free2%04i'",count); //12,15
		ADDVALUE("FREE3","'free3%04i'",count); //12,15
		SKIP_VALUE("AUTOINCREMENT","''",count); //4,10

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}
		char bounce[]={'/','-','\\','|'};
		cout<<bounce[count%sizeof(bounce)]<<"\b";
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}
