

def cds_coord():

    """
    Compares CDS coordinates for duplicated NP entries to identify structural differences.

    Input:
        - 'cds_coordinates.json': CDS annotations per NP
        - 'np_duplicates.json': NP IDs with duplicated AA sequences

    Output:
        - 'cds_comparison_result.json': Stores distinct CDS structure for each duplicated NP group.
    """

    list_of_element = {'nc':[]} # Optional structure to log pairwise comparisons
    list_aa = {} # Final dictionary of differences to write

    import json

    with open("./cds_coordinates.json", 'r') as data_cds_file:
        data_cds_data = json.load(data_cds_file)

        with open("./np_duplicates.json", 'r') as cds_file:
            cds_data = json.load(cds_file)

            
            for elem in cds_data:
                element_np = cds_data.get(elem)
                for i in range(len(element_np)):
                    for j in range(i + 1, len(element_np)):
                        element_1 = (element_np[i].split()[0]).replace('>', '') 
                        element_2 = (element_np[j].split()[0]).replace('>', '')
                        new_element_1 = []
                        if data_cds_data == {}:
                            continue
                        else:
                            for new_el_1 in data_cds_data.get(element_1):
                                new_element_1.append(new_el_1[3]+'_'+new_el_1[4])
                            new_element_2 = []
                            for new_el_2 in data_cds_data.get(element_2):
                                new_element_2.append(new_el_2[3]+'_'+new_el_2[4])
                            if new_element_1 == new_element_2:
                                continue
                            else:
                                if elem in list_aa:
                                    if [(new_el_1[8].split(';'))[0].replace('ID=cds-', ''), new_el_1[0], new_el_1[6], new_element_1] in list_aa.get(elem):
                                        continue
                                    else:
                                        list_aa.get(elem).append([(new_el_1[8].split(';'))[0].replace('ID=cds-', ''), new_el_1[0], new_el_1[6], new_element_1])
                                    if [(new_el_2[8].split(';'))[0].replace('ID=cds-', ''), new_el_2[0], new_el_2[6], new_element_2] in list_aa.get(elem):
                                        continue
                                    else:
                                        list_aa.get(elem).append([(new_el_2[8].split(';'))[0].replace('ID=cds-', ''), new_el_2[0], new_el_2[6], new_element_2])
                                else:
                                    list_aa[elem] = [[(new_el_1[8].split(';'))[0].replace('ID=cds-', ''), new_el_1[0], new_el_1[6], new_element_1], [(new_el_2[8].split(';'))[0].replace('ID=cds-', ''), new_el_2[0], new_el_2[6], new_element_2]]
                                new_list = [(new_el_1[8].split(';'))[0], (new_el_2[8].split(';'))[0], new_element_1, new_element_2]
                                (list_of_element.get('nc')).append(new_list)

  
    with open("./cds_comparison_result.json", 'w') as jsnfile1:
        json.dump(list_aa, jsnfile1)

