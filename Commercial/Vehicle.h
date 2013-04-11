int fill_hshift(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="H_SHIFT";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	int veh;
	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-72,0,0); //roll back few days

	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));

	int count=0;
	DWORD elapsed=0;

	int int_key=0;
	get_table_index(&db,"Config","VEHIINTKEY",&int_key);
	
	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_vehicles=50;
	get_ini_value(dbm.table_name,"count",&max_vehicles);
	cout<<"aim prog count "<<max_vehicles<<endl;

	for(veh=1;veh<=max_vehicles;veh++)
	{
		char keytypes[]={'V','T','M','S'};
		count=veh;

		if(veh==max_vehicles)
		{ // for vehicle mileage report OLVIMS
			ADDVALUE("VEHICLEID","'G0ABCDEF'",count); //12,8
			count=5000;
		}
		else
		{
			ADDVALUE("VEHICLEID","'%08i'",count); //12,8
		}
		ADDVALUE("DESCRIPT","'vehicle%i'",count); //12,40
		ADDVALUE("VEHIYEAR","'%04i'",2000+(count%100)); //5,5
		ADDVALUE("REISSUE","'0'",count); //5,5
		ADDVALUE("DATEREISS","'%s %s'",date,time); //11,19
		ADDVALUE("ENCODED","%i",0); //-7,1
		ADDVALUE("DATEENCOD","'%s %s'",date,time); //11,19
		ADDVALUE("AUTH","%i",-count%2); //-7,1
		ADDVALUE("DATEAUTH","'%s %s'",date,time); //11,19
		ADDVALUE("KEYTYPE","'%c'",keytypes[(count/10)%sizeof(keytypes)]); //12,2
		ADDVALUE("INTKEY","'%09i'",int_key++); //12,9
		ADDVALUE("OLDINTKEY","'%09i'",0); //12,9
		ADDVALUE("CUSTID","'%09i'",count%100); //12,9
		ADDVALUE("CURRODOM","'%i'",count); //4,10
		ADDVALUE("ODOMDATE","'%s %s'",date,time); //11,19
		ADDVALUE("PMRANGE","'%i'",count%40001); //4,10
		ADDVALUE("NEXTPMMILAGE","'%i'",count%100); //4,10
		ADDVALUE("MILESHOURSRANGE","'%i'",100+(count%100)); //5,5
		ADDVALUE("AGENCY","'%i'",(count/100)%100000); //12,5
		ADDVALUE("ORG","'%i'",count%10); //12,3
		ADDVALUE("KO_ODOM","%i",-count%2); //-7,1
		ADDVALUE("KO_PIN","%i",-count%2); //-7,1
		ADDVALUE("KO_USERID","%i",-count%2); //-7,1
		ADDVALUE("KO_RECEIPT","%i",-count%2); //-7,1
		ADDVALUE("KO_FREE1","%i",-count%2); //-7,1
		ADDVALUE("KO_FREE2","%i",-count%2); //-7,1
		ADDVALUE("KO_FREE3","%i",-count%2); //-7,1
		ADDVALUE("MISC1","'misc1_%i'",count/100); //12,255
		ADDVALUE("MISC2","'misc2_%i'",count); //12,255
		ADDVALUE("MISC3","'misc3_%i'",count); //12,255
		ADDVALUE("MISC4","'misc4_%i'",count); //12,255
		ADDVALUE("MISC5","'misc5_%i'",count); //12,255
		ADDVALUE("FREE1","'%i'",count%100); //12,15
		ADDVALUE("FREE2","'0'",count); //12,15
		ADDVALUE("FREE3","'0'",count); //12,15
		ADDVALUE("COMMENT","'%i'",count); //12,40
		ADDVALUE("SERVDATE","'%s %s'",date,time); //11,19
		ADDVALUE("USAGE","'%i'",count%100); //12,10
		ADDVALUE("PROD_A","'%i'",(count%100)==0?1:(count%100)); //5,5
		ADDVALUE("PROD_B","'%i'",(count%8)+1); //5,5
		ADDVALUE("PROD_C","'0'",count); //5,5
		ADDVALUE("PROD_D","'0'",count); //5,5
		ADDVALUE("PROD_E","'0'",count); //5,5
		ADDVALUE("PROD_F","'0'",count); //5,5
		ADDVALUE("PROD_G","'0'",count); //5,5
		ADDVALUE("PROD_H","'0'",count); //5,5
		ADDVALUE("DPA_A","'%i'",(count%100)==0?1:(count%100)); //5,5
		ADDVALUE("DPA_B","'%i'",(count%8)+1); //5,5
		ADDVALUE("DPA_C","'0'",count); //5,5
		ADDVALUE("DPA_D","'0'",count); //5,5
		ADDVALUE("DPA_E","'0'",count); //5,5
		ADDVALUE("DPA_F","'0'",count); //5,5
		ADDVALUE("DPA_G","'0'",count); //5,5
		ADDVALUE("DPA_H","'0'",count); //5,5
		ADDVALUE("TRXLIM_A","'%i'",(count%100)==0?1:(count%100)); //5,5
		ADDVALUE("TRXLIM_B","'%i'",(count%8)+1); //5,5
		ADDVALUE("TRXLIM_C","'0'",count); //5,5
		ADDVALUE("TRXLIM_D","'0'",count); //5,5
		ADDVALUE("TRXLIM_E","'0'",count); //5,5
		ADDVALUE("TRXLIM_F","'0'",count); //5,5
		ADDVALUE("TRXLIM_G","'0'",count); //5,5
		ADDVALUE("TRXLIM_H","'0'",count); //5,5
		ADDVALUE("PRICELEV_A","'%i'",(count%9)==0?1:(count%9)); //12,2
		ADDVALUE("PRICELEV_B","'1'",count); //12,2
		ADDVALUE("PRICELEV_C","'1'",count); //12,2
		ADDVALUE("PRICELEV_D","'1'",count); //12,2
		ADDVALUE("PRICELEV_E","'1'",count); //12,2
		ADDVALUE("PRICELEV_F","'1'",count); //12,2
		ADDVALUE("PRICELEV_G","'1'",count); //12,2
		ADDVALUE("PRICELEV_H","'1'",count); //12,2
		ADDVALUE("MATCH_A","%i",-count%2); //-7,1
		ADDVALUE("MATCH_B","0",count); //-7,1
		ADDVALUE("MATCH_C","0",count); //-7,1
		ADDVALUE("MATCH_D","0",count); //-7,1
		ADDVALUE("MATCH_E","0",count); //-7,1
		ADDVALUE("MATCH_F","0",count); //-7,1
		ADDVALUE("MATCH_G","0",count); //-7,1
		ADDVALUE("MATCH_H","0",count); //-7,1
		ADDVALUE("AIM","%i",-(count/10)%2); //-7,1
		ADDVALUE("AIMKEY","'%012i'",count); //12,12
		ADDVALUE("OLDAIMKEY","'%i'",count); //12,12
		ADDVALUE("AIMAUTH","%i",-(count/5)%2); //-7,1
		ADDVALUE("AIMDATEAUTH","'%s %s'",date,time); //11,19
		ADDVALUE("AIM_KO_ODOM","%i",-count%2); //-7,1
		ADDVALUE("AIM_KO_RECEIPT","%i",-count%2); //-7,1
		ADDVALUE("AIM_KO_USERID","%i",-count%2); //-7,1
		ADDVALUE("PULSEFACT","'0'",count); //4,10
		ADDVALUE("AIMSTATUS","'%i'",count%3); //5,5
		ADDVALUE("TERMINALID","'0'",count); //12,3
		ADDVALUE("TERM_TIME","'%s %s'",date,time); //11,19
		ADDVALUE("AIMREISSUE","'0'",count); //5,5
		ADDVALUE("AIMDATEREISS","'%s %s'",date,time); //11,19
		ADDVALUE("REISSFOR","'0'",count); //5,5
		ADDVALUE("ENCODING","%i",-count%2); //-7,1
		ADDVALUE("AIMROLLED","'0'",count); //5,5
		ADDVALUE("LPRODUCT","'0'",count); //4,10
		ADDVALUE("LQUANTITY","'0'",count); //8,15
		ADDVALUE("LTRANTIME","'%s %s'",date,time); //11,19
		ADDVALUE("LSITEID","'0'",count); //12,50
		SKIP_VALUE("AUTOINCREMENT","''",count); //4,10
		ADDVALUE("C1_CHRONOON","%i",-count%2); //-7,1
		ADDVALUE("C1_CHRONOANALOG","0",count); //-7,1
		ADDVALUE("C1_CHRONOANALOGCHANNEL","'0'",count); //5,5
		ADDVALUE("C1_CHRONOTYPE","'%i'",count%3); //5,5
		ADDVALUE("C2_CHRONOON","%i",-count%2); //-7,1
		ADDVALUE("C2_CHRONOANALOG","0",count); //-7,1
		ADDVALUE("C2_CHRONOANALOGCHANNEL","'0'",count); //5,5
		ADDVALUE("C2_CHRONOTYPE","'%i'",(count+1)%3); //12,50
		ADDVALUE("C1_CHRONOCOMMENT","'0'",count); //12,50
		ADDVALUE("C1_CHRONOREADING","'%i'",(count*10)%999999); //4,10
		ADDVALUE("C1_CHRONDATE","'%s %s'",date,time); //11,19
		ADDVALUE("C1_CHRONOPMRANGE","'0'",count); //4,10
		ADDVALUE("C1_CHRONONEXTPM","'0'",count); //4,10
		ADDVALUE("C1_CHRONOFUELRANGE","'100'",count); //4,10
		ADDVALUE("C2_CHRONOCOMMENT","'0'",count); //12,50
		ADDVALUE("C2_CHRONOREADING","'%i'",(count*10)%999999); //4,10
		ADDVALUE("C2_CHRONDATE","'%s %s'",date,time); //11,19
		ADDVALUE("C2_CHRONOPMRANGE","'0'",count); //4,10
		ADDVALUE("C2_CHRONONEXTPM","'0'",count); //4,10
		ADDVALUE("C2_CHRONOFUELRANGE","'100'",count); //4,10
		ADDVALUE("ODOM_PMNOTICE","'%i'",(count*10)%40001); //4,10
		ADDVALUE("ODOM_EXPECTEDEFF","'%i'",(count*10)%40001); //4,10
		ADDVALUE("C1_PMNOTICE","'0'",count); //4,10
		ADDVALUE("C1_EXPECTEDEFF","'0'",count); //4,10
		ADDVALUE("C2_PMNOTICE","'0'",count); //4,10
		ADDVALUE("C2_EXPECTEDEFF","'0'",count); //4,10
		ADDVALUE("MAKEID","'0'",count); //4,10
		ADDVALUE("MODELID","'0'",count); //4,10
		ADDVALUE("C3_CHRONOON","%i",-count%2); //-7,1
		ADDVALUE("C3_CHRONOANALOG","0",count); //-7,1
		ADDVALUE("C3_CHRONOANALOGCHANNEL","'0'",count); //5,5
		ADDVALUE("C3_CHRONOTYPE","'%i'",(count+2)%3); //5,5
		ADDVALUE("C3_CHRONOCOMMENT","'0'",count); //12,50
		ADDVALUE("C3_CHRONOREADING","'%i'",(count*10)%999999); //4,10
		ADDVALUE("C3_CHRONDATE","'%s %s'",date,time); //11,19
		ADDVALUE("C3_CHRONOPMRANGE","'0'",count); //4,10
		ADDVALUE("C3_CHRONONEXTPM","'0'",count); //4,10
		ADDVALUE("C3_CHRONOFUELRANGE","'100'",count); //4,10
		ADDVALUE("C3_PMNOTICE","'0'",count); //4,10
		ADDVALUE("C3_EXPECTEDEFF","'0'",count); //4,10
		ADDVALUE("KEYPAD_AUTH","%i",-count%2); //-7,1
		ADDVALUE("C1_PREVMAINTENANCE","%i",-count%2); //-7,1
		ADDVALUE("C2_PREVMAINTENANCE","%i",-count%2); //-7,1
		ADDVALUE("C3_PREVMAINTENANCE","%i",-count%2); //-7,1
		ADDVALUE("ODOM_PREVMAINTENANCE","%i",-count%2); //-7,1

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}
		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
		}
	}
	char val[20];
	sprintf(val,"%09d",int_key);
	set_table_index(&db,"Config","VEHIINTKEY","",val);

	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}


int fill_vehiclelocklist(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="VehiLockList";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	int veh;
	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-72,0,0); //roll back few days

	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));

	int count=0;
	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_vehicles=50;
	get_ini_value(dbm.table_name,"count",&max_vehicles);
	cout<<"veh lock list count "<<max_vehicles<<endl;

	for(veh=0;veh<max_vehicles/2;veh++)
	{
		count=veh;
		ADDVALUE("INTKEY","'%09i'",count*2); //12,9
		ADDVALUE("LOCKDATE","'%s %s'",date,time); //11,19
		ADDVALUE("CODE","'%i'",(count%3)+1); //5,5
		ADDVALUE("FREE1","'F1'",count); //12,15
		ADDVALUE("FREE2","'F2'",count); //12,15
		ADDVALUE("FREE3","'F3'",count); //12,15
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