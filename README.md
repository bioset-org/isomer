Isoform Finder
==============

Overview
--------
Isoform Finder is a C++ project built in Visual Studio 2022 for detecting isoforms in sequencing data.
The program processes paired-end FASTQ files (short reads) and extracts only reads that match isoform-specific k-mers.

Pipeline:
1. Index Reference Data
   - Uses GTF and FNA files (from RefSeq)
   - Builds an index of transcript structures and sequences

2. Generate Unique K-mers
   - Creates a list of k-mers (default length = 30 nucleotides)
   - Selects only k-mers unique to the specified isoform IDs

3. Filter FASTQ Reads
   - Reads paired FASTQ files (*_1.fastq and *_2.fastq)
   - Extracts only reads that contain isoform-specific k-mers

4. Output
   - Writes reduced FASTQ files containing only selected reads
   - Isoform abundance can be measured by counting reads in the reduced FASTQ files

Features
--------
- Efficient isoform identification using unique k-mers
- Compatible with RefSeq GTF + FNA reference sets
- Processes paired-end FASTQ files
- Outputs filtered FASTQ files for downstream analysis

Requirements
------------
- Visual Studio 2022 (C++20 recommended)
- Input files:
    * *.gtf  (gene annotation from RefSeq)
    * *.fna  (genomic sequences from RefSeq)
    * *_1.fastq, *_2.fastq (paired-end reads)

Usage
-----
Run the program with the following arguments:

    isomer.exe <gtf_file> <fna_file> <isoform_ids> <fastq_R1_in> <fastq_R2_in> <fastq_R1_out> <fastq_R2_out>

Example:

    C:/genes/kmeromics/isomer/x64/Release/isomer.exe \
      C:/genes/kmeromics/rna-seq/reference/genomic.gtf \
      C:/genes/kmeromics/rna-seq/reference/genomic.fna \
      NM_001256799.2,NM_001365847.1 \
      C:/genes/kmeromics/fastq/sample_1.fastq \
      C:/genes/kmeromics/fastq/sample_2.fastq \
      C:/genes/kmeromics/fastq/results/sample_1.fastq \
      C:/genes/kmeromics/fastq/results/sample_2.fastq

Output
------
- Reduced FASTQ files containing only reads with isoform-specific k-mers:
    * sample_1.fastq
    * sample_2.fastq
- Isoform counts can be measured by counting reads in the reduced FASTQ:
    
    grep -c "^@" C:/genes/kmeromics/fastq/results/sample_1.fastq

Notes
-----
- Default k-mer length is 30 nucleotides (can be changed at compile-time)
- Ensure GTF and FNA files are from the same RefSeq release
- Downstream quantification can be done with standard read counters

License
-------
This project is licensed under the GNU General Public License (GPL v3).
You are free to use, modify, and distribute this software under GPL terms.
For full license text, see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.html
