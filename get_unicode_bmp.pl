#!/usr/bin/perl

use strict;
use warnings;
use Image::Magick;

sub help {
	print "Usage: get_unicode_bmp.pl <BITMAP FILE> [CHAR_LIMIT]\n";
	print "\t* BITMAP FILE (necessary parameter) is the bitmap file with 255x255 length of characters, where each character is 16x16 pixels and the characters start at (32,64).\n";
	print "\t* CHAR_LIMIT (defaults to 0xff) is the limit of characters to get from the unifont bitmap.\n";
	exit;
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
	print "get_unicode_bmp.pl v1.0\n";
	exit;
}
elsif(@ARGV == 1) {
	print "WARNING: NO CHAR LIMIT SPECIFIED!\n\t* Defaulting to 0xff (255), which is the length of the first row of characters.\n\t* The max is 0xfe01 (0xff*0xff).\n";
}
my $bitmap_file = $ARGV[0];
if(!-e $bitmap_file) {
	print "ERROR: \"${bitmap_file}\" FILE NOT FOUND!\n";
	exit;
}
my $char_limit = (@ARGV>1) ? (($ARGV[1] =~ /^0x/) ? hex($ARGV[1]) : $ARGV[1]) : 0xff;

# Configuration
my $x = 32; # Top left character's X position
my $y = 64; # Top left character's Y position
my $char_width = 16;
my $char_height = 16;

# Load the bitmap
my $image = Image::Magick->new;
$image->Read($bitmap_file);

my $width = $image->Get('width');
my $height = $image->Get('height');

# Extract characters
my $char_num = 0;
for (my $row = $y; $row < $height; $row += $char_height) {
	for (my $col = $x; $col < $width; $col += $char_width) {
		if($char_num >= $char_limit) {
			last;
		}
		my $char_image = $image->Clone();
		$char_image->Crop(geometry => "${char_width}x${char_height}+$col+$row");

		# Write the character bitmap to a file
		my $row_hex = sprintf("%02x", ($row-0x40)/0x10);
		my $col_hex = sprintf("%02x", ($col-0x20)/0x10);
		mkdir "./char";
		$char_image->Write("./char/U+${row_hex}${col_hex}.bmp");

		$char_num++;
	}
}
