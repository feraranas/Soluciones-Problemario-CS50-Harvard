from cs50 import get_int


def main():
    # Prompt the user for a Credit Card Number
    while True:
        ccn = get_int("Credit Card Number: ")
        if ccn > 0:
            break
        
    # Transform CCN to String
    ccni = str(ccn)
    
    # Encuentra la longitud del CCN
    longitud = len(ccni)
        
    # Helper function check_sum runs the luhn's algorithm
    if check_sum(longitud, ccni):
        if (ccn >= 34e13 and ccn < 35e13) or (ccn >= 37e13 and ccn < 38e13):
            print("AMEX")
                
        elif (ccn >= 51e14 and ccn < 56e14):
            print("MASTERCARD")
            
        elif (ccn >= 4e12 and ccn < 5e12) or (ccn >= 4e15 and ccn < 5e15):
            print("VISA")
            
        else:
            print("INVALID")
                
    else:
        print("INVALID")
    
    
def check_sum(l, b):

    # Declare three variables to cero. The two sums and a counter
    pares = 0
    impares = 0
    
    for i in (b):
        if l % 2 == 0:
            a = int(i) * 2
            casted = str(a)
            for i in (casted):
                impares += int(i)
            l += 1
            
        else:
            pares += int(i)
            l -= 1

    if (pares + impares) % 10 == 0:
        return True
    else:
        return False


main()