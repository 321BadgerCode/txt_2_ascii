#!/usr/bin/perl

use strict;
use warnings;
use Image::Magick;

# Program to convert a bitmap file to a hex code
# Read each pixel row from file and black pixels coordinate to turning on bit for binary and converting to hex later
# The bitmap file is 16x16 pixels

sub help {
	print "Usage: bmp_2_hex.pl <BITMAP FILE>\n";
	print "\t* BITMAP FILE (necessary parameter) is the bitmap file with 255x255 length of characters, where each character is 16x16 pixels and the characters start at (32,64).\n";
	exit;
}

sub get_filename {
	my $file = shift;
	$file =~ s/.*\///;
	$file =~ s/\..*//;
	return $file;
}

# Function to resize the character to a 16x16 pixel image
sub resize_image {
	my $image = shift;
	my $width = $image->Get("width");
	my $height = $image->Get("height");
	if($width != 16 || $height != 16) {
		$image->Resize(width=>16, height=>16);
	}
}

# Function to make image black & white
sub black_white {
	my $image = shift;
	$image->Quantize(colorspace=>"gray", colors=>2, dither=>0);
}

# Preprocess the image
sub preprocess_image {
	my $image = shift;
	resize_image($image);
	black_white($image);
}

# Command line arguments
if(@ARGV <= 0) {
	print "ERROR: NO BITMAP FILE SPECIFIED!\n";
	help();
}
elsif($ARGV[0] eq "-h" || $ARGV[0] eq "--help") {
	help();
}
elsif($ARGV[0] eq "-v" || $ARGV[0] eq "--version") {
	print "bmp_2_hex.pl v1.0\n";
	exit;
}
my $bitmap_file = $ARGV[0];
if(!-e $bitmap_file) {
	print "ERROR: \"${bitmap_file}\" FILE NOT FOUND!\n";
	exit;
}

# Configuration
my $char_image = Image::Magick->new;
$char_image->Read($bitmap_file);
preprocess_image($char_image);
my $char_hex = "";

# Load the bitmap
for(my $row = 0; $row < 16; $row++) {
	my $byte = 0;
	for(my $col = 0; $col < 16; $col++) {
		my $pixel = $char_image->Get("pixel[$col,$row]");
		my $color = (split(/,/, $pixel))[0];
		if($color == 0) {
			$byte |= 1 << (15-$col);
		}
	}
	$char_hex .= sprintf("%04x", $byte);
}
my $filename = get_filename($bitmap_file);
$filename =~ s/^U\+//;
print "${filename}:${char_hex}\n";
# TODO: create optimized function to remove outer white space border before resizing bitmap
