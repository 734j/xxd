#!/usr/bin/python3
with open('output-TESTING', 'r') as infile:
    with open('output_TESTING_off_only', 'w') as outfile:
        for line in infile:
            print(f"Original line: {repr(line)}")  # Debug: Show full line including invisible characters
            print(f"Sliced part: {repr(line[:9])}")  # Debug: Show the sliced part
            outfile.write(line[:9] + '\n')
