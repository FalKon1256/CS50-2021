import csv
import sys
import copy


def STR_count(sequenceText, STR):
    
    # Initialize count
    MAXcount = 0
    count = 0
    
    # Define a place marker
    mark = 0
    
    # Search for Maximum sequence count
    while ((len(sequenceText) - 1) - mark) >= len(STR):
        
        # Search for 1 char each step when first part of STR sequence not found
        if count == 0:
            
            S = sequenceText[mark: (mark + len(STR))]
            
            # Sequence found
            if S == STR:
                count += 1
                
                if count > MAXcount:
                    MAXcount = count
                
                mark += len(STR)
                continue
            
            # Sequence not found
            mark += 1
            continue
            
        # Search for STR-lenth chars each step when first part of STR sequence found
        else:
            
            S = sequenceText[mark: (mark + len(STR))]
            
            # Sequence interrupted
            if S != STR:
                count = 0
                
                mark += 1
                continue
                
            # Sequence continues
            count += 1
            
            if count > MAXcount:
                MAXcount = count
            
            mark += len(STR)
            continue
    
    # Return Maximum sequence count for each STR
    return MAXcount


def main():
    
    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")
    
    # Read database csv files and save as dict
    database = []
    with open(sys.argv[1], "r") as file:
        databaseFile = csv.DictReader(file)
        
        for row in databaseFile:
            database.append(row)

    # Read DNA sequences csv files and save as list (strip the line change symbol)
    with open(sys.argv[2], "r") as file:
        sequence = file.read().strip("\n")
    
    # Create and initailize a dict for STR names
    strDict = database[0].copy()
    strDict.pop("name")
    
    # Counts the number of each STR by comparing STR name against input DNA sequences file (change string to interger)
    for key in strDict:
        strDict[key] = STR_count(sequence, key)
    
    # Compare DNA sequences file against database to find the person
    for person in database:
        
        # Copy template for matching of each person (change string to interger)
        tmp = person.copy()
        tmp.pop("name")
        
        for key in tmp:
            tmp[key] = int(tmp[key])
        
        # Person name matched
        if tmp == strDict:
            print(person["name"])
            break
        
        # No match for finding
        if person == database[-1]:
            print("No match")


if __name__ == "__main__":
    main()