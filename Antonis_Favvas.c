#include <stdio.h>   
#include <dirent.h>
#include <unistd.h>  
#include <stdlib.h>  
#include <errno.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

int main(int argc, char**argv){
	pid_t pid;
	int *pd;
	int status, counter, i, j, k, l, cnt, c, cp, cp2, z, cntpipe, startindicator, endindicator, flag, orend, orstart, errorflag, identification, flagmode;
	char path[100];
	char str[512];
	char **tokens;
	char *p;
	char *n[512];
	char *erotimatiko=";";
	char *AndAnd="&&";
	char *Or="|";
	char space = ' ';
	char *quit = "quit";
	printf("for interactive mode press 1 for batch mode press 0");
	scanf("%d", &flagmode);
	if((flagmode!=0)&&(flagmode!=1)){
		while((flagmode!=0)&&(flagmode!=1)){
			scanf("%d", &flagmode);
		}
	}
	if(flagmode==0){
		fgets(path, sizeof(path), stdin);
		printf("%s", path);
		FILE *fp;
		fp=fopen(path, "r");
		if(fp==NULL){
			printf("error");
			exit(-1);
		}
		while(!feof(fp)){
			while(fgets(str,sizeof(str),fp)!=NULL){
				//i would execute the follow while for every line of the file.
			}
		}
	}
	
	LOOP:while(1){
		counter=0;
		cp=0;
		cp2=0;
		
		printf("favvas_8675>");
		fgets(str, sizeof(str), stdin);
		/*Παρατηρησα οτι καθώς αποθήκευα το string που μου έδινε ο χρήστης, αποθηκευόταν στο τέλος ο χαρακτήρας αλλαγής γραμμής(κατι το οποίο δεν έπρεπε να γίνει)
		* για αυτό χρησιμοποίησα την επόμενη if.*/
		if(strlen(str)>512){
			printf("too many arguments. The max number of characters for input is 512 \n");
			goto LOOP;
		}
		if(str[0]=='\n'){
			goto LOOP;
		}
		for(i=0;i<(strlen(str)-1);i++){
			if(str[i]!=space){
				flag=1;
			}
		}
		if(flag==0){
			goto LOOP;
		}
		if ((strlen(str) > 0) && (str[strlen (str) - 1] == '\n')){
			str[strlen (str) - 1] = '\0';
		}
		tokens=(char **)malloc(256*sizeof(char*));
		
		i=0;
		p = strtok(str, " ");
		tokens[i]=p;
		while(p!=NULL){
			i=i+1;
			p=strtok(NULL, " ");
			if(p!=NULL){
				tokens[i]=p;
			}
			counter=counter+1;
		}
		
		/*o κωδικας που έχω γραψει λειτουργεί όταν ο χρήστης πληκτρολογήσει την τελευταία του εντολή ακολουθούμενη απο το χαρακτήρα ; ωστόσο με την παρακάτω if o 
		*χρήστης έχει την ευελιξία να τελειώσει την εντολή με ή χωρίς το  ; όπως και σε ένα κανονικό shell.*/ 
		if ((strcmp(tokens[i-1], erotimatiko))){
			tokens[i]=";";
			counter = counter + 1;
		}

		for(i=0;i<counter;i++){
			if((!(strcmp(tokens[i],erotimatiko)))||(!(strcmp(tokens[i],AndAnd)))){
				cp=cp+1;
			}
		}	
		for(j=0;j<cp;j++){
			if(j==0){
				endindicator=-1;
			}
			for(i=(endindicator+1);i<counter;i++){
				if(!(strcmp(tokens[i], Or))){
					flag=1;
					cp2=0;
					for(l=(endindicator+1);l<counter;l++){
						if((!(strcmp(tokens[l],erotimatiko)))||(!(strcmp(tokens[l],AndAnd)))){
							orstart=endindicator+1;
							orend=l;
							break;
						}
						if(!(strcmp(tokens[l], Or))){
							cp2=cp2+1;
						}
					
					}

					int cntpipe=3;
					pd=(int*)malloc((2*cp2)*sizeof(int));
					for(l=0;l<cp2;l++){
						pipe(pd+(l*2));				//δημιουργία pipes.
					}
					for(l=0;l<(cp2+1);l++){
						if(l==0){
							for(z=0;z<512;z++){
								n[z]=NULL;
							}
							cnt=0;
							for(k=(endindicator+1);k<i;k++){
								n[cnt]=tokens[k];
								cnt=cnt+1;
							}
							endindicator=i;
							if(!(strcmp(n[0], quit))){
								printf("key word  (quit) was catched the programm will now exit\n");
								exit(-1);
							}
							pid=fork();
							if(pid==0){
								dup2(pd[1],1);
								for(z=0;z<(cp2*2);z++){
									close(pd[z]);
								}
								identification=execvp(n[0], n);
								if(identification==-1){
									printf("the command %s is not valid \n", n[0]);
									endindicator=orend;
									exit(-1);
								}	
							}
							
						}
						else if(l==cp2){
							startindicator=endindicator+1;
							for(k=(endindicator+1);k<(orend+1);k++){
								if((!(strcmp(tokens[k],erotimatiko)))||(!(strcmp(tokens[k],AndAnd)))){
									endindicator=k;
									break;
								}
	
							}
							for(z=0;z<512;z++){
								n[z]=NULL;
							}
							cnt=0;
							for(k=startindicator;k<endindicator;k++){
								n[cnt]=tokens[k];
								cnt=cnt+1;
							}
							if(!(strcmp(n[0], quit))){
								printf("key word  (quit) was catched the programm will now exit\n");
								exit(-1);
							}
							pid=fork();
							if(pid==0){
								dup2(pd[((cp2*2)-2)],0);
								for(z=0;z<(cp2*2);z++){
									close(pd[z]);
								}
								identification=execvp(n[0], n);
								if(identification==-1){
									printf("the command  %s is not valid \n", n[0]);
									endindicator=orend;
									exit(-1);
								}
							}
						}
						else{
							startindicator=endindicator+1;
							for(k=(endindicator+1);k<orend;k++){
								if(!(strcmp(tokens[k], Or))){
									endindicator=k;
									break;
								}
							}
							for(z=0;z<512;z++){
								n[z]=NULL;
							}
							cnt=0;
							for(k=startindicator;k<endindicator;k++){
								n[cnt]=tokens[k];
								cnt=cnt+1;
							}
							if(!(strcmp(n[0], quit))){
								printf("key word  (quit) was catched the programm will now exit\n");
								exit(-1);
							}
							pid=fork();
							if(pid==0){
								dup2(pd[cntpipe],1);
								dup2(pd[cntpipe-3],0);
								for(z=0;z<(cp2*2);z++){
									close(pd[z]);
								}
								identification=execvp(n[0], n);
								if(identification==-1){
									printf("the command  %s  is not valid \n", n[0]);
									endindicator=orend;
									exit(-1);
								}
								
							}
							cntpipe=cntpipe+2;
						}
					}

					if(pid>0){
						for(z=0;z<(cp2*2);z++){
							close(pd[z]);
						}
						for(i=0;i<(cp2+1);i++){
							wait(&status);
						}
					}
					break;
				}
				else if((!(strcmp(tokens[i],erotimatiko)))||(!(strcmp(tokens[i],AndAnd)))){
					flag=0;
					errorflag=0;
					if(!(strcmp(tokens[i],AndAnd))){
						errorflag=1;
					}
					if(cp==1){
						startindicator=0;
						endindicator=i;
						break;
					}
					else if(j==0){
						endindicator=i;
						startindicator=0;
						break;
					}
					else{
						startindicator=endindicator+1;
						endindicator=i;
						break;
					}
				}
			}
		
			if(flag==0){
				for(l=0;l<512;l++){
					n[l]=NULL;
				}
				l=0;
				for(i=startindicator;i<endindicator;i++){
					n[l]=tokens[i];
					l=l+1;
				}
				n[511]=NULL;
				if(!(strcmp(n[0], quit))){
					printf("key word  (quit) was catched the programm will now exit\n");
					exit(-1);
				}
				pid=fork();
				if(pid<0){
					printf("error cannot fork exit");
					exit(-1);
				}
				else if(pid==0){
					identification=execvp(tokens[startindicator], n);
					if(identification==-1){
						printf("the command %s is not valid \n", n[0]);
						if(errorflag==1){
							break;
						}
					}
				}
				else{
					wait(&status);
				}
			}	
		}
	}
}


		
		
		