int fill_posplux(CString dbname)
{
	DBMaker dbm;
	dbm.table_name="posplux";

	CString str;
	get_ini_str("posplux","database",&str);
	if(!str.IsEmpty())
		dbname=str;

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	dbm.open_db(&dbname);
	dbm.get_fields_array();

	if(!delete_table_records(&dbm.db,dbm.table_name,"")){
		dbm.db.Close();return FALSE;
	}

	char time[20],date[20];
	SYSTEMTIME systime;
//	GetSystemTime(&systime);
	set_system_time(&systime,2010,1,1,0,0,0);
//	increment_time(&systime,-72,0,0); //roll back few days

	//2008-09-29 08:41:35.000
	GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"HH':'mm':'ss",time,sizeof(time));
	GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"yyyy'-'MM'-'dd",date,sizeof(date));

	int count=0;
	DWORD elapsed=0;

//	int int_key=0;
//	get_table_index(&db,"Config","VEHIINTKEY",&int_key);
	
	cout<<"Writing to "<< dbm.table_name << " Table\n";

	int max_count=5;
	get_ini_value(dbm.table_name,"count",&max_count);

	int board;
	for(board=0;board<max_count;board++)
	{
		int row=0;
		int max_buttons=6*6;
		#define MAX_TYPES 7
		char *types[MAX_TYPES]={"PLU","COMBO","Discount","Jump","Modifier","Special","Wash car"}; //'L'

		for(count=0;count<max_buttons;count++)
		{

			int color=0x505000;
			color+=count&0xFF;

			if(count>0 && ((count%6)==0))
				row++;

			ADDVALUE("board","%i",board); //2,3
			ADDVALUE("keyrow","%i",row+1); //2,2
			ADDVALUE("keycol","%i",1+(count%6)); //2,2
			ADDVALUE("legend","'%s %i'",types[count%MAX_TYPES],count); //1,40
			ADDVALUE("keycolor","%i",color); //2,12
			ADDVALUE("type","'%c'",types[count%MAX_TYPES][0]); //1,1
			ADDVALUE("item","'%i'",count); //1,20
			ADDVALUE("modifier","0",count); //2,20
			ADDVALUE("postmodgrp","0",count); //2,2
			ADDVALUE("parms","' '",count); //1,250
			ADDVALUE("bitmap","' '",count); //1,40
			ADDVALUE("stretch","0",count); //-7,1
			ADDVALUE("inactive","0",count); //-7,1
			ADDVALUE("next_board","0",count); //2,3
			ADDVALUE("not_reqd","0",count); //-7,1

			if(dbm.execute_sql_insert()==FALSE){
				dbm.db.Close();return FALSE;
			}
		}
	}
	dbm.close();
	return TRUE;
}

