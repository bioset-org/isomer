import json


def delete_dubl():

    """
    Identifies and removes NP entries that have fully identical or overlapping CDS coordinates.

    Input:
        - '2last_cds_comparison_result.json': structured CDS data per NP

    Output:
        - 'final_result.json': filtered data without coordinate duplicates
    """

    newdata = {}
    with open("./short_comparison_result.json", 'r') as jsnfile:
        data_data = json.load(jsnfile)
         # Iterate through chromosomes (e.g., NC_003070.9)
        for nc_elem in data_data:
            nc_element = data_data.get(nc_elem) 
            # Iterate through NP groups (each is a list of CDS records)
            for np_elem in nc_element: 
                np_element = nc_element.get(np_elem) 
                comp_list_coord = []
                comp_list_np = []
                # Compare each NP against all others (pairwise)
                for np_list_elem1 in range(len(np_element)): 
                    coord_list = np_element[np_list_elem1][2:]
                    list_of_coord_1 = []
                    for coord in coord_list:
                        split_coord_list = (coord.split('_'))
                        for split_coord in split_coord_list:
                            list_of_coord_1.append(int(split_coord))
                    sorted_list1 = sorted(list_of_coord_1) 
                   
                    for np_list_elem2 in range(np_list_elem1 + 1, len(np_element)):
                        
                        coord_list2 = np_element[np_list_elem2][2:]
                        list_of_coord_2 = []
                        for coord2 in coord_list2:
                            split_coord_list2 = (coord2.split('_'))
                            for split_coord2 in split_coord_list2:
                                list_of_coord_2.append(int(split_coord2))
                        sorted_list2 = sorted(list_of_coord_2) 
                        if sorted_list1 == sorted_list2:
                            continue
                        # Check for partial overlap using range comparison
                        else:
                            range_sorted_list1 = list(range(sorted_list1[0], sorted_list1[-1]+1))
                            if sorted_list2[0] in range_sorted_list1:
                                if sorted_list1 in comp_list_coord:
                                    continue
                                else:
                                    comp_list_coord.append(sorted_list1)
                                    comp_list_np.append(np_element[np_list_elem1])
                                if sorted_list2 in comp_list_coord:
                                    continue
                                else:
                                    comp_list_coord.append(sorted_list2)
                                    comp_list_np.append(np_element[np_list_elem2])
                            elif sorted_list2[-1] in range_sorted_list1:
                                if sorted_list1 in comp_list_coord:
                                    continue
                                else:
                                    comp_list_coord.append(sorted_list1)
                                    comp_list_np.append(np_element[np_list_elem1])
                                if sorted_list2 in comp_list_coord:
                                    continue
                                else:
                                    comp_list_coord.append(sorted_list2)  
                                    comp_list_np.append(np_element[np_list_elem2])
                            else:
                                continue
              
                if comp_list_np == []:
                    continue
                else:
                    if nc_elem in newdata:
                        print(newdata.get(nc_elem))  # Debug print (e.g., multiple overlaps per chromosome)
                    else:
                        newdata[nc_elem] = {np_elem:comp_list_np}
    print(newdata)
    with open("./final_result.json", 'w') as jsnfile2:
        json.dump(newdata, jsnfile2)
    