#!/usr/bin/python
with open('output-test', 'r') as infile:
    # Open the output file for writing
    with open('output_test_off_only', 'w') as outfile:
        # Process each line in the input file
        for line in infile:
            # Write the first 9 characters of each line to the output file
            outfile.write(line[:9] + '\n')
