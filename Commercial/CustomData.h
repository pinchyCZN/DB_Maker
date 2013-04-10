enum customs {
   //System Information
   SYS_SYSTEMTYPE             =  1,
   SYS_NUMOFUSERS             =  2,

   //PC Software Customization Between 101 and 999
   CUSTOM_CONNDOT             = 101,
   CUSTOM_VOYAGER             = 102,    //RS 12-19-2000 was MOARMYGUARD/changed from site sig to .bin for Voyager card
   CUSTOM_FOXJAMES			   = 103,
   CUSTOM_AMTRAK              = 104,
   CUSTOM_DOTHAN		         = 105,    //RS 06-01-2001 Not used,see CUSTOM_VKBOTH
   CUSTOM_USPS			         = 106,
   CUSTOM_AA			         = 107,     //american airlines
   CUSTOM_TNUM			         = 108,    //RS 10-20-2000  AIRPLANE TAIL NUMS (only use with COM-VK)
   CUSTOM_BOEING_DOUGLAS	   = 109,
   CUSTOM_CCDISCOUNT		      = 110,   //Credit Card Discounts
   CUSTOM_PPCARD		         = 111,
   CUSTOM_VKBOTH			      = 112, //RS 06-01-2001 Hillborough wants same custom as Dothan
   CUSTOM_WEX				      = 113, //RS 11-14-2001 Wright Express,just like CUSTOM_VOYAGER.....
   CUSTOM_EXPCOST_MULTIPLIER  = 114, //RS 02-06-2002 City of Ocala ask for this
   CUSTOM_FBOMANAGER		      = 115, //WEB 07-25-2005 FBOManager support
   CUSTOM_IMPORTSYNTECH		   = 116, //WEB 09-3-2005 CustomImport support
   CUSTOM_RECONCILE			   = 117, //WEB 09-15-2005 Reconcile support
   CUSTOM_LOCALAUTH_ACCT      = 118, // BKM 10-03-2005 localauth stuff
   CUSTOM_LOCALAUTH_LOCK      = 119, // BKM 10-03-2005 localauth stuff
   CUSTOM_WYOMINGDOT          = 120,  // WEB 11-09-2005 Wyoming DOT
   CUSTOM_COMVKMOBILE         = 121,  //SR 12-21-2006
   CUSTOM_OLVIMS_EXPORT       = 123, // WLR 6-25-2008  McGuire
   CUSTOM_NCDOT               = 124, // WLR 9-11-2008  NCDOT 
   CUSTOM_FMU_PRICING         = 125, //RFL added 11/20/2008 FMU_Pricing
   CUSTOM_17BITINTKEY		   = 126, //SR v5.1.1.26 11-19-2008 For using 17bit INTKEY
   //SR v5.2.x 2-24-2009 5xSW\CR000627 The Veh Records can only be 4 digits until a Customization is made with KeyGen
   //DO NOT USE CUSTOM_VEHICLE_ID
   CUSTOM_VEHICLE_ID		      = 127, // SDH 09-11-2008  Patch Files
   CUSTOM_YELLOWFREIGHT       = 128, //SR v5.1.1.30 1-26-2009 For yellow freight custom
   CUSTOM_SINGLEBTN_DISPENSER = 129, //AS v5.2.0.0 added 02/9/2009 Single Button Dispenser
   CUSTOM_PABP_OFF			   = 130,  // PABP feature is a way to turn PABP off as the system should default to it being on.
   CUSTOM_DOWNLOAD_SERVICE    = 131,  // wlr 4-7-2008 FMDownloadService // updated 11/25/08 
   CUSTOM_OUTSIDE_AGENCY      = 132,    //SR v5.2.0.17 8-28-2009 NCDOT Outside Agency
   CUSTOM_100KUSERS           = 133,   //SR v5.2.0.18 9-08-2009 Added Keyless 100k Users
   CUSTOM_MODOT               = 134,   //SR v5.1.3.41 8-26-2010 MODOT Customization
   CUSTOM_COMDATA_MCFLEET     = 135, //SR 5.1.3.44 11-2-2010 Added COMDATA MC Fleet Vehicle ID
   CUSTOM_FLDOT               = 136, //SR v5.2.0.39 03-24-2011 Added for merging FLDOT patch

   //PDA Features >= 1000
   //WF 9-13-07
   FEATURE_PDA_BLUETOOTH      =  1001,
   FEATURE_PDA_CREDITCARD     =  1002,
   FEATURE_PDA_PROKEE         =  1003,
   FEATURE_PDA_CONTACTTAG     =  1004,
   FEATURE_PDA_CHECKLIST      =  1005,
   FEATURE_PDA_PROCEDURE      =  1006,
   FEATURE_TIM_PRINTER        =  1021,
   FEATURE_TIM_RADIO          =  1022,
   FEATURE_TIM_INJECTOR       =  1023,
   FEATURE_TIM_NORMPRESSURE   =  1024,
   FEATURE_TIMDEVICE          =  1025,
   FEATURE_MANUALADDITIVE     =  1007 //RFL added 1/30/2009 Manual Additive for Million-Air Dallas
};


int customdata_fields[][3]={
	//feature,operator,data
	{110,1,0}, //discount card
	{111,1,0}, //prepaid cards
	{118,1,0}, //Local Authorization Accounts
	{119,1,0}, //Local Authorization Lock
	{123,1,30116}, //OLVIMS exporter
	{129,1,10737}, //Single Button Dispenser Configuration
	0 //end marker
};
int customization_hash(int feature,int sitesig,CString *hash)
{
	CString str;
	str.Format("%04X",sitesig);
	str.MakeReverse();
	int mask=strtol(str,0,16);
	int forever=0x270F016C;
	hash->Format("%08X%08X%08X", feature^mask, mask, forever^mask );
	return TRUE;
}
int fill_customdata(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="CustomData";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	int site_sig=0xFFFE;
	get_ini_value("Config","ssig",&site_sig);

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-24*2,0,0); //roll back few days

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int try_ini_custom=TRUE;
	for(int count=0;count<500;count++)
	{
		int data_fields[3];
		if(try_ini_custom){
			CString values="";
			char cname[20];
			if(count>40){
				try_ini_custom=FALSE;
				count=-1;
				continue;
			}
			sprintf(cname,"custom%d",count);
			if(get_ini_str(dbm.table_name,cname,&values)){
				sscanf(values,"%d,%d,%d",&data_fields[0],&data_fields[1],&data_fields[2]);
				printf("adding custom data %d,%d,%d\n",data_fields[0],data_fields[1],data_fields[2]);
			}
			else
				continue;
		}
		else{
			if(customdata_fields[count][0]==0)
				break;
			data_fields[0]=customdata_fields[count][0];
			data_fields[1]=customdata_fields[count][1];
			data_fields[2]=customdata_fields[count][2];
		}
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		CString hash;
		customization_hash(data_fields[0],site_sig,&hash);

		ADDVALUE("CustomId","'%d'",data_fields[0]); //4,10
		ADDVALUE("Code","'%s'",hash); //12,255
		ADDVALUE("CreateDate","'%s %s'",date,time); //11,19
		ADDVALUE("Operator","'%d'",data_fields[1]); //4,10
		ADDVALUE("Data","'%d'",data_fields[2]); //4,10

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