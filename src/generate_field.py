import random
import sys


HEIGHT = int(sys.argv[1])
WIDTHWEIGHT = int(sys.argv[2])

FILE = 'maps/field.txt'

if len(sys.argv) == 4:
    FILE = sys.argv[3]


with open(FILE, 'w') as file:
    for i in range(HEIGHT):
        for j in range(WIDTHWEIGHT):
            file.write(str(random.randint(0, 1)))
        file.write('\n')
