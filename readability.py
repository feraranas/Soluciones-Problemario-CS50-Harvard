from cs50 import get_string


def main():
    s = get_string("Prompt: ")

    letters = count_letters(s)
    words = count_words(s)
    sentences = count_sentences(s)
    n = coleman_index(letters, words, sentences)

    if n < 1:
        print("Before Grade 1")
    elif n >= 1 and n <= 16:
        print(f"Grade {n}")
    else:
        print("Grade 16+")


def coleman_index(a, b, c):
    L = (a * 100) / b
    S = (c * 100) / b
    return round(0.0588 * L - 0.296 * S - 15.8)


def count_letters(parrafo):
    countl = 0
    for i in (parrafo):
        if i.isalpha():
            countl += 1
    return countl


def count_words(parraf):
    countw = 0
    for i in (parraf):
        if i == " ":
            countw += 1
    return countw + 1


def count_sentences(prompt):
    counts = 0
    for i in (prompt):
        if i == "!" or i == "." or i == "?":
            counts += 1
    return counts


main()