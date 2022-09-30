
# Define Recursion for half-pyramid
def block(layer):

    if layer == 0:
        return

    block(layer - 1)

    print(" " * (spaceNum - layer), end="")
    print("#" * layer, end="")
    print()


# Prompting the user to type in positive interger
while True:

    try:
        h = int(input("Height: "))

    # Re-prompting the user to type in valid interger
    except:
        continue

    if 1 <= h <= 8:
        break

    '''Another method:
    from cs50 import get_int
    h = get_int("Height: ")'''

# Build half-pyramid
spaceNum = h
block(h)