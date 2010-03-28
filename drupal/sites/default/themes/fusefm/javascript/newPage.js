$(document).ready(function(){
  $("a.target_blank").click(function(){
    window.open(this.href,'','scrollbars=no,menubar=no,height=615,width=835,resizable=no,toolbar=no,location=no,status=no');
    return false;
  });
});
