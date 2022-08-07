#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
// Represents an array of all of the pairs of candidates
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    /*
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                printf("Candidate %i name: %s vs Candidate %i name: %s\n", i, candidates[i], j, candidates[j]);
                printf("Votes: %i vs Votes %i\n", preferences[i][j], preferences[j][i]);
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                printf("Candidate %i name: %s vs Candidate %i name: %s\n", i, candidates[i], j, candidates[j]);
                printf("Votes: %i vs Votes %i\n", preferences[i][j], preferences[j][i]);
            }
        }
    }
    */

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();

    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    /* TODO (Alternative implementation)
        Updates this function:
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if ((i == j) || (i > j))
            {
                // No se hace nada
            }
            else if (i < j )
            {
                preferences[ranks[i]][ranks[j]]++;
            }
        }
    }
    */

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    /* TODO: Tally
        Once all of the voters have indicated all of their preferences, determine, for each pair of candidates, who the preferred candidate is and by what margin they are preferred.

        First:
        The function should update the global variable pair_count to be the number of pairs of candidates. (The pairs should thus all be stored between pairs[0] and pairs[pair_count - 1], inclusive).

                    Meaning, we should update --> int pair_count;
                        Updating the global variable pair_count
                             pair_count = candidate_count * (candidate_count - 1) / 2;

        Second:
        Add all pairs of candidates where one candidate is preferred over the other (to the pairs array)
            If a pair of candidates are tied (one is not preferred over the other) should not be added to the array.

                    Meaning, we should update --> pair pairs[36]; this is ->
                    int pairs[36].winner = index of the candidate
                    int pairs[].loser
    */
    // Para cada par de candidatos
    // int dif[candidate_count * (candidate_count - 1) / 2];
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                // dif[pair_count] = preferences[i][j] - preferences[j][i];
                // This next two lines determine who the prefered candidate is
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                // dif[pair_count] = preferences[j][i] - preferences[i][j];
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
            else
            {
                // What are u going to do bout it, eh mate?
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    /* TODO: Sort
        Sort the pairs of candidates in decreasing order of strength of victory, where strength of victory is defined to be the number of voters who prefer the preferred candidate.

        The function should sort the pairs array in decreasing order of strength of victory, where strength of victory is defined to be the number of voters who prefer the preferred candidate.
            Del mayor número al menor numero
                Cada numero se determina por la diferencia de votos entre el ganador y el perdedor

    If multiple pairs have the same strength of victory, you may assume that the order does not matter.
            Para pares con la misma diferencia pueden ir como sea en el ordern


    Para cada par guardado actualmente en pairs[i] de (i = 0) a (i = n - 1)

    pairs[0].winner = i
    pairs[0].loser = j

    pairs[1].winner = i
    pairs[2].loser = j

    .
    .
    .

    pairs[pair_count - 1].winner = i
    pairs[pair_count - 1].loser = j
    */

    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count - 1 - i; j++)
        {
            int a = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            int b = preferences[pairs[j + 1].winner][pairs[j + 1].loser] - preferences[pairs[j + 1].loser][pairs[j + 1].winner];
            if (a < b)
            {
                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = pairs[j];
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    /* TODO: Lock:
    Starting with the strongest pair, go through the pairs of candidates in order and “lock in” each pair to the candidate graph, so long as locking in that pair does not create a cycle in the graph.

    This algorithm must be careful to avoid creating cycles in the candidate graph
    The algorithm locks in the strongest edges first, since those are arguably the most significant
    The algorithm specifies that matchup edges should be “locked in” to the graph one at a time -> based on the “strength” of the victory (the more people who prefer a candidate over their opponent, the stronger the victory).
    So long as the edge can be locked into the graph without creating a cycle, the edge is added; otherwise, the edge is ignored.

    */

    /*
    for (int j = 0; j < pair_count; j++)
    {
         // First check whether locking in a pair doesn't create a cycle in the graph
        // How can I check that there's no edges pointing towards it...
        // Well first I want to know the source of the graph, this is, there's no edges pointing at it so
        if ()
        {
            // Lock in each pair (pretty sure this is right, and iterating among all the pairs will lock the graph correctly. BUT it doesn't check if locking would create a cycle)
            locked[pairs[j].winner][pairs[j].loser] = true;
        }
        else
        {
            // the edge is ignored
        }
    }
    */
    return;
}

// Print the winner of the election
void print_winner(void)
{
    /* TODO
    The function should print out the name of the candidate who is the source of the graph.
    You may assume there will not be more than one source.
    Once the graph is complete, the source of the graph (the one with no edges pointing towards it) is the winner!
    */
    return;
}

