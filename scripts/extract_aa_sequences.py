import json


def get_aa(filename):

    """
    Parses a FASTA file containing protein sequences and stores them in a JSON dictionary.

    Input:
        filename (str): Path to the FASTA file.

    Output:
        Creates a JSON file 'aa_dict.json' with structure:
        {
            "protein_sequence": [">protein_id"]
        }
        where the same sequence may be associated with multiple headers.
    """

    current_header = '' # Current FASTA .faa header (starts with '>')
    current_sequence = ''   # Accumulated amino acid sequence
    aa_dict = {}    # Dictionary: {sequence: [list of headers]}




    # Read the FASTA file line by line
    with open ('./'+filename, 'r') as outfile:
        for i in outfile:
            if '>' in i:
                if current_sequence in aa_dict:
                    (aa_dict.get(current_sequence)).append(current_header)
                else:
                    aa_dict[current_sequence] = [current_header]
                current_header = i.replace('\n', '')
                current_sequence = ''
            else:
                current_sequence = current_sequence + i.replace('\n', '')


    with open("./aa_dict.json", 'w') as jsnfile:
        json.dump(aa_dict, jsnfile)


    
