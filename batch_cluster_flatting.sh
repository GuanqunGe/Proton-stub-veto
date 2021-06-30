# this script runs flat_root.C automatically for a set of SEAview configurations
# By Guanqun, June 16 2021
#!/bin/bash


# location of the vertexed singlephoton file
# also will be location of the flatten file
FileLoc="/uboone/data/users/gge/SinglePhotonAnalysis/vertexed_file/Mar2021_lowproton_seaview_test/v08_00_00_43/vertex/v08_00_00_26_run1_reco2/ProtonBDT/"


# name and tag for different configurations
Identifier="run_LowProton"
Npts=("8pts" "7pts" "6pts" "5pts" "4pts" "3pts" "2pts" "1pts")
#Npts=("7pts" "8pts")
Eps=("8cm" "7cm" "6cm" "5cm" "4cm" "3cm" "2cm" "1cm")
Thres=("25ADC")

# array of all tags
declare -a allTAG=()
for Np in ${Npts[@]}; do
   for Ep in ${Eps[@]}; do
       for Thre in ${Thres[@]}; do
           temp=$Np"_"$Ep"_"$Thre
           allTAG+=($temp)
        done
   done
done

echo ${#allTAG[@]}
# iterate through all configurations
for i in ${allTAG[@]};do
    echo "Flatten vertex root with configuration "$i
   
    # replace the spaceholder default words by specific loc and configurations 
    sed -i "s,FileLOC,$FileLoc,g" flat_root.C
    sed -i "s,CONFIGURATION,$i,g" flat_root.C

    # run root in a non-interactive mode(quit after execution)
    root -l -b -q flat_root.C

    # replace back the default
    sed -i "s,$FileLoc,FileLOC,g" flat_root.C
    sed -i "s,$i,CONFIGURATION,g" flat_root.C
done
