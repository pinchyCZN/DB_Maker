int fill_cardowner(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="CardOwner";

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

	for(int card=0;card<100;card++)
	{
		int count=card;

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		SKIP_VALUE("Index","''",count); //4,10
		ADDVALUE("OwnerID","'%08i'",count); //12,8
		ADDVALUE("Name","'name%04i'",count); //12,50
		ADDVALUE("Address1","'addr1_%04i'",count); //12,40
		ADDVALUE("Address2","'addr2_%04i'",count); //12,40
		ADDVALUE("City","'city_%04i'",count); //12,20
		ADDVALUE("State","'A%c'",'A'+(count%26)); //12,2
		ADDVALUE("Zip","'zip%04i'",count); //12,10
		ADDVALUE("Phone1","'ph1%04i'",count); //12,25
		ADDVALUE("Phone2","'ph2%04i'",count); //12,25
		ADDVALUE("Fax","'fax%04i'",count); //12,25
		ADDVALUE("Email","'ema%04i'",count); //12,70
		ADDVALUE("Contact","'con%04i'",count); //12,50
		ADDVALUE("Comment","'cmt%04i'",count); //12,50
		ADDVALUE("DateTime","'%s %s'",date,time); //11,19
		ADDVALUE("DefaultPrice","%i",-count%2); //-7,1
		ADDVALUE("FreeText1","'free1%04i'",count); //12,40
		ADDVALUE("FreeText2","'free2%04i'",count); //12,40
		ADDVALUE("FreeText3","'free3%04i'",count); //12,40

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
/******************* PREPAID ********************************/
int fill_prepaidcard(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="PrePaidCard";

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

	for(int card=0;card<100;card++)
	{
		int count=card;
		int neg[2]={-1,1};

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		SKIP_VALUE("Index","''",count); //4,10
		ADDVALUE("OwnerID","'%08i'",count%100); //12,8
		ADDVALUE("CardID","'%08i'",count); //12,8
		ADDVALUE("BeginDate","'%s %s'",date,time); //11,19
		ADDVALUE("ExpirationDate","'%s %s'",date,time); //11,19
		ADDVALUE("Auth","%i",-count%2); //-7,1
		ADDVALUE("AuthDate","'%s %s'",date,time); //11,19
		ADDVALUE("Encoded","%i",-count%2); //-7,1
		ADDVALUE("EncodedDate","'%s %s'",date,time); //11,19
		ADDVALUE("ReEncoded","%i",-count%2); //-7,1
		ADDVALUE("ReEncodedDate","'%s %s'",date,time); //11,19
		ADDVALUE("Comment","'comment%05i'",count); //12,50
		ADDVALUE("Reissue","'%i'",count%4); //4,10
		ADDVALUE("IntKey","'%09i'",count); //12,9
		ADDVALUE("PinRequired","%i",-count%2); //-7,1
		ADDVALUE("PinNum","'%04i'",count%10000); //12,4
		ADDVALUE("Credits","'%i'",count%10); //8,15
		ADDVALUE("Balance","'%f'",(double)0.01*neg[count%2]*(count%100) + (double)neg[count%2]*10*count); //8,15
		ADDVALUE("PriceLev","'%i'",(count%8)+1); //12,2
		ADDVALUE("DeletedDate","'%s %s'",date,time); //11,19
		ADDVALUE("Deleted","0",count); //-7,1
		ADDVALUE("Operator","''",count); //12,8
		ADDVALUE("FreeText1","''",count); //12,50
		ADDVALUE("FreeText2","''",count); //12,50
		ADDVALUE("FreeText3","''",count); //12,50
		ADDVALUE("OperatorNum","'%i'",count%4); //4,10

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

/******************* DISCOUNT CARD ****************************/
int fill_discountcard(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="DiscountCard";

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
	
	int owner_index=0;
	get_table_index(&db,"CardOwner","Index",&owner_index);

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(int card=0;card<100;card++)
	{
		int count=card;

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		SKIP_VALUE("Index","''",count); //4,10
		ADDVALUE("OwnerIndex","'%i'",owner_index); //4,10
		owner_index++;
		ADDVALUE("OwnerID","'%08i'",count); //12,8
		ADDVALUE("CardID","'%09I64u'",(__int64)98765432100000+count); //12,40
		ADDVALUE("PartialID","'%04i'",count%10000); //12,4
		ADDVALUE("CardType","'%i'",count%69); //4,10
		ADDVALUE("ExpDate","'1/1/2099 12:00:00'",date,time); //11,19
		ADDVALUE("BeginDate","'%s %s'",date,time); //11,19
		ADDVALUE("Auth","%i",-count%2); //-7,1
		ADDVALUE("AuthDate","'%s %s'",date,time); //11,19
		ADDVALUE("Comment","'comment%i'",count); //12,50
		ADDVALUE("Operator","'%i'",count); //12,8
		ADDVALUE("DeletedDate","'%s %s'",date,time); //11,19
		ADDVALUE("Deleted","0",count); //-7,1
		ADDVALUE("PriceLev","'%i'",(count%8)+1); //12,2
		ADDVALUE("FreeText1","''",count); //12,50
		ADDVALUE("FreeText2","''",count); //12,50
		ADDVALUE("FreeText3","''",count); //12,50
		ADDVALUE("OperatorNum","'%i'",count%10); //4,10
		ADDVALUE("PABPCompliant","'1'",count%2); //4,10

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

/******************* PPC HISTORY ****************************/
int fill_ppchistory(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="PPCHistory";

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

	for(int card=0;card<400;card++)
	{
		int count=card;
		int neg[2]={-1,1};

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		ADDVALUE("SessionID","'seshid%05i'",count); //12,59
		ADDVALUE("CardIndex","'1'",count); //4,10
		ADDVALUE("EventType","'%i'",(count%15)+1); //4,10
		ADDVALUE("CardID","'%08i'",count/20); //12,8
		ADDVALUE("OwnerID","'%08i'",(count/20)%100); //12,8
		ADDVALUE("Operator","'%i'",(count%10)+1); //12,8
		ADDVALUE("OperatorNum","'0'",count); //4,10
		ADDVALUE("EventDate","'%s %s'",date,time); //11,19
		ADDVALUE("DownloadDate","'%s %s'",date,time); //11,19
		ADDVALUE("TotalCost","'%i'",count%1000); //8,15
		ADDVALUE("TransID","'%i'",count%100); //4,10
		ADDVALUE("PPCBalance","'%f'",(double)0.01*neg[count%2]*(count%100) + (double)neg[count%2]*10*count++); //8,15
		ADDVALUE("PPCBalChange","'%f'",(double)0.01*neg[count%2]*(count%100) + (double)neg[count%2]*10*count++); //8,15
		ADDVALUE("PPCCred","'%f'",(double)0.01*neg[count%2]*(count%100) + (double)neg[count%2]*10*count++); //8,15
		ADDVALUE("PPCCredChange","'%f'",(double)0.01*neg[count%2]*(count%100) + (double)neg[count%2]*10*count++); //8,15
		ADDVALUE("Comment","'comment%05i'",card%1000); //12,255
		ADDVALUE("Sechash","'sechash%05i'",count); //12,59

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
/******************* LOCAL AUTH LOCK****************************/
int fill_localauthlock(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="LocalAuthLock";

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

	for(int card=0;card<100;card++)
	{
		int count=card;

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		SKIP_VALUE("AUTOINCREMENT","''",count); //4,10
		ADDVALUE("CARDHOLDERNAME","'card_holder_name%i'",count); //12,50
		ADDVALUE("CARDNUM","'%019i'",count); //12,40
		ADDVALUE("AUTH","%i",-count%2); //-7,1
		ADDVALUE("ENCRYPTED","%i",-((count/10)%2)); //-7,1
		ADDVALUE("CARDTYPE","'%i'",count%69); //4,10
		ADDVALUE("EXPIREDATE","'1/1/2099 12:00:00'",date,time); //11,19
		ADDVALUE("AUTHDATE","'%s %s'",date,time); //11,19
		ADDVALUE("PABPCompliant","'%i'",count%2); //4,10

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
/*this checks all sites for local auth dialog*/
int fill_sitelalock(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="SiteLaLock";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	int lock_index=0;
	get_table_index(&db,"LocalAuthLock","AUTOINCREMENT",&lock_index);

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(int card=0;card<100;card++)
	{
		int count=card;

		ADDVALUE("SITEID","'***'",count); //12,4
		ADDVALUE("LOCKID","'%i'",count+lock_index); //4,10

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

/******************* LOCAL AUTH ACCOUNTS****************************/
int fill_localauthaccounts(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="LocalAuthAccounts";

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

	for(int card=0;card<100;card++)
	{
		int count=card;

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		SKIP_VALUE("AUTOINCREMENT","''",count); //4,10
		ADDVALUE("ACCTHOLDERNAME","'acct_holder_name%i'",count); //12,50
		ADDVALUE("ACCTNUM","'%019i'",count); //12,40
		ADDVALUE("AUTH","%i",-count%2); //-7,1
		ADDVALUE("ENCRYPTED","%i",-((count/10)%2)); //-7,1
		ADDVALUE("CARDTYPE","'%i'",count%69); //4,10
		ADDVALUE("EXPIREDATE","'1/1/2099 12:00:00'",date,time); //11,19
		ADDVALUE("AUTHDATE","'%s %s'",date,time); //11,19
		ADDVALUE("PABPCompliant","'%i'",count%2); //4,10

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
/*this checks all sites for local auth dialog*/
int fill_sitelaaccnt(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="SiteLaAccnt";


	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	int accnt_index=0;
	get_table_index(&db,"LocalAuthAccounts","AUTOINCREMENT",&accnt_index);

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(int count=0;count<100;count++)
	{
		ADDVALUE("SITEID","'***'",count); //12,4
		ADDVALUE("ACCNTID","'%i'",count+accnt_index); //4,10

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
