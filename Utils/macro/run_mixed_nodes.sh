#!/bin/bash
#which=""
## _mjj_nodr

#infile="/mnt/t3nfs01/data01/shome/micheli/HHbbgg_ETH_devel/outfiles/20170828_optimizedCuts_noDR_minDRGJet/Total_preselection_diffNaming_transformedMVA.root"
#infile="/t3home/micheli/HHbbgg_ETH_20190128/HHbbgg_ETH/Training/output_files/20190128_test/Total_preselection_diffNaming_transformedMVA.root"
#infile="/t3home/micheli/HHbbgg_ETH_20190128/HHbbgg_ETH/Training/output_files/20190201_optimized/Total_preselection_diffNaming_transformedMVA.root"
infile="/t3home/micheli/HHbbgg_ETH_20190128/HHbbgg_ETH/Training/output_files/20190201_optimized_3/Total_preselection_diffNaming_transformedMVA.root"


now=$(date '+%Y%m%d_%H%M%S')
echo $now

#run on all nodes
#for node in jsons_hh/node_*.json; do name=$(basename $node | sed 's%.json%%'); 

#to optimize only SM
for node in jsons_hh/node_mixed.json; do name=$(basename $node | sed 's%.json%%'); 

echo "------------------------------------------STARTING NODE -----------------------------------------------------"
echo $name
echo "-------------------------------------------------------------------------------------------------------------"

## if you need to skip some nodes
#if [[ $name == *node_10* || $name == *node_11* || $name == *node_12* || $name == *node_13*|| $name == *node_2* || $name == *node_3* || $name == *node_4* || $name == *node_5* ]];
#then
#continue
#fi



#######################################hh version
## MVA boundaries optimization with fixed cut in MX and Mjj
#    ./categoryOptimizationMultiDim.py --load jsons_hh/optimize_hh_MVA.json --load $node --infile ${infile} --sigfile ${infile}  --label test_SM -o outPlots/out_${now}_${name}_transformed_MVA/

## MX boundaries optimization with optimized MVA boundaries and fixed cut in Mjj
#    ./categoryOptimizationMultiDim.py --load jsons_hh/optimize_hh_MX.json --load $node --infile ${infile} --sigfile ${infile}  --label test_SM -o outPlots/out_${now}_${name}_transformed_MX/ 



## MVA boundaries optimization with optimized MX boundaries and fixed cut in Mjj
    ./categoryOptimizationMultiDim.py --load jsons_hh/optimize_hh_MVA_MXboundaries.json --load $node --infile ${infile} --sigfile ${infile}  --label test_SM -o outPlots/out_${now}_${name}_transformed_MVA_step2/

#mjj cut optimization
#for cat in cat0; do
#for cat in cat0 cat1 cat2 cat3 cat4 cat5 cat6 cat7 cat8; do
#    ./categoryOptimizationMultiDim.py --load jsons_hh/optimize_mjj.json --infile ${infile} --sigfile ${infile}  --label $name -o outPlots/out_${now}_${name}_${cat}_transformed_mjj/ --load $cat.json --load $node
#done

#mjj cut reopt on one cat
#for cat in cat2; do
#./categoryOptimizationMultiDim.py --load jsons_hh/optimize_mjj.json --infile ${infile} --sigfile ${infile}  --label $name -o outPlots/out_${now}_${name}_${cat}_transformed_mjj/ --load ${cat}_nevts.json --load $node
#done
#    wait



done

#scp -r outPlots/out_${now}* lxplus.cern.ch:/afs/cern.ch/user/m/micheli/www/plots/HHBBGGOptimization/outPlots/

wait
