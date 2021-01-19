#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CAPACITY 100
#define BUFFER_LENGTH 100
 
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
typedef struct person{
	char *name;//배열 첫번째의 주소를 가져오기 위해 
	char *number;
	char *email;
	char *group;
} Person;//별명
/* 구조체 struct person을 정의하면서 동시에 그것을 Person으로
renamimg 했다. 이런식으로 사용 할 경우 structure tag인 person을
생략해도 된다 */

Person directory[CAPACITY];//Person 타입의 배열 directory를 선언 

int read_line(FILE *fp,char str[],int n);
void load(char *fileName);
void add(char *name, char *number, char *email, char *group);
int compose_name(char str[], int limit);
void handle_add(char *name);
void save(char *fileName);
int search(char *name);
void print_person(Person p);
void remover(char *name);
void status();
void find(char *name);


int n=0; 
int main(int argc, char *argv[]) {
	char command_line[BUFFER_LENGTH];
	char *command,*argument;
	char name_str[BUFFER_LENGTH];
	
	while(1){
		printf("$ (명령 # argument # argument ) \n");
		if(read_line(stdin,command_line,BUFFER_LENGTH)<=0)
		continue;
		
		command=strtok(command_line," ");
		
		if(strcmp(command,"read")==0){
			argument=strtok(NULL," ");
			if(argument==NULL){
					printf("Invalid arguments.\n");
					continue;
				}
					load(argument);
		}
		else if(strcmp(command,"add")==0){
			if(compose_name(name_str,BUFFER_LENGTH)<=0){
				printf("이름이 필요합니다.\n");
				continue;
			}
			
		handle_add(name_str);
		}
	
		else if(strcmp(command,"find")==0){
			if(compose_name(name_str,BUFFER_LENGTH<=0)){
				printf("이름이 필요합니다.\n");
				continue;
			}
			find(name_str);
			}
			
		else if(strcmp(command,"status")==0){
			status();
		}
		else if(strcmp(command,"delete")==0){
			if(compose_name(name_str,BUFFER_LENGTH)<=0){
				printf("Invalid arguments.\n");
				continue;
			}
			remover(name_str);
		}
		else if(strcmp(command,"save")==0){
			argument=strtok(NULL," ");
			if(strcmp(argument,"as")!=0){
				printf("Invalid arguments.\n");
				continue;
			}
			argument=strtok(NULL," ");
			if(argument==NULL){
				printf("Invalid arguments.\n");
				continue;
			}
			save(argument);
		}
		else if(strcmp(command,"exit")==0)
			break;
			
		}
	return 0;
}




int read_line(FILE *fp,char str[],int n){
	int ch,i=0;
	
	while((ch=fgetc(fp)!='\n'&&ch!=EOF)){
		if(i<n){
			str[i++]=ch;
		}
	}
	str[i]='\0';
	return i;
}

void load(char *fileName){
	char buffer[BUFFER_LENGTH];
	char *name,*number,*email,*group;
	
	FILE *fp=fopen(fileName,"r");
	if(fp==NULL){
		printf("Open failed.\n");
		return ;
	}
	
	while(1){
		if(read_line(fp,buffer,BUFFER_LENGTH)<=0)
			break;

		name=strtok(buffer,"#");
		number=strtok(NULL,"#");
		email=strtok(NULL,"#");
		group=strtok(NULL,"#");
		add(name,number,email,group);
	}
	fclose(fp);
}

void add(char *name, char *number, char *email, char *group){
	int i=n-1;
	while(i>=0 && strcmp(directory[i].name,name)>0){
		directory[i+1]=directory[i];
		i--;
	}
	
	directory[i+1].name=strdup(name);
	directory[i+1].number=strdup(number);
	directory[i+1].email=strdup(email);
	directory[i+1].group=strdup(group);
	
	n++;
}

int compose_name(char str[], int limit){
	char *ptr;
	int length=0;
	
	ptr=strtok(NULL," ");
	if(ptr==NULL)
		return 0;
		
		strcpy(str,ptr);
		length+=strlen(ptr);
		
		while((ptr=strtok(NULL," "))!=NULL){
			if(length+strlen(ptr)+1<limit){
				//overflow를 막기 위한 조건
				//+1은 마지막 '\0'을 위함이다.
				
				str[length++]=' ';
				str[length]='\0';//이해안됨 ㅠ두칸이 뛰어지는건지,생략하고 실행해보기
				 
				strcat(str,ptr);
				length+=strlen(ptr);
			}
		}
		return length;
	}

void handle_add(char *name){
	char number[BUFFER_LENGTH],email[BUFFER_LENGTH],group[BUFFER_LENGTH];
	char empty[]=" ";
	
	printf("  전화번호 :");
	read_line(stdin,number,BUFFER_LENGTH);
	
	printf("  email:   :");
	read_line(stdin,email,BUFFER_LENGTH);
	
	printf("  Group:   :");
	read_line(stdin,group,BUFFER_LENGTH);
	
	add(name, (char *)(strlen(number)>0?number:empty),
			(char *)(strlen(email)>0?email:empty),
			(char *)(strlen(group)>0?group:empty));
		}
		//이름은 없는 경우가 없으므로 
		//존재하지 않는 항목들을 하나의 공백문자로 구성된 문자열로 대체한다.
		
void save(char *fileName){
	int i;
	FILE *fp=fopen(fileName,"w");
	if(fp==NULL){
		printf("Open failed.\n");
		return;
	}
	
	for(i=0;i<n;i++){
		fprintf(fp,"%s#",directory[i].name);
		fprintf(fp,"%s#",directory[i].number);
		fprintf(fp,"%s#",directory[i].email);
		fprintf(fp,"%s#\n",directory[i].group);
	}
	fclose(fp);
}

int search(char *name){
	int i;
	for(i=0;i<n;i++){
		if(strcmp(name,directory[i].name)==0){
			return i;
		}
	}
	return -1;
}

void print_person(Person p){
	printf("%s:\n",p.name);
	printf("   전화번호: %s\n",p.number);
	printf("   이메일  : %s\n",p.email);
	printf("   그룹    : %s\n",p.group);
}

void remover(char *name){
	int i=search(name);
	if(i==-1){
		printf("이름이 '%s'인 사람은 존재하지 않습니다.\n",name);
		return ;
	}
	int j=i;
	for(;j<n-1;j++){
		directory[j]=directory[j+1];
		/* 구조체 변수간의 치환연산이 지원되므로
		멤버 항목들을 따로따로 치환할 필요가 없다.
		*/
	}
	n--;
	printf("'%s'님은 성공적으로 삭제되었습니다.\n",name);
}

void status(){
	int i;
	for(i=0;i<n;i++)
		print_person(directory[i]);
	printf("총 %d 명. \n",n);
}

void find(char *name){
	int index=search(name);
	if(index==-1)
		printf("'%s'님은 존재하지 않습니다.\n",name);
	else
		print_person(directory[index]);
	}
		 
