#include<stdio.h>
#include<stdlib.h>
#include<pthread.h> 
#include<time.h> 

long long num[10000] = {0};
long long sorted[10000] = {0};
int MAX = 0, done = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void Merge(int start, int mid, int end)
{
    int lenA = mid - start + 1;
    int lenB = end - mid;

    int *A = (int*)malloc(sizeof(int)*lenA);
    int *B = (int*)malloc(sizeof(int)*lenB);

    for(int i = 0; i < lenA; i++){ 
        A[i] = num[start + i];
    } 
    for(int i = 0; i < lenB; i++){ 
        B[i] = num[mid + 1 + i];
    } 

    int i = 0, j = 0, index = start;
    while(i < lenA && j < lenB){
        if(A[i] < B[j])
            num[index++] = A[i++];
        else
            num[index++] = B[j++];
	}

    while(i < lenA){
        num[index++] = A[i++];
	}
    while(j < lenB){
        num[index++] = B[j++];
	}
}

void merge_sort(int start, int end)
{
    if (start < end){
        int mid = (end + start) / 2;
        merge_sort(start, mid);
        merge_sort(mid + 1, end);
        Merge(start, mid, end);
    }
}

void* merge_sort_A(void *arg)
{
    merge_sort(0, MAX/2);

    pthread_mutex_lock(&mutex);
    done++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void* merge_sort_B(void *arg)
{
    merge_sort(MAX/2 + 1, MAX);

    pthread_mutex_lock(&mutex);
    done++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void* merge_sort_M(void *arg)
{
    pthread_mutex_lock(&mutex);
    while (done < 2){
        pthread_cond_wait(&cond, &mutex);
	}
    pthread_mutex_unlock(&mutex);

    int i = 0, j = MAX/2 + 1, index = 0;
    while(i <= MAX/2 && j <= MAX){
        if(num[i] < num[j])
            sorted[index++] = num[i++];
        else
            sorted[index++] = num[j++];
	}
    while(i <= MAX/2){
        sorted[index++] = num[i++];
	}
    while(j <= MAX){
        sorted[index++] = num[j++];
	}
}

int main(int argc, char *argv[])
{	
	char c;
	int i = 0, negative = 0;
	FILE* fin = fopen(argv[1], "r");
	FILE *fout = fopen(argv[2], "w");	
	
	while ((c = fgetc(fin)) != EOF) {	
		if(c==' '){
			if(negative) num[i] = -num[i];
			negative = 0;
			i++;
			num[i] = 0; 
		}
		else if(c=='\n'){
			if(negative) num[i] = -num[i];
			MAX = i;
			clock_t start, end;
			
    		start = clock();
			pthread_t sortA, sortB, merge;
			pthread_create(&sortA, NULL, merge_sort_A, (void*)NULL);
	        pthread_create(&sortB, NULL, merge_sort_B, (void*)NULL);
	        pthread_create(&merge, NULL, merge_sort_M, (void*)NULL);
	        pthread_join(merge, NULL);
            end = clock();
            
            int j;
		    for (j = 0; j < MAX; j++){
		        fprintf(fout, "%lld ", sorted[j]);
			}
			fprintf(fout, "%lld", sorted[j]);	    
		    fprintf(fout, "\nduration:%f\n\n", (double)(end-start) / CLOCKS_PER_SEC);
		    negative = 0;
			done = 0;
			i = 0;
			num[i] = 0;
		}
		else if(c=='\r'){
		}
		else if(c=='-'){
			negative = 1;
		}
		else{
			long long b = c-'0';
			num[i] = num[i]*10 + b;
		}
	}
	fclose(fin);
	fclose(fout);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	
    return 0; 
}
