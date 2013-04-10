int fill_site(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="Site";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}
	int config_number=0;
	get_ini_value("Config","config_number",&config_number);
	cout<<"config_number:"<<config_number<<endl;

	int max_site=100;
	if(get_ini_value(dbm.table_name,"max_site",&max_site))
		cout<<"max_site="<<max_site<<endl;

	int connection_type=1;//connect type:0=modem,1=lan,2=direct
	CString str;
	if(get_ini_str(dbm.table_name,"connect_type",&str)){
		str.MakeLower();
		if(str.Find("phone")>=0)
			connection_type=0;
		else if(str.Find("lan")>=0)
			connection_type=1;
		else if(str.Find("direct")>=0)
			connection_type=2;
		else{str="lan";cout<<"error:cant find type:phone,lan,direct. defaulting to lan\n";}
		cout<<"connect type="<<(LPCTSTR)str<<endl;
	}

	CString ipaddr="172.16.40.99";
	if(get_ini_str(dbm.table_name,"ipaddr",&ipaddr))
		cout<<"ipaddr="<<(LPCSTR)ipaddr<<endl;

	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-72,0,0); //roll back few days

	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));

	int count=0;
	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(count=1;count<=max_site;count++) //site cant be all zero
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		increment_time(&systime,0,0,1);

		ADDVALUE("SITEID","'A%03i'",count%1000); //12,4
		ADDVALUE("SITENAME","'site[%04i]%05i'",config_number,count); //12,40
		ADDVALUE("ADDRESS1","'addr1_%04i'",count); //12,40
		ADDVALUE("ADDRESS2","'addr2_%04i'",count); //12,40
		ADDVALUE("CITY","'city_%04i'",count); //12,20
		ADDVALUE("STATE","'A%c'",'A'+count%26); //12,2
		ADDVALUE("ZIP","'zip%i'",count); //12,10
		ADDVALUE("CONTACT","'ctc%i'",count); //12,30
		ADDVALUE("PHONE","'ph%i'",count); //12,25
		ADDVALUE("MODEM_PH","'mod%i'",count%10); //12,40
		ADDVALUE("FAX","'fax%i'",count); //12,25
		ADDVALUE("EMAIL","'eml%i'",count); //12,50
		ADDVALUE("SITE_TYPE","'F'",count); //12,2
		ADDVALUE("CONNECT","'M'",count); //12,2
		ADDVALUE("TMU","'0'",count); //12,2
		ADDVALUE("COMMENT","'comment%05i'",count); //12,40
		ADDVALUE("DEFTAXLVL","''",count); //12,2
		ADDVALUE("DNLDTIME","'%s %s'",date,time); //11,19
		ADDVALUE("AUTOINTFACE","0",count); //-7,1
		ADDVALUE("FREE1","''",count); //12,15
		ADDVALUE("FREE2","''",count); //12,15
		ADDVALUE("FREE3","''",count); //12,15
		ADDVALUE("SELDNLD","%i",-count%2); //-7,1
		ADDVALUE("DNLD_FL","%i",-count%2); //-7,1
		ADDVALUE("SENDLIST_FL","%i",-count%2); //-7,1
		ADDVALUE("TMU_FL","0",count); //-7,1
		ADDVALUE("DNLD_OK","0",count); //-7,1
		ADDVALUE("SENDVEHILIST_OK","0",count); //-7,1
		ADDVALUE("SENDUSERLIST_OK","0",count); //-7,1
		ADDVALUE("TMU_OK","0",count); //-7,1
		ADDVALUE("ALLDL_OK","0",count); //-7,1
		ADDVALUE("TX_COUNT","'%i'",count%100); //5,5
		ADDVALUE("ZTX_COUNT","'%i'",count%100); //5,5
		ADDVALUE("MTX_COUNT","'%i'",count%100); //5,5
		ADDVALUE("MOB_RTX","'0'",count); //5,5
		ADDVALUE("MOB_ZTX","'0'",count); //5,5
		ADDVALUE("MOB_MTX","'0'",count); //5,5
		ADDVALUE("NEWDIALMETHOD","0",count); //-7,1
		ADDVALUE("FREE4","'N'",count); //12,15
		ADDVALUE("SENDAIMLIST_OK","0",count); //-7,1
		ADDVALUE("USERF","0",count); //-7,1
		ADDVALUE("BAUDRATE","'0'",count); //4,10
		ADDVALUE("TRANSTYPE","'1'",count); //4,10
		ADDVALUE("CONNECTTYPE","'%i'",connection_type); //4,10
		ADDVALUE("IPADD","'old_ip_%i'",count%100); //12,25
		ADDVALUE("COMPORT","'3'",count); //4,10
		ADDVALUE("INCLOSEOUTMODE","0",count); //-7,1
		ADDVALUE("CLOSEOUTDATE","'%s %s'",date,time); //11,19
		ADDVALUE("SITECOUNTER","'0'",count); //4,10
		//ADDVALUE("UNITTYPE","'%i'",count<5? 6:count%8); //5,5
		ADDVALUE("UNITTYPE","'%i'",6); //5,5
		ADDVALUE("ip","'%s'",ipaddr); //12,20
		ADDVALUE("port","'23'",count%23); //5,5
		ADDVALUE("SENDOTRLIST_OK","0",count); //-7,1
		ADDVALUE("SENDKEYLESSLIST_OK","0",count); //-7,1
		ADDVALUE("SENDAIMIIPROG","0",count); //-7,1
		ADDVALUE("DNLDAIMIIPROGRAM_OK","-1",count); //-7,1
		ADDVALUE("DNLDAIMIIUPDATE_OK","0",count); //-7,1
		ADDVALUE("DNLDAIMIILOCK_OK","0",count); //-7,1
		ADDVALUE("SENDAIMIIPROGRAM_OK","-1",count); //-7,1
		ADDVALUE("SENDAIMIIUPDATE_OK","0",count); //-7,1
		ADDVALUE("SENDAIMIILOCK_OK","-1",count); //-7,1
		ADDVALUE("SENDLAACCTLIST_OK","0",count); //-7,1
		ADDVALUE("SENDLALOCKLIST_OK","0",count); //-7,1
		SKIP_VALUE("AUTOINCREMENT","''",count); //4,10
		ADDVALUE("LA_LOCKIN","-1",count); //-7,1
		ADDVALUE("ModemStringOverride","0",count); //-7,1
		ADDVALUE("ModemString","''",count); //12,255
		ADDVALUE("PABPCompliant","'%i'",count%2); //4,10
		ADDVALUE("IsOutsideAgency","0",count); //-7,1
		ADDVALUE("FTPConnectionOption","'0'",count); //5,5
		ADDVALUE("TimeSynchronization","%i",-count%2); //-7,1
		ADDVALUE("Offset","'%i'",((count+20)%40)-20); //12,3
		ADDVALUE("CurrentStatus","'%s'","1/1/1970"); //11,19

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