/* 

 client-tools.js

 Client logic for the EWS HTML/Toolbox Configuration Program

 ************************************************************
   (c) Copyright 2008-2009 Marvell International Ltd.      
  
                 Marvell Confidential
 *===========================================================

 This file is intended only for the uses agreed to. 

 You are hereby notified that any dissemination, distribution or copying of 
 this material accept for the purposes agreed to is strictly prohibited.

*/

// Error Handling Colors
var ERROR_HIGHLIGHT_ON_COLOR = "yellow";
var ERROR_HIGHLIGHT_OFF_COLOR = "white";

// Count of Device Functions - Used to Alter Menus and Usage Page
var gDeviceFunctionsCount = <!--#ssi IDm EWS_DEVICE_FUNCTIONS_COUNT -->;

// Provided for any necessary left navigation initialization
initializeLeftNav = function(myDoc) {

    var elementNode = null;
    
    // check for number of device functions
    if (gDeviceFunctionsCount == 1) {

          // take out scan and copy headings in left nav as needed
          try {
              elementNode = myDoc.getElementById("leftRowScanSettings");
              elementNode.parentNode.removeChild(elementNode);
              
              elementNode = myDoc.getElementById("leftRowCopySettings");
              elementNode.parentNode.removeChild(elementNode);

              myDoc.getElementById("leftNavPrinting").innerHTML = 'Printing';
          }
          catch (e) {
              // don't do any recovery
          }
    }

};


// function to perform HTTP Operation using AJAX
doAJAX = function(theHTTPOperation, theURL, theData, performOverride) {

  var xmlhttp = false;
  var doOverride = false;
  
  var myHTTPOperation = "";

  /* The following is conditionally compiled JavaScript so the code may deal 
     with old IE versions and also deal with security-blocked creation of 
     Javascript objects. 
  */

  /*@cc_on @*/

  /*@if (@_jscript_version >= 5)

  try {
    xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
   } catch (e) {
      try {
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
      } catch (E) {
        xmlhttp = false;
      }
   }

  /*@end @*/

  /* see if we still need to get a new xmlhttp object */
  if (!xmlhttp && typeof XMLHttpRequest!='undefined') {
      xmlhttp = new XMLHttpRequest();
  }

  /* check for a request to perform http override for PUT or DELETE */
  if (performOverride) {
      /* caller invoked http override; using post, instead of delete or put */ 
      myHTTPOperation = "POST";
  }
  else {
      myHTTPOperation = theHTTPOperation;
  }
  

  /* open the target URL */
  xmlhttp.open(myHTTPOperation, theURL, true);
  
  /* set the proper header information along with the request */
  xmlhttp.setRequestHeader("Content-type", "application/xml");
  xmlhttp.setRequestHeader("Content-length", theData.length);
  
  /* employ override header if requested to override POST operation at server */
  if (performOverride === true) {
      xmlhttp.setRequestHeader("X-Http-Method-Override", theHTTPOperation);
  }
  
  /* set close connection header */
  xmlhttp.setRequestHeader("Connection", "close");
  
  /* make a local function to handle the AJAX callback */
  xmlhttp.onreadystatechange=function() {

      if (xmlhttp.readyState==4) {
            ajaxCallback(xmlhttp, theHTTPOperation, theURL, theData, performOverride);
      }
  }

  /* if there is data to send with http operation, send data along too */
  if (theData.length == 0) {
      xmlhttp.send(null); /* do the http operation */
  }
  else {
      xmlhttp.send(theData); /* do the http operation and send the data */
  }

};

// function to show a customizable alert box
showAlert = function(left,top,title,message,isError) {
    // "alertDiv," "alertTitle," and "alertMessage" must be in the html page
    // ".alert" style must be defined in a css section
    if (isError == true) {
        document.getElementById("alertImageID").src="/images/alert-icon.gif";
    }
    else {
        document.getElementById("alertImageID").src="/images/info-icon.gif";
    }
    document.getElementById("alertDiv").style.left=left+'px';
    document.getElementById("alertDiv").style.top=top+'px';
    document.getElementById("alertDiv").style.display='block';
    document.getElementById("alertTitle").innerHTML = title;
    document.getElementById("alertMessage").innerHTML = message;
};


// save page implementation
var saveThisPage = function(theURL) {

    alert("Save Not Implemented Yet For: " + theURL);

    return true;

};

// check to see if a string is numeric
var isNumeric = function(x) {
    // Use this function like this: if (isNumeric(myVar)) { }
    // regular expression that validates a value is numeric
    var RegExp = /^(\d*)$/; // Note: this WILL not allow a number that has a decimal point or comma
    // compare the argument to the RegEx
    // the 'match' function returns 0 if the value didn't match
    var result = x.match(RegExp);

    return result;
};

// check to see if a string is host name
var isHostName = function(x) {
    // regular expression that validates a value is host name
    var RegExp = /^[a-zA-Z\d]+(-[a-zA-Z\d]+|[a-zA-Z\d]*)*$/;
    // compare the argument to the RegEx
    // the 'match' function returns 0 if the value didn't match
    var result = x.match(RegExp);

    return result;
};

// check to see if a string is Bonjour service name
var isBonjourName = function(x) {

    // FIX THIS UP LATER
    
    // regular expression that validates a value is domain name
    // var RegExp = /^[a-zA-Z\d]+(-[a-zA-Z\d]+|[a-zA-Z\d]*)*(\.[a-zA-Z\d]+(-[a-zA-Z\d]+|[a-zA-Z\d]*)*)*$/;
    // compare the argument to the RegEx
    // the 'match' function returns 0 if the value didn't match
    // var result = x.match(RegExp);

    var result;
    
    if (x.length === 0) {
        result = false;
    }
    else {
        result = true;
    }
    
    return result;
};

// check to see if a string is domain name
var isDomainName = function(x) {

    // regular expression that validates a value is domain name
    var RegExp = /^[a-zA-Z\d]+(-[a-zA-Z\d]+|[a-zA-Z\d]*)*(\.[a-zA-Z\d]+(-[a-zA-Z\d]+|[a-zA-Z\d]*)*)*$/;
    // compare the argument to the RegEx
    // the 'match' function returns 0 if the value didn't match
    var result = x.match(RegExp);

    return result;
};

// check to see if a string is IPv4 address
var isIPv4 = function(x) {
    // regular expression that validates a value is IPv4 address
    var RegExp = /^((2[0-4]\d|25[0-5]|[01]?\d\d?)\.){3}(2[0-4]\d|25[0-5]|[01]?\d\d?)$/;
    // compare the argument to the RegEx
    // the 'match' function returns 0 if the value didn't match
    var result = x.match(RegExp);

    return result;
};

// check to see if a string is IPv6 address
var isIPv6 = function(x) {
    // regular expression that validates a value is IPv6 address
    var RegExpStr = "(^([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}$)" +
                    "|(^([0-9a-fA-F]{1,4}:){1,1}(:[0-9a-fA-F]{1,4}){1,6}$)" +
                    "|(^([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}$)" + 
                    "|(^([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}$)" +
                    "|(^([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}$)" +
                    "|(^([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}$)" +
                    "|(^([0-9a-fA-F]{1,4}:){1,6}(:[0-9a-fA-F]{1,4}){1,1}$)" +
                    "|(^([0-9a-fA-F]{1,4}:){1,7}:$)" +
                    "|(^:(:[0-9a-fA-F]{1,4}){1,7}$)";
                    
    // compare the argument to the RegEx
    // the 'match' function returns 0 if the value didn't match
    var result = x.match(RegExpStr);    
    return result;
};

// do Number validation
var checkNumber = function(fieldID, numberToCheck, minValue, maxValue) {
    //var status = true;
    
    if (numberToCheck.length === 0){
      //status = false;
      document.getElementById('errorMessage').innerHTML = '<!--#ssi IDs STRING_EWS_ERROR_NUMERIC_VALUE_REQUIRED -->';
      document.getElementById(fieldID).focus();  // set focus to current field  
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_ON_COLOR; // highlight the background
      }
    else if ( numberToCheck>maxValue || numberToCheck<minValue || ( !isNumeric(numberToCheck))) {
      if (minValue === 0) {
          document.getElementById('errorMessage').innerHTML = '<!--#ssi IDs STRING_EWS_ERROR_NON_NEGATIVE_REQUIRED -->';
      }
      else {
          document.getElementById('errorMessage').innerHTML = '<!--#ssi IDs STRING_EWS_ERROR_NUMERIC_RANGE_REQUIRED -->';
      }
      document.getElementById(fieldID).focus();  // set focus to current field  
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_ON_COLOR; // highlight the background
     }
    else {
      document.getElementById('errorMessage').innerHTML = ""; // no error
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_OFF_COLOR; // don't highlight background
    }
        
};

// do Host Name validation
var checkHostName = function(fieldID, hostNameToCheck, limitLength) {
    if (!isHostName(hostNameToCheck) || hostNameToCheck.length>limitLength) {
      document.getElementById('errorMessage').innerHTML = '<!--#ssi IDs STRING_EWS_ERROR_HOST_NAME_REQUIRED -->';
      document.getElementById(fieldID).focus();  // set focus to current field
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_ON_COLOR; // highlight the background
    }
    else {
      document.getElementById('errorMessage').innerHTML = ""; // no error
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_OFF_COLOR; // don't highlight background
    }
};

// do Domain Name validation
var checkDomainName = function(fieldID, domainNameToCheck) {
    if (!isDomainName(domainNameToCheck)) {
      document.getElementById('errorMessage').innerHTML = '<!--#ssi IDs STRING_EWS_ERROR_DOMAIN_NAME_REQUIRED -->';
      document.getElementById(fieldID).focus();  // set focus to current field
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_ON_COLOR; // highlight the background
    }
    else {
      document.getElementById('errorMessage').innerHTML = ""; // no error
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_OFF_COLOR; // don't highlight background
    }
};

// do Bonjour Name validation
var checkBonjourName = function(fieldID, bonjourNameToCheck) {
    if (!isBonjourName(bonjourNameToCheck)) {
      document.getElementById('errorMessage').innerHTML = '<!--#ssi IDs STRING_EWS_ERROR_BONJOUR_NAME_REQUIRED -->';
      document.getElementById(fieldID).focus();  // set focus to current field
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_ON_COLOR; // highlight the background
    }
    else {
      document.getElementById('errorMessage').innerHTML = ""; // no error
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_OFF_COLOR; // don't highlight background
    }
};


// do IPv4 validation
var checkIPv4 = function(fieldID, ipv4ToCheck) {
    if (!isIPv4(ipv4ToCheck)) {
      document.getElementById('errorMessage').innerHTML = '<!--#ssi IDs STRING_EWS_ERROR_IPV4_ADDRESS_REQUIRED -->';
      document.getElementById(fieldID).focus();  // set focus to current field
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_ON_COLOR; // highlight the background
    }
    else {
      document.getElementById('errorMessage').innerHTML = ""; // no error
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_OFF_COLOR; // don't highlight background
    }
};

// do IPV6 validation
var checkIPv6 = function(fieldID, ipv6ToCheck) {
    if (!isIPv6(ipv6ToCheck)) {
      document.getElementById('errorMessage').innerHTML = '<!--#ssi IDs STRING_EWS_ERROR_IPV6_ADDRESS_REQUIRED -->';
      document.getElementById(fieldID).focus();  // set focus to current field
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_ON_COLOR; // highlight the background
    }
    else {
      document.getElementById('errorMessage').innerHTML = ""; // no error
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_OFF_COLOR; // don't highlight background
    }
};

// do Prefix length validation
var checkPrefixLength = function(fieldID, prefixLengthToCheck) {
    if (prefixLengthToCheck != "48" && prefixLengthToCheck != "64" && prefixLengthToCheck != "128") {
      document.getElementById('errorMessage').innerHTML = '<!--#ssi IDs STRING_EWS_ERROR_IPV6_PREFIX_REQUIRED -->';
      document.getElementById(fieldID).focus();  // set focus to current field
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_ON_COLOR; // highlight the background
    }
    else {
      document.getElementById('errorMessage').innerHTML = ""; // no error
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_OFF_COLOR; // don't highlight background
    }
};

// do Community Name validation
var checkCommunityName = function(fieldID, communityNameToCheck, limitLength) {
    if (communityNameToCheck.length>limitLength) {
      document.getElementById('errorMessage').innerHTML = '<!--#ssi IDs STRING_EWS_ERROR_SNMP_COMMUNITY_NAME_REQUIRED -->';
      document.getElementById(fieldID).focus();  // set focus to current field
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_ON_COLOR; // highlight the background
    }
    else {
      document.getElementById('errorMessage').innerHTML = ""; // no error
      document.getElementById(fieldID).style.backgroundColor = ERROR_HIGHLIGHT_OFF_COLOR; // don't highlight background
    }
};

var toggle_view = function( div_id ) {
  var obj = document.getElementById( div_id );
  
  if ( obj.style.display == "none" ) 
  {
    obj.style.display = "block"; 
  } 
  else if ( obj.style.display == "block" ) 
  {
    obj.style.display = "none";
  }
  
};

var new_window_focus = function(url)
{
  new_window = window.open(url);
  new_window.focus();
};

var do_http_get_request = function( url, onreadystatechange_fn )
{
  var http;
  
  if (window.XMLHttpRequest) {
    // code for IE7+, Firefox, Chrome, Opera, Safari
    http = new XMLHttpRequest();
  } 
  else if (window.ActiveXObject) 
  {
    // code for IE6, IE5
    http = new ActiveXObject("Microsoft.XMLHTTP");
  } 
  else 
  {
    alert("Your browser does not support XMLHTTP!");
  }
  
  if ( onreadystatechange_fn )
  {
    http.onreadystatechange = function() 
    {
      onreadystatechange_fn( http );
    }
  }

  http.open("GET", url, true);
  http.send(null);
};

/* Clears/hides an alert screen and window */
var hide_alert = function()
{
  /* clear the screen */
 document.getElementById("alert_screen").style.display = "none"; 
 /* clear the alert */  
 document.getElementById("alert_div").style.display = "none";
 
 location.hash = "";
};

/* Shows the alert screen with the specified parameters
 *  alert_title - text for the heading on the alert
 *  alert_body - text for the body of the alert
 *  btn1_onclk_fn - callback function when button 1 is clicked
 *  btn1_text - text shown on button 1 
 *  btn2_onclk_fn - callback function when button 2 is clicked
 *  btn2_text - text shown on button 2
 */
var show_alert_screen = function( alert_title, alert_body, btn1_onclk_fn, btn1_text )
{
  /* Hide any existing alert */
  hide_alert();

  /* Hide buttons unless needed */ 
  document.getElementById("alert_btn1").style.display = "none";
  document.getElementById("alert_btn2").style.display = "none";
  document.getElementById("alert_btn3").style.display = "none";
  
  /* Fill alert info */
  document.getElementById("alert_title_text").innerHTML = alert_title;
  document.getElementById("alert_info_text").innerHTML = alert_body;
  
  if ( btn1_text )
  {
    document.getElementById("alert_btn1").onclick = function () { btn1_onclk_fn(); };
    document.getElementById("alert_btn1_text").innerHTML = btn1_text;
    document.getElementById("alert_btn1").style.display = "block";
  }
  
  /* Show the alert */
  document.getElementById("alert_btns").style.display = "block";
  document.getElementById("alert_screen").style.display = "block";
  document.getElementById("alert_div").style.display = "block"; 
  
  /* scroll to the alert screen */
  location.hash = "#alert_div";
};

var show_alert_screen_2btns = function( alert_title, alert_body, btn1_onclk_fn, btn1_text, btn2_onclk_fn, btn2_text )
{
  show_alert_screen( alert_title, alert_body, btn1_onclk_fn, btn1_text );

  if ( btn2_text )
  {
    document.getElementById("alert_btn2").onclick = function () { btn2_onclk_fn(); };
    document.getElementById("alert_btn2_text").innerHTML = btn2_text;
    document.getElementById("alert_btn2").style.display = "block";
  }

};
