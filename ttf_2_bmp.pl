#!/usr/local/bin/perl

# ttf2bdf -v -l "19000_4 0000" -c c -p 16 -r 72 cyberbit.ttf

while (<>)
{
    $char = sprintf("%04X", $encoding = $1) if /^ENCODING (\d+)$/;
    
    ($width, $height, $left, $bottom) = ($1, $2, $3, $4)
	if /^BBX (.+) (.+) (.+) (.+)/;

    if (/BITMAP/)
    {
	$bottom += 4; # change to taste
	$left+= 0;

	$bottom + $height > 16 && (warn "$char too high\n") && next;
	$bottom < 0 && (warn "$char too low\n") && next;
	$left + $width > 16 && (warn "$char too wide right\n") && next;
	$left < 0 && (warn "$char too wide left\n") && next;

	$width = $left + $width > 8 ? 16 : 16; # change to taste
	$empty= "\t" .  "-" x $width . "\n";

	print "$char:", $empty x (16 - $height - $bottom);

	while (<>)
	{
	    last if /ENDCHAR/;
	    print "\t";
	    s/^([0-9A-F]+)/unpack("B*",pack("H*",$1))/gei;
	    y/01/-#/; s/\n/----------------\n/;
	    $_ = "-" x $left . $_;
	    s/^(.{$width})-*/$1/;
	    print;
	}

	print $empty x $bottom, "\n";
    }
}
