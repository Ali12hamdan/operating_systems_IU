#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int u[120],v[120];
    

int calculateDot(int left, int right){

	if(right<left)return -1;
	
	int re=0;
	for (int i=left;i<right;i++){
		re +=u[i] * v[i];
	}
	return re;
}
void fillArrayU(int size){
	for(int i=0;i<120;i++)
	{
		u[i]=rand()%size;
	}	
}

void fillArrayV(int size){
	for(int i=0;i<120;i++)
	{
		v[i]=rand()%size;
	}	
}

int main(void){
    FILE *file=fopen("temp.txt","w");
    fillArrayU(100);fillArrayV(100);
    int n;
    fprintf(stdout,"enter n");
    scanf("%d",&n);
    pid_t mainID = getpid();
    if(n<=0){exit(0);}
    if (120%n!=0){
    exit(0);}
    for (int i=0;i<n;i++){
        if (getpid()==mainID)
        {
            fork();
            if(getpid()==mainID)continue;
            int start = i*(120/n);
            int end = start+ (120/n);
            fprintf(file,"%d\n",calculateDot(start, end));
            fclose(file);
            return EXIT_SUCCESS;
        }
    }
    wait(NULL);
    fclose(file);
    
    file=fopen("temp.txt","r");
    int ans=0;
    for (int i=0;i<n;i++){
        int aw;
        fscanf(file,"%d",&aw);
        ans+=aw;
    }
    fprintf(stdout,"%d",ans);
    fclose(file);
	return EXIT_SUCCESS;

}
