function peticionAJAX()
{
   var xmlHttpReq = new XMLHttpRequest();
   xmlHttpReq.onreadystatechange=function()
           {
               if (xmlHttpReq.readyState == 4 && xmlHttpReq.status==200)
                   document.getElementById("resultado").innerHTML = xmlHttpReq.responseText;
           }

   xmlHttpReq.open('GET', 'Cubo?cmd=' + document.f.cmd.value, true); 
   xmlHttpReq.send();
}
