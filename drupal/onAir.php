<?php
// 1: INITIALIZE THE CLIENT 
include('xmlrpc.inc');
$xmlrpc_client = new xmlrpc_client('/xmlrpc.php','192.168.180.116',443,'https');
//$xmlrpc_client->setDebug(1);

// 2: CONSTRUCT THE REQUEST (AN XML-RPC MESSAGE) 

$xmlrpc_msg_now = new xmlrpcmsg('station.program.get.at', array(new xmlrpcval(time(),'int'),new xmlrpcval(1406,'int'))); 
$xmlrpc_msg_next = new xmlrpcmsg('station.program.get.at', array(new xmlrpcval(time()+7200,'int'),new xmlrpcval(1406,'int')));

// 3: SEND THE REQUEST 

$xmlrpc_resp_now = $xmlrpc_client->send($xmlrpc_msg_now);
$xmlrpc_resp_next = $xmlrpc_client->send($xmlrpc_msg_next);

// 4: WORKING WITH THE SERVER'S RESPONSE 

if ($xmlrpc_resp_now == False || $xmlrpc_resp_next == False) { // check for successful transaction 

  die('Unable to contact XML-RPC Server');

}

if ($_GET['when'] == 'now') {
	if (!$xmlrpc_resp_now->faultCode()) {
		$v=$xmlrpc_resp_now->value();
		if ($_GET['link'] == 'yes') {
                        echo "<a href='" . $v->me[struct][node_url]->me[string] . "'";
			if ($_GET['newWindow'] == 'yes') {
				echo " onclick=\"window.open(this.href,'link_window');return false;\"";
			}
			echo ">";
                }

                echo $v->me[struct][title]->me[string];

                if ($_GET['link'] == 'yes') {
                        echo "</a>";
                }
	}
	else
	{
		print "An error occurred: ";
		print "Code: " . htmlspecialchars($xmlrpc_resp_now->faultCode()) . " Reason: '" . htmlspecialchars($xmlrpc_resp_now->faultString()) . "'</pre><br/>";
	}
}

if ($_GET['when'] == 'next') {
	if (!$xmlrpc_resp_next->faultCode()) {
        	$v=$xmlrpc_resp_next->value();
		if ($_GET['link'] == 'yes') {
	        	echo "<a href='" . $v->me[struct][node_url]->me[string] . "'";  
                        if ($_GET['newWindow'] == 'yes') {
                                echo " onclick=\"window.open(this.href,'link_window');return false;\"";
                        }
                        echo ">";
		}
		
		echo $v->me[struct][title]->me[string];
		
		if ($_GET['link'] == 'yes') {
			echo "</a>";
		}
	}
	else
	{
	        print "An error occurred: ";
	        print "Code: " . htmlspecialchars($xmlrpc_resp_next->faultCode()) . " Reason: '" . htmlspecialchars($xmlrpc_resp_next->faultString()) . "'</pre><br/>";
	}
}
?>
