#!/bin/bash
#which=""
## _mjj_nodr

#infile="/mnt/t3nfs01/data01/shome/micheli/HHbbgg_ETH_devel/outfiles/20170828_optimizedCuts_noDR_minDRGJet/Total_preselection_diffNaming_transformedMVA.root"
#infile="/mnt/t3nfs01/data01/shome/nchernya/HHbbgg_ETH_devel/outfiles/20180416/20180416_ggHHbbgg_DiPhotonJets_diffNaming.root"
#infile="/mnt/t3nfs01/data01/shome/nchernya/HHbbgg_ETH_devel/outfiles/20180712_newcode/Total_preselection_diffNaming_transformedMVA.root"
#infile="/mnt/t3nfs01/data01/shome/nchernya/HHbbgg_ETH_devel/outfiles/20180601_Noreg/20180601_Noreg_Total_preselection_diffNaming_transformedMVA.root"
#infile="/mnt/t3nfs01/data01/shome/nchernya/HHbbgg_ETH_devel/outfiles/20180628_newcode/Total_preselection_diffNaming_transformedMVA.root" #newcode
#infile="/mnt/t3nfs01/data01/shome/nchernya/HHbbgg_ETH_devel/outfiles/20180628_newcode_data/Total_preselection_diffNaming_transformedMVA.root" #newcode
infile="/mnt/t3nfs01/data01/shome/nchernya/HHbbgg_ETH_devel/outfiles/20181001_DeepCSV/Total_preselection_diffNaming_transformedMVA.root"



now=$(date '+%Y%m%d_%H%M%S')
echo $now

#for node in inputs/node_*.root; do name=$(basename $node | sed 's%.root%%'); 
for node in jsons_hh_DeepCSV_01102018/node_*.json; do name=$(basename $node | sed 's%.json%%'); 
#for node in jsons_hh_SM_20180601_Noreg/node_*.json; do name=$(basename $node | sed 's%.json%%'); 
echo "------------------------------------------STARTING NODE -----------------------------------------------------"
echo $name
echo "-------------------------------------------------------------------------------------------------------------"

## if you need to skip some nodes
if [[ $name == *node_10* || $name == *node_11* || $name == *node_12* || $name == *node_13*|| $name == *node_14* || $name == *node_2* || $name == *node_3* || $name == *node_4* || $name == *node_5* || $name == *node_6* ||  $name == *node_7* || $name == *node_8* || $name == *node_9* ]];
#if [[ $name == *node_10* || $name == *node_11* || $name == *node_12* || $name == *node_13*|| $name == *node_14* || $name == *node_2* || $name == *node_3* || $name == *node_4* || $name == *node_6* ||  $name == *node_7*  || $name == *node_9* ]];
then
continue
fi



##hh version
## MVA boundaries optimization with fixed cut in MX and Mjj
#    ./categoryOptimizationMultiDim.py --load jsons_hh_DeepCSV_01102018/optimize_hh_MVA.json --load $node --infile ${infile} --sigfile ${infile}  --label test_SM -o outPlots/out_${now}_${name}_transformed_MVA/

## MX boundaries optimization with optimized MVA boundaries and fixed cut in Mjj
   # ./categoryOptimizationMultiDim.py --load jsons_hh_DeepCSV_01102018/optimize_hh_MX.json --load $node --infile ${infile} --sigfile ${infile}  --label test_SM -o outPlots/out_${now}_${name}_transformed_MX/



## MVA boundaries optimization with optimized MX boundaries and fixed cut in Mjj
 #   ./categoryOptimizationMultiDim.py --load jsons_hh_DeepCSV_01102018/optimize_hh_MVA_MXboundaries.json --load $node --infile ${infile} --sigfile ${infile}  --label test_SM -o outPlots/out_${now}_${name}_transformed_MVA_step2/

#mjj cut optimization
#for cat in cat0; do
##for cat in cat0 cat1 cat2 cat3 cat4 cat5 cat6 cat7 cat8; do
for cat in cat0 cat1 cat2 cat3 cat4 cat5 cat6 cat7 cat8 cat9 cat10 cat11; do
    ./categoryOptimizationMultiDim.py --load jsons_hh_DeepCSV_01102018/optimize_mjj.json --infile ${infile} --sigfile ${infile}  --label $name -o outPlots/out_${now}_${name}_${cat}_transformed_mjj/ --load $cat.json --load $node
done

#mjj cut reopt on one cat
#for cat in cat2; do
#./categoryOptimizationMultiDim.py --load jsons_hh_DeepCSV_01102018/optimize_mjj.json --infile ${infile} --sigfile ${infile}  --label $name -o outPlots/out_${now}_${name}_${cat}_transformed_mjj/ --load ${cat}_nevts.json --load $node
#done
#    wait



done

cp -r outPlots/out_${now}* /afs/cern.ch/user/n/nchernya/www/HHbbgg/MVAoptimization/DeepCSV_01102018/

#wait
