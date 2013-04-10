int report_ids[]={
	2, //100 system
	11,//200 transactions
	5, //300 invoices
	5, //400 site
	12,//500 vehicle
	4, //600 user
	2, //700 customer
	4, //800 utility
	6, //900 custom1
	3 //1000 custom2
};


int fill_reportconfig(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="ReportConfig";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"WHERE ReportID<>0"))
	{
		db.Close();return FALSE;
	}

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-24*2,0,0); //roll back few days

	DWORD elapsed=0;
	
	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(int rptid=0;rptid<sizeof(report_ids)/sizeof(int);rptid++)
	{
		for(int count=0;count<report_ids[rptid];count++)
		{
				GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
			GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
			increment_time(&systime,0,0,1);

			ADDVALUE("ReportID","'%03d'",((rptid+1)*100)+count); //4,10
			ADDVALUE("Report","'0'",count); //4,10
			ADDVALUE("Description","'rpid_%i'",report_ids[rptid]); //12,255
			ADDVALUE("SelectAll","-1",count); //-7,1
			//201=transactions by batch:disable CC only
			ADDVALUE("CreditCards","%i",(((rptid+1)*100)+count)==201?0:-1); //-7,1
			ADDVALUE("PageBreaks","0",count); //-7,1
			ADDVALUE("Details","-1",count); //-7,1
			ADDVALUE("ProductSummary","-1",count); //-7,1
			ADDVALUE("HoseSummary","-1",count); //-7,1
			ADDVALUE("CostInfo","-1",count); //-7,1
			ADDVALUE("SubgroupUser","-1",count); //-7,1
			ADDVALUE("SubgroupSubtotal","-1",count); //-7,1
			ADDVALUE("SubgroupSummary","-1",count); //-7,1
			ADDVALUE("NumberTransactions","'100'",count); //4,10
			ADDVALUE("CurrentTransCount","'0'",count); //4,10
			ADDVALUE("FREE1","'0'",count); //12,15
			//free2 01=non cc only 01=all
			ADDVALUE("FREE2","'01'",count); //12,15
			ADDVALUE("FREE3","'0'",count); //12,15
			ADDVALUE("DisplayCCInfo","0",count); //-7,1

			if(dbm.execute_sql_insert(&db)==FALSE){
				db.Close();return FALSE;
			}

			if(GetTickCount() > (elapsed+250)){
				printf("%i %i       \r",rptid,count);
				elapsed=GetTickCount();
			}
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}