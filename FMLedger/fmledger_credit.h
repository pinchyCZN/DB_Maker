int fill_ledger_cc_lock(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="cc_lock";
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

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	for(int count=0;count<100;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("CC_PAN","'%019i'",count+3000); //12,19
		ADDVALUE("CC_ECN","'%05i'",count+100); //12,5
		ADDVALUE("Lock_Date","'%s %s'",date,time); //11,19
		ADDVALUE("Comment","'comment_%i'",count); //12,255

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
int dump_ledgercard(CString dbname)
{
	CDatabase db;
	open_database(&db,&dbname);
	CRecordset rec(&db);
	FILE *flog;
	flog=fopen("carddef.txt","w");

	CString SqlString;
	SqlString.Format("SELECT * FROM CardDef;");
	rec.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);
	while(!rec.IsEOF()){
		int i;
		for(i=0;i<100;i++){
			CString val,str;
			TRY{
				rec.GetFieldValue(i,val);
				str.Format("\"%s\",",val);
				if(flog!=0)fputs(str,flog);
			}
			CATCH(CDBException, e){
				CString error = "ERROR: " + e->m_strError + "\nODBC: " + e->m_strStateNativeOrigin;
				cout<< (LPCSTR)error;
				printf("index=%i\n",i);
				break;
			}
			END_CATCH
		}
		if(flog!=0)fputs("\n",flog);
		rec.MoveNext();
	}
	rec.Close();
	if(flog!=0)fputs("\n\ncarddeffield\n\n",flog);
	SqlString.Format("SELECT * FROM CardDefField;");
	rec.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);
	while(!rec.IsEOF()){
		int i;
		for(i=0;i<100;i++){
			CString val,str;
			TRY{
				rec.GetFieldValue(i,val);
				str.Format("\"%s\",",val);
				if(flog!=0)fputs(str,flog);
			}
			CATCH(CDBException, e){
				CString error = "ERROR: " + e->m_strError + "\nODBC: " + e->m_strStateNativeOrigin;
				cout<< (LPCSTR)error;
				printf("index=%i\n",i);
				break;
			}
			END_CATCH
		}
		if(flog!=0)fputs("\n",flog);
		rec.MoveNext();
	}
	rec.Close();
	db.Close();
	if(flog!=0)
		fclose(flog);
	printf("done with card dump\n");
	return 0;
}
int fill_ledger_carddef(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="CardDef";
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
	for(int count=0;count<3;count++)
	{
		int index=0;
		char *data[3][16]={
			{"DoD For Gov","1","2002-12-31 00:00:00","2012-12-31 00:00:00","B","1","16","0","0","0","0","0","0","0","0","0"},
			{"Air Card","2","2002-12-31 00:00:00","2012-12-31 00:00:00","B","1","16","0","0","0","0","0","0","0","0","0"},
			{"WRIGHT EXPRESS","8","2002-12-31 00:00:00","2020-12-31 00:00:00","","0","19","1","1","12","17","18","0","12","17","18"}
		};

		ADDVALUE("CardDefName","'%s'",data[count][index++]); //12,100
		ADDVALUE("CardDefNum","%s",data[count][index++]); //4,10
		ADDVALUE("BeginDate","'%s'",data[count][index++]); //11,19
		ADDVALUE("EndDate","'%s'",data[count][index++]); //11,19
		ADDVALUE("FormatCode","'%s'",data[count][index++]); //12,10
		ADDVALUE("ValidateFormat","%s",data[count][index++]); //-7,1
		ADDVALUE("CardNumLength","%s",data[count][index++]); //4,10
		ADDVALUE("ValidateCardNumLen","%s",data[count][index++]); //-7,1
		//ADDVALUE("CardPrefix","'%s'",data[count][index++]); //12,20
		//ADDVALUE("Upto","%s",data[count][index++]); //-7,1
		//ADDVALUE("CardPrefix2","'%s'",data[count][index++]); //12,20
		ADDVALUE("MODTen1","%s",data[count][index++]); //-7,1
		ADDVALUE("MODFrom1","%s",data[count][index++]); //4,10
		ADDVALUE("MODTo1","%s",data[count][index++]); //4,10
		ADDVALUE("MODSumOffSet1","%s",data[count][index++]); //4,10
		ADDVALUE("MODTen2","%s",data[count][index++]); //-7,1
		ADDVALUE("MODFrom2","%s",data[count][index++]); //4,10
		ADDVALUE("MODTo2","%s",data[count][index++]); //4,10
		ADDVALUE("MODSumOffSet2","%s",data[count][index++]); //4,10

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

int fill_ledger_carddeffield(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="CardDefField";
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
	for(int count=0;count<12;count++)
	{
		int index=0;
		char *data[12][11]={
			{"1","1","1","%","","1","0","16","0","^",""},
			{"2","1","3","%",";","1","0","16","15","^","="},
			{"8","1","2","",";","0","0","0","-1","","="},
			{"1","3","1","%^^","","0","0","1","0","0",""},
			{"2","3","3","%^^",";=","2","2","3","3","0","0"},
			{"8","3","2","",";=","0","2","0","3","",""},
			{"1","4","1","%^^","","2","0","3","0","0",""},
			{"2","4","3","%^^",";=","0","0","1","1","^","^"},
			{"8","4","2","",";=","0","0","0","1","",""},
			{"1","5","1","%^","","0","0","-1","0","^",""},
			{"2","5","1","%^","","0","0","-1","0","^",""},
			{"8","5","2","",";=","0","7","0","11","",""},
		};

		ADDVALUE("CardDefNum","%s",data[count][index++]); //4,10
		ADDVALUE("FieldID","%s",data[count][index++]); //4,10
		ADDVALUE("Track","%s",data[count][index++]); //4,10
		ADDVALUE("PassTrack1","'%s'",data[count][index++]); //12,80
		ADDVALUE("PassTrack2","'%s'",data[count][index++]); //12,80
		ADDVALUE("SkipTrack1","%s",data[count][index++]); //4,10
		ADDVALUE("SkipTrack2","%s",data[count][index++]); //4,10
		ADDVALUE("StopTrack1","%s",data[count][index++]); //4,10
		ADDVALUE("StopTrack2","%s",data[count][index++]); //4,10
		ADDVALUE("StopChar1","'%s'",data[count][index++]); //12,5
		ADDVALUE("StopChar2","'%s'",data[count][index++]); //12,5

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

int fill_ledger_carddefprefix(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="CardDefPrefixRange";
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
	for(int count=0;count<4;count++)
	{
		int index=0;
		char *data[4][3]={
			{"1","061800","0"},
			{"2","789682","0"},
			{"8","707138","0"},
			{"8","690046","0"}
		};
		SKIP_VALUE("PrefixNumber","",count); //4,10
		ADDVALUE("CardDefNum","%s",data[count][index++]); //4,10
		ADDVALUE("PrefixBegin","'%s'",data[count][index++]); //12,255
		ADDVALUE("PrefixEnd","'%s'",data[count][index++]); //12,255

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