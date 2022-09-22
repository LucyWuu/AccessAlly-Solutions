/* 
=============================================================================================
Lucy Wu's Q1 Submission

Specifications:
---------------
    Input contains one integer D (0 ≤ D ≤ 1 000 000 000), which represents the duration 
    that the clock is observed - IN MINUTES (integer). 

Thought Process
----------------
    Essentially, to find arithmetic sequences, I have to make sure the differences between
    each digit of a specific time are the same. For example, 12:34 has digits 1, 2, 3, 4, 
    and 1-2 = 2-3 = 3-4. I can check the differences by storing each digit in a variable, 
    and performing operations. I can also have a variable - count - to keep track of the 
    arithmetic sequences.
    There are two cases we can check: 
        1. the time has 4 digits (e.g. 12:34)
        2. the time has 3 digits (e.g. 8:03)
        I would make 4 int variables to store the digits: d1, d2, d3, d4
    For case 2, d1 would be 0, and I only need to check the differences between d2, d3,
    and d4.
    

    To check each time number within the given time, I can run a for loop that goes through
    the minutes one by one, until we reach the given time D.

    To represent the time, I can use 2 variables, hrs = 12 and mins = 0. For each loop, I 
    would increase mins by one, and check whether it reaches 60 - in that case, I would 
    increase hrs by 1, and reset mins to 0. I would also need to check that hrs stay in
    the 12 hr cycle - a simple modulo 12 should do the trick.

    In terms of how to extract the time from hrs and mins to d1, d2, d3, and d4, I can
    treat it as two two-digit numbers - division and modulo by 10 would give me the tens
    and ones. 

    Once the time is extracted, all I need to do is the checking as mentioned above.

    After some coding, I realized that I can simplify this problem further. Since every 12
    hours is a cycle, I can divide time D by 720 (12 hours) and only calculate the remaining
    time counts. As for each cycle, there are tested to be 31 arithmetic sequences. So I can
    simply multiply the number of cycles by 31 and add the counts from the remaining time.


Example Cases
-------------
34 -> 1
    Between 12:00 and 12:34, there is only the time 12:34 for which the digits form an
    arithmetic sequence.

180 -> 11
    Between 12:00 and 3:00 (180 mins), there are 11 arithmetic sequences.
=============================================================================================
*/

#include <stdio.h>
int main(void) {
    // initialize variables
	int hrs = 12;
	int mins = 0;
	int count = 0;

    // obtain input D and store it in an int
	int D; 
	scanf("%d", &D);

    // simplify the problem as discussed above
	int cycle = D / 720;
	int mins_left = D % 720;

    // iterate through each minute
	for (int i = 0; i < mins_left; i++) {
        // update the time once it reaches 60 mins or 12 hrs
		if ((++mins) >= 60) {
			mins = 0;
			++hrs;
			hrs = hrs % 12;
		}

        // define d1, d2, d3, d4
		int d1 = hrs / 10;
		int d2 = hrs % 10;
		int d3 = mins / 10;
		int d4 = mins % 10;

        // split into two cases - 4 or 3 digits - and check the differences
		if (d1 > 0) {
			if (d1 - d2 == d2 - d3 && d2 - d3 == d3 - d4)
                // if they share a common difference, count +1
				count++;
		}
		else {
			if (d2 - d3 == d3 - d4)
                // if they share a common difference, count +1
				count++;
		}
	}

    // tally up the counts with the cycles, each containing 31 counts
	printf("%d\n", 31 * cycle + count);

	return 0;
}

// time complexity: O(n), where n is mins_left.
