import extract_aa_sequences
import find_duplicate_nps
import extract_cds_from_gff
import compare_cds_coords
import organize_by_chromosome
import filter_short_cds
import remove_duplicate_coords

# Replace the example files ("proteins.faa", "genomic.gff", etc.) with your own input data before running the functions below.


extract_aa_sequences.get_aa("proteins.faa") 
find_duplicate_nps.duplicate()
extract_cds_from_gff.get_np("genomic.gff")
compare_cds_coords.cds_coord()
organize_by_chromosome.org_chrom()
filter_short_cds()
remove_duplicate_coords()