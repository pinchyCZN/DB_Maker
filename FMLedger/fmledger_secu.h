int fill_ledger_security(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="Security_Operator";
	int	field_count=0;

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"WHERE operator_login<>'ADMIN'"))
	{
		db.Close();return FALSE;
	}

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-24*2,0,0); //roll back few days

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
#define	MAX_LEDGER_OPERATORS 15
	for(int count=1;count<MAX_LEDGER_OPERATORS;count++) //well skip admin here
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		SKIP_VALUE("operator_id","",count); //4,10
		ADDVALUE("operator_login","'%02i'",count); //12,50
		ADDVALUE("first_name","'first_name_%i'",count); //12,255
		ADDVALUE("middle_name","'middle_name_%i'",count); //12,50
		ADDVALUE("last_name","'last_name_%i'",count); //12,255
		ADDVALUE("comment","'comment_%i'",count); //12,255
		ADDVALUE("date_created","'%s %s'",date,time); //11,19
		ADDVALUE("created_by","0",count); //4,10
		ADDVALUE("authorized","1",count); //5,5
		ADDVALUE("date_authorized","'%s %s'",date,time); //11,19
		ADDVALUE("pw_change_date","'%s %s'",date,time); //11,19
		ADDVALUE("deleted","0",count); //5,5
		ADDVALUE("security_lock_out","0",count); //5,5
		ADDVALUE("login_fail_count","0",count); //5,5
		ADDVALUE("lockout_date","'1/1/1970'",date,time); //11,19

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}

		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
		}

	}
/*************** get operator IDS *****************************/
	CRecordset rec(&db);
	CString SqlString;
	unsigned int operator_ids[MAX_LEDGER_OPERATORS];
	unsigned int max_operator_id_count=0;
	SqlString.Format("SELECT [operator_id] FROM [Security_Operator];");
	TRY{
		rec.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);

		for(int i=0;i<MAX_LEDGER_OPERATORS;i++)
		{
			CString opid;
			rec.GetFieldValue("operator_id",opid);
			operator_ids[i]=strtoul(opid,NULL,10);
			max_operator_id_count++;
			rec.MoveNext();
			if(rec.IsEOF())
				break;
		}
	}CATCH(CDBException, e){
		cout<<"cant read from security_operator table\n";
	}END_CATCH
	if(rec.IsOpen())
		rec.Close();
/***************Security_Privilege_Set*************************/
	dbm.table_name="Security_Privilege_Set";
	dbm.get_fields_array(&db);
	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	for(count=0;count<140*MAX_LEDGER_OPERATORS;count++)
	{
		int i=count%140;
//		if(operator_ids[(count/140)%(MAX_LEDGER_OPERATORS)]==1)
//			continue;

		if(i==5)
			continue;
		if(i>138)
			continue;
		if((i>21) && (i<138))
			continue;
		
		ADDVALUE("operator_id","%i",operator_ids[(count/140)%(MAX_LEDGER_OPERATORS)]); //5,5
		ADDVALUE("privilege_id","%i",(count%140)); //4,10
		i=(count/140)%(MAX_LEDGER_OPERATORS);

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}

		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
		}
	}
/***************Security_Password******************************/
	dbm.table_name="Security_Password";
	dbm.get_fields_array(&db);
	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	for(count=0;count<MAX_LEDGER_OPERATORS;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("operator_id","%i",operator_ids[count%MAX_LEDGER_OPERATORS]); //5,5
		ADDVALUE("password","'F2E2AEA7027D36D60E6F06EE9DFA1FD0'",count); //12,255
		ADDVALUE("date_created","'%s %s'",date,time); //11,19
		ADDVALUE("date_expire","'1/1/2099'",count,count); //11,19

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}
		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
		}
	}
/*************** set security role *****************************/
	CString role_id;
	int roles_found=FALSE;
	TRY{
		CString SqlString;
		SqlString.Format("SELECT TOP 1 [Role_id] FROM [Security_Role];");
		rec.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);
		if(!rec.IsEOF())
			rec.GetFieldValue("Role_id",role_id);
		roles_found=TRUE;
	}CATCH(CDBException, e){
		cout<<"security roles not present in this database\n";
	}END_CATCH
	if(rec.IsOpen())
		rec.Close();

	if(roles_found){
		dbm.table_name="Security_Operator_Role";
		dbm.get_fields_array(&db);
		if(!delete_table_records(&db,dbm.table_name,"")){
			db.Close();
		}
		else{
			cout<<"Writing to "<< dbm.table_name << " Table\n";
			for(count=0;count<max_operator_id_count;count++){
				ADDVALUE("Operator_id","%i",operator_ids[count]); //4,10
				ADDVALUE("Role_id","%s",role_id); //4,10
				if(dbm.execute_sql_insert(&db)==FALSE){
					printf("error adding to %s\n",dbm.table_name);
					break;
				}
			}
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}
