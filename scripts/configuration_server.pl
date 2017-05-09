#!/usr/bin/perl
use strict;
no strict 'refs';

use IO::Socket;
use IO::Socket::INET;
use IO::Select;

use Fcntl;
use Errno qw (EWOULDBLOCK);

my $pcount=@ARGV;
if ($pcount == 0) {
}

my $user_input;

# Routines to handle program parameters ########################################
use Getopt::Long;
Getopt::Long::Configure("bundling");

my %prgp_Parms;
my $prgp_usage1;
my $prgp_usage2;

sub AddUsage {
	my ($o, $c) = @_;
	my $tx = sprintf("       %-25s# %s\n", $o, $c);
	$prgp_usage1 .= " [$o]";
	$prgp_usage2 .= $tx;
}

sub SetParm {
	my ($p, $v, $o, $c) = @_;
	$prgp_Parms{$p} = $v;
	my $tx = sprintf("       %-25s# %s\n", $o, $c);
	$prgp_usage1 .= " [$o]";
	$prgp_usage2 .= $tx;
}

sub ShowUsage {
	my ($pname, $result) = @_;
	print "$pname $prgp_usage1\n$prgp_usage2\n";
	exit(0);
}
my $readers;
$|=1;
###############################################################################


my $ProgramName      = "mini_server.pl";
my $opt_usage        = 0;
my $opt_verbose      = 0;
my $opt_debug        = 0;

#######################################################################################


my $opt_sim_file;
my $opt_test_client;

SetParm("t",   \$opt_test_client,  "-t",                   "run as test client");
SetParm("f=s", \$opt_sim_file,     "-f <simfile>",         "use simulator file");
SetParm("d=i", \$opt_debug,        "-d <level>",           "debug level");
SetParm("h",   \$opt_usage,        "-h",                   "help");
SetParm("v",   \$opt_verbose,      "-v",                   "verbose output");

my $result = GetOptions( %prgp_Parms ) or die ShowUsage("$ProgramName");

ShowUsage($ProgramName, $result) if (!$result or $opt_usage);

#######################################################################################
#
# Setup for test output to load directory
#
#######################################################################################

my $opt_port      = 6000;

#######################################################################################

my @initial_commands;
my $readers = IO::Select->new() or die "Can't create IO::Select readers object";

# flush after every write
$| = 1;

my ($socket);
my ($peer_address,$peer_port);
my $senddest;
my @coms;
my %reps;

loadTestFile();

if ($opt_test_client) {
	runAsClientUDP();
	#does not return;
}

runAsServerUDP();

exit();

##########################################################################################
#
# The main entry points into the loader
#
##########################################################################################
sub setupReader {
	$readers = IO::Select->new() or die "Can't create IO::Select readers object";
}

sub runAsClientUDP {
	setupReader();
	$socket = new IO::Socket::INET ( LocalPort => '4000', Proto => 'udp', ReuseAddr => 1)
		or die "ERROR in Socket Creation : $!\n";
	$senddest = sockaddr_in(6000, inet_aton('localhost'));

	autoflush $socket 1;
	$readers->add($socket);

	handleConnectionsClient();
}

sub runAsServerUDP {
	setupReader();

	$socket = new IO::Socket::INET (LocalPort => '6000', Proto => 'udp', ReuseAddr => 1)
		or die "ERROR in Socket Creation : $!\n";

	autoflush $socket 1;
	$readers->add($socket);

	handleConnections();
}

sub handleConnections
{
	my $ready_readers;

	my $seq=0;
	while (1) {
		($ready_readers) = IO::Select->select($readers, undef, undef, 0);
		for my $handle (@{$ready_readers}) {
			my $bytes = $handle->recv($user_input, 2048);
			if (!defined $bytes) {
				exit;
			}
			if (($bytes = length($user_input)) == 0) {
				next;
			}
			printf "(%s:%s) received $bytes\n", $handle->peerhost(), $handle->peerport();
			$senddest = sockaddr_in($handle->peerport(), inet_aton($handle->peerhost()));
			my $out = processMessage($user_input);
			$socket->send($out, 0, $senddest) if (defined $out);;
		}
	}
}

sub processMessage {
	my ($message) = @_;
	printf "MESSAGE:%s\n", $message if ($opt_verbose);
	my @p = split /\s/, $message;
	return $reps{$p[1]};
}

sub handleConnectionsClient
{
	my $ready_readers;

	my $seq=0;
	while (1) {
		my $out = shift @coms;

		exit if (!defined $out);

		printf "Sending :%s\n", $out;
		$socket->send($out, 0, $senddest);
		($ready_readers) = IO::Select->select($readers, undef, undef, 10);
		for my $handle (@{$ready_readers}) {
			my $bytes = $handle->recv($user_input, 2048);
			if (!defined $bytes) {
				exit;
			}
			if (($bytes = length($user_input)) == 0) {
				next;
			}
			printf "Received:%s\n", $user_input;
		}
	}
}

sub loadTestFile {
	return if (!defined $opt_sim_file);

	open IF, $opt_sim_file or die "Can't open:$opt_sim_file";
	while (<IF>) {
		chomp();
		s/^[\s]*//;
		next if (/^$/);
		next if (/^#/);
		my @p = split /\s/;
		printf "Load:%s\n", join('|', @p) if ($opt_verbose);

		if (uc($p[0]) eq 'C') {
			push @coms, $_;
		} else {
			$reps{$p[1]} = $_;
		}
	}
	close IF;
}
##########################################################################################
