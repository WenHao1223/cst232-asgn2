// C++ program to demonstrate
// C-SCAN Disk Scheduling algorithm
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <cmath>
#include <chrono>
using namespace std;
using namespace std::chrono;

void bubbleSort(int req_list[], const int REQ_SIZE)
{
    for (int i = 0; i < REQ_SIZE - 1; i++)
    {
        for (int j = 0; j < REQ_SIZE - i - 1; j++)
        {
            if (req_list[j] > req_list[j + 1])
            {
                int temp = req_list[j];
                req_list[j] = req_list[j + 1];
                req_list[j + 1] = temp;
            }
        }
    }

    // display the sorted requests
    cout << "* SORTED REQUESTS *" << endl;
    cout << "Sorted requests: [";
    for (int i = 0; i < REQ_SIZE; i++)
    {
        cout << req_list[i];
        if (i != REQ_SIZE - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;

    cout << "-----------------" << endl;
}

void C_SCAN(int req_list[], int req_fetched_seq[], int direction, const int REQ_SIZE, const int DISK_SIZE, const int HEAD)
{
    int head = HEAD;
    int total_seek_time = 0;
    int worst_seek_time = 0;      // in units
    int best_seek_time = INT_MAX; // in units
    int index = 0;

    // Sort the request array
    bubbleSort(req_list, REQ_SIZE);

    // Move towards the end of the disk in the given direction
    if (direction == 1) // right
    {
        for (int i = 0; i < REQ_SIZE; i++)
        {
            if (req_list[i] >= head)
            {
                req_fetched_seq[index++] = req_list[i];
                int seek_time = abs(req_list[i] - head);
                // cout << "Seek time: " << seek_time << endl;
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
        // cout << "Repositioning time (before): " << repositioning_time << endl;
        total_seek_time += repositioning_time;
        if (repositioning_time > worst_seek_time)
            worst_seek_time = repositioning_time;
        head = 0;

        bool flag = false;
        for (int i = 0; i < REQ_SIZE; i++)
        {
            if (req_list[i] < HEAD)
            {
                if (!flag)
                {
                    repositioning_time += req_list[i];
                    // cout << "Repositioning time (after): " << repositioning_time << endl;
                    if (repositioning_time > worst_seek_time)
                        worst_seek_time = repositioning_time;
                    flag = true;
                }
                req_fetched_seq[index++] = req_list[i];
                int seek_time = abs(req_list[i] - head);
                // cout << "Seek time: " << seek_time << endl;
                total_seek_time += seek_time;
                if (seek_time > worst_seek_time)
                    worst_seek_time = seek_time;
                if (seek_time < best_seek_time)
                    best_seek_time = seek_time;
                head = req_list[i];
            }
        }
    }
    else // left
    {
        for (int i = REQ_SIZE - 1; i >= 0; i--)
        {
            if (req_list[i] <= head)
            {
                req_fetched_seq[index++] = req_list[i];
                int seek_time = abs(req_list[i] - head);
                // cout << "Seek time: " << seek_time << endl;
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
        total_seek_time += repositioning_time;
        // cout << "Repositioning time (before): " << repositioning_time << endl;
        head = DISK_SIZE - 1;

        bool flag = false;
        for (int i = REQ_SIZE - 1; i >= 0; i--)
        {
            if (req_list[i] > HEAD)
            {
                if (!flag)
                {
                    repositioning_time += (DISK_SIZE - 1 - req_list[i]);
                    // cout << "Repositioning time (after): " << repositioning_time << endl;
                    if (repositioning_time > worst_seek_time)
                        worst_seek_time = repositioning_time;
                    flag = true;
                }
                req_fetched_seq[index++] = req_list[i];
                int seek_time = abs(req_list[i] - head);
                // cout << "Seek time: " << seek_time << endl;
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
    double avg_seek_time = (double)total_seek_time / REQ_SIZE;

    // display configuration
    cout << "* CONFIGURATION *" << endl;
    cout << "Disk size: " << DISK_SIZE << endl;
    cout << "Head position: " << HEAD << endl;
    cout << "Request size: " << REQ_SIZE << endl;
    cout << "Direction: " << (direction == 1 ? "right" : "left") << endl;
    cout << "-----------------" << endl;

    // Display results
    cout << "* RESULTS *" << endl;
    cout << "Total request fetch: " << REQ_SIZE << endl;
    cout << "Request fetch sequence: [";
    for (int i = 0; i < REQ_SIZE; i++)
    {
        cout << req_fetched_seq[i];
        if (i != REQ_SIZE - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;
    cout << "-----------------" << endl;
    cout << "Total seek time / head movements: " << total_seek_time << " units" << endl;
    cout << "Average seek time / head movements: " << avg_seek_time << " units" << endl;
    cout << "Worst seek time / head movements: " << worst_seek_time << " units" << endl;
    cout << "Best seek time / head movements: " << best_seek_time << " units" << endl;
    cout << "-----------------" << endl;
    cout << "Total seek count: " << REQ_SIZE << endl;
    cout << "Average seek count: " << 1 << endl;
    cout << "-----------------" << endl;
}

int main()
{
    int HEAD;      // in units
    int DISK_SIZE; // in units
    int REQ_SIZE;  // in units

    do
    {
        cout << "Enter the initial position of the disk head: ";
        cin >> HEAD;
        if (HEAD < 0)
        {
            cout << "Invalid head position. Please enter a non-negative number." << endl;
        }
    } while (HEAD < 0);

    do
    {
        cout << "Enter the total number of cylinders on the disk: ";
        cin >> DISK_SIZE;
        if (DISK_SIZE <= 0)
        {
            cout << "Invalid disk size. Please enter a positive number." << endl;
        }
    } while (DISK_SIZE <= 0);

    do
    {
        cout << "Enter the total number of requests: ";
        cin >> REQ_SIZE;
        if (REQ_SIZE <= 0)
        {
            cout << "Invalid request size. Please enter a positive number." << endl;
        }
    } while (REQ_SIZE <= 0);

    int *req_list = new int[REQ_SIZE];
    int *req_fetched_seq = new int[REQ_SIZE];
    int direction; // 1 for right, 0 for left

    do
    {
        cout << "Enter the direction of the disk head movement (1 for right, 0 for left): ";
        cin >> direction;
        if (direction != 0 && direction != 1)
        {
            cout << "Invalid direction. Please enter 1 for right or 0 for left." << endl;
        }
    } while (direction != 0 && direction != 1);

    cout << "* Enter the request sequence *" << endl;
    for (int i = 0; i < REQ_SIZE; i++)
    {
        // check if the request is within the disk's range and not a duplicate
        bool is_duplicate;
        do
        {
            is_duplicate = false;
            cout << "Request " << i + 1 << ": ";
            cin >> req_list[i];
            if (req_list[i] < 0 || req_list[i] >= DISK_SIZE)
            {
                cout << "Invalid request. Please enter a request within the disk's range." << endl;
            }
            for (int j = 0; j < i; j++)
            {
                if (req_list[i] == req_list[j])
                {
                    cout << "Duplicate request. Please enter a unique request." << endl;
                    is_duplicate = true;
                    break;
                }
            }
        } while (req_list[i] < 0 || req_list[i] >= DISK_SIZE || is_duplicate);
    }
    C_SCAN(req_list, req_fetched_seq, direction, REQ_SIZE, DISK_SIZE, HEAD);

    delete[] req_list;
    delete[] req_fetched_seq;

    return 0;
}
