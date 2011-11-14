(function ($) {

Drupal.behaviors.listFilter = {
  attach: function (context, settings) {
    $('#discography-block-list-filter select').each(function(){
      var name = $(this).attr('name');
      var unique = [];
      var select = $(this);
      $('.list .'+name).each(function(){
        var item = $(this).html();
        if (name === 'pubdate')
          item = item.slice(0, 4);
        for (var i = 0; i < unique.length; ++i) {  
          if (unique[i] === item)
            break;
        }
        if (i === unique.length) {
          unique.push(item);
          select.append("<option value=\""+item+"\">"+item+"</option>");
        }
      });
    });
    $('#list-count').html($('.album:visible').length);

	  $('#discography-block-list-filter select').change(function () {
	    var val = $(this).val();
      $('#discography-block-list-filter select').val('-');
      $(this).val(val);
      if($(this).val() == '-'){
        $('.list').show();
      } else {
        $('.list').hide();
        var name = $(this).attr('name');
        $('.'+name+':contains('+$(this).val()+')').parents('.list').fadeIn();
      }
      $('#list-count').html($('.list:visible').length);
      return false;
    });
}};

Drupal.behaviors.lyricShow = {
  attach: function (context, settings) {
	$('a.popuplyric').click(function() {
	  var url = this.href + '/popup';
	  if ($('#lyric-dialog').length > 0) {
		var dialog = $('#lyric-dialog');
	  } else {
		var dialog = $('<div id="lyric-dialog" style="display:hidden"></div>').appendTo('body');
	  }
	  dialog.load(
		url,
		{},
		function (responseText, textStatus, XMLHttpRequest) {
		  dialog.dialog({
			minWidth: 600, 
			resizable: false, 
			modal: true,
		  });
		  $('#lyric-tabs').tabs();
	    }
	  );
	  return false;
	});
	
	$('#lyric-tabs').each(function() {this.tabs();});
  }
}

Drupal.behaviors.xiamiPlayer = {
  attach: function (context, settings) {
    $('.toggle_player').toggle(function() {
      var link = $(this).attr('href');
      var player_id = link.substr(link.lastIndexOf('/')+1);
      var player = '<embed src="http://www.xiami.com/widget/'+player_id+'/singlePlayer.swf" type="application/x-shockwave-flash" width="257" height="33" autostart="true" allowScriptAccess="always" wmode="transparent"></embed>';
      $('.player').addClass('element-hidden');
      $('.player td').html('');
      $(this).parent().parent().next().removeClass('element-hidden');
      $(this).parent().parent().next().children().html(player);
      return false;
    },function() {
      $(this).parent().parent().next().addClass('element-hidden');
      return false;
    });
  }
}
Drupal.loadBooklet = {
	attach: function() {
	  var feed  = $('#slideshow span').text();
	  var options = {
		displayTime:2000, 
		transistionTime:600, 
		scaleImages:true, 
		linkTarget : google.feeds.LINK_TARGET_BLANK,
		fullControlPanel : true,
        fullControlPanelSmallIcons : true,
	  };
	  var ss = new GFslideShow(feed, "slideshow", options);
	}
}
Drupal.behaviors.searchBox = {
  attach: function() {
    var dv = '';
    $("input[name='search']").focus(function() {
      dv = $(this).val();
      $(this).val('');
      $(this).addClass('focus');
    }).blur(function() {
      if ($(this).val() == '')
      {
        $(this).val(dv);
      }
      $(this).removeClass('focus');
    });
  }
}
/*
if(typeof(google) != "undefined") {
	google.load("feeds", "1");
	google.setOnLoadCallback(Drupal.loadBooklet);
}
*/
})(jQuery);