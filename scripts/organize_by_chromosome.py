import json

def org_chrom():

    newdata = {}
    with open("./cds_comparison_result.json", 'r') as jsnfile:
        data_data = json.load(jsnfile)
        for aa in data_data:
            if aa in newdata:
                continue
            else:
                newdata[aa] = {}
                list_np_nc = data_data.get(aa)
                for nc in list_np_nc:
                    if nc[1] in (newdata.get(aa)):
                        append_list = [nc[0], nc[2]]
                        for coord in nc[3]:
                            append_list.append(coord)
                        newdata.get(aa).get(nc[1]).append(append_list)
                    else:
                        (newdata.get(aa))[nc[1]] = []
                        append_list = [nc[0], nc[2]]
                        for coord in nc[3]:
                            append_list.append(coord)
                        newdata.get(aa).get(nc[1]).append(append_list)

    print(newdata)

    

    with open("./last_cds_comparison_result.json", 'w') as jsnfile2:
        json.dump(newdata, jsnfile2)

