#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
using namespace std;

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

/*1. Say head = 50 and total tracks = 100 and the sorted queue is {30, 40, 60, 70} and direction = right, total movement = 60 - 50. Then, head = 60.
2. total movement = total movement + (70 - head). Then head = 70. 
3. Since 70 is the last req and there are req before the head, total movement = total movement + (total tracks - 1 - head). Then head = 199.
4. total movement = total movement + (head - 40). Then head = 40.
5. total movement = total movement + (head - 30). Then head = 30.*/

void SCAN(int reqQueue[], int queueSize, int head, int totalTracks, string direction) {
    int totalMov = 0, mov;
    int left = 0, right = 0;
    int fetchSeq[queueSize];
    int j = 0;
    int worst, best;

    for (int i = 0; i < queueSize; i++) {
        if (reqQueue[i] < head) {
            left++;
        }
        if (reqQueue[i] > head) {
            right++;
        }
    }
    
    if(direction == "Right") {
    	worst = abs(reqQueue[left] - head);
	    best = abs(reqQueue[left] - head);
	    
	    // Move to the right req
	    for (int i = left; i < queueSize; i++) {
	    	mov = abs(reqQueue[i] - head);
	        totalMov = totalMov + mov;
	        
	        if(mov > worst)
	        	worst = mov;
	        
	        if(mov < best)
	        	best = mov;
	        
	        head = reqQueue[i];
	        fetchSeq[j] = reqQueue[i];
	        j++;
	    }
	    
	    if(left > 0) {
	    	// Then move to the left
	    	mov = abs(totalTracks - 1 - head);
		    totalMov = totalMov + mov;
		    head = totalTracks - 1;
		    
		    mov = mov + abs(reqQueue[left-1] - head);
		    
		    if(mov > worst)
	        	worst = mov;
	        
	    	if(mov < best)
	    		best = mov;
		
		    for (int i = left - 1; i >= 0; i--) {
		        totalMov = totalMov + abs(reqQueue[i] - head);
		        
		        if(mov > worst)
	        	worst = mov;
	        
	        	if(mov < best)
	        	best = mov;
		        
		        head = reqQueue[i];
		        fetchSeq[j] = reqQueue[i];
	        	j++;
	        	
	        	if((i-1) > 0)
	        		mov = abs(reqQueue[i-1] - head);
		    }    	
		}   
	}
	
	else {
		worst = abs(reqQueue[left-1] - head);
	    best = abs(reqQueue[left-1] - head);
	    
	    for (int i = left - 1; i >= 0; i--) {
	        totalMov = totalMov + abs(reqQueue[i] - head);
	        mov = abs(reqQueue[i] - head);
	        
	        if(mov > worst)
        	worst = mov;
        
        	if(mov < best)
        	best = mov;
	        
	        head = reqQueue[i];
	        fetchSeq[j] = reqQueue[i];
        	j++;
	    }    
	    
	    if(right > 0) {
	    	// Then reverse direction and move right
			mov = abs(head);
	        totalMov = totalMov + mov;	        
	        head = 0;
	        
	        mov = mov + abs(reqQueue[left] - head);
	        
	        // Move to the right req
		    for (int i = left; i < queueSize; i++) {
		    	
		        totalMov = totalMov + abs(reqQueue[i] - head);
		        
		        if(mov > worst)
		        	worst = mov;
		        
		        if(mov < best)
		        	best = mov;
		        
		        head = reqQueue[i];
		        fetchSeq[j] = reqQueue[i];
		        j++;
		        
		        if((i+1) < queueSize)
					mov = abs(reqQueue[i+1] - head);    	
			}
		}
	}
	
	cout << "\nFetch Sequence: ";
    for (int i = 0; i < queueSize; i++) {
        cout << fetchSeq[i] << " ";
    }
    
	cout << "\nTotal seek time / head movements: " << totalMov << endl;
	cout << "Average seek time / head movements: " << fixed << setprecision(2) << ((double)totalMov / queueSize) << endl;
	cout << "Worst seek time / head movements: " << worst << endl;
	cout << "Best seek time / head movements: " << best << endl;
}


int main() {
	int size;
	int headPos;
	int numCylinder;
	string direction;
	
	cout << "Enter the disk's initial head position: ";
	cin >> headPos;
	
	cout << "Enter the total number of cylinders on the disk: ";
	cin >> numCylinder;
	
	cout << "Enter the size of the queue: ";
	cin >> size;
	
	int queue[size];
	
	cout << "Enter the disk requests: ";
    for (int i = 0; i < size; i++) {
        cin >> queue[i];
    }
    
    cout << "Enter direction: ";
    cin >> direction;
	
	bubbleSort(queue, size);
	
	cout << "\nSorted request queue: ";
	
	for(int i = 0; i < size; i++) {
		cout << queue[i] << " ";
	}
	
	cout << "\nCONFIGURATION";
	
	cout << "\nDisk size: " << numCylinder;
	cout << "\nHead position: " << headPos;
	cout << "\nRequest size: " << size;
	cout << "\nDirection: " << direction; 
	
	SCAN(queue, size, headPos, numCylinder, direction);	
}
