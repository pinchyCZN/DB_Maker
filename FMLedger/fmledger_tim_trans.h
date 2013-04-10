int fill_ledger_pdatrans(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="PDATrans";
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

	int max_count=80;
	if(get_ini_value(dbm.table_name,"max_count",&max_count))
		cout<<"max count="<<max_count<<endl;

	DWORD elapsed=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	for(int count=1;count<max_count+1;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);
		SKIP_VALUE("RecNo","",count); //4,10
		ADDVALUE("BaseID","''",count); //12,5
		ADDVALUE("DownloadStatus","%i",count%2); //-6,3
		ADDVALUE("TrxCode","%i",count%10); //-6,3
		ADDVALUE("TrxFlag","%i",count%2); //-6,3
		ADDVALUE("InfoEntry","%i",count%2); //-6,3
		ADDVALUE("TrxNo","%i",count%1000); //4,10
		ADDVALUE("StartTime","'%s %s'",date,time); //11,19
		ADDVALUE("EndTime","'%s %s'",date,time); //11,19
		ADDVALUE("StartDate","'%s %s'",date,time); //11,19
		ADDVALUE("CardNo","'%20i'",count); //12,20
		ADDVALUE("ExpYear","%02i",count%100); //5,5
		ADDVALUE("ExpMonth","%02i",(count%12)+1); //-6,3
		ADDVALUE("TailNo","'tail_%i'",count%1000); //12,12
		ADDVALUE("DocNo","'RU%08u'",10262000+(count%1000)); //12,10
		ADDVALUE("AdjQty1","%i",count); //8,15
		ADDVALUE("AdjQty2","%i",count+1); //8,15
		ADDVALUE("UnAdjQty1","%i",count+2); //8,15
		ADDVALUE("UnAdjQty2","%i",count+3); //8,15
		ADDVALUE("Temperature1","%i",count%100); //8,15
		ADDVALUE("Temperature2","%i",(count+1)%100); //8,15
		ADDVALUE("TempFactor1","%i",count%10); //8,15
		ADDVALUE("TempFactor2","%i",count%10); //8,15
		ADDVALUE("PumpEnd","%i",count%10); //-6,3
		ADDVALUE("PumpEnd2","%i",count%10); //-6,3
		ADDVALUE("GradeCode","'JP%i'",count%10); //12,50
		ADDVALUE("Totalizer1","%i",count%10000); //8,15
		ADDVALUE("Totalizer2","%i",count%10000); //8,15
		ADDVALUE("CIC","'CIC'",count); //12,3
		ADDVALUE("Ullage","%i",count%100); //8,15
		ADDVALUE("UIT","%i",count%100); //4,10
		ADDVALUE("DeviceID","'dev_%i'",count%100); //12,10
		ADDVALUE("DeviceID2","%i",count%100); //4,10
		ADDVALUE("DivRate1","'div_%i'",count%100); //12,10
		ADDVALUE("DivRate2","'div2_%i'",count%100); //12,10
		ADDVALUE("TrxPulse1","%i",count%100); //4,10
		ADDVALUE("TrxPulse2","%i",count%100); //4,10
		ADDVALUE("EvacuatePulse1","%i",count%100); //4,10
		ADDVALUE("EvacuatePulse2","%i",count%100); //4,10
		ADDVALUE("DownloadTime","'%s %s'",date,time); //11,19
		ADDVALUE("FASStatus","%i",count%100); //-6,3
		ADDVALUE("FASTime","'%s %s'",date,time); //11,19
		ADDVALUE("CRC","%i",count%100); //4,10
		ADDVALUE("SellDODAAC","'SELL%03i'",count%1000); //12,6
		ADDVALUE("Odometer","%i",count%10000); //4,10
		ADDVALUE("UnitOfIssue1","'uoi%03i'",count%1000); //12,6
		ADDVALUE("UnitOfIssue2","'2ui%03i'",count%1000); //12,6
		ADDVALUE("Product","'prd%03i'",count%1000); //12,6
		ADDVALUE("CustDODACC","'cus%03i'",count%1000); //12,15
		ADDVALUE("UseCode","'use_%i'",count%1000); //12,15
		ADDVALUE("FundCode","'FC'",count); //12,2
		ADDVALUE("SigCode","'%c'",'A'+(count%26)); //12,1
		ADDVALUE("SuppDODAAC","'sup_%i'",count%1000); //12,15
		ADDVALUE("SubAccount","'sub_%i'",count%1000); //12,15
		ADDVALUE("MDS","''",count); //12,15
		ADDVALUE("NameRank","''",count); //12,50
		ADDVALUE("Location","''",count); //12,50
		ADDVALUE("Addr1","''",count); //12,50
		ADDVALUE("Addr2","''",count); //12,50
		ADDVALUE("Addr3","''",count); //12,50
		ADDVALUE("Phone","''",count); //12,50
		ADDVALUE("Source","%i",count%100); //-6,3
		ADDVALUE("Comment","'comment_%i'",count%1000); //12,20
		ADDVALUE("ByteReserved","%i",count%100); //4,10
		ADDVALUE("Hose","''",count); //12,4
		ADDVALUE("Misc","''",count); //12,6
		ADDVALUE("TransStatus","''",count); //12,20
		ADDVALUE("HandheldID","''",count); //12,15
		ADDVALUE("Org","''",count); //12,4
		ADDVALUE("FillstandNum","'%i'",count%10000); //12,50 (used to be 4,10)
		ADDVALUE("RTBTank","%i",count%10); //4,10
		ADDVALUE("TotQty","%i",count%100); //8,15
		ADDVALUE("TotUI","'totui_%i'",count%1000); //12,50
		ADDVALUE("AdtvID","%i",count%100); //8,15
		ADDVALUE("AdtvActPPM","%i",count%100); //8,15
		ADDVALUE("AdtvLastErr","''",count); //12,255
		ADDVALUE("AdtvReqPPM","%i",count%100); //8,15
		ADDVALUE("DpAvg","%i",count%100); //8,15
		ADDVALUE("DpMin","%i",count%100); //8,15
		ADDVALUE("DpMax","%i",count%100); //8,15
		ADDVALUE("MeterNumber1","%i",count%100); //4,10
		ADDVALUE("MeterNumber2","%i",count%100); //4,10
		ADDVALUE("RTB_Site","''",count); //12,100
		ADDVALUE("UllageUnit","''",count); //12,50
		ADDVALUE("AdtvNumErr","%i",count%100); //4,10
		ADDVALUE("DpNumErr","%i",count%100); //4,10
		ADDVALUE("DpLastErr","''",count); //12,255
		if(dbm.execute_sql_insert(&db)==FALSE){
			db.Close();return FALSE;
		}

		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
			if(getkey2()==0x1b)
				break;
		}

	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}

int getblob(CString dbname)
{
	CDatabase db;
	open_database(&db,&dbname);
	CRecordset rec(&db);
	CString SqlString;
	SqlString.Format("SELECT [SigImage] FROM [TransSignature] WHERE DocNum='RU10262001';");
	rec.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);
	while(!rec.IsEOF())
	{
		CString blobdat="";
		rec.GetFieldValue("SigImage",blobdat);
		int len;
		FILE *f;
		f=fopen("blob.bmp","wb");
		if(f!=0){
			int i;
			len=blobdat.GetLength();
			for(i=0;i<len;i+=2){
				int j,data;
				char str[20];
				sprintf(str,"%s\n",blobdat.Mid(i,2));
				j=sscanf(str,"%02X",&data);
				if(j==1)
					fwrite(&data,1,1,f);
				else
					printf("error\n");
			}
			fclose(f);
		}
		f=fopen("blob.c.txt","wb");
		if(f!=0){
			int i;
			len=blobdat.GetLength();
			for(i=0;i<len;i+=2){
				int j,data;
				char str[20];
				sprintf(str,"%s\n",blobdat.Mid(i,2));
				j=sscanf(str,"%02X",&data);
				data=data&0xFF;
				if(j==1)
					fprintf(f,"0x%02X,",data);
				else
					printf("error\n");
				if((i%48)==0 && i!=0)
					fprintf(f,"\n");
			}
			fclose(f);
		}
		break;
	}
	rec.Close();
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}

int create_bitmap(CString *data,CString docno)
{
	int i;
	char str[4];
	CString temp;
	const BYTE header[0x3E]={0x42,0x4D,0xBA,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x28,0x00,0x00,0x00,0x60,0x01,0x00,0x00,0xBD,0x00,
						0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x7C,0x20,0x00,0x00,0x74,0x12,0x00,0x00,0x74,0x12,0x00,0x00,0x00,0x00,
						0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00};
	extern BYTE charset[];

	for(i=0;i<sizeof(header);i++){
		sprintf(str,"%02X",header[i]);
		*data+=str;
	}
	int doclen=docno.GetLength();
	for(i=0;i<8316;i++){
		BYTE a;
		DWORD x,y;
		int scale=4;
		if(((i%44)/scale)>=docno.GetLength())
			a=0;
		else{
			x=docno.GetAt((i%44)/scale);
			if(x>='a' && x<='z')
				x=x-0x20;
			x=(x-0x20)&0xFF;
			if(x>64)
				x=0;
			y=i/44/(scale*6);
			y=y%8;
			a=charset[x*8+7-y];
			if(scale>1){
				int j,k,b;
				j=(i%44)%scale;
				j=(8/scale)*j;
				a=a<<j;

				b=0;
				for(j=0;j<8;j+=scale){
					int bit=a>>(7-(j/scale));
					bit&=1;
					for(k=0;k<scale;k++){
						b|=bit<<(7-j-k);
					}
				}
				a=b;
			}
		}
		a^=0xFF; //invert bits
		sprintf(str,"%02X",a);
		*data+=str;
	}
/*
	CString blobdat=*data;
		int len;
		FILE *f;
		f=fopen("blob.bmp","wb");
		if(f!=0){
			int i;
			len=blobdat.GetLength();
			for(i=0;i<len;i+=2){
				int j,data;
				char str[20];
				sprintf(str,"%s\n",blobdat.Mid(i,2));
				j=sscanf(str,"%02X",&data);
				if(j==1)
					fwrite(&data,1,1,f);
				else
					printf("error\n");
			}
			fclose(f);
		}
*/
	return 0;
}
int fill_ledger_transsig(CString dbname) //should do pdatrans first
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="TransSignature";
	int	field_count=0;

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}
	int max_count=100;
	if(get_ini_value(dbm.table_name,"max_count",&max_count))
		cout<<"max count="<<max_count<<endl;

	CRecordset rec(&db);
	CString SqlString;
	SqlString.Format("SELECT DISTINCT [DocNo] FROM [PDATrans];");
	TRY{
		if(db.IsOpen())rec.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly);
	}CATCH(CDBException, e){
		printf("PDATrans table doesnt exist apparently\n");
		db.Close();
		return 0;
	}END_CATCH

	DWORD elapsed=0;
	int count=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	while(rec.IsOpen() && !rec.IsEOF() && (count<max_count*2))
	{
		int type=0;
		CString docno;
		rec.GetFieldValue("DocNo",docno);
		for(int i=0;i<2;i++){
			CString blob;
			CString sigtype;
			if(type==0)sigtype="chief";else sigtype="buyer";
			type^=1;
			create_bitmap(&blob,docno);

			ADDVALUE("DocNum","'%s'",docno); //12,50
			ADDVALUE("SigType","'%s'",sigtype); //12,50
			ADDVALUE("SigImage","0x%s",blob); //-4,1073741823
			if(dbm.execute_sql_insert(&db)==FALSE){
				break;
			}
			count++;
		}
		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
			if(getkey2()==0x1b)
				break;
		}
		rec.MoveNext();
	}
	rec.Close();
	printf("created %i signatures\n",count);
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}
/************tim trans generator******************************************/
#define ADDTHIS(name,fmt,value) 	{if(field_counter!=0) SQLParams+=","; \
	SQLParams+="[";SQLParams+=name;SQLParams+="]"; \
	CString ___tstr; ___tstr.Format(fmt,value); \
	if(field_counter!=0) SQLValues+=","; \
	SQLValues+=___tstr; field_counter++; }


int fill_liltimtrans(CString dbname,int max_count)
{
	CDatabase db;
	CString SQLFields,SQLParams,SQLValues;
	char *table_name="transactions";
	int	field_count=0;

	//open_database(&db,&dbname,0);
	SQLFields="";
	//field_count=get_fields_array(&db,table_name,&SQLFields);


	char time[20],date[20];
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	increment_time(&systime,-24*2,0,0); //roll back few days

	DWORD elapsed=0;
	FILE *flog=0;
	flog=fopen("liltim.sql","w");

	cout<<"Writing "<< max_count << " transactions to liltim.sql\n";
	for(int count=1;count<max_count;count++)
	{
		GetTimeFormat(LOCALE_USER_DEFAULT,0,&systime,"hh':'mm':'ss tt",time,sizeof(time));
		GetDateFormat(LOCALE_USER_DEFAULT,0,&systime,"M'/'d'/'yyyy",date,sizeof(date));
		increment_time(&systime,0,0,1);

		int field_counter=0;
		SQLParams="";SQLValues="";

//		ADDTHIS("address1","'%i'",count); //12,255
//		ADDTHIS("address2","'%i'",count); //12,255
//		ADDTHIS("address3","'%i'",count); //12,255
//		ADDTHIS("adtv_act_ppm","'%i'",count); //12,255
//		ADDTHIS("adtv_id","'%i'",count); //12,255
//		ADDTHIS("adtv_last_err","'%i'",count); //12,255
//		ADDTHIS("adtv_m","%i",count); //4,10
//		ADDTHIS("adtv_num_err","'%i'",count); //12,255
//		ADDTHIS("adtv_req_ppm","'%i'",count); //12,255
		ADDTHIS("allow_download","1",count); //4,10
//		ADDTHIS("card_number","'CC%i'",count); //12,255
//		ADDTHIS("cic","'%i'",count); //12,255
//		ADDTHIS("cust_dodaac","'dodac%i'",count); //12,255
		ADDTHIS("device_id","'test'",count); //12,255
		ADDTHIS("divide_rate1","'20/1'",count); //12,255
//		ADDTHIS("divide_rate2","'%i'",count); //12,255
		ADDTHIS("document_number","'RU%08i'",11012600+count); //12,255
		ADDTHIS("downloaded","0",count); //4,10
//		ADDTHIS("dp_avg","'%i'",count); //12,255
//		ADDTHIS("dp_last_err","'%i'",count); //12,255
//		ADDTHIS("dp_max","'%i'",count); //12,255
//		ADDTHIS("dp_min","'%i'",count); //12,255
//		ADDTHIS("dp_num_err","'%i'",count); //12,255
		ADDTHIS("endcode","%i",5); //4,10
		ADDTHIS("end_time","'%i'",151200+count); //12,255
		ADDTHIS("evac_pulses1","%i",count%100); //4,10
		ADDTHIS("evac_pulses2","%i",count%100); //4,10
//		ADDTHIS("exp_date","'%i'",count); //12,255
//		ADDTHIS("fillstand_number","'%i'",count); //12,255
		ADDTHIS("fueling_type","'%i'",1); //12,255
//		ADDTHIS("fund_code","'%i'",count); //12,255
		ADDTHIS("grade_code","'JP8'",count); //12,255
		ADDTHIS("handheld_id","%i",count%5); //4,10
		ADDTHIS("meter1","1",count); //4,10
//		ADDTHIS("meter2","%i",count); //4,10
//		ADDTHIS("fp_number","'%i'",count); //12,255
		ADDTHIS("location","'DOD'",count); //12,255
//		ADDTHIS("mds","'%i'",count); //12,255
//		ADDTHIS("name_rank","'%i'",count); //12,255
//		ADDTHIS("odometer","%i",count); //4,10
//		ADDTHIS("phone","'%i'",count); //12,255
		ADDTHIS("product","'9130010315816'",count); //12,255
		ADDTHIS("pulses1","%i",count%100); //4,10
		ADDTHIS("pulses2","%i",0); //4,10
		ADDTHIS("quantity_adjusted1","%i",(count+3)%999); //4,10
		ADDTHIS("quantity_adjusted2","%i",0); //4,10
		ADDTHIS("quantity_unadjusted1","%i",count%999); //4,10
		ADDTHIS("quantity_unadjusted2","%i",0); //4,10
//		ADDTHIS("rtb_tank","'%i'",count); //12,255
		ADDTHIS("seller_dodaac","'SELLER'",count); //12,255
//		ADDTHIS("sig_code","'%i'",count); //12,255
		ADDTHIS("start_date","'%04i2011'",count); //12,255
		ADDTHIS("start_time","'%06i'",count); //12,255
//		ADDTHIS("subaccount","'%i'",count); //12,255
//		ADDTHIS("supp_dodaac","'%i'",count); //12,255
		ADDTHIS("tailnumber","'RUtail'",count); //12,255
		ADDTHIS("temperature1","%i",count%999); //4,10
		ADDTHIS("temperature2","%i",count); //4,10
		ADDTHIS("temp_factor1","%i",count); //4,10
		ADDTHIS("temp_factor2","%i",count); //4,10
		ADDTHIS("totalizer1","%i",count%999); //4,10
		ADDTHIS("totalizer2","%i",0); //4,10
		ADDTHIS("total_qty","%i",count%999); //4,10
		ADDTHIS("total_uoi","'0:1:GL'",count); //12,255
		ADDTHIS("trans_code","5",count); //4,10
		ADDTHIS("trans_flag","0",count); //4,10
		ADDTHIS("trx_ID","%i",count); //4,10
		ADDTHIS("ullage","%i",count); //4,10
		ADDTHIS("ullage_uoi","'0:1:GL'",count); //12,255
		ADDTHIS("unit_of_issue1","'0:1:GL'",count); //12,255
//		ADDTHIS("unit_of_issue2","'%i'",count); //12,255
//		ADDTHIS("use_code","'%i'",count); //12,255

		CString SqlString;
		SqlString.Format("INSERT INTO [%s] (%s) VALUES(%s);\n",table_name,SQLParams,SQLValues);
		fputs(SqlString,flog);

		if(GetTickCount() > (elapsed+250)){
			printf("%i       \r",count);
			elapsed=GetTickCount();
			if(getkey2()==0x1b)
				break;
		}

	}
	if(flog!=0)fclose(flog);
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}