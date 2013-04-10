int fill_obdrecords(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="OBDRecords";

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

	int veh=0;
	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(veh=0;veh<500;veh++)
	{
		int count;

		count=veh;

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		SKIP_VALUE("OBDRECINDEX","''",count); //4,10
		ADDVALUE("DNLDTIME"," '%s %s'",date,time); //11,19
		ADDVALUE("TRANTIME","'%s %s'",date,time); //11,19
		ADDVALUE("SITEID","'A%03i'",count%10); //12,4
		ADDVALUE("VEHICLEID","'%08i'",(count>>1)%100); //12,8
		ADDVALUE("TROUBLECODE","'%i'",count); //4,10
		ADDVALUE("OBDCODE","'%c%04i'",'A'+(count%26),count%1000); //12,255
		ADDVALUE("OBDDESCRIPTION","'descript%04i'",count%1000); //12,255
		ADDVALUE("SERVICED","%i",-(count>>1)%2); //-7,1
		ADDVALUE("SERVICEDATE","'%s %s'",date,time); //11,19
		ADDVALUE("SERVICEOPERATOR","'ADMIN'",count); //12,50
		ADDVALUE("COMMENTS","'comment%04i'",count%1000); //12,50
		ADDVALUE("SERVICESITE","'ssite%04i'",count%1000); //12,50
		ADDVALUE("SaeFormat","'%i'",count%2); //5,5
		ADDVALUE("FMI","'%i'",count%10); //5,5
		ADDVALUE("MIDSPN","'%i'",count%129); //4,10
		ADDVALUE("isSID","%i",-count%2); //-7,1
		ADDVALUE("PSID","'%i'",count%1000); //5,5
		ADDVALUE("Lamp","'%i'",count%13); //5,5
		ADDVALUE("FMIDescription","'FMIdescript%04i'",count%1000); //12,255
		ADDVALUE("Odom","'%i'",count); //4,10
		ADDVALUE("Chron1Type","'%i'",count%4); //5,5
		ADDVALUE("Chron1","'%i'",count); //4,10
		ADDVALUE("Chron2Type","'%i'",count%4); //5,5
		ADDVALUE("Chron2","'%i'",count); //4,10
		ADDVALUE("Chron3Type","'%i'",count%4); //5,5
		ADDVALUE("Chron3","'%i'",count); //4,10
		ADDVALUE("FirmwareVer","'%04X'",count%256); //12,4
		ADDVALUE("DateProcessed","'%s %s'",date,time); //11,19
		ADDVALUE("IsProcessed","0",-count%2); //-7,1

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}

		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",veh);
			elapsed=GetTickCount();
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}



int fill_obdstat(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="OBDStat";

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

	int veh=0;
	cout<<"Writing to "<< dbm.table_name << " Table\n";

	for(veh=0;veh<500;veh++)
	{
		int count;

		count=veh;

		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		SKIP_VALUE("STATID","''",count); //4,10
		ADDVALUE("VehicleID","'%08i'",(count>>2)%10); //12,8
		ADDVALUE("LastUpdate","'%s %s'",date,time); //11,19
		ADDVALUE("MaxCoolTemp","'%i'",count++%900); //8,15
		ADDVALUE("MaxEngSpd","'%i'",count++%10000); //8,15
		ADDVALUE("MaxVehiSpd","'%i'",count++%100); //8,15
		ADDVALUE("MinOilPres","'%i'",count++%100); //8,15
		ADDVALUE("MaxOilTemp","'%i'",count++%100); //8,15
		ADDVALUE("MinTranPres","'%i'",count++%100); //8,15
		ADDVALUE("MinVolt","'%i'",count++%100); //8,15
		ADDVALUE("MaxVolt","'%i'",count++%100); //8,15
		ADDVALUE("PreFuel","'%i'",count++%100); //8,15
		ADDVALUE("OilLvl","'%i'",count++%100); //8,15
		ADDVALUE("WashLvl","'%i'",count++%100); //8,15
		ADDVALUE("CoolLvl","'%i'",count++%100); //8,15
		ADDVALUE("TranLvl","'%i'",count++%100); //8,15
		ADDVALUE("StartCount","'%i'",count++%100); //4,10
		ADDVALUE("AuxCount","'%i'",count++%100); //4,10
		ADDVALUE("FuelUsed","'%i'",count++%100); //8,15
		ADDVALUE("PTOFuel","'%i'",count++%100); //8,15
		ADDVALUE("FuelEcon","'%i'",count++%100); //8,15
		ADDVALUE("Unit","'%i'",veh%3); //5,5
		count=veh;
		ADDVALUE("MaxCoolTempValid","'%i'",count%2); //-6,3
		ADDVALUE("MaxEngSpdValid","'%i'",count%2); //-6,3
		ADDVALUE("MaxVehiSpdValid","'%i'",count%2); //-6,3
		ADDVALUE("MinOilPresValid","'%i'",count%2); //-6,3
		ADDVALUE("MaxOilTempValid","'%i'",count%2); //-6,3
		ADDVALUE("MinTranPresValid","'%i'",count%2); //-6,3
		ADDVALUE("MinVoltValid","'%i'",count%2); //-6,3
		ADDVALUE("MaxVoltValid","'%i'",count%2); //-6,3
		ADDVALUE("PreFuelValid","'%i'",count%2); //-6,3
		ADDVALUE("OilLvlValid","'%i'",count%2); //-6,3
		ADDVALUE("WashLvlValid","'%i'",count%2); //-6,3
		ADDVALUE("CoolLvlValid","'%i'",count%2); //-6,3
		ADDVALUE("TranLvlValid","'%i'",count%2); //-6,3
		ADDVALUE("StartCountValid","'%i'",count%2); //-6,3
		ADDVALUE("AuxCountValid","'%i'",count%2); //-6,3
		ADDVALUE("FuelUsedValid","'%i'",count%2); //-6,3
		ADDVALUE("PTOFuelValid","'%i'",count%2); //-6,3
		ADDVALUE("FuelEconValid","'%i'",count%2); //-6,3
		ADDVALUE("SITEID","'A%03i'",count%100); //12,4
		ADDVALUE("Odom","'%i'",count); //4,10
		ADDVALUE("Chron1Type","'%i'",count%3); //5,5
		ADDVALUE("Chron1","'%i'",count); //4,10
		ADDVALUE("Chron2Type","'%i'",count%3); //5,5
		ADDVALUE("Chron2","'%i'",count); //4,10
		ADDVALUE("Chron3Type","'%i'",count%3); //5,5
		ADDVALUE("Chron3","'%i'",count); //4,10
		ADDVALUE("FirmwareVer","'%04X'",count%256); //12,4
		ADDVALUE("DateProcessed","'%s %s'",date,time); //11,19
		ADDVALUE("IsProcessed","0",count); //-7,1

		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}
		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",veh);
			elapsed=GetTickCount();
		}
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}