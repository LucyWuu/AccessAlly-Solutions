/*
=============================================================================================
Lucy Wu's Q2 Submission

Specifications:
---------------
    The first line of input contains 8 integers: the number of *units of blood* of Type O negative,
    O positive, A negative, A positive, B negative, B positive, AB negative and AB positive.
    This is followed by a line with the same blood type order, the numbers resemble the patients
    of those blood types.
    Each of these integers will be at least 0 and less than 10^7.

Thought Process
----------------
    To find the maximum number of patients that can receive blood, I would add up the maximum
    number of patients receiving blood from each blood type: O-, O+, A-, A+, B-, B+, AB-, AB+.

    Few things I've noted:
    - The inputs are ordered from the most applicable blood type (O) to the most restricted
    blood type (AB) - (O, A, B, AB, with A, B in swappable order).
    - I should divide the blood types into two categories: negative and positive, because
    negative blood types cannot take positive blood types.
    - From the example given, the approach seems to be first exhausting the blood units with
    their native blood patients, in the order discussed above (O to AB). If not enough native
    blood units are available, refer back to the less restricted blood types, and check whether
    there are leftovers as substitutes for the current blood type patients I'm dealing with.
    - Storing the data in four sets of arrays (neg_units, neg_patients, pos_units, pos_patients)
    would make parallel comparisons and tracing back easier.
    - Blood type A and B units cannot substitute each other, despite one being placed before
    another.
    - In the end, after going through the positive and negative parallel arrays, I can trace
    through neg_units with pos_patients using the same algorithm, because positive patients
    can also take negative blood units.


Example Case
-------------
Sample Input
5 5 3 1 2 11 5 12
2 4 9 2 3 9 7 3

Output for Sample Input
33

Explanation
2 Type O- patients receive Type O- blood
4 Type O+ patients receive Type O+ blood
3 Type A- patients receive Type A- blood
3 Type A- patients receive Type O- blood
1 Type A+ patients receive Type A+ blood
1 Type A+ patients receive Type O+ blood
2 Type B- patients receive Type B- blood
9 Type B+ patients receive Type B+ blood
5 Type AB- patient receives Type AB- blood
3 Type AB+ patients receive Type AB+ blood

Unsolved Questions
-------------------
Does the order matter for A and B?
This is a greedy approach, any ways to optimize the algorithm?
Implement using heap instead of creating arrays of length 100.
Implemented solution is off by one from the example case.
=============================================================================================
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// helper function to convert input from str to int + store them

void conversion(char *str, int *store)
{
    char *token;
    int counter = 0;

    token = strtok(str, " ");
    while (token != NULL)
    {
        store[counter] = atoi(token);
        token = strtok(NULL, " ");
        ++counter;
    }
    counter = 0;
}

// main function
int main(void)
{
    // initialize variables
    const int size = 4;
    char s_units[100];
    int units[8];
    char s_patients[100];
    int patients[8];

    int neg_units[size];
    int neg_patients[size];
    int pos_units[size];
    int pos_patients[size];
    int neg_posn = 0;
    int pos_posn = 0;
    int count = 0;

    // obtain and store inputs - two lines
    fgets(s_units, 100, stdin);
    fgets(s_patients, 100, stdin);

    conversion(s_units, units);
    conversion(s_patients, patients);

    // split the input data into their respective arrays
    for (int i = 0; i < 8; ++i)
    {
        if (i % 2 == 0)
        {
            neg_units[neg_posn] = units[i];
            neg_patients[neg_posn] = patients[i];
            ++neg_posn;
        }
        else
        {
            pos_units[pos_posn] = units[i];
            pos_patients[pos_posn] = patients[i];
            ++pos_posn;
        }
    }


    // try to satisfy in negative blood patients first:
    for (int i = 0; i < size; ++i)
    {
        // when blood units >= patients
        if (neg_units[i] >= neg_patients[i])
        {
            count += neg_patients[i];
            neg_units[i] -= neg_patients[i];
            neg_patients[i] = 0;
        }
        else
        { // otherwise when blood units < patients, track backwards using another loop
            for (int j = i; j >= 0; --j)
            {
                // exclude the case of A substituing B
                if (j == 1 && i == 2)
                {
                    continue;
                }
                // stop when enough substitutes are found
                if (neg_patients[i] == 0)
                {
                    break;
                }
                // otherwise if there are substitutes, use them
                if (neg_units[j] < neg_patients[i])
                {
                    count += neg_units[j];
                    neg_patients[i] -= neg_units[j];
                    neg_units[j] = 0;
                }
            }
        }
    }

    // next, try to satisfy in positive blood patients:
    for (int i = 0; i < size; ++i)
    {
        // when blood units >= patients
        if (pos_units[i] >= pos_patients[i])
        {
            count += pos_patients[i];
            pos_units[i] -= pos_patients[i];
            pos_patients[i] = 0;
        }
        else
        { // otherwise when blood units < patients, track backwards using another loop
            for (int j = i; j >= 0; --j)
            {
                // exclude the case of A substituing B
                if (j == 1 && i == 2)
                {
                    continue;
                }
                // stop when enough substitutes are found
                if (pos_patients[i] == 0)
                {
                    break;
                }
                // otherwise if there are substitutes in positives, use them
                if (pos_units[j] < pos_patients[i])
                {
                    count += pos_units[j];
                    pos_patients[i] -= pos_units[j];
                    pos_units[j] = 0;
                }

                // if still not satisfied, check the remaining negative units as substitutes
                if (pos_patients[i] > 0)
                {
                    if (neg_units[j] < pos_patients[i])
                    {
                        count += neg_units[j];
                        pos_patients[i] -= neg_units[j];
                        neg_units[j] = 0;
                    }
                    else
                    {
                        count += pos_patients[i];
                        neg_units[j] -= pos_patients[i];
                        pos_patients[i] = 0;
                    }
                }
            }
        }
    }

    printf("%d\n", count);

}

// Time complexity: O(n^2)