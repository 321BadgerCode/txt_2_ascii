#!/usr/bin/perl

use warnings;
use Encode;
use Font::FreeType;
use Image::Magick;
use Font::TTF::Font;

#function to return filename that can be used for characters which names are not allowed to be used for filenames such as some unicode characters
sub get_filename {
	my $name = shift;
	$name =~ s/[^a-zA-Z0-9_]/_/g;
	return $name;
}

# Command line arguments
if (!defined $ARGV[0]) {
	die "Usage: $0 <ttf_file>\n";
}
my $ttf_file = $ARGV[0];
if(!-e $ttf_file) {
	die "File $ttf_file does not exist\n";
}

# Get the characters to be converted
my $font = Font::TTF::Font->open($ttf_file);
my @supported_codepoints = $font->{cmap}->reverse;
@supported_codepoints = grep { $_ > 32 } @supported_codepoints;
print "Supported codepoints: ".join(", ", @supported_codepoints)."\n";

mkdir "output" unless -d "output";
foreach my $codepoint (@supported_codepoints) {
	my $char = chr($codepoint);

	# Read the TTF file and get the glyph outline of the character
	my $freetype = Font::FreeType->new;
	my $face = $freetype->face($ttf_file);
	
	$face->set_char_size(24, 24, 100, 100);
	my $string = encode('UTF-8', $char);
	my $ustring = decode('UTF-8', $string);
	my $byte_string = encode('UTF-8', $ustring);
	my $glyph = $face->glyph_from_char($byte_string);

	# Save the bitmap for the character
	my ($bitmap, $left, $top) = $glyph->bitmap;
	my $width = length $bitmap->[0];
	my $height = @$bitmap;
	my $name = $glyph->name || $char;
	print $name."(".$glyph->char_code.") : ".$width."x".$height."\n";

	my ($file) = $glyph->bitmap_magick(FT_RENDER_MODE_MONO);
	$file->Negate();
	my $filename = get_filename($name);
	$file->Write("./output/${filename}.bmp");
}