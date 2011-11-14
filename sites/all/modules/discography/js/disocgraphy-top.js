// Lee FocusIMG 0.5 http://www.xij.cn/blog/?p=38

jQuery.fn.focusImg=function() {
 var obj=$(this);
 objName=obj.attr("id");
 obj.children("ol").attr("id",objName+"-fragment");
 var num=(obj.children("ol").children("li").length)-1;
 var temp_b=new String();
 for(var i=0;i<=num;i++){
  temp_b+="<a id=\""+objName+"-button-"+i+"\" href=\"#"+objName+"-fragment-"+i+"\">"+(i+1)+"</a>";
  obj.children("ol").children("li").eq(i).attr("id",objName+"-fragment-"+i);
 }
 obj.append("<span id='"+objName+"-button'>"+temp_b+"</span>");
 obj.children("span").children("a").eq(0).addClass("a2");

 obj.children("span").children("a").click(function(){
   obj.children("span").children("a").removeClass("a2");
   $(this).addClass("a2");
   var id=($(this).attr("id")).replace("button","fragment");
   obj.children("ol").children("li").css("z-index", 0);
   $("#"+id).css("z-index", 1);
   return false;
 });
 var i=1;
 topBoxRun=function(){
   var now = obj.children("ol").children("li").eq(i);
   obj.children("span").children("a").removeClass("a2");
   var id=(now.attr("id")).replace("fragment","button");
   $("#"+id).addClass("a2");
   obj.children("ol").children("li").css("z-index", 0);
   now.css("z-index", 1).slideDown("slow");
   i++;
	 $count = $('#focusImg-fragment li').size();
   if (i >= $count) {i=0;}
 }
 var t=setInterval(topBoxRun,5000);
}

Drupal.behaviors.focusImage = function () {
  $("#focusImg").focusImg();
}

Drupal.behaviors.readerinit = function () {
  $('#subscribe').hover(function () {$('#subscribe ol').slideDown();}, function () {$('#subscribe ol').hide();});
}
