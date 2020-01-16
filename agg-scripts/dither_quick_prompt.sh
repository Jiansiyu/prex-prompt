#!/bin/bash

cd ~/PREX/prompt/beam-mod/scripts/
./dit_runlist_alldet.sh ~/PREX/prompt/agg-scripts/quick.list
cd ~/PREX/prompt/beam-mod/rootfiles_alldet_pass1
#root -l -b -q 'dithering_plot_cyc.C('${1}')'
#evince plots/cyclenum_slug${1}.pdf &

#cat ~/PREX/prompt/collector/run_list/slug${1}.list
#cd ~/PREX/prompt/beam-mod/scripts/dit_11X12X_txt
#rm instructions.txt
#echo -e "Look at the sensitivities shown in cyclenum_slug${1}.pdf for \nthe runs/supercycles in slug ${1}. \nChange the 1's to 0's in the sensitivity \ntext files of Main, BPMs, and ATs for each run file \nThen check the updated plots to verify it worked:\n" >> instructions.txt
#cat ~/PREX/prompt/collector/run_list/slug${1}.list >> instructions.txt
#gedit instructions.txt&

while IFS= read -r line; do
  run=$line;
  if [ -f Quartz_sensitivity_run${run}.txt ]; then
    gedit Quartz_sensitivity_run${run}.txt
  else
    echo "No Quartz sensitivity for run ${run}"
  fi
  if [ -f BPMs_sensitivity_run${run}.txt ]; then
    gedit BPMs_sensitivity_run${run}.txt
  else
    echo "No BPMs sensitivity for run ${run}"
  fi
  if [ -f AT_sensitivity_run${run}.txt ]; then
    gedit AT_sensitivity_run${run}.txt
  else
    echo "No AT sensitivity for run ${run}"
  fi
done < ~/PREX/prompt/agg-scripts/quick.list
echo -n "Done updating cycle flags? Enter any string when done: "
read doneReading

#cd ~/PREX/prompt/beam-mod/scripts
#./dit_runlist_alldet_pass2.sh ~/PREX/prompt/agg-scripts/quick.list

#cd ~/PREX/prompt/beam-mod/rootfiles_alldet_pass2
#root -l -b -q 'dithering_plot_cyc.C('${1}')'
#evince plots/cyclenum_slug${1}.pdf &

#echo -n "Was this sufficient to remove all outliers for this slug? y/n: "
#read sufficient
#if [ $sufficient = "n" ]; then
#  cd ~/PREX/prompt/beam-mod/scripts/dit_11X12X_txt
#  while IFS= read -r line; do
#    run=$line;
#    if [ -f Quartz_sensitivity_run${run}.txt ]; then
#      gedit Quartz_sensitivity_run${run}.txt
#    else
#      echo "No Quartz sensitivity for run ${run}"
#    fi
#    if [ -f BPMs_sensitivity_run${run}.txt ]; then
#      gedit BPMs_sensitivity_run${run}.txt
#    else
#      echo "No BPMs sensitivity for run ${run}"
#    fi
#    if [ -f AT_sensitivity_run${run}.txt ]; then
#      gedit AT_sensitivity_run${run}.txt
#    else
#      echo "No AT sensitivity for run ${run}"
#    fi
#  done < ~/PREX/prompt/agg-scripts/quick.list
#
#  cd ~/PREX/prompt/beam-mod/scripts
#  ./dit_runlist_alldet_pass2.sh ~/PREX/prompt/agg-scripts/quick.list
#
#  cd ~/PREX/prompt/beam-mod/rootfiles_alldet_pass2
#  root -l -b -q 'dithering_plot_cyc.C('${1}')'
#  evince plots/cyclenum_slug${1}.pdf &
#fi

#cd ~/PREX/prompt/beam-mod/scripts/dit_11X12X_txt/
#./add_runlist_txt.sh ~/PREX/prompt/agg-scripts/quick.list
#root -l -b -q 'sensitivity_plot_BPMs_uptodate.C('${1}')'
#evince plots/sensitivity_plots_slug${1}.pdf &