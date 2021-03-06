#! /bin/sh
runnum=$1;
if [ -z "$runnum" ]
then
    echo "Run Number is empty";
    exit 1;
fi

#  Remove the LRB output files if they exist
shopt -s extglob
# find split file
slopefile_list=$(ls -1 ./LRBoutput/blueR$runnum*slope.root);
shopt -u extglob

for slopefile in $slopefile_list
do
    echo "deleting "  $slopefile;
    rm -f $slopefile;
done

#JAPAN First Pass
timenow=$(date +"%Y-%m%d-%H%M");

./qwparity -r $runnum -c prex_prompt.conf \
    --rootfile-stem prexPrompt_pass1_ \
    --QwLog.loglevel-file 2 \
    --QwLog.logfile ./LogFiles/QwLog_run$runnum\_prompt_pass1_$timenow.txt ;

# Postpan regression to the first pass results
./auto_postpan.sh $runnum;

# Make Summary Plots/Text and sync to HallA onlineWeb
# now make plots from pass1 and postpan output
./summary.sh $runnum;

# Do aggregation after the second pass of japan is done. Assume all slug aggregation is done by the WAC
#Aggregator pass 0
timenow=$(date +"%Y-%m%d-%H%M");
(./aggregator.sh $runnum > /dev/tty ) >& ./LogFiles/Camguin_run$runnum\_$timenow.txt

echo "Done with prompt for run $runnum";
