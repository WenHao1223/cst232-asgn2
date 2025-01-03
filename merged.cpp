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
    int i, key, j;
    for (i = 1; i < NUM_OF_REQ; i++)
    {
        key = req_list[i];
        j = i - 1;
        while (j >= 0 && req_list[j] > key)
        {
            req_list[j + 1] = req_list[j];
            j = j - 1;
        }
        req_list[j + 1] = key;
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

// void SCAN(int req_list[], int direction, const int NUM_OF_REQ, const int DISK_SIZE, const int HEAD)
// {
//     int totalMov = 0, mov;
//     int left = 0, right = 0;
//     int fetchSeq[queueSize];
//     int j = 0;
//     int worst, best;

//     for (int i = 0; i < queueSize; i++)
//     {
//         if (reqQueue[i] < head)
//         {
//             left++;
//         }
//         if (reqQueue[i] > head)
//         {
//             right++;
//         }
//     }

//     if (direction == "Right")
//     {
//         worst = abs(reqQueue[left] - head);
//         best = abs(reqQueue[left] - head);

//         // Move to the right req
//         for (int i = left; i < queueSize; i++)
//         {
//             mov = abs(reqQueue[i] - head);
//             totalMov = totalMov + mov;

//             if (mov > worst)
//                 worst = mov;

//             if (mov < best)
//                 best = mov;

//             head = reqQueue[i];
//             fetchSeq[j] = reqQueue[i];
//             j++;
//         }

//         if (left > 0)
//         {
//             // Then move to the left
//             mov = abs(totalTracks - 1 - head);
//             totalMov = totalMov + mov;
//             head = totalTracks - 1;

//             mov = mov + abs(reqQueue[left - 1] - head);

//             if (mov > worst)
//                 worst = mov;

//             if (mov < best)
//                 best = mov;

//             for (int i = left - 1; i >= 0; i--)
//             {
//                 totalMov = totalMov + abs(reqQueue[i] - head);

//                 if (mov > worst)
//                     worst = mov;

//                 if (mov < best)
//                     best = mov;

//                 head = reqQueue[i];
//                 fetchSeq[j] = reqQueue[i];
//                 j++;

//                 if ((i - 1) > 0)
//                     mov = abs(reqQueue[i - 1] - head);
//             }
//         }
//     }

//     else
//     {
//         worst = abs(reqQueue[left - 1] - head);
//         best = abs(reqQueue[left - 1] - head);

//         for (int i = left - 1; i >= 0; i--)
//         {
//             totalMov = totalMov + abs(reqQueue[i] - head);
//             mov = abs(reqQueue[i] - head);

//             if (mov > worst)
//                 worst = mov;

//             if (mov < best)
//                 best = mov;

//             head = reqQueue[i];
//             fetchSeq[j] = reqQueue[i];
//             j++;
//         }

//         if (right > 0)
//         {
//             // Then reverse direction and move right
//             mov = abs(head);
//             totalMov = totalMov + mov;
//             head = 0;

//             mov = mov + abs(reqQueue[left] - head);

//             // Move to the right req
//             for (int i = left; i < queueSize; i++)
//             {

//                 totalMov = totalMov + abs(reqQueue[i] - head);

//                 if (mov > worst)
//                     worst = mov;

//                 if (mov < best)
//                     best = mov;

//                 head = reqQueue[i];
//                 fetchSeq[j] = reqQueue[i];
//                 j++;

//                 if ((i + 1) < queueSize)
//                     mov = abs(reqQueue[i + 1] - head);
//             }
//         }
//     }

//     cout << "\nFetch Sequence: ";
//     for (int i = 0; i < queueSize; i++)
//     {
//         cout << fetchSeq[i] << " ";
//     }

//     cout << "\nTotal seek time / head movements: " << totalMov << endl;
//     cout << "Average seek time / head movements: " << fixed << setprecision(2) << ((double)totalMov / queueSize) << endl;
//     cout << "Worst seek time / head movements: " << worst << endl;
//     cout << "Best seek time / head movements: " << best << endl;
// }

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
    cout << "* CONFIGURATION *" << endl;
    cout << "Disk size: " << DISK_SIZE << endl;
    cout << "Head position: " << HEAD << endl;
    cout << "Request size: " << NUM_OF_REQ << endl;
    cout << "Direction: " << (direction == 1 ? "right" : "left") << endl;
    cout << "-----------------" << endl;

    // Display results
    cout << "* RESULTS *" << endl;
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
    cout << "-----------------" << endl;
    cout << "Total seek time / head movements: " << total_seek_time << " units" << endl;
    cout << "Average seek time / head movements: " << avg_seek_time << " units" << endl;
    cout << "Worst seek time / head movements: " << worst_seek_time << " units" << endl;
    cout << "Best seek time / head movements: " << best_seek_time << " units" << endl;
    cout << "-----------------" << endl;
    cout << "Total seek count: " << NUM_OF_REQ << endl;
    cout << "Average seek count: " << 1 << endl;
    cout << "-----------------" << endl;

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

    C_SCAN(req_list, direction, NUM_OF_REQ, DISK_SIZE, HEAD);

    delete[] req_list;

    return 0;
}