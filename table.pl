#!/usr/bin/perl
my @sizes=map {10**$_} (0..4);
my @words=map {10**$_} (0..4);
my $trials=50;
print "Trials: $trials | ";
print "Size: $_ | " for (@sizes);
print "\n | --- | ";
print "--- | " for (@sizes);
print "\n";
for (@words) {
	my $wordc=$_;
	print " | Words: $wordc | ";
	for (@sizes) {
		my $size=$_;
		system "./a.out $size $wordc $trials --silent";
		print " | ";
	}
	print "\n";
}