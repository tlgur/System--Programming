#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>


int int2string(int a, char b[])
{
	int ch=a;
	int i=0;
	int count=0;
	char temp;

	if(ch==0)
	{
		b[0]='0'; b[1]='\0';
		return 1;
	}

	while(ch)
	{
		count++;
		b[i++]= (ch%10)+'0';
		ch/=10;
	}
	b[i]='\0';

	for(int j=0; j<(count/2); j++)
	{
		temp=b[j];
		b[j]=b[count-j-1];
		b[count-j-1]=temp;
	}
	return count;//strlen(b)
}

int main(int argc, char* argv[])
{
	int fd;
	int pid;
	int len, r_len=-1;
	int status;
	int CASE=0;
	int index=0, line=1, flag=0;
	int t=0, startindex;

	int count=0, count_flag=0, spos=0;
	int F_index, L_index, count_len;
	char temp; //for get char one by one
	char answer[100]={0,};
	char result[100]={0,};
	char LINE[10];
	char INDEX[10];


	while(1)
	{
		fd=open(argv[1],O_RDONLY);
		if((pid=fork())==0)
		{
			len=read(0,answer, 100);
			if(answer[0]=='"') CASE=3;
			else
				for(int i= 0; i<len; i++)
				{
					if(answer[i]==42)
					{	
						CASE=4;
						break;
					}
					else if(answer[i]==32)
					{
						CASE=2; break;
					}
					else CASE=1;
				}
			for(int i=0; i<len; i++)
				if((answer[i]>=65)&&(answer[i]<=90))
					answer[i]+=32;
			switch(CASE)
			{
				case 1:
					index=-1;
					while(read(fd, &temp, 1)!=0)
					{
						flag=0;
						index++;
						if(t==0) startindex=index;
						result[t++]=temp;
						if(((temp==10)||(temp==9))||(temp==32))
						{
							flag=1;
							t--;
							for(int i=0; i<t; i++)
							{
								if((result[i]>=65)&&(result[i]<=90)) result[i]+=32;
								if(result[i]==answer[i]);
								else flag=0;
							}
							if(t!=len-1) flag=0;
							if(flag==1)
							{
								if(r_len!=-1) write(1, " ", 1);
								r_len=int2string(line, LINE);
								write(1, LINE, r_len);
								write(1, ":", 1);
								r_len=int2string(startindex,INDEX);
								write(1, INDEX, r_len);
							}
							t=0;
						}
						if(temp==10){index=-1; line++;}
					}
					flag=0;

					for(int i=0; i<t; i++)
					{
						if(result[i]==answer[i]) flag=1;
						else flag=0;
					}
					if(t!=len-1) flag=0;
					if(flag==1)
					{
						if(r_len!=-1) write(1, " ", 1);
						r_len=int2string(line, LINE);
						write(1, LINE, r_len);
						write(1, ":", 1);
						r_len=int2string(startindex, INDEX);
						write(1, INDEX, r_len);
					}
					write(1, "\n", 1);
					break;

				case 2:
					count=1; F_index=0; L_index=-2;r_len=-1;
					for(int i=0; i<len; i++)
						if(answer[i]==32) count++;
					while(read(fd, &temp, 1)!=0)
					{
						flag=0;
						count_flag=0;
						F_index=0;
						for(int i=0; i<len;i++)
							if(answer[i]==32)
							{L_index=i-1; break;}
						count_len = L_index+1;

						index++;
						flag=0;
						result[t++]=temp;
						if(((temp==10)||(temp==9))||(temp==32))
						{
							flag=1;
							t--;
							for(int i=0;i<t; i++)
							{
								if((result[i]>=65)&&(result[i]<=90)) result[i]+=32;
								if(result[i]==answer[F_index+i]);
								else flag=0;
							}
							if(t!=count_len) flag=0;

							if(flag==1)
							{
								count_flag=1;t=0;
								for(int i=0; i<(count-1);i++)
								{
									t=0;
									index*=-1;
									lseek(fd,index, SEEK_CUR);
									index=0;

									for(int k=L_index+2; k<len; k++)
										if((answer[k]==32)||(answer[k]==10))
										{
											F_index=L_index+2;
											L_index=k-1;
											break;
										}
									count_len = L_index-F_index+1;

									while(read(fd, &temp, 1)!=0)
									{
										index++;
										flag=0;
										result[t++]=temp;
										if(((temp==10)||(temp==9))||(temp==32))
										{
											flag=1;
											t--;
											for(int k=0; k<t; k++)
											{
												if((result[k]>=65)&&(result[k]<=90)) result[k]+=32;
												if(result[k]==answer[F_index+k]);
												else flag=0;
											}
											if(t!=count_len) flag=0;
											if(flag==1){count_flag++; break;}
											t=0;
										}
										if(temp==10) break;
									}
								}
								if(count_flag==count)
								{
									if(r_len!=-1) write(1, " ", 1);
									r_len=int2string(line, LINE);
									write(1, LINE, r_len);
								}
								while(temp!=10) read(fd, &temp, 1); 
							}
							t=0;
						}
						if(temp==10){line++;index=0;}
					}
					write(1,"\n",1);
					break;
				case 3:
					count_flag=1;
					for(int i=0; i<(len-1); i++)
						answer[i]=answer[i+1];
					len-=3; answer[len]=0;
					index=-1;
					while(read(fd, &temp, 1)!=0)
					{
						index++;
						if((temp>=65)&&(temp<=90)) temp+=32;
						if(temp==answer[t])
						{
							if(t==0) 
							{	
								startindex=index;
								if(count_flag==0) t--;
							}
							t++;
							if(t==len)
							{
								read(fd, &temp, 1);
								if(((temp==32)||(temp==10))||(temp==9))
								{
									if(r_len!=-1) write(1, " ", 1);
									r_len=int2string(line, LINE);
									write(1, LINE, r_len);
									write(1, ":",1);
									r_len=int2string(startindex, INDEX);
									write(1, INDEX, r_len);
								}
								t=0;
								lseek(fd, -1, SEEK_CUR);
								index--;
							}

						}
						else
						{
							if(t!=0)
							{
								lseek(fd, -2, SEEK_CUR);
								index-=2;
								if(temp==10) line--;
							}
							t=0;
						}
						if(temp==10)
						{
							index=-1; 
							line++;
						}
						if(((temp==32)||(temp==10))||(temp==9)) count_flag=1;
						else count_flag=0;
					}
					write(1, "\n", 1);
					break;
				case 4:
					for(int i=0; i<len-1;i++)
						if(answer[i]==42) {spos=i; break;}
					while(read(fd, &temp, 1)!=0)
					{
						flag=0;
						result[t++]=temp;
						if((temp==9)||(temp==32))
						{
							flag=1;
							t--;
							for(int i=0; i<t; i++)
							{
								if((result[i]>=65)&&(result[i]<=90)) result[i]+=32;
								if(result[i]!=answer[i]) flag=0;
							}
							if(t!=spos) flag=0;
							if(flag==1)
							{
								t=0;
								read(fd, &temp, 1);
								while(((temp!=32)&&(temp!=9))&&(temp==10)) read(fd, &temp, 1);
								if(temp==10);
								else
								{
									while(read(fd, &temp, 1)!=0)
									{
										flag=0;
										result[t++]=temp;
										if(((temp==32)||(temp==9))||(temp==10))
										{
											flag=1;
											t--;
											for(int i=0;i<t;i++)
											{
												if((result[i]>=65)&&(result[i]<=90)) result[i]+=32;
												if(result[i]==answer[spos+1+i]);
												else flag=0;
											}
											if(t!=len-2-spos) flag=0;
											if(flag==1)
											{
												if(r_len!=-1) write(1, " ", 1);
												r_len=int2string(line, LINE);
												write(1, LINE, r_len);
											}
											else t=0;
										}
										if(temp==10) break;
									}
								}
							}
							t=0;
						}
						if(temp==10){line++;t=0;}
					}
					write(1, "\n", 1);
					break;
				default:
					break;
			}
			exit(0);
		}
		else wait(&status);

		close(fd);
	}
	return 0;
}
