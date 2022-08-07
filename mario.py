from cs50 import get_int


def main():
    # Define the pyramid's height
    n = pyramid_height()

    # Print the pyramid
    for i in range(n):
        j = 0
        while j < n - (i + 1):
            print(" ", end="")
            j += 1
        
        k = 0
        while k <= i:
            print("#", end="")
            k += 1
        
        print("  ", end="")
        
        l = 0
        while l <= i:
            print("#", end="")
            l += 1
        
        print()


def pyramid_height():
    while True:
        height = get_int("Pyramid height: ")
        if height > 0 and height <= 8:
            break
    return height


main()