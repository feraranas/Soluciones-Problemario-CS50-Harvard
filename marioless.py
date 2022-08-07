from cs50 import get_int


def main():
    # Define the pyramid's height
    n = pyramid_height()

    # Print the pyramid
    for i in range(n):
        for j in range(n):
            if i + j < n - 1:
                print(" ", end="")
            else:
                print("#", end="")
        print()


def pyramid_height():
    while True:
        height = get_int("Pyramid height: ")
        if height > 0 and height <= 8:
            break
    return height

main()