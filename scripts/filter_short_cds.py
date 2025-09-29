import json

def delete_short_piecies():
    indicate = 0
    newdata = {}
    with open("./last_cds_comparison_result.json", 'r') as jsnfile:
        data_data = json.load(jsnfile)
        for element in data_data:
            newdata[element] = {}
            nc_dict = data_data.get(element) 
            for elem in nc_dict: 
                if len(nc_dict.get(elem)) == 1: 
                    continue
                else:
                    np_lists = nc_dict.get(elem)
                    new_np_list = []
                    for np_list in np_lists: 
                        indicate = 0
                        coord_list = np_list[2:] 
                        for coord in coord_list:  
                            raznisza_list = coord.split('_')
                            razn = int(raznisza_list[1])-int(raznisza_list[0])
                            if razn < 15:  
                                indicate = 1
                            else:
                                continue
                        if indicate == 0:
                            append_list = np_list
                            new_np_list.append(append_list)
                        else:
                            continue
                    if len(new_np_list) == 1:
                        continue
                    else:
                        (newdata.get(element))[elem] = new_np_list
    newdata2 = {}

    for new_elem in newdata:
        if newdata.get(new_elem) == {}:
            continue
        else:
            newdata2[new_elem] = newdata.get(new_elem)
    print(newdata2)


    with open("./short_comparison_result.json", 'w') as jsnfile2:
        json.dump(newdata2, jsnfile2)
