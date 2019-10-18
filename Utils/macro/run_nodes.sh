#!/bin/bash
#which=""
## _mjj_nodr

#infile="/work/nchernya/HHbbgg_ETH_devel/outfiles/20191008_runII_optimization/Total_runII_sig_mix.root"
infile="/afs/cern.ch/work/n/nchernya/ETH/DiHiggs/optimization_files/Total_runII_sig_mix.root"

now=$(date '+%Y%m%d_%H%M%S')
echo $now

config_dir=jsons_hh_11_10_2019
for node in ${config_dir}/node_*.json; do name=$(basename $node | sed 's%.json%%'); 
echo "------------------------------------------STARTING NODE -----------------------------------------------------"
echo $name
echo "-------------------------------------------------------------------------------------------------------------"

#######################################hh version
## MVA boundaries optimization with fixed cut in MX and Mjj
    ./categoryOptimizationMultiDim.py --load ${config_dir}/optimize_hh_MVA.json --load $node --infile ${infile} --sigfile ${infile}  --label mixNodes -o outPlots/out_${now}_${name}_transformed_MVA/

	cp -r outPlots/out_${now}_${name}_transformed_MVA /afs/cern.ch/user/n/nchernya/www/HHbbgg/MVAoptimization/fullRunII/ 
## MX boundaries optimization with optimized MVA boundaries and fixed cut in Mjj
#    ./categoryOptimizationMultiDim.py --load ${config_dir}/optimize_hh_MX.json --load $node --infile ${infile} --sigfile ${infile}  --label mixNodes -o outPlots/out_${now}_${name}_transformed_MX/
#	cp -r outPlots/out_${now}_${name}_transformed_MX /afs/cern.ch/user/n/nchernya/www/HHbbgg/MVAoptimization/fullRunII/ 



## MVA boundaries optimization with optimized MX boundaries and fixed cut in Mjj
   # ./categoryOptimizationMultiDim.py --load ${config_dir}/optimize_hh_MVA_MXboundaries.json --load $node --infile ${infile} --sigfile ${infile}  --label mixNodes -o outPlots/out_${now}_${name}_transformed_MVA_step2/

#mjj cut optimization
#for cat in cat0; do
#for cat in cat0 cat1 cat2 cat3 cat4 cat5 cat6 cat7 cat8; do
#    ./categoryOptimizationMultiDim.py --load ${config_dir}/optimize_mjj.json --infile ${infile} --sigfile ${infile}  --label $name -o outPlots/out_${now}_${name}_${cat}_transformed_mjj/ --load $cat.json --load $node
#done

#mjj cut reopt on one cat
#for cat in cat2; do
#./categoryOptimizationMultiDim.py --load ${config_dir}/optimize_mjj.json --infile ${infile} --sigfile ${infile}  --label $name -o outPlots/out_${now}_${name}_${cat}_transformed_mjj/ --load ${cat}_nevts.json --load $node
#done
#    wait



done

#scp -r outPlots/out_${now}* lxplus.cern.ch:/afs/cern.ch/user/m/micheli/www/plots/HHBBGGOptimization/outPlots/

wait
