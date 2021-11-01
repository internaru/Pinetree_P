var pageNav = function(theURL) {

    window.open(theURL, "_self");
    
    return true;

};

function refresh()
{
  pageNav(document.URL);
}

function new_window_focus(url)
{
  new_window = window.open(url);
  new_window.focus();
}

function highlight_topic_cell( topic_id, subtopic_id ) {

  var cell1 = document.getElementById( topic_id );
  cell1.className = "clkbl_clkd_1 clrstyle3";
  
  var cell2 = document.getElementById( subtopic_id );
  cell2.className = "clkbl_clkd_2";
  
}

function toggle_view( div_id ) {
  var obj = document.getElementById( div_id );
  
  if ( obj.style.display == "none" ) 
  {
    obj.style.display = "block"; 
  } 
  else if ( obj.style.display == "block" ) 
  {
    obj.style.display = "none";
  }
  
}

function set_disabled_by_name( name, setting )
{
  obj = document.getElementsByName( name );
  for ( i = 0; i < obj.length; i++ ) {
    obj[ i ].disabled = setting;
  }
}

/* Obscures a textbox */
function obscure_text_box ( obscure, text_box_id ) {
  var input_type = "password";
  
  if ( obscure == false ) {
    input_type = "text";
  }

  document.getElementById( text_box_id ).type = input_type;
  
}

function show_passkey( show, hidden_input, visible_input ) 
{
  if ( show == true )
  {
    /* Show the passkey */  
    document.getElementById(hidden_input).style.display = "none";
    document.getElementById(visible_input).style.display = "block";
  
    /* Focus on the shown passkey */
    document.getElementById(visible_input).focus();
  }
  else if ( show == false )
  {
    /* Copy shown passkey to hidden passkey */
    document.getElementById(hidden_input).value = document.getElementById(visible_input).value;

    document.getElementById(hidden_input).style.display = "block";
    document.getElementById(visible_input).style.display = "none";
  }
}

/* Obscures WEP keys input textboxes if checked == true */
function hide_show_wep_keys( hide, key1_id, key2_id, key3_id, key4_id ) {
  
  obscure_text_box( hide, key1_id );
  obscure_text_box( hide, key2_id );
  obscure_text_box( hide, key3_id );
  obscure_text_box( hide, key4_id );
}

/* Checks the key in key_id textbox and shows a warning message on key_info_div_id div */
function check_wep_key( key_id, key_info_div_id ) {

  var key = document.getElementById( key_id ).value;
  
  if ( key.length == 0 || key.length == 5 || key.length == 13 ) {
    document.getElementById( key_info_div_id ).innerHTML = "";
  }
  else {
    document.getElementById( key_info_div_id ).innerHTML = "<!--#ssi IDs STRING_EWS_WEP_KEY_LENGTH_INFO -->";
  }
}

/* Checks the key in key_id textbox and shows a warning message on key_info_div_id div */
function check_wpa_key( key_id, key_info_div_id ) {

  var key = document.getElementById( key_id ).value;
  
  if ( key.length > 7 && key.length < 64 ) {
    document.getElementById( key_info_div_id ).innerHTML = "";
    return 0;
  }
  else {
    document.getElementById( key_info_div_id ).innerHTML = "<!--#ssi IDs STRING_EWS_WPA_PASSPHRASE_LENGTH_INFO -->";
    return -1;
  }
}

function do_http_get_request ( url, onreadystatechange_fn )
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
}

/* Clears/hides an alert screen and window */
function hide_alert()
{
  /* clear the screen */
 document.getElementById("alert_screen").style.display = "none"; 
 /* clear the alert */  
 document.getElementById("alert_div").style.display = "none";
 
 location.hash = "";
}

/* Shows the alert screen with the specified parameters
 *  alert_title - text for the heading on the alert
 *  alert_body - text for the body of the alert
 *  btn1_onclk_fn - callback function when button 1 is clicked
 *  btn1_text - text shown on button 1 
 *  btn2_onclk_fn - callback function when button 2 is clicked
 *  btn2_text - text shown on button 2
 */
function show_alert_screen( alert_title, alert_body, btn1_onclk_fn, btn1_text )
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
}

function show_alert_screen_2btns( alert_title, alert_body, btn1_onclk_fn, btn1_text, btn2_onclk_fn, btn2_text )
{
  show_alert_screen( alert_title, alert_body, btn1_onclk_fn, btn1_text );

  if ( btn2_text )
  {
    document.getElementById("alert_btn2").onclick = function () { btn2_onclk_fn(); };
    document.getElementById("alert_btn2_text").innerHTML = btn2_text;
    document.getElementById("alert_btn2").style.display = "block";
  }

}

function show_alert_screen_3btns( alert_title, alert_body, 
                                   btn1_onclk_fn, btn1_text, 
                                   btn2_onclk_fn, btn2_text,
                                   btn3_onclk_fn, btn3_text  )
{
  show_alert_screen_2btns( alert_title, alert_body, btn1_onclk_fn, btn1_text, btn2_onclk_fn, btn2_text );

  if ( btn3_text )
  {
    document.getElementById("alert_btn3").onclick = function () { btn3_onclk_fn(); };
    document.getElementById("alert_btn3_text").innerHTML = btn3_text;
    document.getElementById("alert_btn3").style.display = "block";
  }
}

function get_printer_id ( mfg_id, mdl_id )
{
  var printer_1284_string = "<!--#ssi IDr OID_VENDOR_VPI_1284_STRING -->";
  var key_value_pairs = printer_1284_string.split(";");
  var mdl, mfg;
  
  for ( i=0; i < key_value_pairs.length; i++ )
  {
    /* Find the manufacturer string */
    if ( ( key_value_pairs[i].search("MFG") != -1 ) || ( key_value_pairs[i].search("MANUFACTURER") != -1 ) )
    {
      mfg = key_value_pairs[i].split(":")[1];
    }
    /* Find the model string */
    if ( ( key_value_pairs[i].search("MDL")  != -1 ) || ( key_value_pairs[i].search("MODEL") != -1 ) )
    {
      mdl = key_value_pairs[i].split(":")[1];
    }
  }
  
  if ( mdl )
  {
    document.getElementById(mdl_id).innerHTML = mdl;
  }
  
  if ( mfg )
  {
    document.getElementById(mfg_id).innerHTML = mfg;
  }
}

function expand_topic( expand, topic_name )
{
  if ( expand == true)
  {
    document.getElementById( topic_name+"_subtopics" ).style.display = "block";
    document.getElementById( topic_name+"_nav_icon_img" ).src = "/images/block_arrow_up.png";
  }
  else
  {
    document.getElementById( topic_name+"_subtopics" ).style.display = "none";
    document.getElementById( topic_name+"_nav_icon_img" ).src = "/images/block_arrow_down.png";
  }
}

function toggle_topic( topic_name )
{
  var topic_display = document.getElementById( topic_name +"_subtopics" ).style.display;
  
  /* show the desired subtopics */
  if ( topic_display == "none")
  {
    expand_topic(true, topic_name );
  }
  else
  {
    expand_topic( false, topic_name );
  } 
}

function goto_menu()
{
  pageNav('/home/menu');
}


