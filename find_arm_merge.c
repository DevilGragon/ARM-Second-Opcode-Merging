#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void main(int argc,char *argv[])
{
	FILE* file1;
	FILE* file2;
	char* file;
	int len;
	int enter_count=0;
	int count,find_keyword_sub,find_keyword_ldr,temp1,temp2,restore,i,j,k,p,q;
	int m=0;
	int space;
	char origin[3]={'0'};
	int last_keyword_ldr=0;
	int flag=0;
	bool mark=0;
	bool space_flag=0;
	char subarray[20]={'0'};
	char ldrarray[20]={'0'};
	char temparray[20]={'0'};
	char tempA[3]={'0'};
	char tempB[3]={'0'};
	//char last_tempB[3]={'0'};


	file1=fopen(argv[1],"r");
	{
		if(file1==NULL)
		{
			perror("argv[1]");
			exit(1);
		}
	}
	file2=fopen("fix_output.txt","w");
	{
		if(file2==NULL)
		{
			perror("fix_output.txt");
			exit(2);
		}
	}

	fseek(file1,0,SEEK_END);
	len=ftell(file1);
	find_keyword_ldr=0;

	file=(char *)malloc(len*sizeof(char));
  rewind(file1);
  fread(file,sizeof(char),len,file1);

	if(file==NULL)
	{
		perror("Out Of Memory");
		exit(3);
	}
	
	for(count=0;count<=len;count++)
	{
	loop:	if(flag==0)
		{
			if(file[count]=='s')
			{
				if(file[count+1]=='u')
				{
					if(file[count+2]=='b')
					{
						if(file[count+3]=='\t')
						{
						  find_keyword_sub=count;
							flag=1;
						}
					}
				}
			}
		}
		if(flag==1)
		{
			if(file[count]=='\n')
			{
				enter_count++;
				if(enter_count==1)
				{
					restore=count; //restore a couple of sub&ldr start location
				}
			}
			if(count==(len-1))
			{
				count=restore;
				flag=0;
				enter_count=0;
				space_flag=0;
				goto loop;
			}
			if(file[count]=='l' && enter_count>0 && enter_count<10)
			{
				if(file[count+1]=='d')
				{
					if(file[count+2]=='r')
					{
						if(file[count+3]=='\t')
						{
							find_keyword_ldr=count;
							//if(find_keyword_ldr>=last_keyword_ldr)
							if(find_keyword_ldr!=last_keyword_ldr)
							{
								flag=2;
								last_keyword_ldr=find_keyword_ldr;  //restore the last ldr keyword location to avoid find ldr keyword the same location
							}
							else
							{
								count++;
								goto loop;
							}
						}
					}
				}
			}
		}
		if(flag==2)
		{
			i=find_keyword_sub;
      k=0;
			for(j=0;file[i+j]!='\n';j++)
			{
				subarray[j]=file[i+j];
				if(file[i+j]=='\t')
				{
					temp1=j+1;
					mark=1;
				}
				if(file[i+j]==' ' && space_flag==0)
				{
					space=j+1; //this value locate the origin op
					for(m=0;file[i+space]!=',';m++)
					{
						origin[m]=file[i+space];
						space++;
					}
					space_flag=1;
				}
				if(origin[0]=='s' && space_flag==1)
				{
					if(origin[1]=='p')
					{
						count=restore;
						flag=0;
						space_flag=0;
						enter_count=0;
						goto loop;
					}
				}
				while(mark==1 && file[i+temp1]!=',')
				{
					tempA[k]=file[i+temp1];
					k++;
					temp1++;
				}
				if(file[i+j]=='#')
				{
					//m=i+j;
					p=j;
				}
			}
      subarray[j]=file[i+j];
			i=find_keyword_ldr;
			mark=0;
			k=0;
			for(j=0;file[i+j]!='\n';j++)
			{
				ldrarray[j]=file[i+j];
				if(file[i+j]=='[')
				{
					temp2=j+1;
					q=j;
					mark=1;
				}
				while(mark==1 && file[i+temp2]!=']')
				{
					tempB[k]=file[i+temp2];
					k++;
					temp2++;
				}
				/*if(file[i+j]==']')
				{*
				 *	//n=i+j;
				 *	q=j;
				}*/
			}
			mark=0;
			if((tempA[0]==tempB[0]) && (tempA[1]==tempB[1]) && (tempA[2]==tempB[2]))
			{
				for(k=0;k<q+1;k++)
				{
					temparray[k]=ldrarray[k];
        }
				//temparray[k]=ldrarray[k];
				q++;
				for(k=0;k<m;k++)
				{
					temparray[q]=origin[k];
					q++;
				}
				temparray[q]='-';
				q=q+1;
				k=0;
				while(subarray[p+k+1]!='\n')
				{
					temparray[q+k]=subarray[p+k+1];
					k++;
				}
				temparray[q+k]=']';
				temparray[q+k+1]='\n';
			}
      else if((tempA[1]!=tempB[1]) || (tempA[2]!=tempB[2]))
      {
        flag=1;
				count=find_keyword_ldr+1;  //in here we add last_keyword_ldr by one because we need to avoid this ldr command because of rX dismatched.
				for(k=0;k<3;k++)
				{
					tempA[k]='\0';
					tempB[k]='\0';
				}
        goto loop;
      }
			for(k=0;temparray[k]!='\n';k++)
			{
				fprintf(file2,"%c",temparray[k]);
        //printf("%c",temparray[k]);
			}
			flag=0;
			space_flag=0;
      fprintf(file2,"%c",temparray[k]);
      count=restore;
      find_keyword_sub=0;
      //find_keyword_ldr=0;
      enter_count=0;
		}
	}
	if((fclose(file1))!=0)
	{
		perror("Close Reading File Error");
		exit(4);
	}
	if((fclose(file2))!=0)
	{
		perror("Close Writing File Error");
		exit(5);
	}
	free(file);
}


/* Testing code
	sub	r0, r2, #20
	sub	r1, r2, #28
	sub	r5, r2, #12
	sub	r11, r2, #8
	ldr	lr, [r0]
	sub	r0, r2, #24
	ldr	r12, [r1]
	sub	r1, r2, #32
	ldr	r3, [r1]
	ldr	r0, [r0]
*/
