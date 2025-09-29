import json


def get_np (filename):

    """
    Extracts CDS (coding sequence) annotations from a GFF file for a list of NP IDs.

    Input:
        filename (str): Path to the GFF3 annotation file.
        Reads NP IDs from 'np_list.txt'.

    Output:
        Writes a JSON file 'cds_coordinates.json' with structure:
        {
            "NP_ID": [[GFF_field1, GFF_field2, ..., GFF_field9], ...]
        }
    """

    # Read NP IDs from np_list.txt
    with open ('./np_list.txt', 'r') as outfile:
        for i in outfile:
            continue

    new_data = {}

    # Read and parse the GFF file
    with open("./"+filename, 'r') as gff_file:
        for gff_str in gff_file:
            a = gff_str.split()
            if "##" in a[0]:
                continue
            elif "#!" in a[0]:
                continue
            else:
                if a[2] == 'CDS':
                    b = a[8].split(';')   
                    if b[0].replace('ID=cds-', '') in i:
                        if b[0].replace('ID=cds-', '') in new_data:
                            
                            new_data.get(b[0].replace('ID=cds-', '')).append(a)
                           
                        else:
                            
                            new_data[b[0].replace('ID=cds-', '')] = [a]
                        
    with open("./cds_coordinates.json", 'w') as jsnfile:
        json.dump(new_data, jsnfile)