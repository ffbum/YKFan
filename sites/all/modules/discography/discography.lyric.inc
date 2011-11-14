<?php
// $Id: node.pages.inc,v 1.28.2.1 2008/02/27 19:44:44 goba Exp $

/**
 * @file
 * Page callbacks for adding, editing, deleting, and revisions management for content.
 */

function discography_lyric_post_page($album) {
  print drupal_render(drupal_get_form('discography_lyric_form', $album));
  exit;
}

function discography_lyric_edit_page($lyric) {
  $album = discography_album_load($lyric->aid);
  print drupal_render(drupal_get_form('discography_lyric_form', $album, $lyric));
  exit;
}

function discography_lyric_form($form, &$form_state, $album = NULL, $lyric = NULL) {
  $option = array();
  if ($lyric) {
    $option[sprintf("%02d%03d", $lyric->did, $lyric->tid)] = $album->tracks[$lyric->did][$lyric->tid]->title;
  } else {
    foreach ($album->tracks as $did=>$disc) {
      foreach ($disc as $tid=>$track) {
        if ($tid != 0) {
           $option[sprintf("%02d%03d", $did, $tid)] = $track->title;
        }
      }
    }
  }
  $destination = $_GET['destination'] ? $_GET['destination'] : 'discography/'.$album->path;
  $form['#action'] = url('discography/lyric/submit',  array('query' => array('destination' => $destination)));
	$form['#attributes'] = array('enctype' => 'multipart/form-data');
  $form['album'] = array('#markup' => '<h3>专辑：'.$album->title.'</h3>');
  $form['tracks'] = array(
    '#type' => 'select',
    '#title' => '选择曲目',
    '#options' => $option,
  );
  $form['languages'] = array(
    '#type' => 'select',
    '#title' => '选择语言',
    '#options' => array('原文', '罗马字拼写或Gablish听写', '中文翻译', '英文翻译', '日文翻译', '其它语言翻译'),
    '#default_value' => ($lyric->language ? $lyric->language : 0),
    '#description' => ' 请选择歌词语言类型。“原文”为官方在Booklet中的歌词原文，可以包括任何语言。“翻译”可以是fans自己的翻译，也可以是官方的翻译。',
  );

	if ($lyric->lrc) {
		$filename = file_create_url('public://lyrics/lyric_'.$lyric->lid.'.lrc');
		$form['lrcfile'] = array('#markup' => l('查看歌词文件', $filename, array('attributes' => array('target' => '_BLANK'))));
	} else {
		$form['lrcfile'] = array(
			'#type' => 'file',
			'#title' => '上传LRC歌词文件',
		);
	}
  $form['lyric'] = array(
    '#type' => 'textarea',
    '#title' => '歌词',
    '#rows' => 20,
    '#default_value' => ($lyric->lyric ? $lyric->lyric : ''),
    '#description' => ' 请输入歌词内容，翻译歌词最好包括翻译后曲名，注意分行排版。并请尽量在末尾注明翻译或者摘录人。',
  );
  $form['aid'] = array(
    '#type' => 'hidden',
    '#value'=> $album->aid,
  );
  $form['lid'] = array(
    '#type' => 'hidden',
    '#value'=> ($lyric->lid ? $lyric->lid : 0),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => '提交歌词',
  );

  return $form;
}

function discography_upload_lyric($lid, $del=false) {
  $directory_path = 'public://lyrics';
  file_prepare_directory($directory_path, FILE_CREATE_DIRECTORY);
  $filename = $directory_path.'/lyric_'.$lid.'.lrc';
	
  //删除旧歌词
  if ($del == true) {
    file_unmanaged_delete($filename);
    return;
  }
	
  //插入歌词
  $validators = array(
    'file_validate_extensions' => array('lrc'),
  );
	if (@move_uploaded_file($_FILES['files']['tmp_name']['lrcfile'], drupal_realpath($filename))) {
    db_update('discography_lyric')->fields(array('lrc' => 1))->condition('lid', $lid)->execute();
  }
}

function discography_lyric_submit() {
  sscanf($_POST['tracks'], "%02d%03d", $did, $tid);

  if (is_numeric($_POST['lid']) && $lid = $_POST['lid']) {
    db_update('discography_lyric')
      ->fields(array(
        'lyric' => $_POST['lyric'],
        'language' => $_POST['languages'],
        'changed' => time(),
      ))
      ->condition('lid', $lid)
      ->execute();
  } else {
    $lid = db_insert('discography_lyric')
      ->fields(array(
        'aid' => $_POST['aid'],
        'did' => $did,
        'tid' => $tid,
        'lyric' => $_POST['lyric'],
        'language' => $_POST['languages'],
        'created' => time(),
        'changed' => time(),
        'moderate' => 0,
      ))
      ->execute();
  }
	
	discography_upload_lyric($lid);
  drupal_goto();
}

function discography_lyric_delete_confirm($form, &$form_state, $lyric) {
  $form['lid'] = array(
    '#type' => 'value',
    '#value' => $lyric->lid,
  );
  return confirm_form($form,
    '你确定要删除这篇歌词码？'.$lyric->lid,
    isset($_GET['destination']) ? $_GET['destination'] : 'discography/admin/lyric',
    '无法复原此操作！',
    '删除',
    '取消'
  );
}
function discography_lyric_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    discography_lyric_delete($form_state['values']['lid']);
  }

  //$form_state['redirect'] = '<front>';
}
function discography_lyric_delete($lid) {
  db_delete('discography_lyric')->condition('lid', $lid)->execute();
	discography_upload_lyric($lid, true);

  drupal_set_message('已删除歌词！后悔也来不及了！');
}

function discography_lyric_moderate($lyric, $jump = true) {
  db_update('discography_lyric')
    ->fields(array(
      'moderate' => $lyric->moderate ? 0 : 1,
      'changed' => time(),
    ))
    ->condition('lid', $lyric->lid)
    ->execute();

  $destination = isset($_GET['destination']) ? $_GET['destination'] : 'discography/admin/lyric';
	variable_set('discography_album_lastupdate', time());
  $lyric->moderate ? drupal_set_message('歌词已经取消审核，不能在专辑页面显示！') : drupal_set_message('歌词已经通过审核，可以直接通过专辑页面浏览了～');
  if ($jump)
    drupal_goto($destination);
}

function discography_lyric_view($album, $did, $tid) {
  drupal_add_library('system', 'ui.tabs');
  drupal_add_js(drupal_get_path('module', 'discography') . '/js/discography.js');
  drupal_add_css(drupal_get_path('module', 'discography'). '/js/theme/cupertino/jquery.ui.theme.css', array('group' => 'CSS_THEME', 'weight' => 9999)); // jQuery Theme
	
  $track = $album->tracks[$did][$tid];
	$output = '<div style="margin-bottom:1em;width:500px;">';
  $output .= '<h2>'.check_plain($track->title).'</h2>';
  $output .= '<span class="submitted">来自专辑：'.l($album->title, 'discography/'.$album->path).'</span><br />';
  $output .= $track->composer ? '<strong>作曲：</strong>'.$track->composer.'<br />' : '';
  $output .= $track->arranger ? '<strong>编曲：</strong>'.$track->arranger.'<br />' : '';
  $output .= $track->lyricist ? '<strong>作词：</strong>'.$track->lyricist.'<br />' : '';
  $output .= $track->performer ? '<strong>演唱：</strong>'.$track->performer : '';
	$output .= '</div>';
	
	//设置标题用于SOE
	drupal_set_title(check_plain($track->title).' 歌词');

  $languages = array('原文', '罗马字拼写或Gablish听写', '中文翻译', '英文翻译', '日文翻译', '其它语言翻译');
  
  $result = db_select('discography_lyric', 'l')
    ->fields('l')
    ->condition('aid', $album->aid)
    ->condition('did', $did)
    ->condition('tid', $tid)
    ->condition('moderate', 1)
    ->orderBy('language')
    ->orderBy('lid')
    ->execute();

	$directory_path = '';

	$index = array(1, 1, 1, 1, 1, 1);
	$lyrics = array();
  foreach ($result as $lyric) {
		if ($lyric->lrc) {
			$filename = file_create_url('public://lyrics/lyric_'.$lyric->lid.'.lrc');
			$downpath = '<br />'.l('请点击右键另存为下载LRC歌词', $filename, array('attributes' => array('target' => '_BLANK')));
		} else {
			$downpath = '';
		}
		$lyrics[] = array('title' => $languages[$lyric->language].($index[$lyric->language] > 1 ? $index[$lyric->language] : ''), 
											'content' => check_markup($lyric->lyric).$downpath, );
    $index[$lyric->language]++;
		
  }
	$output .= '
	<div id="lyric-tabs" style="width:500px;border:none;font-size: 1em;">
	<ul>';
	foreach ($lyrics as $key => $lyric) {
	
		$output .= '
		<li><a href="#tab-'.$key.'">'.$lyric['title'].'</a></li>';
	}
	$output .= '
	</ul>';
	foreach ($lyrics as $key => $lyric) {
		$output .= '
		<div id="tab-'. $key .'" style="width:450px">'.$lyric['content'].'</div>';
	}
	$output .= '
	</div>';
  if (arg(5) == 'popup') {
    print($output);
    exit;
  }
  return $output;
}

function discography_lyric_feed_page() {
	global $base_url, $language;
	$languages = array('原文', '罗马字拼写或Gablish听写', '中文翻译', '英文翻译', '日文翻译', '其它语言翻译');
	
	if (arg(3) == 'all') {
		$moderate = -1;	//全部源包括未审核
	} else {
		$moderate = 0;  //仅仅已审核
	}
  
  $query = db_select('discography_lyric', 'l')
    ->fields('a', array('aid', 'path'))
    ->fields('l')
    ->condition('l.moderate', $moderate, '>')
    ->orderBy('l.created', 'DESC')
    ->orderBy('l.lid', 'DESC');
  $query->addField('t', 'title', 'ttitle');
  $query->addField('a', 'title', 'atitle');
  $query->innerJoin('discography_album', 'a', 'a.aid=l.aid');
  $query->innerJoin('discography_track', 't', 'l.aid=t.aid AND l.did=t.did AND l.tid=t.tid');

  $result = $query->execute();
	//$result = db_query("SELECT t.title AS ttitle, a.title AS atitle, a.aid, a.path, l.* FROM {discography_lyric} l INNER JOIN {discography_album} a ON a.aid=l.aid INNER JOIN {discography_track} t ON l.aid=t.aid AND l.did=t.did AND l.tid=t.tid WHERE l.moderate > %d ORDER BY l.created DESC, l.lid DESC", $moderate);
	$items = '';
	foreach ($result as $lyric) {
		$extra = array(array('key' => 'pubDate', 'value' => gmdate('r', $lyric->created)), array('key' => 'dc:creator', 'value' => 'YKFan'), array('key' => 'guid', 'value' => $lyric->lid .' at '. $base_url, 'attributes' => array('isPermaLink' => 'false')));
		$item_text = '来自专辑：'. l($lyric->atitle, 'discography/'. $lyric->path).'的第'. $lyric->did .'碟，第'. $lyric->tid .'曲。<br />';
		$item_text .= check_markup($lyric->lyric);
		$items .= format_rss_item('《'. $lyric->ttitle .'》的'. $languages[$lyric->language].'歌词', url('discography/lyric/'. $lyric->aid .'/'. $lyric->did .'/'. $lyric->tid), $item_text, $extra);
	}

	$channel = array(
    'version'     => '2.0',
    'title'       => variable_get('site_name', 'Drupal').'的歌词库',
    'link'        => $base_url,
    'description' => '提供YKFan歌词库的更新',
    'language'    => $language->language
  );
	
	$namespaces = array('xmlns:dc' => 'http://purl.org/dc/elements/1.1/');
  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"". $channel["version"] ."\" xml:base=\"". $base_url ."\" ". drupal_attributes($namespaces) .">\n";
  $output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], $items, $channel['language']);
  $output .= "</rss>\n";

  drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');
  print $output;
}