<?php
// $Id: node.pages.inc,v 1.28.2.1 2008/02/27 19:44:44 goba Exp $

/**
 * @file
 * Page callbacks for adding, editing, deleting, and revisions management for content.
 */

/**
* Discography search page.
*/
 function discography_page() {
  drupal_set_breadcrumb(array());
  
  $output = '<div style="text-align:center;">';
  $output .= l(theme('image', array('path' => drupal_get_path('module', 'discography').'/discography.jpg', 'alt' => '全专辑列表')), 'discography/list/all', array('html' => TRUE));
  $output .= '</div>';
  $output .= discography_search_form();
  $output .= '<div id="discography-lists">特别列表：'.discography_lists().'</div>';

  $stats = variable_get('discography_stats', array('0','0','0','0','0'));
  $output .= '<div style="text-align:center;">作品库中共有 <em>'.$stats[0].'</em> 张专辑， <em>'.$stats[1].'</em> 首曲目， <em>'.$stats[2].'</em> 位艺术家， <em>'.$stats[3].'</em> 份歌词， <em>'.$stats[4].'</em> 份中文翻译歌词</div>';
  $lastupdate = format_date(variable_get('discography_album_lastupdate', time()), 'small', 'Y年m月d日');
  $output .= '<div class="meta" style="line-height:25px" ><span style="float:right">如列表页面反应较慢，推荐使用Firefox 3或Google Chrome浏览器。</span><span style="floadt:left">最后更新：<em>'.$lastupdate.'</em></span></div>';

  return array('content' => array('#markup' => $output));
}

function discography_search_form($inline = false) {
  $keyword = isset($_GET['keyword']) ? $_GET['keyword'] : '';
  $option = (isset($_GET['options']) && $_GET['options'] >= 0 && $_GET['options'] < 4) ? $_GET['options'] : 0;
  $id = $inline ? 'discography-searchform-inline' : 'discography-searchform-page';

  $output = '<div id="'.$id.'"><form action="/ykfan7/discography/search"  accept-charset="utf-8" method="get">';
  $output .= '<div class="form-item" id="edit-keyword-wrapper"><input type="text" maxlength="128" name="keyword" id="edit-keyword" size="70" value="'.$keyword.'" title="请输入搜索关键词，支持简体中文（如攻壳机动队），中文翻译名称（如信长之野望），简称缩写（如mf），片假名罗马字（如Trianguler）。请灵活利用。" class="form-text" /></div>';
  $output .= '<div class="form-item"><input type="submit" name="op" id="edit-search-button" value="搜索" class="form-submit" /></div>';
  $output .= '<div class="form-radios"><div class="form-item" id="edit-options-0-wrapper">';
  $output .= '<label class="option" for="search-album"><input type="radio" name="options" id="search-album" value="0" '.($option==0 ? 'checked="checked"' : '').'class="form-radio" /> 专辑</label>';
  $output .= '<label class="option" for="search-track"><input type="radio" name="options" id="search-track" value="1" '.($option==1 ? 'checked="checked"' : '').'class="form-radio" /> 曲目</label>';
  $output .= '<label class="option" for="search-artist"><input type="radio" name="options" id="search-artist" value="2" '.($option==2 ? 'checked="checked"' : '').'class="form-radio" /> 艺术家</label>';
  $output .= '<label class="option" for="search-lyric"><input type="radio" name="options" id="search-lyric" value="3" '.($option==3 ? 'checked="checked"' : '').'class="form-radio" /> 歌词</label></div></div>';
  $output .= '</form></div>';
  return $output;
}

function discography_search_page() {
  //导航设置
  //$breadcrumb[] = l('首页', NULL);
  //$breadcrumb[] = l('搜索', 'discography');
  //drupal_set_breadcrumb($breadcrumb);
  //关键词预处理
  if (isset($_GET['keyword'])) {
    $keyword = trim($_GET['keyword']);
  } else {
    drupal_goto('discography');
  }
/*
  drupal_add_library('system', 'ui.dialog');
  drupal_add_library('system', 'ui.tabs');
	drupal_add_js(drupal_get_path('module', 'discography') . '/js/discography.js');
  drupal_add_css(drupal_get_path('module', 'discography'). '/js/theme/cupertino/jquery.ui.theme.css', array('group' => 'CSS_THEME', 'weight' => 9999)); // jQuery Theme
*/
  //查询类型
  $option = (isset($_GET['options']) && is_numeric($_GET['options']) && $_GET['options'] >= 0 && $_GET['options'] < 4) ? $_GET['options'] : -1;
  $options = array('album', 'track', 'artist', 'lyric');
  $options_cn = array('专辑', '曲目', '艺术家', '歌词');
  
  //内联搜索表单
  //$output = discography_search_form(true);
  // 标签
  $build['tabs'] = array(
    '#prefix' => '<ul class="tabs primary">',
    '#suffix' => '</ul>',
    'all' => array(
      '#theme' => 'menu_local_task',
      '#link' => array('title' => '全部', 'href' => 'discography/search', 'localized_options' => array('query' => array('keyword' => $keyword))),
      '#active' => false,
    ),
  );
  foreach($options as $id => $type) {
    $build['tabs'][$type] = array(
      '#theme' => 'menu_local_task',
      '#link' => array('title' => $options_cn[$id], 'href' => 'discography/search', 'localized_options' => array('query' => array('keyword' => $keyword, 'options' => $id))),
      '#active' => false,
    );
  }

  //开始时间
  list($usec, $sec) = explode(" ", microtime());
  $time_start = ((float)$usec + (float)$sec);

  //自动按照不同的类别调用搜索函数
  if ($option >= 0) {
    $search_function = 'discography_search_'.$options[$option];
    $items = $search_function($keyword);
    //输出分页器
    $build['pager'] = array(
      '#theme' => 'pager',
      '#weight' => 5,
    );
    
    $build['tabs'][$options[$option]]['#active'] = true;
    $build['result'] = array('#theme' => 'discography_search_page_list');
    $build['result']['items'] = $items;
  } else {  //不带类别的搜索
    // 分别搜索每个类别，如果有结果，则输出    
    $result_items = array();
    foreach($options as $id => $type) {
      $search_function = 'discography_search_'.$type;
      $items = $search_function($keyword);
      if (count($items) > 0) {
        $result_items[$id] = $items;
      }
    }

    $build['tabs']['all']['#active'] = true;
    $build['result'] = array('#theme' => 'discography_search_page_all');
    $build['result']['items'] = $result_items;
  }

  //结束时间
  list($usec, $sec) = explode(" ", microtime());
  $time_end = ((float)$usec + (float)$sec);
  $time = intval(($time_end - $time_start)*10000)/10000;

  $build['result']['info'] = array(
    'option' => $option,
    'keyword' => $keyword,
    'time' => $time,
  );
  return $build;
}

function discography_search_keyitem($keyword, $option) {
  //在搜索索引中查找符合关键词的项
  $words = explode(' ', $keyword);
  $weightsqls = array();  //生成查询域关键词子句的数组
  $weightvalues = array();  //为查询域的关键词占位符提供值

  $query = db_select('discography_search_index', 'i');
  $query->condition('i.type', $option);
  $query->addField('i', 'title', 'title');
  $query->addField('i', 'ids', 'ids');
  
  $i = 0;
  foreach($words as $key) {
    $query->where("LOCATE(:keyword$i, keyword) > 0", array(':keyword'.$i => $key)); //为每个关键词添加一个查询条件
    $weightsqls[] = "LOCATE(:keyword$i, keyword)";  //每个关键词的位置
    $weightvalues[":keyword$i"] = $key; //占位符数组
    $i++;
  }
  $query->addExpression(implode(' + ', $weightsqls), 'weight', $weightvalues);  //关键词的位置和作为结果域
  $query->orderBy('weight');
  $result = $query->execute();

  //生成关键词对应的条目
  $keyitems = array();  //所有查询到的条目基本信息
  foreach($result as $keyitem) {
    //echo $keyitem->title;
    $key_ids = explode(';', $keyitem->ids); //分离出专辑id串
    foreach($key_ids as $key_id) {
      $ids = explode(',', $key_id); //分离具体的id
      
      $id_arr['title'] = $keyitem->title;
      $id_arr['aid'] = isset($ids[0]) ? $ids[0] : 0;
      $id_arr['did'] = isset($ids[1]) ? $ids[1] : 0;
      $id_arr['tid'] = isset($ids[2]) ? $ids[2] : 0;
      $keyitems[] = $id_arr;
    }
  }

  return $keyitems;
}

function discography_search_album($keyword, $page_start = 0, $limit_page = 10) {
  //当前页码
  $page = isset($_GET['page']) ? $_GET['page'] : 0;
  //每页显示项目数
  $limit_page = 10;

  $keyitems = discography_search_keyitem($keyword, 'album');

  $items = array();
  for($i=$page*$limit_page; $i < $page*$limit_page+$limit_page && $i < count($keyitems); $i++) {
    $items[] = discography_album_load($keyitems[$i]['aid']);
  }

  //设置分页器的参数
  _discography_pager_args_builder($page, count($keyitems), $limit_page);

  return $items;
}

function discography_search_track($keyword) {
  //当前页码
  $page = isset($_GET['page']) ? $_GET['page'] : 0;
  //每页显示项目数
  $limit_page = 10;

  $keyitems = discography_search_keyitem($keyword, 'track');

  $items = array();
  for($i=$page*$limit_page; $i < $page*$limit_page+$limit_page && $i < count($keyitems); $i++) {
    $albums[$keyitems[$i]['aid']] = discography_album_load($keyitems[$i]['aid']);
    $items[] = $albums[$keyitems[$i]['aid']]->tracks[$keyitems[$i]['did']][$keyitems[$i]['tid']];
  }
  //设置分页器的参数
  _discography_pager_args_builder($page, count($keyitems), $limit_page);

  return $items;
}

function discography_search_artist($keyword) {
  //当前页码
  $page = isset($_GET['page']) ? $_GET['page'] : 0;
  //每页显示项目数
  $limit_page = 10;

  $keyitems = discography_search_keyitem($keyword, 'artist');
  $items = array();
  
  foreach($keyitems as $keyitem) {
    $items[$keyitem['title']][] = $keyitem;
  }

  //设置分页器的参数
  _discography_pager_args_builder($page, count($items), $limit_page);

  return $items;
}

function discography_search_lyric($keyword) {
  $query = db_select('discography_lyric', 'l')->extend('PagerDefault');
  $query
    ->fields('l')
    ->where("LOCATE(:keyword, l.lyric) > 0", array(':keyword' => $keyword))
    ->limit(10);
  return $query->execute()->fetchAll();
}

function _discography_pager_args_builder($page, $count, $limit_page = 10) {
  global $pager_page_array, $pager_total, $pager_total_items;
  $pager_page_array = explode(',', $page);
  $element = 0;
  $pager_total_items[$element] = $count;
  $pager_total[$element] = ceil($pager_total_items[$element] / $limit_page);
  $pager_page_array[$element] = max(0, min((int)$pager_page_array[$element], ((int)$pager_total[$element]) - 1));
}

function theme_discography_search_page_list($variables) {
  global $pager_total_items;

  $options = array('album', 'track', 'artist', 'lyric');
  $options_cn = array('专辑', '曲目', '艺术家', '歌词');

  $output = '';
  /*
  if (count($variables['contents']['items']) == 1) {  //只有单条结果时直接跳转
    switch($variables['contents']['info']['option'])
    {
      case 0:
      case 1:
        drupal_goto('discography/'.$variables['contents']['items'][0]->path); //如果只有一条搜索结果，直接跳转
      break;
    }
  }
  */
  //搜索结果列表
  if (count($variables['contents']['items']) > 0) {
    //搜索结果信息
    $output .= '
<div class="meta" style="line-height:30px;">
在 <strong>'.$options_cn[$variables['contents']['info']['option']].'</strong> 中共有 <strong>'. $pager_total_items[0] .'</strong> 项符合 <strong>'. $variables['contents']['info']['keyword'] .'</strong> 的查询结果（搜索用时 <strong>'. $variables['contents']['info']['time'] .'</strong> 秒）
</div>';
    //遍历列出结果
    foreach($variables['contents']['items'] as $item) {
      $output .= theme('discography_search_'.$options[$variables['contents']['info']['option']], array('item' => $item));
    }

  } else {  //无结果搜索页面
    $output .= '<div class="meta">'. $options_cn[$variables['contents']['info']['option']] .'</div><br /><p>找不到和您的查询 "<strong>'. $variables['contents']['info']['keyword'] .'</strong>" 相符的内容。</p><p>建议：<ul><li>请检查输入字词有无错误。</li><li>请换用另外的查询字词。</li><li>请改用较常见的字词。</li>';
	
	  foreach($options_cn as $id => $optionone) {
		  if ($variables['contents']['info']['option'] != $id)
			$output .= '<li>请到<a href="/discography/search?keyword='.$variables['contents']['info']['keyword'].'&op=搜索&options='.$id.'">'.$optionone.'</a>中搜索你想要的内容。</li>';
	  }
	  $output .= '<li>请到<a href="/search_report">这里</a>向我们汇报以便我们改进。</li></ul></p>';
  }

  return $output;
}

function theme_discography_search_page_all($variables) {
  $result_items = $variables['contents']['items'];
  $info = $variables['contents']['info'];
  $options = array('album', 'track', 'artist', 'lyric');
  $options_cn = array('专辑', '曲目', '艺术家', '歌词');

  $output = '';
  $total_count = 0;
  foreach($result_items as $id => $items) {
    $output .= '<div>';
    $output .= '<h3>'.$options_cn[$id].'</h3>';
    if (count($items) > 5) {
      $output .= '<span class="more">'.l('更多相关'.$options_cn[$id], 'discography/search', array('query' => array('keyword' => $info['keyword'], 'options' => $id))).'</span>';
      $items = array_slice($items, 0, 5);
    }
    
    foreach ($items as $item) {
      $output .= theme('discography_search_'.$options[$id], array('item' => $item));
      $total_count++;
    }
    $output .= '</div>';
  }

  if ($total_count == 0) {
    $output .= '<div class="meta"><p>找不到和您的查询 "<strong>'. $info['keyword'] .'</strong>" 相符的内容。</p><p>建议：<ul><li>请检查输入字词有无错误。</li><li>请改用较常见的字词。</li>';
	  $output .= '<li>请到<a href="/search_report">这里</a>向我们汇报以便我们改进。</li></ul></p>';
  }
  if ($total_count == 1 && $id == 0 || $id == 1) {
     drupal_goto('discography/'.$item->path); //如果只有一条搜索结果，直接跳转
  }
  return $output;
}

/*
** 搜索结果页面的专辑
*/
function theme_discography_search_album($variables) {
  //print_r($album);
  $album = $variables['item'];
  //$count = count($albums);
  $source_options = variable_get('discography_album_sources', array('其它'));
  $album_types = array('album' => '专辑', 'single' => '单曲', 'unrealeased' => '未发行');

  $output = '';
    //if ($count == 1 && $notabs == true) {
     // drupal_goto('discography/'.$album->path); //如果只有一条搜索结果，直接跳转
    //}
  $source = ($source_options[$album->source] ? $source_options[$album->source] : '其它');

  $output .= '<h3>'.l('['.$album_types[$album->type].'] '.$album->title, 'discography/'.$album->path).'</h3>';
  $output .= '<span class="submitted">详细：';
  $output .= $album->artist.' | ';
  $output .= $album->pubcat.' | ';
  $output .= $album->pubdate.' | ';
  $output .= $album->pubcom.' | ';
  $output .= $source.'</span>';

  return $output;
}

/*
** 搜索结果页面的曲目
*/
function theme_discography_search_track($variables) {
  $track = $variables['item'];

  $output = '';
	
  //取得所有歌词（未完成）
  $lyrics = array();
  $imgpath = drupal_get_path('module', 'discography').'/img/';

  //$result = db_query("SELECT lid, aid, did,tid FROM {discography_lyric} WHERE moderate=1");
  //foreach ($result as $lyric) {
  //  $lyrics[$lyric->aid][$lyric->did][$lyric->tid] = $lyric->lid;
  //}

  //foreach ($tracks as $track) {
		//dprint_r($lyrics[$track->aid][$did]);
  $lyric_link = isset($lyrics[$track->aid][$track->did][$track->tid]) ? l('<img alt="歌词" style="margin:0 0 -5px 5px" src="/'.$imgpath.'lyric.gif" />', "discography/lyric/$track->aid/$track->did/$track->tid", array('html' => true, "attributes" => array("class" => array('popuplyric'),))) : '';
  $output .= '<div class="search-track"><h3>'.l($track->title, 'discography/'.$track->path).$lyric_link.'</h3>';
	$artists_output = array();
  isset($track->composer) ? $artists_output[] = '作曲：'.$track->composer : NULL;
  isset($track->arranger) ? $artists_output[] = '编曲：'.$track->arranger : NULL;
  isset($track->lyricist) ? $artists_output[] = '作词：'.$track->lyricist : NULL;
  isset($track->performer) ? $artists_output[] = '歌手：'.$track->performer : NULL;
  $output .= '<span class="submitted">'.implode('&nbsp; | &nbsp;', $artists_output).'</span>';
  $output .= '来自：'.l($track->atitle, 'discography/'.$track->path);
  $output .= '</div>';
  //}
  return $output;
}

/*
** 搜索结果页面的艺术家
*/
function theme_discography_search_artist($variables) {
  $output = '';

  $artist = $variables['item'];
  
  $output .='<div class="search-artist"><h3>'.l($artist[0]['title'], 'discography/artist/'.$artist[0]['title']).'</div>';
  //$output .= print_r($artist, true);
  return $output;
}

/*
** 搜索结果页面的歌词
*/
function theme_discography_search_lyric($variables) {
  $lyric = $variables['item'];
  $output = '';
  $languages = array('原文', '发音', '中文翻译', '日文翻译', '英文翻译');
  $album = discography_album_load($lyric->aid);
  $output .= '<h3>'.l('['.$languages[$lyric->language].']'.$album->tracks[$lyric->did][$lyric->tid]->title, "discography/lyric/$album->aid/$lyric->did/$lyric->tid").'</h3>';
  $output .= '<span class="submitted">来自专辑：'.l($album->title, 'discography/'.$album->path).' | 歌手：'.$album->tracks[$lyric->did][$lyric->tid]->performer.'</span>';
  $output .= '<p>'.discography_truncate_lyric($lyric->lyric, $_GET['keyword']).'</p>';
  return $output;
}

function discography_lists() {
  $result = db_query("SELECT nid, title FROM {node} WHERE type='discography_list' AND sticky=1");
  $lists = array();
  foreach ($result as $list) {
    $lists[] = '♫'.l($list->title, 'node/'.$list->nid);
  }
  return implode('&nbsp;&nbsp;', $lists);
}
