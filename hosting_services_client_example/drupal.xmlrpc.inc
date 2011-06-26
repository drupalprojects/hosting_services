<?php
/***************************************************/
require_once('includes/bootstrap.inc');
require_once('includes/common.inc');
require_once('includes/xmlrpc.inc');

class DrupalXmlrpc {

  function __construct( $domain = '', $apiKey = '', $endPoint = '', $verbose = FALSE ) 
  {
    // set local domain or IP address
    // this needs to match the domain set when you created the API key
    $this->domain = $domain;
   
    // set API key
    $this->kid = $apiKey;
   
    // set target web service endpoint
    $this->endpoint = $endPoint;

    // extended debugging
    $this->verbose = $verbose;
   
    // call system.connect to get our required anonymous sessionId:
    $retVal = $this->send( 'system.connect', array() );
    $this->session_id = $retVal['sessid'];

    if ($this->verbose) {
       $func = 'DrupalXmlrpc->__construct:';
       if ($this->session_id)
            error_log( $func.' got anonymous session id fine' );
       else error_log( $func.' failed to get anonymous session id!' );
    }
  }

  /***********************************************************************
  * Function for sending xmlrpc requests
  */
  public function send( $methodName, $functionArgs = array(), $functionObjects = array() )
  {
    $protocolArgs = array();

    // only the system.connect method does not require a sessionId:
    if ($methodName == 'system.connect') {
       $protocolArgs = array( $this->endpoint, $methodName );
    }
    else {
       $timestamp = (string)time();
       $nonce = $this->getUniqueCode("10");
   
       // prepare a hash
       $hash_parameters = array( $timestamp, $this->domain, $nonce, $methodName );
       $hash = hash_hmac("sha256", implode(';', $hash_parameters), $this->kid);
   
       // prepared the arguments for this service:
       // note, the sessid needs to be the one returned by user.login
       $protocolArgs = array( $this->endpoint, $methodName, $hash, $this->domain, $timestamp, $nonce, $this->session_id );
    
    // this won't function in the case of structs, - in some cases, each one needs to be an arg, not 
    // have the whole passed array appended 
        
    if( isset($functionObjects) ) {
        array_push( $protocolArgs, $functionObjects );
       }
    if( isset($functionArgs) ) { 
        array_splice( $protocolArgs, count($protocolArgs), 0, $functionArgs );
    }
  }
  return call_user_func_array( "_xmlrpc", $protocolArgs );

}

  /***************************************************
   * login and return user object
   */
  public function userLogin( $userName = '', $userPass = '' ) 
  {
    if ($this->verbose)
         error_log( 'DrupalXmlrpc->userLogin() called with userName "'.$userName.'" and pass "'.$userPass.'"' );
 
    // clear out any lingering xmlrpc errors:
    xmlrpc_error( NULL, NULL, TRUE );

    $retVal = $this->send( 'user.login', array( $userName, $userPass ), NULL );
    
    if (!$retVal && xmlrpc_errno()) {
       if ($this->verbose)
          error_log( 'userLogin() failed! errno "'.xmlrpc_errno().'" msg "'.xmlrpc_error_msg().'"' );
       return FALSE;
    }
    else {
       // remember our logged in session id:
       $this->session_id = $retVal['sessid'];

       // we might need the user object later, so save it:
       $user = new stdClass();
       $user = (object)$retVal['user'];
       $this->authenticated_user = $user;
       return $user;
    }
  }

  /***************************************************
   * logout, returns 0 for okay, or -1 for error.
   */
  public function userLogout()
  {
    $retVal = $this->send( 'user.logout' );
    if (!$retVal) {
       if ($this->verbose)
          error_log( 'userLogout() failed! errno "'.xmlrpc_errno().'" msg "'.xmlrpc_error_msg().'"' );
       return -1;
    }

    return 0; // meaning okay
  }

  /***************************************************
  * Function for generating a random string, used for
  * generating a token for the XML-RPC session
  */
  private function getUniqueCode($length = "") 
  {
    $code = md5(uniqid(rand(), true));
    if ($length != "") 
         return substr($code, 0, $length);
    else return $code;
  }
}
?>