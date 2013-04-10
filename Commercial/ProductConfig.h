int fill_productconfig(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="ProductConfig";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

//	if(!delete_table_records(&db,dbm.table_name,"WHERE PRODUCT<>1 AND PRODUCT <>2 AND PRODUCT<>63"))
	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(int count=1;count<100;count++)
	{
//		if((count==1) || (count==2) || (count==63))
//			continue;
		ADDVALUE("PRODUCT","'%i'",count); //5,5
		ADDVALUE("PRICE","'%i'",count); //5,5
		ADDVALUE("DESCRIPT","'product%i'",count); //12,50
		ADDVALUE("SHORTDESC","'PD%i'",count); //12,5
		ADDVALUE("BASECOST","'%i'",count%10); //7,7
		ADDVALUE("MAXPRODALLOTED","'%i'",count%100); //5,5
		ADDVALUE("ISSUEUNIT","'%i'",((count-1)%2)+1); //4,10
		ADDVALUE("TRXLIMIT","'%i'",((count-1)%100)+1); //4,10
		ADDVALUE("STSPROD","'%i'",((count-1)%100)+1); //5,5
		ADDVALUE("ADDITIVE","'0'",count); //5,5
		ADDVALUE("ALTERNATE","'0'",count); //4,10

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

int fill_priceconfig(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="PriceConfig";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(int product=1;product<100;product++)
	{
		for(int count=1;count<=8;count++)
		{
			ADDVALUE("PRICE","'%i'",product); //5,5
			ADDVALUE("LEVEL","'%i'",((count-1)%8)+1); //5,5
			ADDVALUE("ADJUST","'%.3f'", 1.111*( ((count-1)%8)+1)); //12,50
			ADDVALUE("TOTAL","'%.3f'",(1.111*( ((count-1)%8)+1))+(product%10)); //8,15

			if(dbm.execute_sql_insert(&db)==FALSE){
				db.Close();return FALSE;
			}
			if(GetTickCount() > (elapsed+250)){
				printf("%i %i      \r",product,count);
				elapsed=GetTickCount();
			}
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}
