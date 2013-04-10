BOOL SiteSignatureDecode( unsigned int *ss, char *key, BOOL bSigned, short *iUserCheck ) {

   BYTE c1, c2, c4, c5;
   BYTE a,b,c,d;
   BOOL rv = TRUE;
   WORD c3, m;
   char t1[20];
   char t2[10];
   
   memset (t1, 0x00, sizeof (t1));
   if(bSigned)
      memcpy (t1, key, 12);
   else
   {
      memcpy (t1, key, 3);
      memcpy (&(t1[3]), &(key[4]), 6);
      memcpy (&(t1[9]), &(key[11]), 3);
   }
   memset (t2, 0x00, sizeof (t2));
   memcpy (t2, t1, 2);
   c1 = (BYTE) atoi (t2);
   
   memset (t2, 0x00, sizeof (t2));
   memcpy (t2, &(t1[2]), 2);
   c2 = (BYTE) atoi (t2);
   
   memset (t2, 0x00, sizeof (t2));
   memcpy (t2, &(t1[4]), 4);
   c3 = (WORD) atoi (t2);
   
   memset (t2, 0x00, sizeof (t2));
   memcpy (t2, &(t1[8]), 2);
   c4 = (BYTE) atoi (t2);
   
   memset (t2, 0x00, sizeof (t2));
   memcpy (t2, &(t1[10]), 2);
   c5 = (BYTE) atoi (t2);
   
   d = c4;
   c = c5 - d;
   b = c2 - c;
   a = c1 - b;
   
   m = (WORD) ((a+1)*(b+1)*(c+1)*(d+1) % 10000);
   
   if (c3 == 0)
      rv = FALSE;
   
   if (rv && (a > 15 || b > 15 || c > 15 || d > 15))
      rv = FALSE;
   
   if (rv && m == c3)
   {
      // 10/28/98 QZ Add for Multi-User Check
      *iUserCheck = ((a+1)*(b+1)*(c+1)*(d+1) + 12) % 100;
      *ss = (unsigned int) ((a << 12) + (b << 8) + (c << 4) + d);
      *ss = *ss ^ 0xD7A5;
   }
   else
      rv = FALSE;
   return (rv);
}
//hex to triple group
void EncodeSiteSignKey(unsigned long ss, CString& strKey)
{
   BYTE c1, c2, c4, c5;
   WORD c3;
   BYTE a, b, c, d;

   ss = ss ^ 0xD7A5;

   a = (BYTE) ((ss & 0xF000) >> 12);
   b = (BYTE) ((ss & 0x0F00) >> 8);
   c = (BYTE) ((ss & 0x00F0) >> 4);
   d = (BYTE) ((ss & 0x000F));

   c1 = a + b;
   c2 = b + c;
   c3 = (WORD) ((a+1)*(b+1)*(c+1)*(d+1) % 10000);
   c4 = d;
   c5 = c + d;

   CString strTemp;
   strTemp.Format("%2.2d%2.2d%4.4d%2.2d%2.2d", c1,c2,c3,c4,c5);
   strKey.Format("%s-%s-%s", strTemp.Left(3), strTemp.Mid(3, 6),
      strTemp.Right(3));
}

BOOL GetSiteSignFromString( const CString& StrSign, unsigned int& lSiteSign ) {
    short iUserCheck;// <- dummy
    char szTemp[21];
  	strcpy(szTemp, StrSign.Mid(18, 20));
    _int64 lTemp = _atoi64(szTemp);
    _i64toa((lTemp / 17 - 491), szTemp, 10);
    
    if(lTemp == 0 || lTemp % 17 != 0 ||
        !SiteSignatureDecode(&lSiteSign, szTemp,
            TRUE, &iUserCheck))
    {
       OutputDebugString( "Site Signature Key is Invalid!" );
       return FALSE;

    }
    return TRUE;
}

void EncodeSiteSig(DWORD sitesig, CString *str)
{
	CString m_strSign;

	EncodeSiteSignKey(sitesig,m_strSign);

	_int64 iSign = _atoi64(m_strSign.Left(3) + m_strSign.Mid(4,6) +	m_strSign.Right(3));
	char szSecret[20];
	_i64toa((iSign + 491) * 17, szSecret, 10);
	
	CString sitesign = "a3@!%(*D_f4)2&^#j1";
	sitesign+=szSecret;
	*str=sitesign;
}

/*
combos: VMN-VEHICLE KEY
		COM-VEH,USER,EITHER,BOTH KEY
		VVI-USER KEY
		OTR-VEHICLE KEY
		KEYLESS-VEHICLE KEY
*/
char *system_types[]={"VMN","KEYLESS","OTR","COM","VVI",0};
char *system_key_types[]={"VEHICLE KEY","USER KEY","EITHER KEY","BOTH KEY",0};

int fill_config(CString dbname)
{
	CDatabase db;
	DBMaker dbm;
	dbm.table_name="Config";

	if(!check_ini_file(dbm.table_name)) 
		return FALSE;
	open_database(&db,&dbname);
	dbm.get_fields_array(&db);

	if(!delete_table_records(&db,dbm.table_name,"")){
		db.Close();return FALSE;
	}

	int site_sig=0xFFFE;
	get_ini_value(dbm.table_name,"ssig",&site_sig);
	printf("using site sig:0x%04X\n",site_sig);

	CString systemtype="VMN";
	if(get_ini_str(dbm.table_name,"systemtype",&systemtype))
		cout<<"systemtype:"<<(LPCTSTR)systemtype<<endl;
	CString syskeyinfo="VEHICLE";
	if(get_ini_str(dbm.table_name,"syskeyinfo",&syskeyinfo))
		cout<<"syskeyinfo:"<<(LPCTSTR)syskeyinfo<<endl;

	int config_number=0;
	get_ini_value(dbm.table_name,"config_number",&config_number);
	cout<<"config_number:"<<config_number<<endl;

	int count=0;

	cout<<"Writing to "<< dbm.table_name << " Table\n";
	
	ADDVALUE("MISC1LABEL","'mlabel1'",count); //12,15
	ADDVALUE("MISC2LABEL","'mlabel2'",count); //12,15
	ADDVALUE("MISC3LABEL","'mlabel3'",count); //12,15
	ADDVALUE("MISC4LABEL","'mlabel4'",count); //12,15
	ADDVALUE("MISC5LABEL","'mlabel5'",count); //12,15
	ADDVALUE("VEHI_KO_ODOM","0",count); //-7,1
	ADDVALUE("VEHI_KO_PIN","0",count); //-7,1
	ADDVALUE("VEHI_KO_USERID","-1",count); //-7,1
	ADDVALUE("VEHI_KO_RECEIPT","0",count); //-7,1
	ADDVALUE("USER_KO_ODOM","0",count); //-7,1
	ADDVALUE("USER_KO_PIN","0",count); //-7,1
	ADDVALUE("USER_KO_VEHIID","0",count); //-7,1
	ADDVALUE("USER_KO_RECEIPT","0",count); //-7,1
	ADDVALUE("REQUIRECUSTID","-1",count); //-7,1
	ADDVALUE("ALLOWEXPORT","-1",count); //-7,1
	ADDVALUE("USEMANUALISSUE","0",count); //-7,1
	ADDVALUE("NAME","'name_%04i'",config_number); //12,40
	ADDVALUE("ADDRESS1","'addr1_%04i'",config_number); //12,40
	ADDVALUE("ADDRESS2","'addr2_%04i'",config_number); //12,40
	ADDVALUE("CITYSTATEZIP","'city_state_zip_%05i'",config_number); //12,40
	ADDVALUE("VEHIINTKEY","'%09i'",1); //12,9
	ADDVALUE("USERINTKEY","'%09i'",1); //12,9
	ADDVALUE("SYSTEMTYPE","'%s'",systemtype); //12,16
	ADDVALUE("MAXVMNVEHIKEYS","'65000'",count); //4,10
	ADDVALUE("MAXCOMVEHIKEYS","'65000'",count); //4,10
	ADDVALUE("MAXVVIUSERKEYS","'65000'",count); //4,10
	ADDVALUE("MAXCOMUSERKEYS","'65000'",count); //4,10
	ADDVALUE("MAXVVIVEHIIDS","'20000'",count); //4,10
	ADDVALUE("MAXVMNUSERIDS","'8000'",count); //4,10
	ADDVALUE("SYSKEYINFO","'%s KEY'",syskeyinfo); //12,15
	ADDVALUE("LASTONLINE","'0'",count); //5,5
	{
		CString sitestr;
		EncodeSiteSig((DWORD)site_sig,&sitestr);

	//ADDVALUE("SITESIGN","'%s'","a3@!%(*D_f4)2&^#j11722430507319"); //12,50
	ADDVALUE("SITESIGN","'%s'",sitestr); //12,50
	}
	ADDVALUE("SYSPASSWD","'STS'",count); //12,50
	ADDVALUE("SYSSIGN","''",count); //12,50
	ADDVALUE("TYPEPASSWD","'STS'",count); //12,50
	ADDVALUE("POLLOPT","'1011111111111111111111111'",count); //12,30
	ADDVALUE("POLLFILETYPE","'1'",count); //5,5
	ADDVALUE("FMTYPE","'AMII'",count); //12,4
	ADDVALUE("AIMKEYNO","'0'",count); //12,9
	ADDVALUE("AIM_KO_ODOM","0",count); //-7,1
	ADDVALUE("AIM_KO_RECEIPT","0",count); //-7,1
	ADDVALUE("AIM_KO_USERID","0",count); //-7,1
	ADDVALUE("KO_FREE1","0",count); //-7,1
	ADDVALUE("KO_FREE2","0",count); //-7,1
	ADDVALUE("KO_FREE3","0",count); //-7,1
	ADDVALUE("FREE1","''",count); //12,15
	ADDVALUE("FREE2","''",count); //12,15
	ADDVALUE("FREE3","''",count); //12,15
	ADDVALUE("MAXDCID","'300'",count); //4,10
	ADDVALUE("MAXPPCID","'300'",count); //4,10
	ADDVALUE("PPCINTKEY","'0'",count); //4,10
	ADDVALUE("MAXPPCINTKEY","'65000'",count); //4,10
	ADDVALUE("7DIGITODOM","0",count); //-7,1
	ADDVALUE("CREDITCARD","-1",count); //-7,1
	ADDVALUE("SYSCATEGORY","''",count); //12,50
	ADDVALUE("KEYPADENABLE","0",count); //-7,1
	ADDVALUE("TMUDELIVERY","0",count); //-7,1
	ADDVALUE("SITESPECIFIC","0",count); //-7,1
	ADDVALUE("AUTHLISTOPT","'111111111111'",count); //12,30
	ADDVALUE("AIM2LISTOPT","'11'",count); //12,10
	ADDVALUE("PADSSSKIPFLAG","0",count); //-7,1
	ADDVALUE("PATCHLISTTIMEOUT","'100'",count); //5,5

	if(dbm.execute_sql_insert(&db)==FALSE){
		db.Close();return FALSE;
	}
	db.Close();
	cout<<"done\n---\n";
	return TRUE;
}
