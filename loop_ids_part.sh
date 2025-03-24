#!/bin/bash

# Input file
input_file="gb_id_1.txt"

# Check if the input file exists
if [ ! -f "$input_file" ]; then
    echo "Error: $input_file not found."
    exit 1
fi

# Get the current directory (to return to it later)
original_dir=$(pwd)
total_lines=$(wc -l < "$input_file")

submit_jobs() {
	local n1="$1"
	local n2="$2"
	sed -n "${n1},${n2}p" "$input_file" | while IFS= read -r number; do
	    mkdir -p "$number"
	    cd "$number" || exit 1
	    cp $original_dir/in.lammps $original_dir/$number
    	    cp $original_dir/refconfig $original_dir/$number
            cp $original_dir/bart-loop.sh $original_dir/$number
            cp $original_dir/Cu01.eam.alloy $original_dir/$number
            cp $original_dir/9775-1048.data $original_dir/$number
            cp $original_dir/art-loop.submit $original_dir/$number
            #sbatch art-loop.submit $number #> "output_$number.txt"
    	    sbatch --export=NUMBER=$number art-loop.submit
	    # Return to the original directory
            cd "$original_dir"
        done #< "$input_file"
}

# Adjust n2 if it exceeds the total number of lines
if [ $total_lines -ge $total_lines ]; then
    submit_jobs 1 $total_lines
else
    submit_jobs 1 $total_lines
fi
