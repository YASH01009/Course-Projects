set ns  [new Simulator]

$ns color 1 blue
$ns color 2 red

set nf [open out.nam w]
$ns namtrace-all $nf

proc finish {} {
        global ns nf
        $ns flush-trace
        close $nf
        exec nam out.nam
        exit 0
        }
        
set a [$ns node]
set b [$ns node]
set c [$ns node]
set d [$ns node]

$ns duplex-link $a $b 512Kb 100ms DropTail
$ns simplex-link $c $b 512Kb 100ms DropTail
$ns duplex-link $b $d 512Kb 100ms DropTail

#TCP_Config
set tcp0 [new Agent/TCP]
$ns attach-agent $a $tcp0

set sink0 [new Agent/TCPSink]
$ns attach-agent $b $sink0
	
$ns connect $tcp0 $sink0

set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp0

#UDP_Config
set udp [new Agent/UDP]
$ns attach-agent $c $udp

set null [new Agent/Null]
$ns attach-agent $b $null

$ns connect $udp $null

set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp

#TCP_Config
set tcp1 [new Agent/TCP]
$ns attach-agent $b $tcp1

set sink1 [new Agent/TCPSink]
$ns attach-agent $d $sink1
	
$ns connect $tcp1 $sink1

set ftp1 [new Application/FTP]
$ftp1 attach-agent $tcp1

$ns at 1 " $ftp0 start"
$ns at 3 " $ftp0 stop"
$ns at 4 " $cbr start"
$ns at 6 " $cbr stop"
$ns at 7 " $ftp1 start"
$ns at 9 " $ftp1 stop"
$ns at 10 " finish"

$ns run