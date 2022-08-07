import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py FILENAME.csv FILENAME.txt")
        exit(1)

    if (sys.argv[1] == "databases/large.csv"):
        strs = ["AGATC", "TTTTTTCT", "AATG", "TCTAG", "GATA", "TATC", "GAAA", "TCTG"]
    else:
        strs = ["AGATC" , "AATG" , "TATC"]

    lista = []

    # TODO: Read database file into a variable
    with open(sys.argv[1], "r") as database:

        # Save file into variable, treat as Dict
        DictReader = csv.DictReader(database)

        # Iterate over each row and append dictionary in the list
        for row in DictReader:
            for tmp in range(len(strs)):
                row[strs[tmp]] = int(row[strs[tmp]])
            lista.append(row)

    # TODO: Read DNA sequence file into a variable
    dnaFile = open(sys.argv[2], "r")

    # Save file into variable, treat as reader
    reader = csv.reader(dnaFile)

    dna = ""
    for i in reader:
        for j in i:
            dna += j

    strCounts = []

    # TODO: Find longest match of each STR in DNA sequence
    for ij in range(len(strs)):
        strCounts.append(longest_match(dna, strs[ij]))

    # TODO: Check database for matching profiles
    for i in range(len(lista)):
        if sys.argv[1] == "databases/large.csv":
            if (lista[i][strs[0]] == strCounts[0] and
                lista[i][strs[1]] == strCounts[1] and
                lista[i][strs[2]] == strCounts[2] and
                lista[i][strs[3]] == strCounts[3] and
                lista[i][strs[4]] == strCounts[4] and
                lista[i][strs[5]] == strCounts[5] and
                lista[i][strs[6]] == strCounts[6] and
                lista[i][strs[7]] == strCounts[7]):
                print(lista[i]["name"])
                return
            else:
                continue
        else:
            if (lista[i][strs[0]] == strCounts[0] and
                lista[i][strs[1]] == strCounts[1] and
                lista[i][strs[2]] == strCounts[2]):
                print(lista[i]["name"])
                return
            else:
                continue

    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
