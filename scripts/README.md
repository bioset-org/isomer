# Identical Isoform Analysis Pipeline

A lightweight Python pipeline to **identify identical isoforms** from RefSeq-derived inputs and curate results by removing short fragments and duplicate/overlapping coordinate sets.

---

## What it does

- Parses a protein FASTA (`.faa`) and builds a dictionary of AA sequences → headers.
- Finds NP (protein) entries with **multiple** associated AA headers (duplicates).
- Compares CDS **coordinate structures** between duplicated NPs to detect **identical isoforms**.
- Organizes results by chromosome, filters out **short CDS fragments** (< 15 bp), and removes **identical/overlapping** coordinate sets to yield a clean final JSON.

---

## Inputs

- `proteins.faa` — RefSeq protein FASTA file.
- `genomic.gff` — RefSeq genome annotation (GFF3).

> Replace the example filenames in the code (pipeline.py) with your actual data paths before running.

Run the full pipeline from the root of the repo:

```bash
python pipeline.py
```
---

## Processing steps & outputs

1. **Extract amino acids**  
   `extract_aa_sequences.get_aa("proteins.faa")` → `aa_dict.json`

2. **Find duplicate NP entries**  
   `find_duplicate_nps.duplicate()` → `np_duplicates.json`, `np_list.txt`

3. **Extract CDS for those NPs**  
   `extract_cds_from_gff.get_np("genomic.gff")` → `cds_coordinates.json`

4. **Compare CDS coordinate structures**  
   `compare_cds_coords.cds_coord()` → `cds_comparison_result.json`

5. **Group by chromosome**  
   `organize_by_chromosome.org_chrom()` → `last_cds_comparison_result.json`

6. **Filter short CDS fragments (< 15 bp)**  
   `filter_short_cds.delete_short_piecies()` → `short_comparison_result.json`

7. **Remove identical/overlapping coordinate sets**  
   `remove_duplicate_coords.delete_dubl()` → `final_result.json`

---

## Minimal requirements

- Python 3.8+
- Standard library (`json`)

---

## Notes & assumptions

- GFF3 is expected to contain `CDS` features with `ID=cds-...` tags in the 9th column.
- The comparison step treats isoforms as *identical* when their **ordered** CDS interval sets match.
- The “short fragment” threshold is 15 bp (configurable by editing the function).

---

## Citation

If this pipeline contributes to your research, please cite this repository (https://github.com/bioset-org/isomer).
