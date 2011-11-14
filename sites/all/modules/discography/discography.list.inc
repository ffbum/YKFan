<?php
// $Id: node.pages.inc,v 1.28.2.1 2008/02/27 19:44:44 goba Exp $

/**
 * Implementats hook_access().
 */
function discography_list_node_access($node, $op, $account) {
  switch ($op) {
    case 'create':
    case 'update':
    case 'delete':
      // Anonymous users cannot post even if they have the permission.
      if (user_access('admin discography', $account) && $account->uid)
        return NODE_ACCESS_ALLOW;
  }
  return NODE_ACCESS_IGNORE;
}

/**
 * Implements hook_form().
 */
function discography_list_form($node, $form_state) {
  //print_r($node);
  $form['title'] = array(
    '#title' => '列表名称',
    '#type' => 'textfield',
    '#default_value' => isset($node->title) ? $node->title : '',
    '#weight' => 0,
    '#required' => true,
  );

  $form['sql'] = array(
    '#type' => 'fieldset',
    '#title' => '自动生成列表',
    '#collapsible' => TRUE,
    '#weight' => 1,
    '#description' => '专辑列表可用列名：a.aid（专辑id）、title（专辑名称）、pubdate（发行日期，格式YYYY-MM-DD）、pubcat（发行编号）、pubcom（发行公司）、price（含税价）、cover（封面，1为有0为无）、type（专辑类型）、source（专辑出处）、brief（专辑简介）、path（专辑路径）、booklet（专辑Booklet相册地址）、name（艺术家名称）、duty（艺术家类型）<br />曲目列表可用列名：a.aid（专辑id）、name（艺术家名称）、duty（艺术家类型）',
  );
  $form['sql']['sqlwhere'] = array(
    '#title' => 'MYSQL过滤条件',
    '#type' => 'textfield',
    '#default_value' => isset($node->sqlwhere) ? $node->sqlwhere : '',
    '#description' => 'MYSQL语句中Where部分，用于选择需要的内容。',
  );
  $form['sql']['sqlorderby'] = array(
    '#title' => 'MYSQL排序方式',
    '#type' => 'textfield',
    '#default_value' => isset($node->sqlorderby) ? $node->sqlorderby : '',
    '#description' => 'MYSQL语句中Order部分，用于指定排序方式。',
  );
  $form['sql']['nodelist'] = array(
    '#title' => '过滤列表',
    '#type' => 'textarea',
    '#default_value' => isset($node->nodelist) ? $node->nodelist : '',
    '#description' => '用列表方式输入，一旦此处填写MYSQL条件和排序将自动失效。每行一个项目，专辑列表为“aid”，曲目列表为“aid,did,tid”。',
  );
  $form['sql']['listtype'] = array(
    '#title'=> '列表类型',
    '#type' => 'radios',
    '#options' => array('专辑列表', '曲目列表'),
    '#default_value' => isset($node->listtype) ? $node->listtype : 0,
    );
  $form['sql']['preview'] = array(
    '#value'=> '预览列表',
    '#type' => 'button',
    '#ahah' => array(
      'path' => 'discography/list/js',
      'wrapper' => 'body_wapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['body_header'] = array(
    '#title' => '页头内容',
    '#type' => 'textarea',
    '#rows' => 5,
    '#default_value' => isset($node->body_header) ? $node->body_header : '',
    '#weight' => 2,
  );
  //$body_list = discography_list_get_list($node->sqlwhere, $node->sqlorderby, $node->listtype);
  $form['body_list'] = array(
    '#value' => '自动列表预览区',
    '#prefix' => '<div id="body_wapper">',
    '#suffix' => '</div>',
    '#weight' => 3,
  );
  $form['body_footer'] = array(
    '#title' => '页脚内容',
    '#type' => 'textarea',
    '#rows' => 5,
    '#default_value' => isset($node->body_footer) ? $node->body_footer : '',
    '#weight' => 4,
  );
  return $form;
}

function discography_list_insert($node) {
  db_insert('discography_list')
    ->fields(array(
      'nid' => $nid->nid,
      'sqlwhere' => $node->sqlwhere,
      'sqlorderby' => $node->sqlorderby,
      'nodelist' => $node->nodelist,
      'body_header' => $node->body_header,
      'body_footer' => $node->body_footer,
      'listtype' => $node->listype,
    ))
    ->execute();
  //db_query("INSERT INTO {discography_list} VALUES (%d, '%s', '%s', '%s', '%s', '%s', %d)", $node->nid, $node->sqlwhere, $node->sqlorderby, $node->nodelist, $node->body_header, $node->body_footer,$node->listtype);
	discography_add_event('添加列表「'. $node->title.'」', 'node/'. $node->nid);
}

function discography_list_update($node) {
  db_update('discography_list')
    ->fields(array(
      'sqlwhere' => $node->sqlwhere,
      'sqlorderby' => $node->sqlorderby,
      'nodelist' => $node->nodelist,
      'body_header' => $node->body_header,
      'body_footer' => $node->body_footer,
      'listtype' => $node->listype,
    ))
    ->condition('nid', $node->nid)
    ->execute();
  //db_query("UPDATE {discography_list} SET sqlwhere='%s', sqlorderby='%s', nodelist='%s', body_header='%s', body_footer='%s', listtype=%d WHERE nid=%d", $node->sqlwhere, $node->sqlorderby, $node->nodelist, $node->body_header, $node->body_footer, $node->listtype, $node->nid);
}

function discography_list_delete($node) {
  db_delete('discography_list')->condition('nid', $node->nid)->execute();
  //db_query("DELETE FROM {discography_list} WHERE nid=%d", $node->nid);
	discography_add_event('删除列表「'. $node->title.'」', 'node/'. $node->nid);
}

function discography_list_load($nodes) {
  
  $result = db_select('discography_list', 'l')
    ->fields('l', array('nid', 'sqlwhere','sqlorderby','nodelist','body_header','body_footer','listtype'))
    ->condition('nid', array_keys($nodes), 'IN')
    ->execute();
  foreach($result as $list) {
    $nodes[$list->nid]->sqlwhere = $list->sqlwhere;
    $nodes[$list->nid]->sqlorderby = $list->sqlorderby;
    $nodes[$list->nid]->nodelist = $list->nodelist;
    $nodes[$list->nid]->body_header = $list->body_header;
    $nodes[$list->nid]->body_footer = $list->body_footer;
    $nodes[$list->nid]->listtype = $list->listtype;
  }
}

function discography_list_view($node, $view_mode) {
  drupal_add_js('facebox/facebox.js');
  drupal_add_css('facebox/facebox.css');
  drupal_add_js(drupal_get_path('module', 'discography') . '/js/discography.js');
	
  $element = array(
    '#tag' => 'meta', 
    '#attributes' => array(
      'name' => 'description', 
      'content' => discography_truncate_lyric(drupal_html_to_text($node->body), 'bakayaro', 150),
    ),
  );
  drupal_add_html_head($element, 'description');

  $node->body['zh-hans'][0]['safe_value'] = $node->body['zh-hans'][0]['value'];
  return $node;
}

function discography_list_node_form_submit(&$form, &$form_state) {
  $form_state['values']['body'] = check_markup($form_state['values']['body_header'],2). discography_list_build_list($form_state['values']['sqlwhere'], $form_state['values']['sqlorderby'], $form_state['values']['nodelist'], $form_state['values']['listtype']). check_markup($form_state['values']['body_footer'],2);
  $form_state['values']['teaser'] = '';
}

function discography_list_build_list($sqlwhere, $sqlorderby, $nodelist, $listtype) {
  if (!$sqlwhere && !$nodelist) {
    return $output;
  }
  if ($listtype == 0) {
    $albums = array();
    if ($nodelist) {
      $lines = explode("\n", $nodelist);
      foreach ($lines as $line) {
        $line = trim($line);
        if (is_numeric($line)) {
          $albums[$line] = discography_album_load($line);
        }
      }
    } else {
      $sqlorderby = $sqlorderby ? $sqlorderby : 'pubdate DESC';
      $result = db_query("SELECT DISTINCT a.aid FROM {discography_album} a INNER JOIN {discography_artist} s ON s.aid = a.aid WHERE $sqlwhere ORDER BY $sqlorderby");
      while ($album = db_fetch_object($result)) {
        $albums[$album->aid] = discography_album_load($album->aid);
      }
      if ($result === NULL) {
        drupal_set_message('SQL语句有错，请仔细检查。');
        return '';
      }
    }
    if (count($albums) == 0) {
      return '';
    }
    return discography_list_albums($albums);
  }
  if ($listtype == 1) {
    $tracks = array();
    if ($nodelist) {
      $lines = explode("\n", $nodelist);
      foreach ($lines as $line) {
        $line = trim($line);
        list($aid, $did, $tid) = explode(',', $line);
        $album = discography_album_load($aid);
        $tracks[] = $album->tracks[$did][$tid];
      }
    } else {
      $sqlorderby = $sqlorderby ? $sqlorderby : 'aid,did,tid DESC';
      $result = db_query("SELECT a.aid, a.did, a.tid FROM {discography_track} a INNER JOIN {discography_artist} s ON s.aid = a.aid AND s.did=a.did AND s.tid=a.tid WHERE $sqlwhere ORDER BY $sqlorderby");
      while ($track = db_fetch_object($result)) {
        $album = discography_album_load($track->aid);
        $tracks[] = $album->tracks[$track->did][$track->tid];
      }
      if ($result === NULL) {
        drupal_set_message('SQL语句有错，请仔细检查。');
        return '';
      }
    }
    if (count($tracks) == 0) {
      return '没有符合的内容。';
    }
    return discography_list_tracks($tracks);
  }
  return '';
}

function discography_list_albums($albums) {
  $output = '';
  $count = 0;
  $cover_path = file_directory_path().'/covers/';
  $album_types = array('album' => '专辑', 'single' => '单曲', 'unrealeased' => '未发行');
  $source_options = variable_get('discography_album_sources', array('其它'));
  $filters = array();
  $filters['artist']['-'] = '♪全部艺术家♪';
  $filters['pubdate']['-'] = '♪全部年份♪';
  $filters['type']['-'] = '♪全部类别♪';
  $filters['source']['-'] = '♪全部出处♪';
  foreach ($albums as $aid=>$album) {
    $filters['artist'][$album->artist] = $album->artist;
    $year = substr($album->pubdate, 0, 4);
    $filters['pubdate'][$year] = $year;
    $filters['type'][$album->type] = $album->type;
    $source = ($source_options[$album->source] ? $source_options[$album->source] : '其它');
    $filters['source'][$source] = $source;
    $output .= '<div class="album"><div class="album-list"><div class="album-list-cover" >';
    if ($album->cover) {
      $output .= '<img alt="专辑封面" src="/'.$cover_path.'cover_thumb_'.$aid.'.jpg" />';
    } else {
      $output .= '<img alt="专辑暂无封面" src="/'.$cover_path.'nocover_thumb.gif" />';
    }
    $output .= '</div>';

    $output .= '<div class="album-list-content"><h3>'.l('['.$album_types[$album->type].'] '.$album->title, 'discography/'.$album->path).'</h3>';
    $output .= '<div class="description">专辑艺术家：<span class="artist">'.$album->artist.'</span>&nbsp;|&nbsp;';
    $output .= '编号：'.$album->pubcat.'&nbsp;|&nbsp;';
    $output .= '发行商：'.$album->pubcom.'&nbsp;|&nbsp;';
    $output .= '发行日期：<span class="pubdate">'.$album->pubdate.'</span>';
    $output .= ($album->price ? '&nbsp;|&nbsp;定价：'.$album->price.'日元（含税）' : '');
    $output .= '&nbsp;|&nbsp;出自：<span class="source">'.$source.'</span>';
    $output .= '<span class="type" style="display:none;">'.$album->type.'</span>';
    $output .= '</div>';
    $output .= '<div class="brief">'.discography_truncate_lyric($album->brief, 'bakayaro', 250).'</div></div></div></div>';
    $count++;
  }


  $filter = '<div class="list-filters" id="album-filter">显示'.drupal_get_form('discography_list_filter_form', $filters).'的专辑列表共<em class="count">'.$count.'</em>张</div>';
  //$output = $filter.$output;

  return $output;
}

/**
 * 功能描述
 *
 * @param $参数名
 *   描述
 * @return
 *   描述
 */
function discography_list_filter_form(&$form_state, $filters, $type='album') {
  foreach ($filters as $id => $options) {
    $form[$id]= array('#type' => 'select', '#options' => $options,);
  }
  return $form;
}

/**
 * 功能描述
 *
 * @param $参数名
 *   描述
 * @return
 *   描述
 */
 /*
function theme_discogsraphy_list_filter_form(&$form) {
  $output = '<div id="list-filters">显示';
  $output .= drupal_render($form['artist']);
  $output .= drupal_render($form['pubdate']);
  $output .= drupal_render($form['type']);
  $output .= drupal_render($form['source']);
  $output .= drupal_render($form);
  $output .= '的专辑列表</div>';

  return $output;
}
*/
/**
 * 功能描述
 *
 * @param $参数名
 *   描述
 * @return
 *   描述
 */
function discography_list_tracks($tracks) {
  //取得所有歌词
  $lyrics = array();
  $imgpath = drupal_get_path('module', 'discography').'/img/';

  $result = db_query("SELECT lid, aid, did,tid FROM {discography_lyric} WHERE moderate=1");
  while ($lyric = db_fetch_object($result)) {
    $lyrics[$lyric->aid][$lyric->did][$lyric->tid] = $lyric->lid;
  }

  $filters = array();
  $filters['album']['-'] = '♪全部专辑♪';
  //$filters['lyric'] = array('-' => '♪全部曲目♪', '歌词有' => '歌词无', '编曲' => '编曲','作词' => '作词','歌手' => '歌手');
  $count = 0;
  $output = '';
  foreach ($tracks as $track) {
    if ($track->tid == 0) {
      continue;
    }
    $filters['album'][$track->atitle] = $track->atitle;
    $output .= '<div class="track"><div class="track-list">';
    $output .= '<div class="track-list-title">';
    $output .= '<h3>'.check_plain($track->title).'</h3>';
    $output .= '<span class="track-list-album">（来自专辑：'.l($track->atitle, 'discography/'.$track->path).' 第<em>'.$track->did.'</em>碟 / 第<em>'.$track->tid.'</em>曲）</span>';
    $output .= '<span class="lyric">'.($lyrics[$track->aid][$track->did][$track->tid] ? l('<img alt="查看歌词" src="/'.$imgpath.'lyric.gif" />', "discography/lyric/$track->aid/$track->did/$track->tid/popup", array("html" => true, "attributes" => array("rel" => 'facebox'))) : '').'</span>';
    $output .= '</div>';
    $output .= '<div class="track-list-artists">';
    $track->composer ? $artists_output[] = '作曲：'.$track->composer : NULL;
    $track->arranger ? $artists_output[] = '编曲：'.$track->arranger : NULL;
    $track->lyricist ? $artists_output[] = '作词：'.$track->lyricist : NULL;
    $track->performer ? $artists_output[] = '歌手：'.$track->performer : NULL;
    $output .= implode('&nbsp; | &nbsp;', $artists_output);
    unset($artists_output);
    $output .= '</div></div></div>';
    $count++;
  }

  $filter = '<div class="list-filters" id="track-filter">显示'.drupal_get_form('discography_list_filter', $filters).'的曲目列表共<em class="count">'.$count.'</em>首</div>';
  return $filter.$output;
}

function discography_list_preview_js() {
  $listtype = $_POST['listtype'];
  $sqlwhere = $_POST['sqlwhere'];
  $nodelist = $_POST['nodelist'];
  $sqlorderby = $_POST['sqlorderby'];
  $output = discography_list_build_list($sqlwhere, $sqlorderby, $nodelist, $listtype);
  drupal_json(array('status' => TRUE, 'data' => $output));
  return;
}
/**
 * 功能描述
 *
 * @param $参数名
 *   描述
 * @return
 *   描述
 */

function discography_rebuild_list() {
  $result = db_query("SELECT nid FROM {node} WHERE 'type'='discography_list'");
  while ($list = db_fetch_object($result)) {
    $node = node_load($list->nid);
    $node->body = check_markup($node->body_header).discography_list_build_list($node->sqlwhere, $node->sqlorderby, $node->listtype).check_markup($node->body_footer);
    $node->teaser = check_markup($node->body_header);
    node_save($node);
  }
}