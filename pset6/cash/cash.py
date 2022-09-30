
# Prompting the user to type in positive interger
while True:

    try:
        dollars = float(input("Change owed: "))

    # Re-prompting the user to type in valid interger
    except:
        continue

    if dollars > 0:
        break

# Round dollars to cents
cents = round(dollars * 100)


Q = D = N = P = 0

# Quarters change
if cents >= 25:
    Q = int(cents / 25)
    cents %= 25

# Dimes change
if cents >= 10:
    D = int(cents / 10)
    cents %= 10

# Nickels change
if cents >= 5:
    N = int(cents / 5)
    cents %= 5

# Pennies change
if cents >= 1:
    P = int(cents / 1)
    cents %= 1

# Print miniumum number of coins
if cents == 0:
    print(Q + D + N + P)