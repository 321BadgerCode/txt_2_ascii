#!/bin/bash

# Get ttf file from command line argument
if [ -z "$1" ]; then
	echo "Usage: ${0} <ttf_file>"
	exit 1
fi
ttf_file=$1
if [ ! -f "${ttf_file}" ]; then
	echo "Error: ${ttf_file} does not exist."
	exit 1
fi

# Prompt user if should delete output folder if it already exists
if [ -d "./output/" ]; then
	read -p "\"./output/\" folder already exists. Do you want to delete it? (y/n) " -n 1 -r
	echo
	if [[ $REPLY =~ ^[Yy]$ ]]; then
		rm -rf ./output/
		perl ./get_ttf_2_bmp.pl ${ttf_file}
	fi
fi

# Create ./emoji.txt file if it doesn't exist
if [ ! -f "./emoji.txt" ]; then
	touch ./emoji.txt
else
	read -p "\"./emoji.txt\" file already exists. Do you want to clear it? (y/n) " -n 1 -r
	echo
	if [[ $REPLY =~ ^[Yy]$ ]]; then
		> ./emoji.txt
		for i in ./output/*.bmp; do echo "${i}" && perl ./bmp_2_hex.pl $i >> ./emoji.txt; done
	fi
fi

# Get hex art from ./emoji.txt
art=$(cat ./emoji.txt | perl ./get_hex_art.pl)

# Get art without tabs & w/ char names on different line
just_art=$(echo "${art}" | sed -e 's/\t//g' -e 's/.*:/&\n/g')

# Save art to ./hex_art.txt
if [ ! -f "./hex_art.txt" ]; then
	touch ./hex_art.txt
	echo "${just_art}">./hex_art.txt
else
	read -p "\"./hex_art.txt\" file already exists. Do you want to clear it? (y/n) " -n 1 -r
	echo
	if [[ $REPLY =~ ^[Yy]$ ]]; then
		echo "${just_art}">./hex_art.txt
	fi
fi

# Prompt user to search for a string in the hex art
echo -e "\nEnter \"exit\" to exit the program."
echo "Enter '*' at the end of your search to display possible matches for the string."
while [ true ]; do {
	read -p "Enter string to search for in the hex art: " -r

	if [[ $REPLY == "exit" ]]; then
		break
	fi

	# If the string ends with *, display the possible matches for the string
	if [[ $REPLY == *"*" ]]; then
		REPLY=$(echo $REPLY | sed 's/\*//g')
		echo "Possible matches for \"$REPLY\":"
		cat ./hex_art.txt | grep $REPLY | sed 's/://g' | sed 's/^/* /'
		continue
	fi

	if [[ $REPLY == "" ]]; then
		continue
	fi
	cat ./hex_art.txt | grep -A 16 -w $REPLY
};done