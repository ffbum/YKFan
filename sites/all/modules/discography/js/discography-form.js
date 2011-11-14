(function ($) {

/*jSelectDate*/
Drupal.behaviors.addDatePicker = {
  attach: function (context) {
  //$("input.pubdate:not(.addjSelectDate-processed)", context).addClass('addjSelectDate-processed').jSelectDate({css:"date", yearBeign: 1986,yearEnd: 2020,disabled : false,showLabel : false});
  //$.datepicker.setDefaults($.datepicker.regional['zh-CN']);
  var months = ['一月','二月','三月','四月','五月','六月','七月','八月','九月','十月','十一月','十二月'];
  var days = ['日', '一', '二', '三', '四', '五', '六'];
  $('input.pubdate').datepicker({
    monthNames:    months,
    dayNamesMin:   days,
    dateFormat: 'yy-mm-dd'
  });
  //$(this).jSelectDate;
  
  }
}

/*dxCombobox*/
/*
Drupal.behaviors.adddxCombobox = function(context) {$('#edit-source:not(.adddxCombobox-processed)', context).addClass('adddxCombobox-processed').dxCombobox();}
*/
/*PreviewImage*/
/*
Drupal.behaviors.previewImage = function (context) {
  $("#edit-cover:not(.previewImage-processed)", context).addClass('previewImage-processed').change(function(){
    var patn = /\.jpg$|\.jpeg$|\.gif$|\.png$/i;
    var imgfile = $(this).val();
    if(patn.test(imgfile)){
      document.getElementById('cover-wapper').filters.item("DXImageTransform.Microsoft.AlphaImageLoader").src = imgfile;
      //$('#cover-wapper').get(0).filters.item("DXImageTransform.Microsoft.AlphaImageLoader").src = imgfile;
      var tempimg=new Image(); 
      tempimg.src= imgfile; 
      Drupal.resizePreview(tempimg);
    } else{
      alert("Not image!");
      $("#cover-img").attr('src', '/sites/all/modules/discography/nocover.thumbnail.png');
      //$('#cover-wapper').get(0).filters.item("DXImageTransform.Microsoft.AlphaImageLoader").src = '/sites/all/modules/discography/nocover.thumbnail.png';
      $('#cover-description').html('Please select cover of this album.');
    }
  });
}

Drupal.resizePreview = function (o) {
  var ow=o.width;
   var oh=o.height;
   if (ow > oh) {
     oh = oh*100/ow;
     ow = 100;
   } else {
     ow = ow*100/oh;
     oh = 100;
   }
   $('#cover-wapper').css("width", ow + 'px');
   $('#cover-wapper').css("height", oh + 'px');
   $("#cover-img").remove();
   $('#cover-description').html('Now size:'+o.width+'X'+o.height);
}
*/
/*Fast Insert*/
/*
Drupal.behaviors.inputHints = function () {
	$('.form-text,.form-select').inputHintBox({className:'simple_box',source:'attr',attr:'rel',incrementLeft:135, incrementTop:40});
}


Drupal.insertInline = {
  attach: function (id, a) {
    var i = $('#'+id);
    //alert($(a).html());
    if(i.val() == '') {
      i.val($(a).html());
    } else {
      i.val(i.val() + ',' + $(a).html());
    }
    return false;
  }
}
*/
})(jQuery);