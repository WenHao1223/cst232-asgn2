#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <cmath>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// Sort the requests using insertion sort

void insertionSort(int req_list[], const int NUM_OF_REQ)
{
    int i, key, index;
    for (i = 1; i < NUM_OF_REQ; i++)
    {
        key = req_list[i];
        index = i - 1;
        while (index >= 0 && req_list[index] > key)
        {
            req_list[index + 1] = req_list[index];
            index = index - 1;
        }
        req_list[index + 1] = key;
    }

    cout << "\n* SORTED REQUESTS *\n";
    cout << "Sorted requests: [";

    for (int i = 0; i < NUM_OF_REQ; i++)
    {
        cout << req_list[i];
        if (i != NUM_OF_REQ - 1)
        {
            cout << ", ";
        }
    }

    cout << "]" << endl
         << endl;
}

// SCAN Algorithm

void SCAN(int req_list[], int direction, const int NUM_OF_REQ, const int DISK_SIZE, const int HEAD)
{
    int head = HEAD;
    int total_seek_time = 0, seek_time = 0, repositioning_time = 0, worst_seek_time = 0, best_seek_time = INT_MAX, index = 0;
    int left_seq = 0, right_seq = 0;               // Number of left items & number of right items from the initial head position
    int *req_processed_list = new int[NUM_OF_REQ]; // Temporary array to store processed requests

    insertionSort(req_list, NUM_OF_REQ);

    for (int i = 0; i < NUM_OF_REQ; i++)
    {
        // If item in list is < head, then left item + 1
        if (req_list[i] < head)
        {
            left_seq++;
        }
        // If item in list is > head, then right item + 1
        if (req_list[i] > head)
        {
            right_seq++;
        }
    }

    // Right direction
    if (direction == 1)
    {

        // Move to the right requests, start from the first item after the initial head position
        for (int i = left_seq; i < NUM_OF_REQ; i++)
        {
            seek_time = abs(req_list[i] - head);
            total_seek_time = total_seek_time + seek_time;

            if (seek_time > worst_seek_time)
                worst_seek_time = seek_time;

            if (seek_time < best_seek_time)
                best_seek_time = seek_time;

            head = req_list[i]; // Assign the head to the current serviced request
            req_processed_list[index] = req_list[i];
            index++;
        }

        // Check if there are items on the left side of the initial head position
        if (left_seq > 0)
        {
            repositioning_time = abs(DISK_SIZE - 1 - head);                      // From the current serviced request (head) to the end of the disk
            head = DISK_SIZE - 1;                                                // Assign head to the end of the disk
            seek_time = repositioning_time + abs(req_list[left_seq - 1] - head); // From the end of the disk (head) to the rightmost of the list of left items

            // Move to the left requests, start from the last item before the initial head position
            for (int i = left_seq - 1; i >= 0; i--)
            {
                total_seek_time = total_seek_time + seek_time;

                if (seek_time > worst_seek_time)
                    worst_seek_time = seek_time;

                if (seek_time < best_seek_time)
                    best_seek_time = seek_time;

                head = req_list[i]; // Assign the head to the current serviced request
                req_processed_list[index] = req_list[i];
                index++;

                // Check if there are any items before the current serviced request, if there are, then calculate the seek time.
                if ((i - 1) >= 0)
                    seek_time = abs(req_list[i - 1] - head);
            }
        }
    }

    // Left
    else
    {

        // Move to the left requests, start from the last item before the initial head position
        for (int i = left_seq - 1; i >= 0; i--)
        {
            total_seek_time = total_seek_time + abs(req_list[i] - head);
            seek_time = abs(req_list[i] - head);

            if (seek_time > worst_seek_time)
                worst_seek_time = seek_time;

            if (seek_time < best_seek_time)
                best_seek_time = seek_time;

            head = req_list[i]; // Assign the head to the current serviced request
            req_processed_list[index] = req_list[i];
            index++;
        }

        // Check if there are items on the right side of the initial head position
        if (right_seq > 0)
        {
            repositioning_time = abs(head);                                  // From the current serviced request (head) to the end of the disk
            head = 0;                                                        // Assign head to the end of the disk
            seek_time = repositioning_time + abs(req_list[left_seq] - head); // From the end of the disk (head) to the leftmost of the list of right items

            // Move to the right requests, start from the first item after the initial head position
            for (int i = left_seq; i < NUM_OF_REQ; i++)
            {
                total_seek_time = total_seek_time + seek_time;

                if (seek_time > worst_seek_time)
                    worst_seek_time = seek_time;

                if (seek_time < best_seek_time)
                    best_seek_time = seek_time;

                head = req_list[i]; // Assign the head to the current serviced request
                req_processed_list[index] = req_list[i];
                index++;

                // Check if there are any items after the current serviced request, if there are, then calculate the seek time.
                if ((i + 1) < NUM_OF_REQ)
                    seek_time = abs(req_list[i + 1] - head);
            }
        }
    }

    // Display configurations
    cout << "---- SCAN ----" << endl;
    cout << "\n* CONFIGURATION *" << endl;
    cout << "Disk size: " << DISK_SIZE << endl;
    cout << "Head position: " << HEAD << endl;
    cout << "Request size: " << NUM_OF_REQ << endl;
    cout << "Direction: " << (direction == 1 ? "right" : "left") << endl;
    cout << "\n-----------------" << endl;

    // Display results
    cout << "\n* RESULTS *" << endl;
    cout << "Total request fetch: " << NUM_OF_REQ << endl;
    cout << "Request fetch sequence: [";

    for (int i = 0; i < NUM_OF_REQ; i++)
    {
        cout << req_processed_list[i];
        if (i != NUM_OF_REQ - 1)
        {
            cout << " --> ";
        }
    }

    cout << "]" << endl;
    cout << "\n-----------------" << endl;

    cout << "\nTotal seek time / head movements: " << total_seek_time << " units" << endl;
    cout << "Average seek time / head movements: " << fixed << setprecision(2) << ((double)total_seek_time / NUM_OF_REQ) << " units" << endl;
    cout << "Worst seek time / head movements: " << worst_seek_time << " units" << endl;
    cout << "Best seek time / head movements: " << best_seek_time << " units" << endl;
    cout << "\n-----------------" << endl;
    cout << "\nTotal seek count: " << NUM_OF_REQ << endl;
    cout << "Average seek count: " << 1 << endl;
    cout << "\n-----------------" << endl;

    delete[] req_processed_list;
}

// C-SCAN Algorithm

void C_SCAN(int req_list[], int direction, const int NUM_OF_REQ, const int DISK_SIZE, const int HEAD)
{
    int head = HEAD;
    int total_seek_time = 0, worst_seek_time = 0, index = 0;
    int best_seek_time = INT_MAX;
    int *req_processed_list = new int[NUM_OF_REQ]; // Temporary array to store processed requests
    double avg_seek_time = 0;

    // Perform insertion sort on the request array
    insertionSort(req_list, NUM_OF_REQ);

    if (direction == 1) // Right direction
    {
        for (int i = 0; i < NUM_OF_REQ; i++)
        {
            if (req_list[i] >= head)
            {
                req_processed_list[index++] = req_list[i];
                int seek_time = abs(req_list[i] - head);
                total_seek_time += seek_time;

                if (seek_time > worst_seek_time)
                    worst_seek_time = seek_time;
                if (seek_time < best_seek_time)
                    best_seek_time = seek_time;

                head = req_list[i];
            }
        }

        // Move to the end of the disk and then to the beginning
        int repositioning_time = (DISK_SIZE - 1 - head) + (DISK_SIZE - 1);
        head = 0;

        bool flag = false;
        for (int i = 0; i < NUM_OF_REQ; i++)
        {
            if (req_list[i] < HEAD)
            {
                if (!flag)
                {
                    total_seek_time += repositioning_time;

                    if (repositioning_time > worst_seek_time)
                        worst_seek_time = repositioning_time;

                    repositioning_time += req_list[i];

                    if (repositioning_time > worst_seek_time)
                        worst_seek_time = repositioning_time;

                    flag = true;
                }

                req_processed_list[index++] = req_list[i];

                int seek_time = abs(req_list[i] - head);
                total_seek_time += seek_time;

                if (seek_time > worst_seek_time)
                    worst_seek_time = seek_time;
                if (seek_time < best_seek_time)
                    best_seek_time = seek_time;

                head = req_list[i];
            }
        }
    }

    else // Left direction
    {

        for (int i = NUM_OF_REQ - 1; i >= 0; i--)
        {
            if (req_list[i] <= head)
            {
                req_processed_list[index++] = req_list[i];
                int seek_time = abs(req_list[i] - head);
                total_seek_time += seek_time;

                if (seek_time > worst_seek_time)
                    worst_seek_time = seek_time;
                if (seek_time < best_seek_time)
                    best_seek_time = seek_time;

                head = req_list[i];
            }
        }

        // Move to the start of the disk and then to the end
        int repositioning_time = head + (DISK_SIZE - 1);
        head = DISK_SIZE - 1;

        bool flag = false;
        for (int i = NUM_OF_REQ - 1; i >= 0; i--)
        {
            if (req_list[i] > HEAD)
            {
                if (!flag)
                {
                    total_seek_time += repositioning_time;
                    repositioning_time += (DISK_SIZE - 1 - req_list[i]);

                    if (repositioning_time > worst_seek_time)
                        worst_seek_time = repositioning_time;

                    flag = true;
                }

                req_processed_list[index++] = req_list[i];

                int seek_time = abs(req_list[i] - head);
                total_seek_time += seek_time;

                if (seek_time > worst_seek_time)
                    worst_seek_time = seek_time;
                if (seek_time < best_seek_time)
                    best_seek_time = seek_time;

                head = req_list[i];
            }
        }
    }

    // Calculate average values
    avg_seek_time = (double)total_seek_time / NUM_OF_REQ;

    // Display configurations
    cout << "---- C-SCAN ----" << endl;
    cout << "\n* CONFIGURATION *" << endl;
    cout << "Disk size: " << DISK_SIZE << endl;
    cout << "Head position: " << HEAD << endl;
    cout << "Request size: " << NUM_OF_REQ << endl;
    cout << "Direction: " << (direction == 1 ? "right" : "left") << endl;
    cout << "\n-----------------" << endl;

    // Display results
    cout << "\n* RESULTS *" << endl;
    cout << "Total request fetch: " << NUM_OF_REQ << endl;
    cout << "Request fetch sequence: [";

    for (int i = 0; i < NUM_OF_REQ; i++)
    {
        cout << req_processed_list[i];
        if (i != NUM_OF_REQ - 1)
        {
            cout << " --> ";
        }
    }

    cout << "]" << endl;
    cout << "\n-----------------" << endl;
    cout << "\nTotal seek time / head movements: " << total_seek_time << " units" << endl;
    cout << "Average seek time / head movements: " << avg_seek_time << " units" << endl;
    cout << "Worst seek time / head movements: " << worst_seek_time << " units" << endl;
    cout << "Best seek time / head movements: " << best_seek_time << " units" << endl;
    cout << "\n-----------------" << endl;
    cout << "\nTotal seek count: " << NUM_OF_REQ << endl;
    cout << "Average seek count: " << 1 << endl;
    cout << "\n-----------------" << endl;

    delete[] req_processed_list;
}

int main()
{
    int HEAD = 0, DISK_SIZE = 0, NUM_OF_REQ = 0;
    int direction = 0; // 1 for right, 0 for left

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

    int *req_list = new int[NUM_OF_REQ]; // Dynamic array for the list of requests

    do
    {
        cout << "Enter the direction of the disk head movement (1 for right, 0 for left): ";
        cin >> direction;

        if (direction != 0 && direction != 1)
        {
            cout << "\n[Invalid direction. Please enter 1 for right or 0 for left.]\n\n";
        }

    } while (direction != 0 && direction != 1);

    cout << "\n* Enter the request sequence *" << endl;

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

            for (int index = 0; index < i; index++)
            {
                if (req_list[i] == req_list[index])
                {
                    cout << "\n[Duplicate request. Please enter a unique request.]\n\n";
                    is_duplicate = true;
                    break;
                }
            }

        } while (req_list[i] < 0 || req_list[i] >= DISK_SIZE || is_duplicate);
    }

    SCAN(req_list, direction, NUM_OF_REQ, DISK_SIZE, HEAD);
    cout << endl;
    C_SCAN(req_list, direction, NUM_OF_REQ, DISK_SIZE, HEAD);

    delete[] req_list;

    return 0;
}