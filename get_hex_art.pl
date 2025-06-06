#!/usr/local/bin/perl -p

sub unpack { 
	local ($_) = @_;
	$_ = unpack ("B*", pack ("H*", $_));
	$width=length($_)/16;
	s/(.{$width})/\t$1\n/g;
	y/01/-#/;
	$_;
};

sub pack { 
	local ($_) = @_;
	y/-#/01/;
	$_= unpack ("H*", pack ("B*", $_));
	y/a-f/A-F/;
	$_;
}

s/([0-9A-F]{64})/&unpack($1)/ie || 
s/([0-9A-F]{32})/&unpack($1)/ie || 
s/\t([-#]+)\n/&pack($1)/e;