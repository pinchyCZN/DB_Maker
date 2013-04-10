int fill_ledger_link_dodaac_nsn(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="Link_DoDAAC_NSN";
	int	field_count=0;

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_product=50;
	if(get_ini_value(dbm.table_name,"max_product",&max_product))
		cout<<"max_product="<<max_product<<endl;

	CString SqlString;
	CRecordset rec(&db);
	SqlString.Format("SELECT [NSN] FROM [NSN] WHERE [PRODUCT_CODE] IN (SELECT [GRADE] FROM [GRADE] WHERE [FMU_INDEX]<=%i);",max_product);
	if(db.IsOpen())rec.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);

	for(int count=1;count<20;count++)
	{
		int breakout=0;
		do{
			CString nsn;
			if(rec.IsOpen())
				rec.GetFieldValue("NSN",nsn);
			

			ADDVALUE("DODAAC","'"_DODAAC_"'",count%100,(count+1)%100); //12,50
			ADDVALUE("NSN","'%s'",nsn); //12,255

			if(dbm.execute_sql_insert(&db)==FALSE){
				db.Close();return FALSE;
			}

			if(GetTickCount() > (elapsed+250)){
				printf("%i       \r",count);
				elapsed=GetTickCount();
			}
			rec.MoveNext();
			breakout++;
			if(breakout>=max_product)
				break;
		}while(!rec.IsEOF());
		rec.MoveFirst();
	}
	rec.Close();
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}
