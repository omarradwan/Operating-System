int checkview(char*,char* );
int checkcopy(char*,char*);
int checkexecute(char*,char*);
int checkdelete(char*,char*);
int checkcreate(char*,char*);
int mod(int , int );
int div(int , int );
int readFileandsec(char* );
int fileFound(char* );
int main()
{
	while(1){
int check;	
char* command;
char firstfile[500];/*m3rfsh lsa max size of file */
char secondfile[500];
int i =0 ;
int k= 5;
char prompt[7];
prompt[0]='s';
prompt[1]='h';
prompt[2]='e';
prompt[3]='l';
prompt[4]='l';
prompt[5]='>';
prompt[6]='\0';


interrupt(0x21, 0, prompt,0, 0); 
interrupt(0x21, 1, command,0, 0);

check=checkview(command,"view\0");
if(check==1){
	int found;
	char buffer2[512];
	char buffer[13312];
	int i=5;
	int k=0;

    while(command[i]!=0xd){
    	buffer2[k]=command[i];
    	      k++;
    	      i++;
    	 }
    	 buffer2[k]='\0';
    	found=fileFound(buffer2);
    	 if(found==1){
	interrupt(0x21, 3, buffer2, buffer, 0); /*read the file into buffer*/
interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
    	interrupt(0x21, 5,0, 0, 0);
    }
    else {
    	interrupt(0x21, 0,"file is not found \0", 0, 0);
    	interrupt(0x21, 5,0, 0, 0);
    }
}

check=checkexecute(command,"execute tstpr2\0");
if(check==2){
	interrupt(0x21,4,"tstpr2\0",0x2000,0);
}

check=checkdelete(command,"delete\0");/*lsa m3mltsh handle l lw view deleted file*/

if(check==3){
		char buffer2[512];
		int i=7;
	int k=0;
		int found;
		 while(command[i]!=0xd){
    	buffer2[k]=command[i];
    	      k++;
    	      i++;
    	 }
    	 found=fileFound(buffer2);
    	 buffer2[k]='\0';
    	 if(found==1){
	interrupt(0x21,7,buffer2,0x2000,0);
	interrupt(0x21, 5,0, 0, 0);
}
else {
	interrupt(0x21, 0,"file is not found \0", 0, 0);
    	interrupt(0x21, 5,0, 0, 0);
	}
}




check=checkcopy(command,"copy\0");
if (check==4){
	int secs=0;
	char directory[512];
	char buffer[13312];
	char buffer1[13312];
		int first=0;
  	int second =0;
	interrupt(0x21,2,directory,2, 0);
    while(command[k]!=' '){
    	firstfile[i]=command[k];
    	k++;
    	i++;
    }
   firstfile[k]='\0';
    secs=readFileandsec(firstfile);
    first=div(secs,10);
                  second=mod(secs,10);

            interrupt(0x10, 0xE*256+(first+48), 0, 0, 0);
              interrupt(0x10, 0xE*256+(second+48), 0, 0, 0);
    k++;
    i=0;
    while(command[k]!=0xd){
            secondfile[i]=command[k];
            i++;
            k++;
    }
    secondfile[k]='\0';
	interrupt(0x21, 3,firstfile, buffer, 0);
     interrupt(0x21, 8,secondfile, buffer, secs);
     interrupt(0x21, 5,0,0,0);
      
}




check=checkdir(command,"dir\0");
if(check==1){
int i=0;
  	int j=0;
  	int k=0;
  	int pos=0;
  	int numsec=0;
  	int first=0;
  	int second =0;
  	char c;
  	char directory[512];
  	char fileName[13312];
  	interrupt(0x21,2,directory,2, 0);
  	while(i<512){
  		if(directory[i]!=0x00){
  		for(k=0;k<6;k++){
  			if(directory[i+k]!=0x00){
  interrupt(0x10, 0xE*256+directory[i+k], 0, 0, 0);
  		              
           }
           else{
           	break;
           }
  		}
  		interrupt(0x10, 0xE*256+' ', 0, 0, 0);
  				for(j=6;j<32;j++){
  			if(directory[i+j] != 0x00){
                 numsec++;
  			}
  			else{
                  first=div(numsec,10);
                  second=mod(numsec,10);

            interrupt(0x10, 0xE*256+(first+48), 0, 0, 0);
              interrupt(0x10, 0xE*256+(second+48), 0, 0, 0);
  				break;
  			}
  			}
  			interrupt(0x10, 0xE*256+' ', 0, 0, 0);
  	}
  	i=i+32;
  	numsec=0;
  	}

  	  interrupt(0x21, 5,0,0,0);
     

}

check=checkcreate(command,"create\0");
  if(check==6){
int length=0;
int i=0;   
int k=0;
int noOfSectors=0;
char buffer[6];
char buffer1[13312];
char * command2 ;
command=command+7;
while(*command!=0xd){
buffer[k]=*command;
command++;
k++;
}
 interrupt(0x21, 0, prompt,0, 0); 
interrupt(0x21, 1, command2,0, 0);
while(command2[i]!=0xd){
   while(command2[i]!=0xd){
     buffer1[length]=command2[i];
     i++;
     length++;
   }
  i=0;
  interrupt(0x21, 0, prompt,0, 0); 
  interrupt(0x21, 1, command2,0, 0);

  }
noOfSectors=div(length,512);
if (noOfSectors==0){
	noOfSectors=1;
}
interrupt(0x21,8,buffer,buffer1,noOfSectors);

interrupt(0x21, 5,0,0,0);

  }

if(check==0){
	interrupt(0x21, 0,"bad command\0", 0, 0);
	interrupt(0x21, 5,0,0,0);


}

}

}








int checkview(char* c,char* s){
	int i=0;
	while (i<4){
         if(c[i]!=s[i]){
          return 0;
         }
         i++;
         }
         return 1;	
	

}
int checkexecute(char* c,char* s){
	int i=0;
	while (s[i]!='\0'){
         if(c[i]!=s[i]){
          return 0;
         }
         i++;
         }
         return 2;	
	

}

int checkdelete(char* c,char* s){
	int i=0;
	while (s[i]!='\0'){
         if(c[i]!=s[i]){
          return 0;
         }
         i++;
         }
         return 3;	
	

}
int checkcopy(char* c,char* s){
	int i=0;
	while (i<4){
         if(c[i]!=s[i]){
          return 0;
         }
         i++;
         }
         return 4;	

 }
int checkdir(char* c,char* s){
	int i=0;
	while (i<3){
         if(c[i]!=s[i]){
          return 0;
         }
         i++;
         }
         return 1;	
	

}


int checkcreate(char* c,char* s){
	int i=0;
	while (i<6){
         if(c[i]!=s[i]){
          return 0;
         }
         i++;
         }
         return 6 ;	
	

}


int mod(int a ,int b){
int count =0;
if (a >= b)
while(a >= b){
	a = a-b;
	
}

return a;

}

int div(int a ,int b){
int count =0;
if (a >= b)
while(a >= b){
	a = a-b;
	count ++;
}


return count;

}
int readFileandsec(char* name){
  	int i=0;
  	int j=0;
  	int k=0;
  	int test=1;
  	char directory[512];
  	char fileName[6];
  	char* base = name;
  	int fileAddress = 0;
  		interrupt(0x21,2,directory,2, 0);
  	for(i=0;i<512;i++){
  		for(j=0;j<6;j++){
  			if(directory[i+j] != name[j]){
  					test = 0;
  					break;
  			}

  		}

  		if(test == 0){
  			//printString("3w\0");
  			i = i+31;
        test =1;

  		}

  		else{

  			i+=6;
  			break;
  		}
  	}

  	if(test == 1){
  		  		for(k=0;k<26;k++){
  			if(directory[i] != 0x00){	
          i++;

  			}
  			else{

  				return k;
  			}
  			}
  		}
  		return 0;
  	}

  	  	int fileFound(char* file){
    int i =0;
    int j = 0 ;
    int test =1 ;
    int count =6;
    char directory[512];
   interrupt(0x21,2,directory,2, 0);
    while(i<512){

      for (j = 0; j < count; ++j)
      {
        if(directory[i+j] != file[j]){
            test = 0;
            break;
        }


      }
      if(test == 0 && i < 479){
        //printString("3w\0");
        i = i+32;
        test = 1;
      }
      else{

        i+=6;
        break;
      }


    }
    if (test == 1)
    return 1 ;
    else
      return 0 ;


}