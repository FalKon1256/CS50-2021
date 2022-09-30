
# Define recursion for pyramid
def block(layer):

    if layer == 0:
        return

    block(layer - 1)

    # Left-part of the pyramid
    print(" " * (spaceNum - layer), end="")
    print("#" * layer, end="")

    # Middle-part of the pyramid
    print(" " * 2, end="")

    # Right-part of the pyramid
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

# Build pyramid
spaceNum = h
block(h)