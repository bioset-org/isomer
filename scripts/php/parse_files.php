<?php
while(1)
{
    $data=file("runs.txt");
    $found=0;
    for($i=0;$i<count($data);$i++)
    {
        $line=$data[$i];
        if(trim($line)=="")
            continue;
        $pts=explode("\t", trim($line));
        if(count($pts)>1)
            continue;
        $found=1;
        $run=$pts[0];
        system("sratoolkit/bin/prefetch.exe $run");
        system("sratoolkit/bin/fasterq-dump.exe $run");
        extract_reads($run);
        unlink("{$run}_1.fastq");
        unlink("{$run}_2.fastq");
        unlink("{$run}.fastq");
        unlink("$run/$run.sralite");
        rmdir("$run");
        $new_data=file("runs.txt");
        $out=fopen("runs.txt", "w");
        foreach($new_data as $str)
        {
            $str=trim($str);
            if($str==$run)
                $str.="\tparsed";
            fwrite($out, "$str\n");
        }
        fclose($out);
    }
    if($found!=1)
        break;
}
function extract_reads($run)
{
    $f=fopen("isoforms", "r");
    $files=[$run];
    $isoforms=[];
    while($line=fgets($f))
    {
        $pts=explode("\t", trim($line));
        $gene1=$pts[1];
        $gene2=$pts[3];
        $isoforms[]=$gene1;
        $isoforms[]=$gene2;
    }
    $isoforms_str=implode(",", $isoforms);
    echo "../../isomer_cpp/bin/isomer.exe C:/isomer/scripts/php/reference/genomic.gtf C:/isomer/scripts/php/reference/genomic.fna $isoforms_str C:/genes/kmeromics/fastq/{$run}_1.fastq C:/genes/kmeromics/fastq/{$run}_2.fastq C:/genes/kmeromics/fastq/results/{$run}_1.fastq C:/genes/kmeromics/fastq/results/{$run}_2.fastq\n";
    if(file_exists("C:/genes/kmeromics/fastq/{$run}_1.fastq"))
        system("../../isomer_cpp/bin/isomer.exe C:/isomer/scripts/php/reference/genomic.gtf C:/isomer/scripts/php/reference/genomic.fna $isoforms_str C:/genes/kmeromics/fastq/{$run}_1.fastq C:/genes/kmeromics/fastq/{$run}_2.fastq C:/genes/kmeromics/fastq/results/{$run}_1.fastq C:/genes/kmeromics/fastq/results/{$run}_2.fastq");
    else
        system("../../isomer_cpp/bin/isomer.exe C:/isomer/scripts/php/reference/genomic.gtf C:/isomer/scripts/php/reference/genomic.fna $isoforms_str C:/genes/kmeromics/fastq/{$run}.fastq - C:/genes/kmeromics/fastq/results/{$run}_1.fastq C:/genes/kmeromics/fastq/results/{$run}_2.fastq");
}