<?php
// $Id: node.pages.inc,v 1.28.2.1 2008/02/27 19:44:44 goba Exp $

/**
 * @file
 * Page callbacks for adding, editing, deleting, and revisions management for content.
 */


function discography_album_form($form, &$form_state, $album = NULL) {
  discography_set_admin_breadcrumb();

  //drupal_add_js(drupal_get_path('module', 'discography') . '/js/jquery.jSelectDate-0.2.js');
  //drupal_add_js(drupal_get_path('module', 'discography') . '/js/jquery.dimensions.min.js');
  //drupal_add_js(drupal_get_path('module', 'discography') . '/js/jquery.inputHintBox.js');
  //drupal_add_js(drupal_get_path('module', 'discography') . '/js/discography-form.js');
  //drupal_add_library('system', 'ui.dialog');
  drupal_add_library('system', 'ui.datepicker');
	drupal_add_js(drupal_get_path('module', 'discography') . '/js/discography-form.js');
  drupal_add_css(drupal_get_path('module', 'discography'). '/js/theme/cupertino/jquery.ui.theme.css', array('group' => 'CSS_THEME', 'weight' => 9999)); // jQuery Theme

  $form = array(
    //'#cache' => FALSE,
    '#attributes' => array('enctype' => 'multipart/form-data'),
  );

  $form['album']['aid'] = array('#type' => 'value', '#value' => (isset($album->aid) ? $album->aid : 0));
  $form['album']['title'] = array(
    '#title' => '专辑名称',
    '#type' => 'textfield',
    '#size' => 70,
    '#default_value' => isset($album->title) ? $album->title : '',
    '#required' => true,
    '#attributes' => array('title' => '专辑原文名称，以专辑Booklet为准。'),
  );

  $form['album']['artist'] = array(
    '#title' => '艺术家',
    '#type' => 'textfield',
    '#size' => 70,
    '#default_value' => isset($album->artist) ? $album->artist : '',
    '#required' => true,
    '#attributes' => array('title' => '专辑艺术家，以专辑Booklet为准，如有多位艺术家请以逗号分开。'),
  );
  $form['album']['pubcat'] = array(
    '#title' => '编号',
    '#type' => 'textfield',
    '#size' => 12,
    '#default_value' => isset($album->pubcat) ? $album->pubcat : '',
    '#attributes' => array('title' => '专辑发行编号。'),
  );
  $form['album']['pubdate'] = array(
    '#title' => '发行日',
    '#type' => 'textfield',
    '#size' => 10,
    '#attributes' => array('class' => array('pubdate')),
    '#default_value' => isset($album->pubdate) ? $album->pubdate : format_date(time(), 'custom', 'Y-m-d'),
    '#weight' => -1,
  );
  $form['album']['pubcom'] = array(
    '#title' => '发行商',
    '#type' => 'textfield',
    '#default_value' => isset($album->pubcom) ? $album->pubcom : '',
    '#size' => 26,
    '#attributes' => array('style' => 'ime-mode:disabled;'),
    '#attributes' => array('title' => '专辑发行公司。'),
  );
  $form['album']['price'] = array(
    '#title' => '定价',
    '#type' => 'textfield',
    '#maxlength' => 5,
    '#size' => 5,
    '#default_value' => isset($album->price) ? $album->price : '',
    '#field_suffix' =>'円',
    '#attributes' => array('style' => 'ime-mode:disabled;'),
    '#attributes' => array('title' => '专辑定价，使用日文含税价。'),
  );
  $form['album']['brief'] = array(
    '#title' => '简介',
    '#type' => 'textarea',
    '#default_value' => isset($album->brief) ? $album->brief : '',
    '#description' => '专辑简要介绍，不要分段，最好不超过200字。',
  );
  $form['album']['comment'] = array(
    '#title' => '说明',
    '#type' => 'textarea',
    '#rows' => 2,
    '#default_value' => isset($album->comment) ? $album->comment : '',
    '#description' => '专辑其它信息说明，如初回特典，绝版情况等，请注意排版。',
  );
  $form['album']['taobao'] = array(
    '#title' => '淘宝',
    '#type' => 'textfield',
    '#size' => 60,
    '#default_value' => isset($album->taobao) ? $album->taobao : '',
    '#attributes' => array('style' => 'ime-mode:disabled;'),
    '#attributes' => array('title' => '淘宝代购商品链接。'),
  );
  $type_options = array('album' => '专辑', 'single' => '单曲', 'unrealeased' => '未发行');
  $form['album']['type'] = array(
    '#title' => '类型',
    '#type' => 'select',
    '#options' => $type_options,
    '#default_value' => isset($album->type) ? $album->type : 'album',
  );
  $source_options = variable_get('discography_album_sources', array('其它'));
  $form['album']['source'] = array(
    '#title' => '来源',
    '#type' => 'select',
    '#options' => $source_options,
    '#attributes' => array('style' => 'width:400px;'),
    '#default_value' => isset($album->source) ? $album->source : 0,
    '#attributes' => array('title' => '专辑相关的动画、电影或游戏作品。'),
  );
  $form['album']['path'] = array(
    '#title' => '路径',
    '#type' => 'textfield',
    '#maxlength' => 10,
    '#size' => 15,
    '#default_value' => isset($album->path) ? $album->path : '',
    '#attributes' => array('style' => 'ime-mode:disabled;'),
    '#required' => true,
    '#attributes' => array('title' => '专辑名缩写，只能使用英文小写字母和数字，不得和其它专辑重复。'),
  );
  $form['album']['booklet'] = array(
    '#title' => 'Booklet',
    '#type' => 'textfield',
    '#size' => 20,
    '#default_value' => isset($album->booklet) ? $album->booklet : '',
    '#attributes' => array('style' => 'ime-mode:disabled;',
    'title' => '专辑在Booklet相册里的RSS地址。'),
  );
  //封面
  if (isset($album->cover)) {
    $cover = 'public://covers/cover_'.$album->aid.'.jpg';
  } else {
    $cover = 'public://cvers/nocover.gif';
  }
  $form['album']['cover'] = array(
    '#type' => 'file',
    '#size' => 38,
    '#attributes' => array('style' => 'ime-mode:disabled;'),
    '#description' => '<div id="cover-description">选择专辑封面，尽量在500×500左右，不要过大或过小。</div>',
  );
  
  $form['album']['cover-path'] = array('#type' => 'value', '#value' => theme('image_style', array('style_name' => 'thumbnail', 'path' => $cover, 'alt' => '专辑封面')));




  //生成默认
  $form['tracks']['#tree'] = TRUE;
  $form['tracks']['#theme'] = 'discography_tracks_form';
  $default_track = 10;  //默认曲数

  $track_numbers[1] = $default_track; //默认1碟10曲
  // 如果是编辑专辑则按照本来的结构
  if (isset($album->tracks) && count($album->tracks) > 0) {
    foreach($album->tracks as $did => $disc) {
      $track_numbers[$did] = max(1, count($disc)-1);
    }
  }

  //如果是ajax调用则根据按钮修改结构
  if(isset($form_state['clicked_button'])) {
    foreach($form_state['values']['tracks'] as $did => $disc) {
      if (is_numeric($did)) {
        $track_numbers[$did] = count($disc)-1;
      }
    }

    $clicked_button = $form_state['clicked_button'];
    $max_did = count($form_state['values']['tracks']) - 2;  //当前编辑框碟数
    //$form['tracks']['asdfa'] = array('#type' => 'textarea', '#value' => print_r($form_state['values']['tracks'], true));
    if ($clicked_button['#parents'][1] == 'adddisc') {  //增加碟
      $track_numbers[] = $default_track;  //增加一碟默认10轨
    }
    if ($clicked_button['#parents'][1] == 'removedisc') {  //减少碟
      array_pop($track_numbers);
    }
    
    if ($clicked_button['#parents'][1] > 0) {  //增添轨道
      $click_did = $clicked_button['#parents'][1];
      $max_tid = count($form_state['values']['tracks'][$click_did]);
      if ($clicked_button['#value'] == '增加一轨') {
         $track_numbers[$click_did]++;
      }
      if ($clicked_button['#value'] == '移除一轨') {
         $track_numbers[$click_did] = max(1, $track_numbers[$click_did]-1);
      }
    }
  }

  foreach($track_numbers as $did => $track_number) {
    $disc = isset($album->tracks[$did]) ? $album->tracks[$did] : NULL;
    $form['tracks'][$did][0] = _discography_disc_form($did, $disc);
    for($i = 1; $i <= $track_number; $i++) {
      $track = isset($album->tracks[$did][$i]) ? $album->tracks[$did][$i] : NULL;
      $form['tracks'][$did][$i] = _discography_track_form($track);
    }
  }
  
  $form['tracks']['adddisc'] = array(
      '#type' => 'button',
      '#value' => '增加一碟',
      //'#submit' => array('discography_album_button_submit'), // If no javascript action.
      '#ajax' => array(
        //'path' => 'discography/album/form/js/adddisc/'.$disc_max,
        'callback' => 'discography_album_form_track_callback',
        'wrapper' => 'tracks-wrap',
        'effect' => 'fade',
        'progress' => array('type' => 'noprogress'),
      ),);
  $form['tracks']['removedisc'] = array(
      '#type' => 'button',
      '#value' => '移除一碟',
      //'#submit' => array('discography_album_button_submit'), // If no javascript action.
      '#ajax' => array(
        //'path' => 'discography/album/form/js/remdisc/'.$disc_max,
        'callback' => 'discography_album_form_track_callback',
        'wrapper' => 'tracks-wrap',
        'effect' => 'fade',
        'progress' => array('type' => 'noprogress'),
      ),);

  $form['submit'] = array('#type' => 'submit', '#value' => '保存专辑', );
  if (isset($album->aid)) {
    $form['delete'] = array('#type' => 'submit', '#value' => '删除专辑', '#submit' => array('discography_album_delete_submit'),);
  }

  return $form;
}

function _discography_disc_form($did = 1, $disc = null) {
  $form = array();
  $form['title'] = array(
        '#type' => 'textfield',
        '#default_value' => isset($disc[0]->title) ? $disc[0]->title : '碟'.$did,
        '#size' => 100,
        '#required' => true,
        '#attributes' => array('title' => '碟名，一般保持默认，根据Booklet，只有少数多碟专辑或CD+DVD专辑需要，如Genius Party OST。'),
      );
    $form['addtrack'] = array(
      '#type' => 'button',
      '#value' => '增加一轨',
      //'#submit' => array('discography_album_button_submit'), // If no javascript action.
      '#ajax' => array(
        //'path' => 'discography/album/form/js/addtrack/'.$did,
        'callback' => 'discography_album_form_track_callback',
        'wrapper' => 'tracks-wrap',
        'effect' => 'fade',
        'progress' => array('type' => 'noprogress'),
      ),
    );
    $form['removerack'] = array(
      '#type' => 'button',
      '#value' => '移除一轨',
      //'#submit' => array('discography_album_button_submit'), // If no javascript action.
      '#ajax' => array(
        //'path' => 'discography/album/form/js/remtrack/'.$did,
        'callback' => 'discography_album_form_track_callback',
        'wrapper' => 'tracks-wrap',
        'effect' => 'none',
        'progress' => array('type' => 'noprogress'),
      ),);
  return $form;
}

function _discography_track_form($track = null) {
  //$id = "edit-tracks-$track->did-$track->tid-";
  $form = array();
  $form['title'] = array(
        '#type' => 'textfield',
        '#default_value' => isset($track->title) ? $track->title : '',
        '#size' => 60,
        '#attributes' => array('class' => array('tracktiltes')),
      );
  $form['composer'] = array(
        '#type' => 'textfield',
        '#default_value' => isset($track->composer) ? $track->composer : '',
        '#size' => 15,
        //'#attributes' => array('title' => '作曲者，以Booklet为准，以半角逗号分开。<br />'._discography_get_default_artsits('composer', $id)),
      );
  $form['arranger'] = array(
        '#type' => 'textfield',
        '#default_value' => isset($track->arranger) ? $track->arranger : '',
        '#size' => 15,
        //'#attributes' => array('title' => '编曲者，以Booklet为准，以半角逗号分开。<br />'._discography_get_default_artsits('arranger', $id)),
      );
  $form['lyricist'] = array(
        '#type' => 'textfield',
        '#default_value' => isset($track->lyricist) ? $track->lyricist : '',
        '#size' => 15,
        //'#attributes' => array('title' => '作词者，以Booklet为准，以半角逗号分开。<br />'._discography_get_default_artsits('lyricist', $id)),
      );
  $form['performer'] = array(
        '#type' => 'textfield',
        '#default_value' => isset($track->performer) ? $track->performer : '',
        '#size' => 15,
        //'#attributes' => array('title' => '演唱者，以Booklet为准，以半角逗号分开。<br />'._discography_get_default_artsits('performer', $id)),
      );
  return $form;
}

function theme_discography_album_form($variables) {
  $form = $variables['form'];
  $header = array();

  // 提取表单域标签并且设置原标签为隐藏，否则不能显示默认的验证信息
  $title_labels = array();
  foreach($form['album'] as $id => $element) {
    $title_labels[$id] = theme('form_element_label', array('element' => $element));
    if(isset($form['album'][$id]['#title_display']))
      $form['album'][$id]['#title_display'] = 'invisible';
  }

  $rows[] = array(
    array('data'=>$title_labels['title'], 'class' => 'active'), 
    array('data' => drupal_render($form['album']['title']), 'colspan' => '5'), 
    array('data'=>$title_labels['source'], 'class' => 'active'),  
    array('data' => drupal_render($form['album']['source']), 'colspan' => '3')
  );
  $rows[] = array(
    array('data'=>$title_labels['artist'],'class' => 'active'), 
    array('data' => drupal_render($form['album']['artist']), 'colspan' => '5'), 
    array('data'=>$title_labels['path'],'class' => 'active'),  
    drupal_render($form['album']['path']), 
    array('data'=>$title_labels['type'],'class' => 'active'), 
    drupal_render($form['album']['type'])
  );
  $rows[] = array(
    array('data'=>$title_labels['pubcat'],'class' => 'active'), 
    drupal_render($form['album']['pubcat']), 
    array('data'=>$title_labels['price'], 'class' => 'active'), 
    drupal_render($form['album']['price']), 
    array('data'=>$title_labels['pubdate'], 'class' => 'active'), 
    drupal_render($form['album']['pubdate']), 
    array('data'=>$title_labels['pubcom'], 'class' => 'active'), 
    drupal_render($form['album']['pubcom']), 
    array('data' =>$title_labels['booklet'], 'class' => 'active'), 
    drupal_render($form['album']['booklet'])
  );
  $rows[] = array(
    array('data'=>$title_labels['brief'],'class' => 'active'), 
    array('data' => drupal_render($form['album']['brief']), 'colspan' => '5'),
    array('data' => '<div style="float:left;margin-right:5px;border:1px solid black;width:100px;height:100px;"><div id="cover-wapper" style="filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(sizingMethod=scale);">'.$form['album']['cover-path']['#value'].'</div></div>'.drupal_render($form['album']['cover']), 'colspan' => '4')
  );
  $rows[] = array(
    array('data'=>$title_labels['comment'], 'class' => 'active', 'rowspan'=>2), 
    array('data' => drupal_render($form['album']['comment']), 'colspan' => '5', 'rowspan'=>2), 
    array('data'=>$title_labels['taobao'], 'class' => 'active'),  
    array('data' => drupal_render($form['album']['taobao']), 'colspan' => '3'),
  );
  $rows[] = array(
    array('data'=>'预留', 'class' => 'active'), 
    array('data'=> drupal_render($form['album']['taobao']), 'colspan' => '3')
  );

  $output = '<div class="help">新建专辑信息，请使用IE或者Firefox等支持javascript的浏览器，否则将不能显示提示信息，也不能动态增加碟和曲目。*号为必填项。</div>';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'id' => 'album-table'));
  $output .= drupal_render_children($form);
  return $output;
}

function theme_discography_tracks_form($variables) {
  //return print_r($form, true);
  $form = $variables['form'];
  $output = '';
  foreach($form as $did => $disc) {
    if (!is_numeric($did)) {
      continue;
    }
    //碟名行
    $disc_rows = array();
    $disc_rows[] = array(
      array('data' => '碟名', 'class' => 'active'), 
      array('data' => drupal_render($form[$did][0]['title']), 'class' => 'active'), 
      array('data' => drupal_render($form[$did][0]['addtrack']), 'class' => 'active'), 
      array('data' => drupal_render($form[$did][0]['removerack']), 'class' => 'active')
    );
    $output .= theme('table', array('header' => array(), 'rows' => $disc_rows));
    //曲目行
    $header = ($did == 1 ? array('', '曲名', '作曲', '编曲', '作词', '演唱') : array());
    $rows = array();
    foreach($disc as $tid => $track) {
      if (!is_numeric($tid) || $tid == 0) {
        continue;
      }
      $rows[] = array(
        $tid, 
        drupal_render($form[$did][$tid]['title']), 
        drupal_render($form[$did][$tid]['composer']), 
        drupal_render($form[$did][$tid]['arranger']), 
        drupal_render($form[$did][$tid]['lyricist']), 
        drupal_render($form[$did][$tid]['performer'])
      );
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  $output .= '<table style="width:160px;"><tr><td>'.drupal_render($form['adddisc']).'</td><td>'.drupal_render($form['removedisc']).'</td></tr></table>';
  $output .= drupal_render_children($form);
  return '<div id="tracks-wrap">'.$output.'</div>';
}

function discography_album_form_validate($form, &$form_state) {
  //drupal_add_js(drupal_get_path('module', 'discography') . '/js/jquery.jSelectDate-0.2.js');
  //drupal_add_js(drupal_get_path('module', 'discography') . '/js/jquery.dimensions.min.js');
  //drupal_add_js(drupal_get_path('module', 'discography') . '/js/jquery.inputHintBox.js');
  //drupal_add_js(drupal_get_path('module', 'discography') . '/js/discography-form.js');

//form_set_error('booklet', print_r($form, true));
  if (!preg_match('/^[a-zA-Z0-9]+$/i', $form_state['values']['path'])) {
    form_set_error('path', '路径只能为英文和数字');
  }
  if (!$form_state['values']['aid']) {  //新建专辑才检查路径
    $count = db_select('discography_album', 'a')
      ->condition('path', trim($form_state['values']['path']))
      ->fields('a', array('aid'))
      ->execute()
      ->rowCount();
    //$count =(db_query("SELECT aid FROM {discography_album} WHERE path='%s'", trim($form_state['values']['path']))->execute())->rowCount();
    if ($count) {
      form_set_error('path', '路径不能和已有专辑重复');
    }
  }
  if (!empty($form_state['values']['price']) && !is_numeric($form_state['values']['price'])) {
    form_set_error('price', '价格只能输入数字');
  }
  if (!empty($form_state['values']['booklet']) && !valid_url($form_state['values']['booklet'], true)) {
    form_set_error('booklet', 'url格式错误');
  }
  if (!empty($form_state['values']['taobao']) && !valid_url($form_state['values']['taobao'], true)) {
    form_set_error('taobao', 'url格式错误');
  }
}

function discography_album_form_submit($form, &$form_state) {
  //drupal_set_message(print_r($form_state['values'], true));

  if (is_numeric($form_state['values']['aid']) && $form_state['values']['aid'] > 0) {
    discography_album_update($form, $form_state);
  } else {
    discography_album_insert($form, $form_state);
  }
  
  // 更新最后修改时间
  variable_set('discography_album_lastupdate', time());
  //cache_clear_all();  //最好只清楚相关cache

  if (isset($_REQUEST['destination'])) {
    $form_state['redirect'] = drupal_get_destination();
    unset($_REQUEST['destination']);
  } else {
    $form_state['redirect'] = 'discography/'.$form_state['values']['path'];
  }
}

function discography_album_insert($form, &$form_state) {
  //插入 album 数据库
  $aid = db_insert('discography_album')
    ->fields(array(
      'title' => trim($form_state['values']['title']),
      'pubdate' => $form_state['values']['pubdate'],
      'pubcat' => trim($form_state['values']['pubcat']),
      'pubcom' => trim($form_state['values']['pubcom']),
      'price' => $form_state['values']['price'] ? $form_state['values']['price'] : 0,
      'cover' => 0,
      'type' => $form_state['values']['type'],
      'source' => $form_state['values']['source'],
      'brief' => $form_state['values']['brief'],
      'path' => $form_state['values']['path'],
      'booklet' => $form_state['values']['booklet'],
      'comment' => $form_state['values']['comment'],
      'taobao' => $form_state['values']['taobao'],
    ))
    ->execute();

  discography_uplaod_image($aid);
  discography_update_track($form_state, $aid);
  discography_update_artist($form_state, $aid);
	discography_add_event('添加专辑「'. $form_state['values']['title'].'」', 'discography/'. $form_state['values']['path']);
}

function discography_album_update($form, &$form_state) {
  $aid = $form_state['values']['aid'];
  //更新 album 数据库

  db_update('discography_album')
    ->fields(array(
      'title' => trim($form_state['values']['title']),
      'pubdate' => $form_state['values']['pubdate'],
      'pubcat' => trim($form_state['values']['pubcat']),
      'pubcom' => trim($form_state['values']['pubcom']),
      'price' => $form_state['values']['price'],
      'cover' => 0,
      'type' => $form_state['values']['type'],
      'source' => $form_state['values']['source'],
      'brief' => $form_state['values']['brief'],
      'path' => $form_state['values']['path'],
      'booklet' => $form_state['values']['booklet'],
      'comment' => $form_state['values']['comment'],
      'taobao' => $form_state['values']['taobao'],
    ))
    ->condition('aid', $aid)
    ->execute();

  discography_uplaod_image($aid);
  discography_update_track($form_state, $aid);
  discography_update_artist($form_state, $aid);

  //cache_clear_all();
	discography_add_event('更新专辑「'. $form_state['values']['title'].'」', 'discography/'. $form_state['values']['path']);
}

function discography_album_delete_submit($form, &$form_state) {
  $aid = $form_state['values']['aid'];

  db_delete('discography_album')
    ->condition('aid', $aid)
    ->execute();
  discography_uplaod_image($aid, true);
  discography_update_track('', $aid, true);
  discography_update_artist('', $aid, true);

  variable_set('discography_album_lastupdate', time());
  //cache_clear_all();
	//discography_add_event('删除专辑「'. $form_state['values']['title'].'」', 'discography/'. $form_state['values']['path']);
  $form_state['redirect'] = drupal_get_destination() ? drupal_get_destination() : 'discography';
}

function discography_uplaod_image($aid, $del=false) {
  $directory_path = 'public://covers';
  file_prepare_directory($directory_path, FILE_CREATE_DIRECTORY);
  $filename = $directory_path.'/cover_'.$aid.'.jpg';
  //$filename_thumb = $directory_path.'/'.'cover_thumb_'.$aid.'.jpg';
  //删除图像
  if ($del == true) {
    file_unmanaged_delete($filename);
    //file_delete($filename_thumb);
    return;
  }
  //插入封面图像
  if (@move_uploaded_file($_FILES['files']['tmp_name']['cover'], drupal_realpath($filename))) {
    /*
    $info = image_get_info($file->filepath);
    if($info['width'] > $info['height']) {
      $tw = 100;
      $th = $info['height']*100/$info['width'];
    } else {
      $th = 100;
      $tw = $info['width']*100/$info['height'];
    }
    */
    //if (file_copy($file, $filename, FILE_EXISTS_REPLACE)) {
      //image_resize($filename, $filename_thumb, $tw, $th);
      db_update('discography_album')->fields(array('cover' => 1))->condition('aid', $aid)->execute();
      //db_query("UPDATE {discography_album} SET cover=%d WHERE aid=%d", 1, $aid);
    //}
  }
}

function discography_update_track($form_state, $aid, $del=false) {
  //删除全部相关曲目
  db_delete('discography_track')
    ->condition('aid', $aid)
    ->execute();
  
  if ($del) {return;}
  
  //插入曲目
  $did = 1;
  $tid = 0;
  foreach($form_state['values']['tracks'] as $key => $disc) {
    if (!is_numeric($key)) {continue;}
    foreach($disc as $track) {
      if (!empty($track['title'])) {
        db_insert('discography_track')
          ->fields(array(
            'aid' => $aid,
            'did' => $did,
            'tid' => $tid,
            'title' => trim($track['title']),
          ))
          ->execute();
        $tid++;
      }
    }
    $did++;
    $tid = 0;
  }
}

function discography_update_artist($form_state, $aid, $del=false) {
  //删除原艺术家
  db_delete('discography_artist')
    ->condition('aid', $aid)
    ->execute();

  if ($del == true) {return;}

  $artist_array = array();
  $artist_types = array('composer', 'arranger', 'lyricist', 'performer');

  //插入专辑艺术家
  $artists = explode(',', $form_state['values']['artist']);
  foreach ($artists as $artist) {
    if (!empty($artist)) {
      db_insert('discography_artist')
        ->fields(array(
          'aid' => $aid,
          'did' => 0,
          'tid' => 0,
          'name' => trim($artist),
          'duty' => 'artist',
        ))
        ->execute();
    }
  }
  //插入曲目艺术家
  $did = 1;
  $tid = 0;
  foreach($form_state['values']['tracks'] as $key => $disc) {
    if(!is_numeric($key)) {continue;}
    foreach($disc as $track) {
      if (!empty($track['title'])) {
        foreach($artist_types as $artist_type) {
          if (isset($track[$artist_type])) {
            $artists = explode(',', $track[$artist_type]);
            foreach ($artists as $artist) {
              if (!empty($artist)) {
                db_insert('discography_artist')
                  ->fields(array(
                    'aid' => $aid,
                    'did' => $did,
                    'tid' => $tid,
                    'name' => trim($artist),
                    'duty' => $artist_type,
                  ))
                ->execute();
              }
            }
          }
        }
        $tid++;
      }
    }
    $did++;
    $tid = 0;
  }
}

function discography_album_delete_confirm($form, &$form_state, $album) {
  $form['aid'] = array(
    '#type' => 'value',
    '#value' => $album->aid,
  );
  return confirm_form($form,
    '你确定要删除【'.$album->title.'】这张专辑码？',
    isset($_GET['destination']) ? $_GET['destination'] : 'discography/album/edit/'.$album->aid,
    '无法复原此操作!',
    '删除',
    '取消'
  );
}

function discography_album_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    discography_album_delete_submit(NULL, $form_state);
  }

  $form_state['redirect'] = drupal_get_destination() ? drupal_get_destination() : 'discography';
}

function discography_album_button_submit($form, &$form_state) {
  drupal_set_message('对不起，你的浏览器不支持Javascript，所以不能增删曲目和碟片，请使用Chrome、IE或者Firefox浏览器。');
  return flase;
}

function discography_xiami_add_form($form, &$form_state, $album) {
  $form['url'] = array(
    '#title' => '专辑页面地址',
    '#type' => 'textfield',
    '#default_value' => $album->xiami ? 'http://www.xiami.com/album/'.$album->xiami : '',
    '#required' => true,
  );
  $form['album'] = array(
    '#type' => 'value',
    '#value' => $album,
  );
  $form['submit'] = array(
    '#value' => '提交',
    '#type' => 'submit',
  );
  return $form;
}

function discography_xiami_add_form_validate($form, &$form_state) {
  if (!valid_url($form_state['values']['url'], true)) {
    form_set_error('url', '请输入正确的虾米专辑页面地址');
    return;
  }
  $path = parse_url($form_state['values']['url']);
  if ($path['host'] != 'www.xiami.com' || substr($path['path'], 1, 5) != 'album' ||!is_numeric(basename($path['path']))) {
    form_set_error('url', '请输入正确的虾米专辑页面地址');
    return;
  }
}

function discography_xiami_add_form_submit($form, &$form_state) {
  include(drupal_get_path('module', 'discography').'/simplehtmldom/simple_html_dom.php');

  $updated = false;
  $album = $form_state['values']['album'];
  $url = $form_state['values']['url'];
  $path = parse_url($url);
  $xid = basename($path['path']);
  // 更新专辑虾米id
  db_update('discography_album')
    ->condition('aid', $album->aid)
    ->fields(array('xiami' => $xid))
    ->execute();

  $html = file_get_html('http://'.$path['host'].$path['path']);
  $discs = $html->find('table.track_list');
  foreach ($discs as $did => $disc) {
    $tracks = $disc->find('td.song_name a');
    foreach ($tracks as $tid => $track) {
      db_update('discography_track')
        ->condition('aid', $album->aid)
        ->condition('did', $did+1)
        ->condition('tid', $tid+1)
        ->fields(array('xiami' => basename($track->href)))
        ->execute();
      $updated = true;
    }
  }
  if ($updated) 
    drupal_set_message('感谢提交虾米地址！');
  else
    drupal_set_message('您提交的虾米地址有误或者系统错误，请重试或联系管理员。');

  drupal_goto('discography/'.$album->path);
  //$page = file_get_contents($url);
  //preg_match_all('/<div id=\"track\" class=\"album_tracks mgt20\">(.*)<\/div>/i', $page, $track);
  //preg_match_all('/<table\sclass=\"([a-z0-9_]+)\">(.*)<\/table>/', $page, $track);
  //$html=new DOMDocument(); 
  //$html->loadHTMLFile($url); 
  //$title=$html->getElementsByTagName('title'); 
  
}

function discography_album_page($album) {
  $cover_path = 'public://covers/';

  $breadcrumb[] = l('首页', NULL);
  $breadcrumb[] = l('作品', 'discography');
  drupal_set_breadcrumb($breadcrumb);

  
  drupal_add_library('system', 'ui.dialog');
  drupal_add_library('system', 'ui.tabs');
	drupal_add_js(drupal_get_path('module', 'discography') . '/js/discography.js');
  drupal_add_css(drupal_get_path('module', 'discography'). '/js/theme/cupertino/jquery.ui.theme.css', array('group' => 'CSS_THEME', 'weight' => 9999)); // jQuery Theme

  if ($album->booklet) {
    drupal_add_html_head('
		<script type="text/javascript" src="http://www.google.com/jsapi?key=ABQIAAAAv9wxMTKQQ3CSxVyusFI6axR_h0wjt8SgP2DJBzNke53wqc0ZPhSMf_5_ZNguNmNkjfEkJuZW-zgOWA"></script>
		<script type="text/javascript" src="http://www.google.com/uds/solutions/slideshow/gfslideshow.js"></script>
		');
  }

  $imgpath = drupal_get_path('module', 'discography').'/img/';
  $output = '<div class="grid-2 alpha">';

  if ($album->cover) {
    $output .= '<div class="cover">'.l(theme('image_style', array('style_name' => 'thumbnail', 'path' => $cover_path.'cover_'.$album->aid.'.jpg', 'alt' => '专辑封面')), file_create_url($cover_path. 'cover_'.$album->aid.'.jpg'), array('query' => array('height'=>500, 'width'=>500,'modal'=>0), 'attributes' => array('rel'=>'facebox[.cover]'), 'html' => true,)).'</div>';
  } else {
    $output .= '<div class="cover"><img alt="没有专辑封面" src="/'.$cover_path.'nocover_thumb.gif" /></div>';
  }

  $output .= '<h3>'.$album->artist.'</h3>';
  $output .= '<div class="albuminfo"><span class="blue"><strong>编号：</strong>'.$album->pubcat.'</span>';
  $output .= '<span><strong>定价：</strong>'.$album->price.'日元（含税）</span>';
  $date = explode('-', $album->pubdate);
  $output .= '<span class="blue"><strong>日期：</strong>'.$date[0].'年'.$date[1].'月'.$date[2].'日</span>';
  $output .= '<span><strong>发行：</strong>'.$album->pubcom.'</span></div>';
  $output .= '<div class="operation">';
  $output .= $album->taobao ? l('<img alt="购买专辑" src="/'.$imgpath.'buyalbum.gif" />', $album->taobao, array('html' => true)).'<br />' : '';
  $output .= l('<img alt="歌词投稿" src="/'.$imgpath.'lyricpost.gif" />', 'discography/lyric/post/'.$album->aid, array('query' => array('height'=>600, 'width'=>500,'modal'=>0), 'attributes' => array('class'=>array('popuplyric')), 'html' => true)).'<br />';
  $output .= l('<img alt="错误汇报" src="/'.$imgpath.'errorreport.gif" />', 'comment/reply/32', array('fragment' => 'comment-form', 'html' => true)).'<br />';
  if (user_access('admin discography')) {
    $output .= l('<img alt="编辑专辑" src="/'.$imgpath.'editalbum.gif" />', 'discography/album/edit/'.$album->aid, array('html' => true)).'<br />';
    $output .= l('<img alt="导出专辑" src="/'.$imgpath.'exportalbum.gif" />', 'discography/album/export/'.$album->aid, array('html' => true));
  }
  $output .= l('提交虾米', 'discography/xiami/add/'.$album->aid);
  $output .= '</div>';
  $output .= $album->booklet ? '<div id="slideshow" class="gss">Booklet加载中…<span style="display:none;">'.$album->booklet.'</span></div>' : '';
  if ($album->xiami)
    $output .= '<embed src="http://www.xiami.com/widget/6272414_'.$album->xiami.'_150_346_4DB653_009609/albumPlayer.swf" allowScriptAccess="always" quality="high" type="application/x-shockwave-flash" width="150" height="346" wmode="opaque"></embed>';
    //$output .= '<embed src="http://www.xiami.com/widget/6272414_439390_150_346_4DB653_009609/albumPlayer.swf"  quality="high" wmode="opaque" type="application/x-shockwave-flash" width="150" height="346"></embed>';

  // 右列
  $output .= '</div><div class="grid-10 omega">';
  $output .= '<h3><img alt="专辑简介" src="/'.$imgpath.'albuminfo.gif" /></h3><div>'.check_markup($album->brief).'</div>';
  //曲目
  $track_table = '';
  $header = array(array('data' => '', 'style'=>'width:10px;'), '曲名', array('data' => '', 'style'=>'width:5px;'),'作曲', '编曲', '作词', '演唱', array('data' => '歌词', 'style'=>'width:25px;'));
  //取得所有歌词
  $lyrics = array();
  $result = db_select('discography_lyric', 'l')
    ->fields('l', array('lid', 'did', 'tid'))
    ->condition('aid', $album->aid)
    ->condition('moderate', 1)
    ->execute();
  //$result = db_query("SELECT lid,did,tid FROM {discography_lyric} WHERE aid=:aid AND moderate=1", array(':aid' => $album->aid));
  foreach ($result as $lyric) {
    $lyrics[$lyric->did][$lyric->tid] = $lyric->lid;
  }

  $xiami_uid = 6272414;
  foreach($album->tracks as $did => $disc) {
    $rows = array();
    foreach ($disc as $tid => $track) {
      if ($tid == 0) {
        $discname = check_plain($track->title);
      } else {
        $artist['composer'] = isset($track->composer) ? $track->composer : '';
        $artist['arranger'] = isset($track->arranger) ? $track->arranger : '';
        $artist['lyricist'] = isset($track->lyricist) ? $track->lyricist : '';
        $artist['performer'] = isset($track->performer) ? $track->performer : '';

        //print_r($track);
				if(preg_match('/(菅野よう子|yoko kanno|kanno yoko|sample red|gabriela)/i', implode('|', $artist)))
					$track_class = 'track_yk';
				else
					$track_class = 'track_none';
				$row = array(
          $tid, 
          check_plain($track->title), 
          $track->xiami ? l('▼', 'discography/player/track/'.$xiami_uid.'_'.$track->xiami, array('attributes' => array('class' => array('toggle_player')))) : '',
          array('data' => check_plain($artist['composer']), 'style' => 'width:70px;'), 
          array('data' => check_plain($artist['arranger']), 'style' => 'width:70px;'), 
          array('data' => check_plain($artist['lyricist']), 'style' => 'width:70px;'), 
          array('data' => check_plain($artist['performer']), 'style' => 'width:90px;'), 
          (isset($lyrics[$did][$tid]) ? l('<img alt="歌词" src="/'.$imgpath.'lyric.gif" />', "discography/lyric/$album->aid/$did/$tid", array('html' => true, "attributes" => array("class" => array('popuplyric')))) : '')
        );
				//$rows[] = array('data' => $row, 'class' => $track_class);
        $rows[] = $row;
        if($track->xiami) {
          //$player = '<embed src="http://www.xiami.com/widget/6272414_'.$track->xiami.'/singlePlayer.swf" type="application/x-shockwave-flash" width="257" height="33" autostart="true" allowScriptAccess="always" wmode="transparent"></embed>';
          $player = 'hidden player';
          $rows[] = array('data' => array(array('data' => $player, 'colspan' => 8)), 'class'=>array('element-hidden', 'player'));
        }
      }
    }
    $track_table .= '<h4 class="" style="border-bottom:1px solid #b4d7f0">'.$discname.'</h4>'.theme('table', array('header' => $header, 'rows' => $rows));
  }
  $output .= '<h3><img alt="曲目列表" src="/'.$imgpath.'tracklist.gif" /></h3><div>'.$track_table.'</div>';
  $output .= $album->comment ? '<h3><img alt="其它说明" src="/'.$imgpath.'comment.gif" /></h3><div>'.check_markup($album->comment).'</div>' : '';
  $output .= '</div>';

	drupal_set_title($album->title);

  return $output;
}
/*
function _discography_get_default_artsits($type, $id) {
  static $artists = array();
  if (!count($artists)) {
    $result = db_query("SELECT name, duty FROM {discography_artist} GROUP BY name, duty ORDER BY MAX(aid) DESC");
    foreach ($result as $artist) {
      $artists[$artist->duty][] = $artist->name;
    }
    //dprint_r($artists);
  }
  $artists_links = array();
  $i = 0;
  foreach ($artists[$type] as $artist) {
    if ($i < 10) {
      $artists_links[] = '<a href="#" onclick ="return Drupal.insertInline(\''.$id.$type.'\', this);">'.$artist.'</a>';
      $i++;
    }
  }
  return implode(', ', $artists_links);
}

function _discography_get_default_pubcoms($id) {
  static $pubcoms_links = array();
  if (!count($pubcoms_links)) {
    $result = db_select('discography_album', 'a')->fields('a', array('pubcom'))->orderBy('a.aid', 'DESC')->distinct()->range(0, 3)->execute();
    //$result = db_query_range("SELECT DISTINCT pubcom FROM {discography_album} ORDER BY aid DESC", 0, 3);
    foreach ($result as $pubcom) {
      if($pubcom->pubcom) {
        $pubcoms_links[] = '<a href="#" onclick="return Drupal.insertInline(\''.$id.'\', this);">'.$pubcom->pubcom.'</a>';
      }
    }
  }
  return implode(', ', $pubcoms_links);
}*/

function discography_album_form_track_callback($form, $form_state) {
  return $form['tracks'];
}

/**
 * 功能描述
 *
 * @param $参数名
 *   描述
 * @return
 *   描述
 */
function discography_album_export($album) {
  if (arg(4) == 'frame') {
    $output = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
    <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="zh-hans" lang="zh-hans">
    <head>
      <title>CMようこ | 菅野洋子中文网</title>
      <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
      <link type="text/css" rel="stylesheet" media="all" href="/sites/all/themes/framework/style.css?y" />
      <script type="text/javascript">
  　　</script>
    </head>
    <body style="font-size:11pt;color:#333;width:100%;padding-left:30px;" >';
    $output .= '<span style="font-size:1.5em;color:rgb(0, 102, 153);font-weight:bold;">☆商品描述☆</span><br /><hr /><br />';
    $output .= '<div style="margin-left:10px;"><span style="font-size:1.2em;font-weight:bold;">'.$album->title.'</span><br />';
    $output .= '<span style="font-weight:bold;">'.$album->artist.'</span><br />';

    $cover_path = file_directory_path().'/covers/';
    if ($album->cover) {
      $output .= '<div class="cover"><img alt="专辑封面" src="http://final.ykfan.cn/'.$cover_path.'cover_thumb_'.$album->aid.'.jpg" /></div>';
    } else {
      $output .= '<div class="cover"><img alt="没有专辑封面" src="http://final.ykfan.cn/'.$cover_path.'nocover_thumb.gif" /></div>';
    }
    $output .= '编号：'.$album->pubcat.'<br />';
    $output .= '定价：'.$album->price.'日元（含税）<br />';
    $date = explode('-', $album->pubdate);
    $output .= '日期：'.$date[0].'年'.$date[1].'月'.$date[2].'日</span><br />';
    $output .= '发行：'.$album->pubcom.'<br /><br />';
    $output .= '<span style="font-weight:bold;">简介：</span>'.check_markup($album->brief, 3).'<br /><br />';
    foreach($album->tracks as $did => $disc) {
      foreach ($disc as $tid=>$track) {
        if ($tid == 0) {
          $output .= '<span style="font-weight:bold;">'.check_plain($track->title).'</span><br />';
        } else {
          $output .= sprintf('%02d', $tid).'.'.check_plain($track->title).'<br />';
          $artists = array();
          $track->composer ? $artists[] = '作曲：'.check_plain($track->composer) : '';
          $track->arranger ? $artists[] = '编曲：'.check_plain($track->arranger) : '';
          $track->lyricist ? $artists[] = '作词：'.check_plain($track->lyricist) : '';
          $track->performer ? $artists[] = '歌手：'.check_plain($track->performer) : '';
          $output .= '<span style="color:#999;">'.implode('／',$artists).'</span><br /><br />';
          $rows[] = array(sprintf('%02d', $tid), check_plain($track->title), array('data' => check_plain($track->composer), 'style' => 'width:70px;'), array('data' => check_plain($track->arranger), 'style' => 'width:70px;'), array('data' => check_plain($track->lyricist), 'style' => 'width:70px;'), array('data' => check_plain($track->performer), 'style' => 'width:90px;'), ($lyrics[$did][$tid] ? l('<img alt="歌词" src="/'.$imgpath.'lyric.gif" />', "discography/lyric/$album->aid/$did/$tid/popup", array('html' => true, "attributes" => array("rel" => 'facebox'))) : ''));
        }
      }
      $output .= '<br />';
    }
    $output .= $album->comment ? check_markup($album->comment) : '';
    $output .= '</div><span style="font-size:1.5em;color:rgb(0, 102, 153);font-weight:bold;">☆买家须知☆</span><br /><hr /><br />';
    $output .= '
    <ul style=""><li>本店所有包运费商品除非特别说明均为日本直送包运费价格，需一次付清全款</li>
      <li>本价格不保证商品有初回特典，如需特典，请在发售至少三周前先与我联系</li>
      <li>非预订未发售商品配送和邮寄通常需要10到30天不等，如对时间有要求请先联系我询问</li>
      <li>一般情况下不接受退订，故请考虑好再拍下</li>
      <li>如果订购的商品因为缺货等原因无法订到，将全额退款</li>
      <li>直送商品请确保收货地址准确（收件人如门卫等应了解您的姓名的拼音），否则若出现遗失，本店将不承担责任</li>
      <li>近期日元汇率变动较大，可能价格会有少量变动，故拍下前请尽量先与我联系</li>
    </ul>';
    $output .= '<span style="font-size:1.5em;color:rgb(0, 102, 153);font-weight:bold;">☆联系方式☆</span><br /><hr /><br />';
    $output .= '<div style="margin-left:10px;">
      旺旺：<a target="_blank" href="http://amos1.taobao.com/msg.ww?v=2&uid=ffbum&s=1" ><img border="0" src="http://amos1.taobao.com/online.ww?v=2&uid=ffbum&s=1" alt="点击这里给我发消息" /></a><br />
      QQ：3547168<br />
      E-mail：yuanyeff@<span style="display:none;">null</span>gmail.com</div><br /><br />';
    $output .= '</body></html>';
    print $output;
  } else {
    $output = '<iframe src="'.request_uri().'/frame" width="90%" height="1800" marginwidth="0" framespacing="0" marginheight="0" frameborder="0" style="padding:10px;border:0px;" ></iframe>';
    return $output;
  }
}