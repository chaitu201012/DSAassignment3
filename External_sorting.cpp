#include <bits/stdc++.h> 
using namespace std; 


FILE* openfile(char* tempFile, char* mode) 
{ 
	FILE* fp = fopen(tempFile, mode); 
	if (fp == NULL) { 
		perror("Error while opening the file.\n"); 
		exit(EXIT_FAILURE); 
	} 
	return fp; 
} 


struct Node { 

	int data; 
	int index; //index of the array from which the element is taken

};

void exchangeNodes(Node *a,Node *b){
    
    Node temp= *a;
    *a=*b;
    *b=temp;
}

class minHeap{

private:
    Node *arrayHeap;
    int sizeOfHeap;
public:
    minHeap(Node arr[],int size){
        arrayHeap=arr,
        sizeOfHeap=size;
        for(int internalIndex=(sizeOfHeap-1)/2;internalIndex>=0;internalIndex--)
            minHeapify(internalIndex);       
        
    }

    void replaceFirstElement(Node l){ 
        arrayHeap[0]=l;
        minHeapify(0);
    }

    void minHeapify(int index){
        int leftIndex= 2*index+1;
        int rightIndex=2*index+2;
        int smallindex=index;
        if(leftIndex<sizeOfHeap && arrayHeap[leftIndex].data<arrayHeap[index].data){
            smallindex=leftIndex;
        }
        if(rightIndex<sizeOfHeap && arrayHeap[rightIndex].data<arrayHeap[smallindex].data){
            smallindex=rightIndex;
        }
        if (smallindex!=index){
            exchangeNodes(&arrayHeap[index],&arrayHeap[smallindex]);
            minHeapify(smallindex);

        }

    }

    Node getMinimum(){ return arrayHeap[0];}

};

void mergeFiles(char* output_file, int n, int k) 
{ 
	FILE* out[k]; 
	for (int i = 0; i < k; i++) { 
		char tempFile[2]; 

		snprintf(tempFile, sizeof(tempFile), "%d", i); 

		out[i] = openfile(tempFile, "r"); 
	} 
	//cout<<"check point 1\n";
	FILE* output = openfile(output_file, "w"); 
	Node* harr = new Node[k]; 
	int i; 
	for (i = 0; i < k; i++) { 
		if (fscanf(out[i], "%d ", &harr[i].data) != 1) 
			break; 
		harr[i].index = i; 
	} 
	minHeap hp(harr, i); 

	int count = 0; 
	//cout<<"check point 2\n";
	while (count != i) { 
		Node root = hp.getMinimum(); 
		fprintf(output, "%d ", root.data); 

		if (fscanf(out[root.index], "%d,", &root.data) != 1) { 
			root.data = INT_MAX; 
			count++; 
		} 

		hp.replaceFirstElement(root); 
	} 
	//cout<<"check point 3\n";
	for (int i = 0; i < k; i++) 
		fclose(out[i]); 

	fclose(output); 
} 




void mergeArrays(int arr[], int left, int mid, int right) 
{ 
	int i, j, k; 
	int n1 = mid - left + 1; 
	int n2 = right - mid; 

	int L[n1], R[n2]; 

	for (i = 0; i < n1; i++) 
		L[i] = arr[left + i]; 
	for (j = 0; j < n2; j++) 
		R[j] = arr[mid + 1 + j]; 
	i = 0; 

	j = 0; 

	k = left; 
	while (i < n1 && j < n2) { 
		if (L[i] <= R[j]) 
			arr[k++] = L[i++]; 
		else
			arr[k++] = R[j++]; 
	} 
	while (i < n1) 
		arr[k++] = L[i++]; 

	while (j < n2) 
		arr[k++] = R[j++]; 
} 



void sort(int arr[], int left, int right) 
{ 
	if (left < right) { 
		
		int mid= left + (right - left) / 2; 
		sort(arr, left, mid); 
		sort(arr, mid + 1, right); 

		mergeArrays(arr, left, mid, right); 
	} 
} 


void kWayExternalSort( char* input_file, char* output_file, int k, int partition_size) { 

	FILE* infp = fopen(input_file, "r"); 
	if (infp == NULL) { 
		perror("Error while opening the file.\n"); 
		exit(EXIT_FAILURE); 
	} 
	FILE* outputFiles[k]; 
	char tempFile[3]; 
	for (int i = 0; i < k; i++) { 
		snprintf(tempFile, sizeof(tempFile),"%d", i); //creating d character length ouput files  
	    outputFiles[i] = openfile(tempFile,  "w");  //Storing them in the ouput files array
	} 
	//cout<<"checkpoint 1\n";
	int* arr = (int*)malloc(partition_size * sizeof(int));  //creating arr of size parition 

	bool stillThereIsinput = true; 
	int next_Op_File = 0; 

	int i; 
	while (stillThereIsinput && next_Op_File<k) { 
        i=0;
		for (i = 0; i < partition_size; i++) { 
			if (fscanf(infp, "%d,", &arr[i]) != 1) { 
				stillThereIsinput = false; 
				break; 
			} 
		} 
		//cout<<"checkpoint 2"<<i<<endl;
		sort(arr, 0, i - 1); 
		//cout<<"checkpoint 2"<<i<<endl;
		for (int j = 0; j < i; j++) {
			//cout<<next_Op_File<<endl;
			fprintf(outputFiles[next_Op_File], "%d ", arr[j]); 
		}

		next_Op_File++; 
	} 

	//cout<<"checkpoint 2\n";

	for (int i = 0; i < k; i++) 
		fclose(outputFiles[i]);

	fclose(infp); 
	//cout<<"completed initial processing\n";
	mergeFiles(output_file,partition_size, k); 
} 

void deletefiles(int k) {
	FILE* outputFiles[k]; 
	char tempFile[2]; 
	for (int i = 0; i < k; i++) { 
		snprintf(tempFile, sizeof(tempFile),"%d", i); //creating d character length ouput files  
	    //outputFiles[i] = openfile(tempFile,  "w");  //Storing them in the ouput files array
		const int result = remove( tempFile );
		if( result != 0 ){
			printf( "%s\n", strerror( errno ) ); // No such file or directory
		} 

	}

}

int main(int argc, char *argv[]){
	
	int k = 10;  // 

	int partition_size = 1000000; // The size of each partition 

	char input_file[50];
	char output_file[50]; 
	strcpy(input_file,argv[1]);
	strcpy(output_file,argv[2]);
    //cout<<"inputFile "<<input_file<<endl;
    //cout<<"outputFile"<<output_file<<endl;

	kWayExternalSort(argv[1], argv[2], k, partition_size); 
	deletefiles(k);

	return 0; 
} 


