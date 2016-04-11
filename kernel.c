

void printString(char*);
void readAndPrint();
void readString(char*);
void handleInterrupt21(int , int , int , int);
void readSector(char* , int);
int mod(int  ,int );
int div(int ,int );
void readFile(char*,char*);
void executeProgram (char* , int);
void terminateProgram();
void writeSector(char* , int);
void deleteFile(char*);
void writeFile(char* , char* , int);
int main()
 {	
	makeInterrupt21();
	interrupt(0x21,4,"shell\0",0x2000,0);
      
}




void printString(char* chars)
{
	int i=0;
	while(chars[i]!='\0')
	{
		char c = chars[i];
		interrupt(0x10, 0xE*256+c, 0, 0, 0);
		i++;
	}
}



void readAndPrint()
{
	while(1)
	{
		char x = interrupt(0x16, 0, 0, 0, 0);
		interrupt(0x10, 0xE*256+x, 0, 0, 0);	
	}
}



void readString(char* array )
{
	char* tmp = array;	
	int i = 0;
	char x = 'm';
	while(1)
	{
		if(x == 0xd)
			break;

		x = interrupt(0x16,0, 0, 0,0);
		interrupt(0x10, 0xE*256+x, 0, 0, 0);
		*array = x;
		 if(x == 0x8 && i!=0)
		 {
		 	i-=2;
		 	array-=2;
			interrupt(0x10, 0xE*256+0, 0, 0, 0);
		 	interrupt(0x10, 0xE*256+x, 0, 0, 0);

		 }
	    i++;
	    array++;
	}
	*array = 0xa;
	array++;
	*array = 0x0;
	//printString("\n\0");
	//printString(tmp);
}

void readSector(char* buffer, int sector){

	int  CL=mod (sector ,18 )+1;
	 int x=div(sector,18);

	int  DH =mod(x,2);

	int CH=div(sector,36);

	int CX=CH*256+CL;
	int DX=DH*256+0;
	int AX= 2*256 + 1;
	interrupt(0x13, AX, buffer,CX, DX);


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

int mod(int a ,int b){
int count =0;
if (a >= b)
while(a >= b){
	a = a-b;
	
}

return a;

}

/*void handleInterrupt21(int ax, int bx, int cx, int dx){
	printString("Hello mora\0");
}*/

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	if(ax == 0)
		printString(bx);
	else if(ax == 1)
		readString(bx);
	else if(ax == 2)
		readSector(bx, cx);
	else if(ax == 3)
		readFile(bx,cx);
	else if(ax == 4)
		executeProgram(bx, cx);
	else if(ax == 5)
 		terminateProgram();
 	else if (ax == 6)
		writeSector(bx,cx);
	else if (ax == 7)
		deleteFile(bx);
	else if(ax  == 8)
 		writeFile(bx,cx,dx);
	else
		printString("error\0");
}

	
void readFile(char* name, char* file){
  	int i=0;
  	int j=0;
  	int k=0;
  	int test=1;
  	char directory[512];
  	char fileName[6];
  	char* base = name;
  	int fileAddress = 0;
  	readSector(directory,2);
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
  				readSector(fileAddress+file,directory[i]);
  				fileAddress = fileAddress+512;
          i++;

  			}
  			else{
  				break;
  			}
  			}
  		
  	}
  	
  }
  	
	void executeProgram (char* c , int seg)
	{
		char buffer[13312];
		int i = 0;
		readFile(c,buffer);
		while(i < 13312)
		{
			putInMemory(seg , i , buffer[i]);
			i++;
		}
		launchProgram(seg);

	}

	void terminateProgram()
	{
   char shell[6];
    shell[0]='s';
    shell[1]='h';
    shell[2]='e';
    shell[3]='l';
    shell[4]='l';
    shell[5]='\0';
   interrupt(0x21,4,shell,0x2000,0);
    while(1);
	}
	void writeSector(char* buffer, int sector)
	{

		int CL = mod (sector ,18 )+1;
		int x = div(sector,18);
		int DH = mod(x,2);
		int CH = div(sector,36);
		int CX = CH * 256 + CL;
		int DX = DH * 256 + 0;
		int AX = 3*256 + 1;
		interrupt(0x13, AX, buffer,CX, DX);
	}

	void deleteFile(char* c)
	{
		char map[512];
		char dir[512];
		int i = 0;
		char* base = c;
		int flag = 1;
		readSector(map , 1);
		readSector(dir , 2);
		
		while(i < 512)
		{
			int j = i;
			c = base;
			flag = 1;
			while(*c != 0x0)
			{
				if(*c != dir[j])
				{
					flag = 0;
					break;
				}
				j++;
				c++;
			}

			if(flag == 1)
				break;	

			i+=32;		
		}

		dir[i] = 0x00;
		i+=6;

		while(dir[i] != 0x00)
		{
			map[dir[i]] = 0x00;
			i++;
		}
      
		writeSector(map , 1);
		writeSector(dir , 2);


	}



void writeFile(char* name, char* buffer, int secNum)
	{

				int i = 0;
				char map[512];
				char directory[512];
				int foundDirectory = 0;
				int currentDirectoryIndex = 0;
				int currentSectorIndex = 0;
				int j = 0 ;
				int foundSector = 0 ;
				readSector(map,1);
				readSector(directory,2);
				while(i < 512){
					if(directory[i] == 0x00){
						foundDirectory = 1;
						currentDirectoryIndex = i;
						break;
					}

					i = i+32 ;

				}

				if(foundDirectory == 0){
					printString("no free directory found ");
					return ;

				}
					for (i = 0; i < 6; ++i)
					{	if( *name != 0x00){
						directory[currentDirectoryIndex] =  *name ;
						name++;
					}
						else
							directory[currentDirectoryIndex] =  0x00 ;

							currentDirectoryIndex++ ;
							

					}

					for ( i = 0; i < secNum; ++i)
					{
						/* code */
					
					while(j < 512){

						if (map[j] == 0x00)
						{
							foundSector = 1 ;
							currentSectorIndex = j ;
							map[currentSectorIndex] = 0xFF ;
							 j++;

							break ;
						}
							j++;

						 	}

					if (foundSector == 0 )
					{
						printString("no free sectors found !");
						return ;
					}

					directory[currentDirectoryIndex] = currentSectorIndex;
					writeSector(buffer , currentSectorIndex);
					buffer += 512 ;
					foundSector = 0 ;
					//currentSectorIndex++ ;
					currentDirectoryIndex++ ;

				}	


				while(mod(currentDirectoryIndex,32)){
						directory[currentDirectoryIndex] = 0x00;
						currentDirectoryIndex++;
					}
					
					
				

				writeSector(map ,1);
				writeSector(directory, 2);
		}
