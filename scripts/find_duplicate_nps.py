import json


def duplicate ():

    """
    Identifies NP IDs that have more than one associated amino acid sequence.

    Input: 
        Reads from 'aa_dict.json' — a JSON dictionary where keys are NP IDs 
        and values are lists of amino acid sequences.

    Output:
        Writes 'np_duplicates.json' containing only NP IDs with multiple sequences.
    """


    double_dict = {} # Dictionary to store NP IDs with multiple AA sequences
    np_list = []

    with open ('./aa_dict.json', 'r') as outfile:
        data = json.load(outfile)
    n = 0


    for i in data:
        if len(data.get(i)) == 1:
            continue
        else:
            double_dict[i] = data.get(i)
            for np in data.get(i):
                np_list.append((np.split()[0]).replace(">", ""))
                n = n + 1
           

    with open("./np_duplicates.json", 'w') as jsnfile:
        json.dump(double_dict, jsnfile)

    with open('./np_list.txt', 'w') as np_file:
        np_file.write('\n'.join(np_list))