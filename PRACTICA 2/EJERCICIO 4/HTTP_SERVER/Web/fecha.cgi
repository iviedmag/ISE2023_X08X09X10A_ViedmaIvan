t <html><head><title>RTC</title>
t <script language=JavaScript type="text/javascript" src="xml_http.js"></script>
t <script language=JavaScript type="text/javascript">
# Define URL and refresh timeout
t var formUpdate = new periodicObj("fecha.cgx", 500);
t function periodicUpdateAd() {
t   updateMultiple(formUpdate);
t   ad_elTime = setTimeout(periodicUpdateAd, formUpdate.period);
t }
t periodicUpdateAd();
t </script></head>
i pg_header.inc
t <h2 align=center><br>RTC/SNTP MODULE</h2>
t <p><font size="2">This page shows the date and time of the web server on <b>RTC Module</b>.<br></font></p>
t <form action=fecha.cgi method=post name=cgi>
t <input type=hidden value="fecha" name=pg>
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#aaccff>
t  <th width=40%>Item</th>
t  <th width=60%>Setting</th></tr>
# Here begin data setting which is formatted in HTTP_CGI.C module
t <tr><td><img src="pabb.gif">FECHA:</td>
t   <td align="center">
t <input type="text" readonly style="background-color: transparent; border: 0px"
c h 1  size="20" id="fecha" value="%s"></td></tr>
t <tr><td><img src="pabb.gif">HORA:</td>
t   <td align="center">
t <input type="text" readonly style="background-color: transparent; border: 0px"
c h 2  size="20" id="hora" value="%s"></td></tr>
t </font></table>
i pg_footer.inc
. End of script must be closed with period.
