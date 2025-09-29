<?php
$files=scandir("results");

$f=fopen("isoforms", "r");
$runs=get_runs();
$isoforms=[];
while($line=fgets($f))
{
    $pts=explode("\t", trim($line));
    $gene1=$pts[1];
    $gene2=$pts[3];
    $isoforms[]="$pts[4] $gene1";
    $isoforms[]="$pts[4] $gene2";
}
foreach($files as $file)
{
    if(!strstr($file, "fastq"))
        continue;
    $pts=explode("_", $file);
    if(isset($srrs["$pts[0]"]))
        continue;
    $srr=$runs["$pts[0]"];
    $run=$pts[0];
    $run_data=[];
    foreach($isoforms as $isoform)
        $run_data["$isoform"]=0;
    $f=fopen("results/$file", "r");
    while($line=fgets($f))
    {
        $pts=explode(" ", trim($line));
        if(count($pts)<2)
            continue;
        $transcript=$pts[count($pts)-1];
        if(!is_numeric($transcript))
            continue;
        $transcript=$isoforms[$transcript+1];
        if(!isset($transcripts["$transcript"]))
            $transcripts["$transcript"]=0;
        $transcripts["$transcript"]++;
        $run_data["$transcript"]++;
    }
    $run_stat["$run"]=["counts"=>$run_data, "stat"=>$runs["$run"]];
}
$stat_file=fopen("statistics.csv", "w");
$headers=[];
$headers[]="Run";
foreach($isoforms as $p)
    $headers[]=$p;
foreach($runs["Run"] as $p)
    $headers[]=$p;
fputcsv($stat_file, $headers);
foreach ($run_stat as $sample)
{
    $arr=[$sample["stat"][0]];
    foreach($sample["counts"] as $v)
        $arr[]=$v;
    foreach($sample["stat"] as $v)
        $arr[]=$v;
    fputcsv($stat_file, $arr);
}
function get_runs()
{
    $data=file("SraRunInfo.csv");
    foreach($data as $str)
    {
        $arr=str_getcsv(trim($str));
        $out["$arr[0]"]=$arr;
    }
    return $out;
}
ksort($transcripts);
print_r($transcripts);