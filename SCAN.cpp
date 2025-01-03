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


void SCAN(int reqQueue[], int queueSize, int HEAD, int totalTracks, int direction) {
    int head = HEAD;
	int total_seek_time = 0, seek_time, repositioning_time;
    int left_seq = 0, right_seq = 0; //number of left items & number of right items from the initial head position
    int fetchSeq[queueSize];
    int j = 0;
    int worst_seek_time, best_seek_time;
    
    bubbleSort(reqQueue, queueSize);

    for (int i = 0; i < queueSize; i++) {
    	//if item in list is < head, then left item + 1
        if (reqQueue[i] < head) {
            left_seq++;
        }
        //if item in list is > head, then right item + 1
        if (reqQueue[i] > head) {
            right_seq++;
        }
    }
    
    //right
    if(direction == 1) {
    	
		worst_seek_time = abs(reqQueue[left_seq] - head);
    	best_seek_time = abs(reqQueue[left_seq] - head);		
    	
	    // move to the right requests, start from the first item after the initial head position
	    for (int i = left_seq; i < queueSize; i++) {
	    	seek_time = abs(reqQueue[i] - head);
	        total_seek_time = total_seek_time + seek_time;
	        
	        if(seek_time > worst_seek_time)
	        	worst_seek_time = seek_time;
	        
	        if(seek_time < best_seek_time)
	        	best_seek_time = seek_time;
	        
	        head = reqQueue[i]; //assign the head to the current serviced request
	        fetchSeq[j] = reqQueue[i];
	        j++;
	    }
	    
	    //check if there are items on the left side of the initial head position
	    if(left_seq > 0) {
	    	repositioning_time = abs(totalTracks - 1 - head); //from the current serviced request (head) to the end of the disk
	    	head = totalTracks - 1; //assign head to the end of the disk
	    	seek_time = repositioning_time + abs(reqQueue[left_seq-1] - head); //from the end of the disk (head) to the rightmost of the list of left items	    
		    
			// move to the left requests, start from the last item before the initial head position
		    for (int i = left_seq - 1; i >= 0; i--) {
		        total_seek_time = total_seek_time + seek_time;
		        
		        if(seek_time > worst_seek_time)
	        	worst_seek_time = seek_time;
	        
	        	if(seek_time < best_seek_time)
	        	best_seek_time = seek_time;
		        
		        head = reqQueue[i]; //assign the head to the current serviced request
		        fetchSeq[j] = reqQueue[i];
	        	j++;
	        	
	        	//check if there are any items before the current serviced request, if there are, then calculate the seek time. 
	        	if((i-1) >= 0)
	        		seek_time = abs(reqQueue[i-1] - head);
		    }    	
		}   
	}
	
	//left
	else {
		worst_seek_time = abs(reqQueue[left_seq-1] - head);
	    best_seek_time = abs(reqQueue[left_seq-1] - head);
	    
	    // move to the left requests, start from the last item before the initial head position
	    for (int i = left_seq - 1; i >= 0; i--) {
	        total_seek_time = total_seek_time + abs(reqQueue[i] - head);
	        seek_time = abs(reqQueue[i] - head);
	        
	        if(seek_time > worst_seek_time)
        	worst_seek_time = seek_time;
        
        	if(seek_time < best_seek_time)
        	best_seek_time = seek_time;
	        
	        head = reqQueue[i]; //assign the head to the current serviced request
	        fetchSeq[j] = reqQueue[i];
        	j++;
	    }    
	    
	    //check if there are items on the right side of the initial head position
	    if(right_seq > 0) {
	    	repositioning_time = abs(head); //from the current serviced request (head) to the end of the disk     
	        head = 0; //assign head to the end of the disk	        
	        seek_time = repositioning_time + abs(reqQueue[left_seq] - head); //from the end of the disk (head) to the leftmost of the list of right items	 
	        
	        // move to the right requests, start from the first item after the initial head position
		    for (int i = left_seq; i < queueSize; i++) {		    	
		        total_seek_time = total_seek_time + seek_time;
						        
		        if(seek_time > worst_seek_time)
		        	worst_seek_time = seek_time;
		        
		        if(seek_time < best_seek_time)
		        	best_seek_time = seek_time;
		        
		        head = reqQueue[i]; //assign the head to the current serviced request
		        fetchSeq[j] = reqQueue[i];
		        j++;
		        
		        //check if there are any items after the current serviced request, if there are, then calculate the seek time.
		        if((i+1) < queueSize)
					seek_time = abs(reqQueue[i+1] - head);    	
			}
		}
	}
	
	// Display configurations
    cout << "---- SCAN ----" << endl;
    cout << "* CONFIGURATION *" << endl;
    cout << "Disk size: " << totalTracks << endl;
    cout << "Head position: " << HEAD << endl;
    cout << "Request size: " << queueSize << endl;
    cout << "Direction: " << (direction == 1 ? "right" : "left") << endl;
    cout << "-----------------" << endl;
	
	// Display results
    cout << "* RESULTS *" << endl;
    cout << "Total request fetch: " << queueSize << endl;
    cout << "Request fetch sequence: [";
	
	for(int i = 0; i < queueSize; i++) {
		cout << fetchSeq[i];
		if (i != queueSize - 1)
        {
            cout << " --> ";
        }		
	}
	
	cout << "]" << endl;	
	 cout << "-----------------" << endl;
    
	cout << "\nTotal seek time / head movements: " << total_seek_time << " units" << endl;
	cout << "Average seek time / head movements: " << fixed << setprecision(2) << ((double)total_seek_time / queueSize) << " units" << endl;
	cout << "Worst seek time / head movements: " << worst_seek_time << " units" << endl;
	cout << "Best seek time / head movements: " << best_seek_time << " units" << endl;
	cout << "-----------------" << endl;
    cout << "Total seek count: " << queueSize << endl;
    cout << "Average seek count: " << 1 << endl;
    cout << "-----------------" << endl;
}


int main() {
	int NUM_OF_REQ;
	int HEAD;
	int DISK_SIZE;
	int direction;
	
	do
    {
        cout << "Enter the initial head position: ";
        cin >> HEAD;

        if (HEAD < 0)
        {
            cout << "\n[Invalid head position. Please enter a non-negative number.]\n\n";
        }

    } while (HEAD < 0);

    do
    {
        cout << "Enter the total number of cylinders on the disk: ";
        cin >> DISK_SIZE;

        if (DISK_SIZE <= 0)
        {
            cout << "\n[Invalid disk size. Please enter a positive number.]\n\n";
        }

    } while (DISK_SIZE <= 0);

    do
    {
        cout << "Enter the total number of requests: ";
        cin >> NUM_OF_REQ;

        if (NUM_OF_REQ <= 0)
        {
            cout << "\n[Invalid number of requests. Please enter a positive number.]\n\n";
        }

    } while (NUM_OF_REQ <= 0);
	
	int req_list[NUM_OF_REQ];
	
	do
    {
        cout << "Enter the direction of the disk head movement (1 for right, 0 for left): ";
        cin >> direction;

        if (direction != 0 && direction != 1)
        {
            cout << "\n[Invalid direction. Please enter 1 for right or 0 for left.]\n\n";
        }

    } while (direction != 0 && direction != 1);
    
    cout << "* Enter the request sequence *" << endl;

    for (int i = 0; i < NUM_OF_REQ; i++)
    {
        // Check if the request is within the disk's range and not a duplicate
        bool is_duplicate;
        do
        {
            is_duplicate = false;
            cout << "Request " << i + 1 << ": ";
            cin >> req_list[i];

            if (req_list[i] < 0 || req_list[i] >= DISK_SIZE)
            {
                cout << "\n[Invalid request. Please enter a request within the disk's range.]\n\n";
            }

            for (int j = 0; j < i; j++)
            {
                if (req_list[i] == req_list[j])
                {
                    cout << "\n[Duplicate request. Please enter a unique request.]\n\n";
                    is_duplicate = true;
                    break;
                }
            }

        } while (req_list[i] < 0 || req_list[i] >= DISK_SIZE || is_duplicate);
    }
	
	SCAN(req_list, NUM_OF_REQ, HEAD, DISK_SIZE, direction);	
	
	return 0;
}
