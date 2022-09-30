
# Prompting the user to type in the text
text = input("Text: ")

# Initialize variables for calculation
letter = sentence = 0
word = 1

for char in text:

    # Calculate letters
    if str.isupper(char) or str.islower(char):
        letter += 1

    # Calculate sentences
    # if (char == "." or "?" or "!"):
    if char == "." or char == "!" or char == "?":
        sentence += 1

    # Calculate words
    if char == " ":
        word += 1

# Calculate L & S for Coleman-Liau formula
L = (letter * 100) / word

S = (sentence * 100) / word

index = round(0.0588 * L - 0.296 * S - 15.8)

# Determine grading
if index >= 16:
    print("Grade 16+")

elif index < 1:
    print("Before Grade 1")

else:
    print(f"Grade: {index}")