#include <math.h>

//char *tcodes[]={"00","01","02","03","04","05","06","07","08","09",
//			      "0A","10","11","12","13","14","15","16","17","18",
//				  "19","20","30","98","99","A0","S1","S2","S3","S4",0}; //30 types

char *tcodes[]={
"00","User Fuel",
"01","Lube truck issue",
"02","Tanker Truck",
"03","Semi Manual",
"04","Manual Issue",
"05","User Fuel",
"06","Fuel Drop",
"07","Fuel Adjustment",
"08","Transfer to Site",
"09","Oil Check",
"0A","AIM Fuel",
"10","man. Vehicle Fuel",
"11","man. Lube truck issue",
"12","man. Tanker Truck",
"13","man. Semi Manual",
"14","man. Manual Issue",
"15","man. User Fuel",
"16","man. Fuel Drop",
"17","man. Fuel Adjustment",
"18","man. Transfer to Site",
"19","man. Oil Check",
"20","Remote Start",
"30","MFA Petro-Card 24",
"98","PrePaid Card",
"99","Credit Card",
"A0","ADC trans",
"S1","Gate Opener",
"S2","Door opener",
"S3","Car Wash",
"S4","Access Point"
};
int fill_maintrans(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="MainTrans";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

//	double quantitys[]={0, .001,  .01,  .1,  1, 10, 1, .1, .01, .001};
//	double unitcosts[]={0, .0001, .001, .01, .1, 1, 10, 1, .1,  .01 };
	double quantitys[]={.001,  .01,  .1,  1, 10, 1, .1, .01, .001, 1234.12};
	double unitcosts[]={.0001, .001, .01, .1, 1, 10, 1, .1,  .01, 1234.12};

	int tc,veh;
	DWORD elapsed;

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);

	if(get_ini_systime(dbm.table_name,"starttime",&systime))
	{
		printf("start time:%04d/%d/%d %02d:%02d:%02d\n",systime.wYear,systime.wMonth,systime.wDay,systime.wHour,
															systime.wMinute,systime.wSecond);
	}
	else
	{
		float rollback=2;
		if(systime.wDay<=1)
			rollback=(float)0.1;
		else if(systime.wDay==2)
			rollback=1;

		int rdays=1;
		if(get_ini_value(dbm.table_name,"rollback",&rdays))
			rollback=rdays;
		increment_time(&systime,(int)-24*rollback,0,0); //roll back some days
		printf("\nstarting transactions at day=%i hour=%i (rollback=%1.1f days)\n",systime.wDay,systime.wHour,rollback);
	}
	int hour=0,min=1,sec=0;
	get_ini_value(dbm.table_name,"hour",&hour);
	get_ini_value(dbm.table_name,"sec",&sec);
	get_ini_value(dbm.table_name,"min",&min);
	printf("increment hour=%i,min=%i,sec=%i\n",hour,min,sec);

	int start_veh_id=0;
	if(get_ini_value(dbm.table_name,"start_veh_id",&start_veh_id))
		cout<<"start_veh_id="<<start_veh_id<<endl;

	int max_veh=4;
	if(get_ini_value(dbm.table_name,"max_veh",&max_veh))
		cout<<"max vehicles="<<max_veh<<endl;

	int max_count=10;
	if(get_ini_value(dbm.table_name,"max_count",&max_count))
		cout<<"max count="<<max_count<<endl;

	CString exclude_tc;
	if(get_ini_str(dbm.table_name,"exclude",&exclude_tc))
		cout<<"excluding certain transactions:\n"<<(LPCTSTR)exclude_tc<<endl;

	int reset_time=FALSE;
	CString tmp;
	if(get_ini_str(dbm.table_name,"reset_time",&tmp)){
		cout<<"reseting time after each transaction code"<<endl;
		reset_time=TRUE;
	}


	SYSTEMTIME origtime=systime;
	elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(tc=0;tc<(sizeof(tcodes)/sizeof(char *)/2);tc++)
	{
		if(exclude_tc.Find(tcodes[tc*2])!=-1)
			continue;
		if(reset_time)
			systime=origtime; //reset time for each transaction code
		for(veh=start_veh_id;veh<(max_veh+start_veh_id);veh++)
		{
			for(int count=0;count<max_count;count++) //120* count
			{

				GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
				GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
				increment_time(&systime,hour,min,sec);

				//quantitys[9]=(double)(rand()%5000)/90.0;
				//unitcosts[9]=(double)(rand()%500)/90.0;
				if((tc==0 || (tc==20)) && (count==(max_count-1)) && (veh==(max_veh-1)))
				{
					veh=12345678;
					count=123456789;
				}

				if((count/10)%10==9)
				{
//					SYSTEMTIME temptime=systime;
//					increment_time(&temptime,-24*(rand()%30),0,0);
//					GetTimeFormat(LOCALE_USER_DEFAULT,0,&temptime,"hh':'mm':'ss tt",time,sizeof(time));
//					GetDateFormat(LOCALE_USER_DEFAULT,0,&temptime,"M'/'d'/'yyyy",date,sizeof(date));
				}
		
				ADDVALUE("TC","'%s'",tcodes[tc*2]); //12,2
				ADDVALUE("SITEID","'A%03i'",count%20); //12,4
				if(veh==0){ // for vehicle mileage report OLVIMS
					ADDVALUE("VEHICLEID","'G0ABCDEF'",veh); //12,8
				}
				else{
					ADDVALUE("VEHICLEID","'%08i'",veh); //12,8
				}
				ADDVALUE("USERID","'%09i'",count); //12,9
				ADDVALUE("ODOMETER","'%i'",(count+1)*4); //4,10
				ADDVALUE("HOSE","'%i'",count%8); //5,5
				ADDVALUE("PRODUCT","'%i'",(veh%2)+1); //5,5
				ADDVALUE("TRANTIME","'%s %s'",date,time); //11,19
//				ADDVALUE("QUANTITY","'%.4f'",quantitys[(count)%10]*(count==9?1:count%10)); //8,15
		ADDVALUE("QUANTITY","'%i'",1); //8,15
				ADDVALUE("TOTALCOST","'%.4f'",quantitys[(count)%10]*(count==9?1:count%10) * unitcosts[(count)%10]*(count==9?1:count%10)); //8,15
				ADDVALUE("UNITCOST","'%.4f'",unitcosts[(count)%10]*(count==9?1:count%10)); //8,15
				ADDVALUE("DNLDTIME","'%s %s'",date,time); //11,19
				ADDVALUE("AGENCY","'acy%03i'",count%1000); //12,7
				ADDVALUE("INTKEY","'%08i'",count%10000); //12,9
				ADDVALUE("DIV_R","'%i'",count%100); //5,5
				ADDVALUE("JDATE","'%04i%03i'",systime.wYear,systime.wDay); //4,10
				ADDVALUE("MTIME","'%02i:%02i:%02i'",systime.wHour,systime.wMinute,systime.wSecond); //12,8
				ADDVALUE("TANK","'%i'",count%10); //5,5
				ADDVALUE("SITEID2","'B%03i'",count%1000); //12,4
				ADDVALUE("TANK2","'0'",count); //5,5
				ADDVALUE("ARCHIVED","''",count); //12,2
				ADDVALUE("FREE1","'%i'",count%6); //12,15
				ADDVALUE("FREE2","''",count); //12,15
				ADDVALUE("FREE3","''",count); //12,15
				ADDVALUE("FINCODE","'%i'",count%100); //5,5
				ADDVALUE("CCARDID","'cc%014i'",count); //12,20
				ADDVALUE("TRANNUM","'%i'",count%4096); //4,10
				ADDVALUE("INTKEY2","''",count); //12,9
				ADDVALUE("CCARDTYPE","'%i'",count%1000); //4,10
				ADDVALUE("PPCBALANCE","'%i'",count%2); //8,15
				ADDVALUE("PPCCREDIT","'%i'",count%2); //8,15
				ADDVALUE("CCTRANID","'%i'",count%100); //12,9
				ADDVALUE("PRICELEVEL","'%i'",count%10); //5,5
				ADDVALUE("CUSTOMDATA1","'%08.8d~12345~12'",count); //12,255
				ADDVALUE("CUSTOMDATA2","'0~%s'",tcodes[(tc*2)+1]); //12,255
				ADDVALUE("AIMSTAT","'%i'",count%5); //12,15
				ADDVALUE("MODIFIED","0",count); //-7,1
				ADDVALUE("OPNUM","'0'",count); //4,10
				ADDVALUE("EXCEPTION","0",count); //-7,1
				ADDVALUE("TEMPSTATUS","'A%04i'",count%99); //12,15
				ADDVALUE("COSTERR","'N%i'",count%10); //12,2
				ADDVALUE("PPCSTAT","'PPC%i'",count%10); //12,4
				SKIP_VALUE("AUTONUM","''",count); //4,10
				ADDVALUE("SITECOUNTER","'%i'",count%2); //4,10
				ADDVALUE("CUSTID","'%09i'",count%100); //12,9
				ADDVALUE("FMUTRANTYPE","'%i'",count%100); //5,5
				ADDVALUE("TRANINITIATOR","'%i'",count%100); //5,5
				ADDVALUE("TRANINITIATORMAPPED","'%i'",count%100); //5,5
				ADDVALUE("GRADENUM","'%i'",count%100); //5,5
				ADDVALUE("CCIDL4","''",count); //12,4
				ADDVALUE("EQUIPID","''",count); //12,12
				ADDVALUE("CHRONOMETER1","'%i'",10000+(count%100000)); //4,10
				ADDVALUE("CHRONOMETER2","'%i'",20000+(count%100000)); //4,10
				ADDVALUE("TAILNUMBER","'%i'",count%1000); //12,8
				ADDVALUE("SUPPRESS","0",count); //-7,1
				ADDVALUE("CCTRANNUM","'%i'",count%1000); //4,10
				ADDVALUE("AIM2ID","'%012i'",count); //12,12
				ADDVALUE("CHRONOMETER3","'%i'",30000+(count%100000)); //4,10
				ADDVALUE("Encrypt_KeyID","'%i'",count%1000); //5,5
				ADDVALUE("PABPCompliant","'%i'",count%1); //4,10
				ADDVALUE("ADDITIVE","'%i'",count%100); //5,5
				ADDVALUE("ALTERNATE","'%i'",count%100); //4,10
				ADDVALUE("FILL","''",count); //12,2

				if(dbm.execute_sql_insert(&db)==FALSE){
					db.Close();return FALSE;
				}
				if(GetTickCount() > (elapsed+250)){
					printf("%i %i %i                         \r",tc,veh,count);
					elapsed=GetTickCount();
				}
			}			
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}


char *terminal_msg[]={"Terminal will SHUTDOWN","Terminal has been SHUTDOWN","TERMINAL STATUS GOOD","has reached the Daily Quantity Limit for",0};
int fill_transallmsg(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="TransAllMsg";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	int max_count=10;
	if(get_ini_value(dbm.table_name,"max_count",&max_count))
		cout<<"max count="<<max_count<<endl;

	CString exclude_tc;
	if(get_ini_str(dbm.table_name,"exclude",&exclude_tc))
		cout<<"excluding certain transactions:\n"<<(LPCTSTR)exclude_tc<<endl;

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-72,0,0); //roll back few days

	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));

	int count=0;
	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(int tc=0;tc<(sizeof(tcodes)/sizeof(char *)/2);tc++)
	{
		if(exclude_tc.Find(tcodes[tc*2])!=-1)
			continue;
		for(count=0;count<max_count;count++)
		{
			GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
			increment_time(&systime,0,0,1);

			ADDVALUE("TC","'%s'",tcodes[tc*2]); //12,2
			ADDVALUE("ODOMETER","'%i'",count%100000); //4,10
			ADDVALUE("USERID","'%09i'",(count%1000)+1); //12,9
			ADDVALUE("HOSE","'%i'",(count%8)+1); //5,5
			ADDVALUE("PRODUCT","'%i'",(count%99)+1); //5,5
			ADDVALUE("LQUANTITY","'%i'",count%1000); //12,5
			ADDVALUE("INTKEY","'%09i'",count%10000); //12,9
			ADDVALUE("JDATE","'%04i%03i'",systime.wYear,systime.wDay); //4,10
			ADDVALUE("MTIME","'%02i:%02i:%02i'",systime.wHour,systime.wMinute,systime.wSecond); //12,8
			ADDVALUE("AGENCY","'%i'",count%1000); //12,7
			ADDVALUE("DNLDTIME","'%s %s'",date,time); //11,19
			ADDVALUE("TRAN_NO","'%i'",count%4096); //5,5
			ADDVALUE("QUANTITY","'%i'",count%100); //8,15
			ADDVALUE("TRANTIME","'%s %s'",date,time); //11,19
			ADDVALUE("SITEID","'A%03i'",count%100); //12,4
			ADDVALUE("MOBILESITEID","'%04i'",count%1000); //12,4
			ADDVALUE("TANK","'%i'",count%100); //5,5
			ADDVALUE("VEHICLEID","'%08i'",count%1000); //12,8
			ADDVALUE("DIV_R","'%i'",count%15); //5,5
			ADDVALUE("MESSAGE","'%s'",terminal_msg[count%4]); //12,255
			ADDVALUE("DOWNTIME","'%i'",count%10); //4,10
			ADDVALUE("MESSAGE2","'msg2%06i'",count); //12,60
			ADDVALUE("ONLINETYPE","'%i'",count%10); //5,5
			ADDVALUE("FREE1","'fr1%05i'",count); //12,15
			ADDVALUE("FREE2","'fr2%05i'",count); //12,15
			ADDVALUE("FREE3","'fr3%05i'",count); //12,15
			ADDVALUE("CCARDID","'cc%05i'",count); //12,19
			ADDVALUE("TRANNUM","'%04i'",count%4096); //4,10
			ADDVALUE("INTKEY2","'%04i'",count%10000); //12,9
			ADDVALUE("CCARDTYPE","'%i'",count%100); //4,10
			ADDVALUE("PPCBALANCE","'%i'",count%10); //8,15
			ADDVALUE("PPCCREDIT","'%i'",count%10); //8,15
			ADDVALUE("CCTRANID","'%i'",count%10); //12,9
			ADDVALUE("PRICELEVEL","'%i'",count%10); //5,5
			ADDVALUE("CUSTOMDATA1","'custdata1%05i %s'",count,tcodes[(tc*2)+1]); //12,255
			ADDVALUE("CUSTOMDATA2","'custdata2%05i'",count); //12,255
			ADDVALUE("FMUTRANTYPE","'%i'",count%10); //5,5
			ADDVALUE("TRANINITIATOR","'%i'",count%15); //5,5
			ADDVALUE("TRANINITIATORMAPPED","'%i'",count%15); //5,5
			ADDVALUE("AIM2ID","'%012i'",count); //12,12

			if(dbm.execute_sql_insert(&db)==FALSE){
				db.Close();return FALSE;
			}
			if(GetTickCount() > (elapsed+250)){
				printf("%i %i       \r",tc,count);
				elapsed=GetTickCount();
			}
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return 0;
}
int fill_pieduptrans(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="PIEDuplicateTrans";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

//	double quantitys[]={0, .001,  .01,  .1,  1, 10, 1, .1, .01, .001};
//	double unitcosts[]={0, .0001, .001, .01, .1, 1, 10, 1, .1,  .01 };
	double quantitys[]={.001,  .01,  .1,  1, 10, 1, .1, .01, .001, 1234.12};
	double unitcosts[]={.0001, .001, .01, .1, 1, 10, 1, .1,  .01, 1234.12};

	int tc,veh;
	DWORD elapsed;

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);

	if(get_ini_systime(dbm.table_name,"starttime",&systime))
	{
		printf("start time:%04d/%d/%d %02d:%02d:%02d\n",systime.wYear,systime.wMonth,systime.wDay,systime.wHour,
															systime.wMinute,systime.wSecond);
	}
	else
	{
		float rollback=2;
		if(systime.wDay<=1)
			rollback=(float)0.1;
		else if(systime.wDay==2)
			rollback=1;

		int rdays=1;
		if(get_ini_value(dbm.table_name,"rollback",&rdays))
			rollback=rdays;
		increment_time(&systime,(int)-24*rollback,0,0); //roll back some days
		printf("\nstarting transactions at day=%i hour=%i (rollback=%1.1f days)\n",systime.wDay,systime.wHour,rollback);
	}
	int hour=0,min=1,sec=0;
	get_ini_value(dbm.table_name,"hour",&hour);
	get_ini_value(dbm.table_name,"sec",&sec);
	get_ini_value(dbm.table_name,"min",&min);
	printf("increment hour=%i,min=%i,sec=%i\n",hour,min,sec);

	int start_veh_id=0;
	if(get_ini_value(dbm.table_name,"start_veh_id",&start_veh_id))
		cout<<"start_veh_id="<<start_veh_id<<endl;

	int max_veh=4;
	if(get_ini_value(dbm.table_name,"max_veh",&max_veh))
		cout<<"max vehicles="<<max_veh<<endl;

	int max_count=5;
	if(get_ini_value(dbm.table_name,"max_count",&max_count))
		cout<<"max count="<<max_count<<endl;

	CString exclude_tc;
	if(get_ini_str(dbm.table_name,"exclude",&exclude_tc))
		cout<<"excluding certain transactions:\n"<<(LPCTSTR)exclude_tc<<endl;


//	getkey();
//	return 0;
	elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(tc=0;tc<(sizeof(tcodes)/sizeof(char *)/2);tc++)
	{
		if(exclude_tc.Find(tcodes[tc*2])!=-1)
			continue;
		for(veh=start_veh_id;veh<(max_veh+start_veh_id);veh++)
		{
			for(int count=0;count<max_count;count++) //120* count
			{

				GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
				GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
				increment_time(&systime,hour,min,sec);

				ADDVALUE("TC","'%s'",tcodes[tc*2]); //12,2
				ADDVALUE("SITEID","'A%03i'",count%20); //12,4
				if(veh==0){ // for vehicle mileage report OLVIMS
					ADDVALUE("VEHICLEID","'G0ABCDEF'",veh); //12,8
				}
				else{
					ADDVALUE("VEHICLEID","'%08i'",veh); //12,8
				}
				ADDVALUE("USERID","'%09i'",count); //12,9
				ADDVALUE("ODOMETER","%i",count); //4,10
				ADDVALUE("HOSE","%i",(count%8)+1); //5,5
				ADDVALUE("PRODUCT","%i",(count%99)+1); //5,5
				ADDVALUE("TRANTIME","'%s %s'",date,time); //11,19
				ADDVALUE("QUANTITY","%i",count%99); //8,15
				ADDVALUE("TOTALCOST","%i",count%99); //8,15
				ADDVALUE("UNITCOST","%i",count%99); //8,15
				ADDVALUE("DNLDTIME","'%s %s'",date,time); //11,19
				ADDVALUE("AGENCY","'%07i'",count%99); //12,7
				ADDVALUE("INTKEY","'%i'",count%99); //12,9
				ADDVALUE("DIV_R","%i",count%99); //5,5
				ADDVALUE("JDATE","%i",count%99); //4,10
				ADDVALUE("MTIME","'%i'",count%99); //12,8
				ADDVALUE("TANK","%i",count%99); //5,5
				ADDVALUE("SITEID2","'B%03i'",(count+1)%99); //12,4
				ADDVALUE("TANK2","%i",count%8); //5,5
				ADDVALUE("ARCHIVED","'%c'",'A'+(count%26)); //12,2
				ADDVALUE("FREE1","''",count); //12,15
				ADDVALUE("FREE2","''",count); //12,15
				ADDVALUE("FREE3","''",count); //12,15
				ADDVALUE("FINCODE","%i",count%16); //5,5
				ADDVALUE("CCARDID","'%04i'",count%999); //12,20
				ADDVALUE("TRANNUM","%i",count%999); //4,10
				ADDVALUE("INTKEY2","'%i'",count%9); //12,9
				ADDVALUE("CCARDTYPE","%i",count%99); //4,10
				ADDVALUE("PPCBALANCE","%i",count%99); //8,15
				ADDVALUE("PPCCREDIT","%i",count%99); //8,15
				ADDVALUE("CCTRANID","'%i'",count%99); //12,9
				ADDVALUE("PRICELEVEL","%i",count%9); //5,5
				ADDVALUE("CUSTOMDATA1","'%08.8d~12345~12'",count); //12,255
				ADDVALUE("CUSTOMDATA2","'0~%s'",tcodes[(tc*2)+1]); //12,255
				ADDVALUE("AIMSTAT","'ABC%04i'",count%999); //12,15
				ADDVALUE("MODIFIED","%i",-count%2); //-7,1
				ADDVALUE("OPNUM","%i",count%9); //4,10
				ADDVALUE("EXCEPTION","%i",-count%2); //-7,1
				ADDVALUE("TEMPSTATUS","'A%04i'",count%99); //12,15
				ADDVALUE("COSTERR","'%c'",'A'+(count%26)); //12,2
				ADDVALUE("PPCSTAT","'%04i'",count%999); //12,4
				SKIP_VALUE("AUTONUM","",count); //4,10
				ADDVALUE("SITECOUNTER","%i",count%999); //4,10
				ADDVALUE("CUSTID","'%09i'",count); //12,9
				ADDVALUE("FMUTRANTYPE","%i",count%99); //5,5
				ADDVALUE("TRANINITIATOR","%i",count%99); //5,5
				ADDVALUE("TRANINITIATORMAPPED","%i",count%99); //5,5
				ADDVALUE("GRADENUM","%i",count%99); //5,5
				ADDVALUE("CCIDL4","'%04i'",count%999); //12,4
				ADDVALUE("EQUIPID","'%012i'",count%999); //12,12
				ADDVALUE("CHRONOMETER1","%i",count); //4,10
				ADDVALUE("CHRONOMETER2","%i",count); //4,10
				ADDVALUE("TAILNUMBER","'T%07i'",count); //12,8
				ADDVALUE("SUPPRESS","%i",-count%2); //-7,1
				ADDVALUE("CCTRANNUM","%i",count%999); //4,10
				ADDVALUE("AIM2ID","'%012i'",count); //12,12
				ADDVALUE("CHRONOMETER3","%i",count); //4,10
				ADDVALUE("Encrypt_KeyID","%i",count%999); //5,5
				ADDVALUE("PABPCompliant","%i",count%2); //4,10
				ADDVALUE("ADDITIVE","%i",count%999); //5,5
				ADDVALUE("ALTERNATE","%i",count%999); //4,10
				ADDVALUE("FILL","'0%c'",'A'+count%26); //12,2

				if(dbm.execute_sql_insert(&db)==FALSE){
					db.Close();return FALSE;
				}
				if(GetTickCount() > (elapsed+250)){
					printf("%i %i %i          \r",tc,veh,count);
					elapsed=GetTickCount();
				}
			}			
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}

int fill_exporttrans(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="ExportTrans";

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

	int max_count=100;
	if(get_ini_value(dbm.table_name,"max_count",&max_count))
		cout<<"max count="<<max_count<<endl;
	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(int count=0;count<max_count;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,1,1);

		ADDVALUE("TC","'%s'",tcodes[(count*2)%(sizeof(tcodes)/sizeof(char *))]); //12,2
		ADDVALUE("SITEID","'A%03i'",count%1000); //12,4
		ADDVALUE("VEHICLEID","'%08i'",count); //12,8
		ADDVALUE("USERID","'%09i'",count); //12,9
		ADDVALUE("ODOMETER","%i",count); //4,10
		ADDVALUE("HOSE","%i",(count%8)+1); //5,5
		ADDVALUE("PRODUCT","%i",count%100); //5,5
		ADDVALUE("TRANTIME","'%s %s'",date,time); //11,19
		ADDVALUE("QUANTITY","%i",count%1000); //8,15
		ADDVALUE("TOTALCOST","%i",count%100); //8,15
		ADDVALUE("UNITCOST","%i",count%100); //8,15
		ADDVALUE("DNLDTIME","'%s %s'",date,time); //11,19
		ADDVALUE("AGENCY","'AGY%i'",count%10); //12,7
		ADDVALUE("INTKEY","'%09i'",count); //12,9
		ADDVALUE("DIV_R","%i",count%100); //5,5
		ADDVALUE("JDATE","%i",count%100); //4,10
		ADDVALUE("MTIME","'MTIME'",count); //12,8
		ADDVALUE("TANK","%i",count%100); //5,5
		ADDVALUE("SITEID2","'A%03i'",(count+1)%1000); //12,4
		ADDVALUE("TANK2","%i",count%100); //5,5
		ADDVALUE("STATUS","'ST'",count); //12,2
		ADDVALUE("FREE1","'FREE1'",count); //12,15
		ADDVALUE("FREE2","'FREE2'",count); //12,15
		ADDVALUE("FREE3","'FREE3'",count); //12,15
		ADDVALUE("FINCODE","%i",count%20); //5,5
		ADDVALUE("CCARDID","'%010i'",count); //12,20
		ADDVALUE("TRANNUM","%i",count%4096); //4,10
		ADDVALUE("INTKEY2","'%09i'",count); //12,9
		ADDVALUE("CCARDTYPE","%i",count%100); //4,10
		ADDVALUE("PPCBALANCE","%i",count%100); //8,15
		ADDVALUE("PPCCREDIT","%i",count%100); //8,15
		ADDVALUE("CCTRANID","'%i'",count%100); //12,9
		ADDVALUE("PRICELEVEL","%i",count%100); //5,5
		ADDVALUE("CUSTOMDATA1","'cust1_%i'",count); //12,255
		ADDVALUE("CUSTOMDATA2","'cust2_%i'",count); //12,255
		ADDVALUE("AIMSTAT","'AIM_%i'",count%100); //12,15
		ADDVALUE("MODIFIED","%i",-count%2); //-7,1
		ADDVALUE("OPNUM","%i",count%100); //4,10
		ADDVALUE("EXCEPTION","%i",-count%2); //-7,1
		ADDVALUE("TEMPSTATUS","'stat_%i'",count%100); //12,15
		ADDVALUE("COSTERR","'CE'",count); //12,2
		ADDVALUE("PPCSTAT","'PP%i'",count%10); //12,4
		ADDVALUE("AUTONUM","%i",count); //4,10
		ADDVALUE("SITECOUNTER","%i",count%100); //4,10
		ADDVALUE("CUSTID","'ID_%i'",count%100); //12,9
		ADDVALUE("FMUTRANTYPE","%i",count%100); //5,5
		ADDVALUE("TRANINITIATOR","%i",count%100); //5,5
		ADDVALUE("TRANINITIATORMAPPED","%i",count%100); //5,5
		ADDVALUE("GRADENUM","%i",count%100); //5,5
		ADDVALUE("CCIDL4","'CC%02i'",count%100); //12,4
		ADDVALUE("EQUIPID","'EQ_%i'",count%100); //12,12
		ADDVALUE("CHRONOMETER1","%i",count); //4,10
		ADDVALUE("CHRONOMETER2","%i",count); //4,10
		ADDVALUE("TAILNUMBER","'TN_%i'",count%100); //12,8
		ADDVALUE("AIM2ID","'%04i'",count%100); //12,12
		ADDVALUE("CHRONOMETER3","%i",count); //4,10
		ADDVALUE("Encrypt_KeyID","%i",count%100); //5,5
		ADDVALUE("PABPCompliant","%i",count%4); //4,10
		ADDVALUE("ADDITIVE","%i",count%100); //5,5

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}
		if(GetTickCount() > (elapsed+250)){
			printf("%i      \r",count);
			elapsed=GetTickCount();
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}
