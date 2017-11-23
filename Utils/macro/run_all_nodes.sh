
#which=""
## _mjj_nodr

#for node in inputs/node_*.root; do name=$(basename $node | sed 's%.root%%'); 
for node in jsons_hh/node_*.json; do name=$(basename $node | sed 's%.json%%'); 
echo $node
echo $name
## for node in inputs$which/node_SM.root; do name=$(basename $node | sed 's%.root%%'); 
###original command
#    ./categoryOptimizationMultiDim.py --load optimize_hh.json --infile inputs$which/background_no_fakes.root --sigfile $node  --label $name -o www/higgs/hh/cat_opt$which/no_fakes_no_SMH_MXcategories_$name; 
##hh version
    ./categoryOptimizationMultiDim.py --load jsons_hh/optimize_hh_MVA.json --load $node --infile infiles/Total_preselection_diffNaming_transformedMVA.root --sigfile infiles/Total_preselection_diffNaming_transformedMVA.root  --label test_SM -o outPlots/out_test_${name}_transformed/


    ## ./categoryOptimizationMultiDim.py --load optimize_hh.json --infile inputs$which/background_no_fakes.root --sigfile $node  --label $name -o www/higgs/hh/cat_opt$which/no_fakes_no_SMH_3x3_$name &
    ## ./categoryOptimizationMultiDim.py --load optimize_hh.json --infile inputs$which/background_no_fakes.root --sigfile $node  --label $name -o www/higgs/hh/cat_opt$which/no_fakes_no_SMH_3x3_mjj_cut_$name &


    ## for cat in cat012 cat345 cat567 cat036 cat147 cat258; do
    ## 	./categoryOptimizationMultiDim.py --load optimize_mjj.json --infile inputs$which/background_no_fakes.root --sigfile $node  --label $name -o www/higgs/hh/cat_opt$which/no_fakes_no_SMH_3x3_mjj_cut_${cat}_$name --load $cat.json &
    ## done
    ## wait
    
#    for cat in cat0 cat1 cat2 cat3 cat4 cat5 cat6 cat7 cat8; do
#    	./categoryOptimizationMultiDim.py --load optimize_mjj.json --infile inputs$which/background_no_fakes.root --sigfile $node  --label $name -o www/higgs/hh/cat_opt$which/no_fakes_no_SMH_3x3_mjj_cut_${cat}_$name --load $cat.json &
#    done
#    wait


done

scp -r outPlots lxplus.cern.ch:/afs/cern.ch/user/m/micheli/www/plots/HHBBGGOptimization/

wait
