#include "blackdos.h"

int stringCompare(char*, char*, int);
void printDir();
int isChar(char* test);
int isCapital(char* ch);
int getSpaceIndex(char* string);
int getLength(char* string);
void copyFile(char* fileName1, char* fileName2);


void main(){
   char buffer[13312];
   char input[80];
   char content[140];
   char arg1[7];
   char arg2[7];
   int i, j, index, length;
   interrupt(33,0,"Welcome to the Shell!\r\n\r\n\0", 0, 0);

   while(1){
     PRINTS("blackdos ~(__^>");
     interrupt(33, 1, input, 0, 0);
     PRINTS("\n");
     if(stringCompare(input, "boot", 4))
     {
	    interrupt(33, 11, 0, 0, 0);
     }
     else if(stringCompare(input, "cls", 3))
     {
        interrupt(33,12,5,16,0);
     }
     else if(stringCompare(input, "dir", 3))
     {
	    printDir();
     }
     else if(stringCompare(input, "echo", 4))
     {
	    PRINTS(input + 5);
	    PRINTS("\r\n\0");
     }
     else if(stringCompare(input, "copy", 4))
     {
	    index = getSpaceIndex(input) + 1;
	    length = getLength(input);

	    if (isCapital(input[5] || isCapital(input[index])))
	    {
           interrupt(33, 15, 1, 0, 0);
		   break;
	    }
	    for (i = 5; i < index - 1; i++)
	    {
		   arg1[i - 5] = input[i];
	    }
	    for (i = index; i < length + 1; i++)
	    {
           arg2[i - index] = input[i];
	    }
	    arg1[6] = '\0';
	    arg2[6] = '\0';
       
        copyFile(arg1, arg2);
     }
     else if(stringCompare(input, "del", 3))
     {
	    if (isCapital(input[4]))
	    {
	   	   interrupt(33, 15, 1, 0, 0);
		   break;
	    }
	    interrupt(33, 7, input + 4, 0, 0);
	    PRINTS("\n");
     }
     else if (stringCompare(input, "run", 3))
     {
        interrupt(33, 4, input + 4, 4, 0);
     }
     else if (stringCompare(input, "help", 4))
     {
	    PRINTS("User manual.\r\n\0");
	    PRINTS("Enter commands to be executed:\r\n\0");
	    PRINTS("boot: reboot the system.\r\n\0");
     	PRINTS("cls: clear the screen.\r\n\0");
	    PRINTS("copy file1 file2: Create file2 and copy file1 into file2 w/o deleting file1.\r\n\0");
	    PRINTS("del filename: delete filename.\r\n\0");
	    PRINTS("dir: print out directory contents.\r\n\0");
	    PRINTS("echo comment: display comment on the screen.\r\n\0");
	    PRINTS("helo: display the user manual.\r\n\0");
	    PRINTS("run filename: execute filename.\r\n\0");
	    PRINTS("tweet filename: creares new file called filename and stores the user given contents.\r\n\0");
	    PRINTS("type filename: prints the contents of filename to the screen.\r\n\0");
     }
     else if (stringCompare(input, "tweet", 5))
     {
        if (isCapital(input[6]))
	    {
           interrupt(33, 15, 1, 0, 0);
	       break;
	    }
	    PRINTS("Enter your tweet: ");
	    SCANS(content);
	    interrupt(33, 8, input + 6, content, 1);
	    PRINTS("\n");
     }
     else if (stringCompare(input, "type", 4))
     {
	    interrupt(33, 3, input + 5, buffer, 0);
	    PRINTS(buffer);
	    PRINTS("\r\n\0");
     }
   }
} 

int stringCompare(char* indexed, char* fileName, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		if (indexed[i] != fileName[i])
		{
			return 0;
		}
	}
	return 1;
}

int getSpaceIndex(char* string)
{
	int i = 5;
	while (string[i] != ' ')
	{
		i++;
	}
	return i;
}

int getLength(char* string)
{
	int i = 0;
	while (string[i] != 0)
	{
		i++;
	}
	return i;
}

int isChar(char* test)
{
	if ((test >= 32) && (test <= 126))
	{
		return 1;
	}
	return 0;
}

int isCapital(char* ch)
{
	if (ch >= 'A' && ch <= 'Z')
	{
		return 1;
	}
	else return 0;
}

void copyFile(char* fileName1, char* fileName2)
{
	char buffer[13312];
	char directory[512];
	int i, j, k, filestart, fileend, numOfSectors = 0;
	char currentFile[6];

	interrupt(33, 2, directory, 2, 0);

	for (i = 0; i < 16; i++)
	{
		filestart = i * 32;
		fileend = filestart + 6;
		for (j = 0; j < 6; j++)
		{
			currentFile[j] = directory[filestart + j];
		}
		if (stringCompare(currentFile, fileName1, 6))
		{
			for (k = fileend; directory[k] != 0x00; k++)
			{
				numOfSectors++;
			}
		}
	}
	interrupt(33, 3, fileName1, buffer);
	interrupt(33, 8, fileName2, buffer, numOfSectors);
}

void printDir()
{
   char buffer[512];
   char fname[512];
   int index = 0;
   int i;
   int runs = 0;
   int size = 0;
   int total = 0;

   READS(buffer,2);
   while(buffer[index] != 0x0)
   {
      for (i = 0; i < 6; ++i)
      {
         fname[i] = buffer[index + i];
      }
      for (i = 6; i<32; ++i)
      {
         if (buffer[index + i] == 0x0)
         {
            break;
         }
         else
         {
            ++size;
         }
      }
      total = total + size;
      if(isCapital(fname[0]))
      {
      }
      else
      {
         PRINTS("File name: \0");
         PRINTS(fname);
         PRINTS("        File size: \0");
         PRINTN(size);
         PRINTS("\r\n\0");
      }
      size = 0;
      runs = runs + 6;
      index = index + 32;
   }
   PRINTS("\r\nTotal Sectors used: \0");
   PRINTN(total);
   PRINTS("\r\nSectors free: \0");
   PRINTN(48-total);
   PRINTS("\r\n\0");
} 
