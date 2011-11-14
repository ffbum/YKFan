<?php
// $Id: node.pages.inc,v 1.28.2.1 2008/02/27 19:44:44 goba Exp $

/**
 * @file
 * Page callbacks for adding, editing, deleting, and revisions management for content.
 */

/*
** 关键词索引编辑页面
*/
function discography_keywords_page() {
  if (isset($_POST['filter'])) {
    ajax_deliver('<div id="keywords_wapper">'.drupal_render(discography_keywords_table($_POST['filter'])).'</div>');
    exit;
  }
  discography_set_admin_breadcrumb();
  $build['message_wapper']['#markup'] = '<div id="message_wapper"></div>';
  $build['help-msg'] = array('#markup' => '<div>&nbsp;&nbsp;关键词用于检索时能通过中文、英文或者俗名等检索到相应的结果，多个关键词以空格分开。关键词通常包括：中文名称、片假名英文写法、艺术家英文名、专辑相关作品俗称简称、一些通用简称（如OST）、原声碟序号（如1、2）。名称中的繁体字转简体字不需要单独设立关键词。总的原则就是：对该名称最通用的描述。</div>');
  $build['filter-form'] = drupal_get_form('discography_keywords_filter_form');
  $build['index-table'] = discography_keywords_table();
  $build['index-table']['#prefix'] = '<div id="keywords_wapper">';
  $build['index-table']['#suffix'] = '</div>';
  $build['rebuild-button'] = drupal_get_form('discography_keywords_build_form');
  return $build;
}

/*
** 关键词类型选择表单
*/
function discography_keywords_filter_form($form, &$form_state) {
  $form['filter'] = array(
    '#type' => 'select',
    '#title' => '过滤显示',
    '#options' => array('只显示专辑名', '只显示曲名', '只显示艺术家名'),
    '#default_value' => isset($_GET['filter']) && $_GET['filter'] ? $_GET['filter'] : 0,
    '#ajax' => array(
      'path' => 'discography/admin/keywords',
      //'callback' => 'discography_keywords_table',
      'wrapper' => 'keywords_wapper',
      'event' => 'change',
      'effect' => 'fade',
    ),
  );
  return $form;
}

/*
** 关键词列表表格
*/
function discography_keywords_table($filter = 0) {
  // 取得全部索引
  $result = db_select('discography_search_index', 'i')->fields('i')->execute();
  //$result = db_query("SELECT * FROM {discography_search_index}");
  $indexs = array();
  foreach($result as $index) {
    $indexs[$index->title][$index->type]['custom'] = $index->custom;
    $indexs[$index->title][$index->type]['keyword'] = $index->keyword;
  }
  $header = array('名称',array('data' => '类型', 'style' => 'width: 50px;'),'自定义关键词', '相关专辑');
  $typename = array('album' => '专辑名', 'track' => '曲名', 'artist' => '艺术家');

  //不同类型不同的查询变量
  $querys[0] = db_select('discography_album', 'album')->extend('PagerDefault');
  $querys[0]->addField('album', 'title', 'title');
  $querys[0]->addExpression("'album'", 'type');
  $querys[1] = db_select('discography_track', 'track')->extend('PagerDefault');
  $querys[1]->addField('track', 'title', 'title');
  $querys[1]->addExpression("'track'", 'type');
  $querys[2] = db_select('discography_artist', 'artist')->extend('PagerDefault');
  $querys[2]->addField('artist', 'name', 'title');
  $querys[2]->addExpression("'artist'", 'type');
  
  $filter = isset($_REQUEST['filter']) && $_REQUEST['filter'] ? $_REQUEST['filter'] : $filter;

  $result = $querys[$filter]->distinct()->limit(10)->execute();

  $i = 0;
  foreach($result as $index) {
    $custom = isset($indexs[$index->title][$index->type]['custom']) ? $indexs[$index->title][$index->type]['custom'] : '';
    $key = isset($indexs[$index->title][$index->type]['keyword']) ? $indexs[$index->title][$index->type]['keyword'] : '';
    $link = l('打开', 'discography/keywords/albums/'.$index->type.'/'.$index->title);
    $rows[] = array($index->title, $typename[$index->type], drupal_render(drupal_get_form('discography_keywords_custom_form', $custom, $index->title, $index->type, $key)), $link);
  }
  
  $build['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    );
  $build['pager'] = array(
    '#theme' => 'pager',
    '#parameters' => array('filter' => $filter),
    );
  return $build;
}

/*
** 关键词在线编辑表单
*/
function discography_keywords_custom_form($form, &$form_state, $custom, $title, $type, $key) {
  $form['custom'] = array(
    '#type' => 'textfield',
    '#size' => 90,
    '#attributes' => array('title' => $key),
    '#default_value' => $custom,
    '#ajax' => array(
      'callback' => 'discography_keywords_save_custom_js',
      'wrapper' => 'message_wapper',
      'event' => 'change',
      'effect' => 'fade',
    ),
  );
  $form['title']  = array(
    '#type' => 'hidden',
    '#value' => $title,
  );
  $form['type']  = array(
    '#type' => 'hidden',
    '#value' => $type,
  );
  return $form;
}

/**
 * 关键词条目相关页面
 */
function discography_artist_page($type, $title) {
  $output = $title;
  if ($type == 'album') {
    $result = db_query("SELECT title, aid FROM {discography_album} WHERE title='%s'", $title);
  }
  if ($type == 'track') {
    $result = db_query("SELECT b.title, b.aid FROM {discography_track} a INNER JOIN {discography_album} b ON a.aid=b.aid WHERE a.title='%s'", $title);
  }
  if ($type == 'artist') {
    $result = db_query("SELECT b.title, b.aid FROM {discography_artist} a INNER JOIN {discography_album} b ON a.aid=b.aid WHERE a.name='%s'", $title);
  }

  while ($album = db_fetch_object($result)) {
    $output .= '<br />'.l($album->title, 'discography/album/edit/'.$album->aid);
  }
  return $output;
}


/**
 * 重建索引表单
 */
function discography_keywords_build_form($form, &$form_state) {
  $form['rebuild'] = array(
    '#type' => 'submit',
    '#value' => '重建索引',
  );
  /*
  $form['import'] = array(
    '#type' => 'submit',
    '#value' => '导入旧Discography',
    '#submit' => array('discography_import'),
  );
  */
  return $form;
}

/**
 * 重建索引表单处理
 */
function discography_keywords_build_form_submit($form, &$form_state) {
  //读入全部关键词索引
  $result = db_query("SELECT * FROM {discography_search_index}");
  
  $indexs = array();
  foreach($result as $index) {
    $indexs[$index->title][$index->type] = $index->custom;
  }

  //删除全部关键词索引
  db_delete('discography_search_index')->execute();

  //重新生成插入关键词索引
  $result = db_query("SELECT title,'album' AS type, GROUP_CONCAT(aid SEPARATOR ';') AS ids FROM {discography_album} GROUP BY title UNION SELECT title,'track' AS type, GROUP_CONCAT(CONCAT_WS(',',aid,did,tid) SEPARATOR ';') AS ids FROM {discography_track} GROUP BY title UNION SELECT name AS title,'artist' AS type, GROUP_CONCAT(CONCAT_WS(',',aid,did,tid) SEPARATOR ';') AS ids FROM {discography_artist} GROUP BY name");

  foreach ($result as $title) {
    $custom = $indexs[$title->title][$title->type] ? $indexs[$title->title][$title->type] : ''; //如果旧索引存在则保留
    db_insert('discography_search_index')
      ->fields(array(
        'title' => $title->title,
        'type' => $title->type,
        'custom' => $custom,
        'keyword' => discography_keywords_index_translate($title->title, $custom),
        'ids' => $title->ids,
      ))
      ->execute();
  }
}

/*
* 内联编辑关键词用的ajax回调函数
*/
function discography_keywords_save_custom_js($form, $form_state) {
  $title = $form['title']['#value'] ? $form['title']['#value'] : '';
  $type = $form['type']['#value'] ? $form['type']['#value'] : '';
  $custom = $form['custom']['#value'] ? $form['custom']['#value'] : '';

  db_merge('discography_search_index')  //合并插入更新
    ->key(array('title' => $title, 'type' => $type))
    ->fields(array('custom' => $custom, 'keyword' => discography_keywords_index_translate($title, $custom)))
    ->execute();

  return '<div id="message_wapper"><div class="messages status">已保存到【'.$title.'】关键词：'.discography_keywords_index_translate($title, $custom).'</div></div>';
}

/*
* 内置关键词基本转换
*/
function discography_keywords_index_translate($title, $custom) {
  include_once(drupal_get_path('module', 'discography') ."/tw-zh.inc"); //载入简繁转换类
  $titles = explode(' ', $title.' '.FanJianConvert::tradition2simple($title));
  $customs = explode(' ', $custom);


  $dict = array('攻殻機動隊' => '攻壳机动队', '信長の野望' => '信长之野望', 'MACROSS' => '超时空要塞'); //常用词
  foreach ($titles as $key) {
    if ($dict[$key]) {
      $titles[] = $dict[$key];
    }
  }
  $keys = array_unique(array_merge($titles, $customs));
  return implode(' ', $keys);
}

/*
function discography_import() {
  $result1 = db_query("SELECT * FROM {discography_album_old} ORDER BY pubdate ASC");
  while ($album = db_fetch_object($result1)) {
    $album->cover = 0;
    $album->type = 'album';
    $album->artist = str_replace(array('·', '/', '、', '?', '，', '／'), ',', $album->artist);
    $album->price = 0;
    $album->booklet = '';
    $path = db_result(db_query("SELECT dst FROM {url_alias_old} WHERE src='%s'", 'node/'.$album->nid));
    $album->path = str_replace('discography/', '', $path);
    //insert database
    db_query("INSERT INTO {discography_album} (title, pubdate, pubcat, pubcom, price, cover, type, source, brief, path, booklet) VALUE ('%s', '%s', '%s', '%s', %d, %d, '%s', '%s', '%s', '%s', '%s')", $album->title, $album->pubdate, $album->pubcat, $album->pubcom, $album->price, $album->cover, $album->type,$album->source, $album->brief, $album->path, $album->booklet);
    $aid = db_last_insert_id('discography_album', 'aid');

    $artists = explode(',', $album->artist);
    foreach ($artists as $artist) {
      if (trim($artist)) {
        db_query("INSERT INTO {discography_artist} VALUES (%d, %d, %d, '%s', '%s')", $aid, 0, 0, trim($artist), 'artist');
      }
    }
    //drupal_set_message(dprint_r($artists, true));
    $result2 = db_query("SELECT * FROM {discography_track_old} WHERE aid=%d ORDER BY did, tid", $album->nid);
    $artist_type = array('composer', 'arranger', 'lyricist', 'performer');
    $discs = array();
    $lyric_lang = array();
    while ($track = db_fetch_object($result2)) {
      db_query("INSERT INTO {discography_track} VALUES (%d, %d, %d, '%s')", $aid, $track->did, $track->tid, $track->title);
      //插入歌词
      if ($track->lyric) {
        db_query("INSERT INTO {discography_lyric} VALUES (NULL, %d, %d, %d, '%s', %d, 1)", $aid, $track->did, $track->tid, $track->lyric, 0);
      }
      if ($track->rlyric) {
        db_query("INSERT INTO {discography_lyric} VALUES (NULL, %d, %d, %d, '%s', %d, 1)", $aid, $track->did, $track->tid, $track->rlyric, 1);
      }
      if ($track->clyric) {
        db_query("INSERT INTO {discography_lyric} VALUES (NULL, %d, %d, %d, '%s', %d, 1)", $aid, $track->did, $track->tid, $track->clyric, 2);
      }
      $discs[$aid][$track->did] = $track->did;
      foreach ($artist_type as $duty) {
        $track->$duty = str_replace(array('·', '/', '、', '?', '，', '／'), ',', $track->$duty);
        $artists = explode(',', $track->$duty);
        foreach ($artists as $artist) {
          if (trim($artist)) {
            db_query("INSERT INTO {discography_artist} VALUES (%d, %d, %d, '%s', '%s')", $aid, $track->did, $track->tid, trim($artist), $duty);
          }
        }
      }
    }
    foreach ($discs as $aid => $disc) {
      foreach ($disc as $did) {
        db_query("INSERT INTO {discography_track} VALUES (%d, %d, %d, '%s')", $aid, $did, 0, '碟'.$did);
      }
    }
  }
}
*/