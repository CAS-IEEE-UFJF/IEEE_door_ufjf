//preciso dessas 2 funções para abrir e fechar o slider;
function w3_open()
{
  document.getElementById("mySidebar").style.display = "block";
  document.getElementById("marter_container_left").style.width = "200px";
}

function w3_close()
{
  document.getElementById("mySidebar").style.display = "none";
  document.getElementById("marter_container_left").style.width = "48px";
}

function w3_open_right()
{
  document.getElementById("mySidebar_right").style.display = "block";
  document.getElementById("marter_container_right").style.width = "300px";
}

function w3_close_right()
{
  document.getElementById("mySidebar_right").style.display = "none";
  document.getElementById("marter_container_right").style.width = "48px";
}

if (window.innerWidth > 1000)
{
  w3_open();
  w3_open_right();
}
