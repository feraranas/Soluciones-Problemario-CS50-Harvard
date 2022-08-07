from cs50 import get_float


def main():
    n = owed_money()
    owe = round(n * 100)
    
    count = 0
    
    quarters = 25
    dimes = 10
    nickels = 5
    pennies = 1
    
    while owe >= quarters:
        owe -= quarters
        count += 1
    
    while owe >= dimes:
        owe -= dimes
        count += 1
    
    while owe >= nickels:
        owe -= nickels
        count += 1
        
    while owe >= pennies:
        owe -= pennies
        count += 1
    
    print(count)

    
def owed_money():
    while True:
        owed = get_float("Money Owed: ")
        if owed > 0:
            break
    return owed


main()