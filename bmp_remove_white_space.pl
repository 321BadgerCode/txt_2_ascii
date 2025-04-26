#!/usr/bin/perl

# usage: for i in ./char/*.bmp; do echo "${i}" && perl ./bmp_remove_white_space.pl $i; done

use strict;
use warnings;
use Image::Magick;

# Find minimum black pixels on x and y axis of image to crop white space out
sub find_min_black_pixels {
	my $image = shift;
	my $x_min = $image->Get('columns');
	my $y_min = $image->Get('rows');
	my $x_max = 0;
	my $y_max = 0;
	my $x;
	my $y;
	my $pixel;
	for ($x = 0; $x < $image->Get('columns'); $x++) {
		for ($y = 0; $y < $image->Get('rows'); $y++) {
			$pixel = $image->Get("pixel[$x,$y]");
			my $color = (split(/,/, $pixel))[0];
			if ($color == 0) {
				if ($x < $x_min) {
					$x_min = $x;
				}
				if ($x > $x_max) {
					$x_max = $x;
				}
				if ($y < $y_min) {
					$y_min = $y;
				}
				if ($y > $y_max) {
					$y_max = $y;
				}
			}
		}
	}
	return ($x_min, $y_min, $x_max, $y_max);
}
# More efficient method to find_min_black_pixels
sub find_min_black_pixels2 {
	my $image = shift;
	my $x_min = $image->Get('columns');
	my $y_min = $image->Get('rows');
	my $x_max = 0;
	my $y_max = 0;
	my $x;
	my $y;
	my $pixel;
	for ($x = 0; $x < $image->Get('columns'); $x++) {
		for ($y = 0; $y < $image->Get('rows'); $y++) {
			$pixel = $image->Get("pixel[$x,$y]");
			my $color = (split(/,/, $pixel))[0];
			if ($color == 0) {
				$x_min = $x if $x < $x_min;
				$x_max = $x if $x > $x_max;
				$y_min = $y if $y < $y_min;
				$y_max = $y if $y > $y_max;
			}
		}
	}
	return ($x_min, $y_min, $x_max, $y_max);
}

# Remove white space from image
sub remove_white_space {
	my $image = shift;
	my ($x_min, $y_min, $x_max, $y_max) = find_min_black_pixels($image);
	$image->Crop(width => $x_max - $x_min + 1, height => $y_max - $y_min + 1, x => $x_min, y => $y_min);
	return $image;
}

# Command line arguments
if (@ARGV != 1) {
	print "Usage: $0 <input.bmp>\n";
	exit 1;
}
my $input = $ARGV[0];
if (! -e $input) {
	print "Error: $input does not exist\n";
	exit 1;
}

# Main
my $image = Image::Magick->new;
$image->Read($input);
$image = remove_white_space($image);

# Save image
$image->Write($input);

# TODO: optimize (it's currently VERY slow when computing for 65k+ 16x16 images).
