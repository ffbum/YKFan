<?php
// $Id: node.pages.inc,v 1.28.2.1 2008/02/27 19:44:44 goba Exp $

/**
 * @file
 * Page callbacks for adding, editing, deleting, and revisions management for content.
 */

function discography_admin_page() {
  $output = '';

  $query = db_select('discography_lyric', 'l')->condition('l.moderate', 0);
  $query->addExpression('COUNT(l.lid)', 'count');
  if($query->execute()->fetchField()) {
    drupal_set_message('有未审核的歌词，请到'.l('歌词管理', 'discography/admin/lyric').'页面审核。', 'warning');
  }
  $blocks[0] = array('title' => '内容创建', 'description' => '创建网站的活动内容', 'position' => 'left', 'show' => true);
  
  $content[]= array('title' => '新建专辑', 'href' => 'discography/album/add', 'description' => '建立新的专辑信息', 'localized_options' => array());
  $content[]= array('title' => '新建列表', 'href' => 'node/add/discography-list', 'description' => '建立新的专辑列表，需要Mysql的基础知识，也可以直接使用做好的html列表。', 'localized_options' => array());
  //$blocks[]= array('title' => '新建事件', 'href' => 'node/add/event', 'description' => '建立新的时间性的活动、事件，如LIVE预告、限时活动等。', 'localized_options' => array());
  //$blocks[]= array('title' => '新建关联商品', 'href' => 'node/add/good', 'description' => '建立新的YK相关商品信息，原则是商品中能够听到YK的音乐，如动画电影DVD、游戏等。', 'localized_options' => array());
  $content[]= array('title' => '新建资料文章', 'href' => 'node/add/story', 'description' => '在资料库里添加新的文章，包括随笔翻译、访谈、乐评等精华文章。', 'localized_options' => array());
  $blocks[0]['content'] = theme('admin_block_content', array('content' => $content));
  //$blocks[] = 
  unset($content);

  $blocks[1] = array('title' => '内容管理', 'description' => '管理现有的网站内容', 'position' => 'right', 'show' => true);
  $content[]= array('title' => '专辑管理', 'href' => 'discography/admin/album', 'description' => '管理现有的专辑信息，包括编辑和删除。', 'localized_options' => array());
  $content[]= array('title' => '列表管理', 'href' => 'discography/admin/list', 'description' => '管理现有作品列表，包括编辑、删除、以及显示方式。', 'localized_options' => array());
  $content[]= array('title' => '歌词管理', 'href' => 'discography/admin/lyric', 'description' => '管理歌词信息，在这里可以对用户提交的歌词进行审核，也可以编辑或者删除。', 'localized_options' => array());
  $content[]= array('title' => '评论管理', 'href' => 'discography/admin/comment', 'description' => '管理网站评论，网站资料库中的文章是可以由匿名用户提交评论的，为了避免垃圾信息，所以需要经常检查清理。', 'localized_options' => array());
  $content[]= array('title' => '链接管理', 'href' => 'node/1/edit', 'description' => '管理首页"链接"页面的内容。', 'localized_options' => array());
  $content[]= array('title' => '头条管理', 'href' => 'discography/admin/slide', 'description' => '定制首页滚动图片新闻。', 'localized_options' => array());
  $blocks[1]['content'] = theme('admin_block_content', array('content' => $content));
  unset($content);

  $blocks[2] = array('title' => '后台设定', 'description' => '后台设定需要的数据', 'position' => 'right', 'show' => true);
  $content[]= array('title' => '专辑出处设定', 'href' => 'discography/admin/source', 'description' => '编辑专辑信息中的“出处”信息，以供添加专辑时选择。具体见内。', 'localized_options' => array());
  $content[]= array('title' => '关键词设定', 'href' => 'discography/admin/keywords', 'description' => '编辑搜索时需要的关键词，使更容易获得搜索结果。', 'localized_options' => array());
  $content[]= array('title' => '重设作品统计', 'href' => 'discography/admin/stat/reset', 'description' => '重新设定作品总数统计，一般每隔一个小时自动执行一次，不需手动运行。', 'localized_options' => array());
  $content[]= array('title' => '重建所有列表', 'href' => 'discography/admin/list/reset', 'description' => '重新从数据库中构建所有列表，一般每隔一个小时自动执行一次，不需手动运行。', 'localized_options' => array());
  //$blocks[] = 
  $blocks[2]['content'] = theme('admin_block_content', array('content' => $content));
  unset($content);
  
  $blocks[3] = array('title' => '网站管理', 'description' => '网站总体管理', 'position' => 'left', 'show' => true);
  $content[]= array('title' => '站点信息', 'href' => 'discography/admin/site-information', 'description' => '修改网站的基本信息，比如网站名称、口号、电子邮件l地址、目标、首页等等。', 'localized_options' => array());
  $content[]= array('title' => '站点维护', 'href' => 'discography/admin/site-maintenance', 'description' => '将站点设为离线(offline)模式以便进行维护，或者将其站点设为在线(online)模式。', 'localized_options' => array());
  $content[]= array('title' => '注销登陆', 'href' => 'logout', 'description' => '退出当前用户的登录，可以提高浏览速度。', 'localized_options' => array());
  $blocks[3]['content'] = theme('admin_block_content', array('content' => $content));

  return array(
    '#theme' => 'admin_page',
    '#blocks' => $blocks,
  );
}

function discography_admin_album_page() {
  discography_set_admin_breadcrumb();

  $types = array('album' => '专辑', 'single' => '单曲', 'unreleased' => '未发行');
  $destination = drupal_get_destination();

  $header = array('专辑', '类型', '发行日期', '操作');
  $result = db_select('discography_album', 'a')->extend('PagerDefault')->fields('a')->orderBy('aid', 'DESC')->limit(20)->execute();
  foreach ($result as $album) {
    $rows[] = array(
      l($album->title, 'discography/'.$album->path), 
      $types[$album->type], 
      $album->pubdate, 
      l('编辑', 'discography/album/edit/'.$album->aid, 
      array('query' => $destination)).'&nbsp;'.l('删除', 'discography/album/delete/'.$album->aid, 
      array('query' => $destination))
    );
  }

  return array(
    '#prefix' => '<div class="help">这里可以管理所有专辑，按专辑添加先后顺序排列。</div>',
    'albums' => array('#theme' => 'table', '#header' => $header, '#rows' => $rows),
    'pager' => array('#theme' => 'pager'),
  );
}

function discography_admin_lyric_page($form, &$form_state) {
  discography_set_admin_breadcrumb();
  drupal_add_library('system', 'ui.dialog');
  drupal_add_js(drupal_get_path('module', 'discography') . '/js/discography.js');
  drupal_add_css(drupal_get_path('module', 'discography'). '/js/theme/cupertino/jquery.ui.theme.css', array('group' => 'CSS_THEME', 'weight' => 9999)); // jQuery Theme.

  $languages = array('原文', '发音', '中文', '英文', '日文', '其它');

  $destination = drupal_get_destination();

  $header = array(
    'ttitle' => '曲名', 
    'atitle' => '专辑',
    'language' => array('data' => '语言', 'style' => 'width:45px;'), 
    'lyric' => '歌词', 
    'operation' => array('data' => '操作', 'style' => 'width:65px;'), 
    'moderate' => array('data' => '审核', 'style' => 'width:45px;'),
  );

  $query = db_select('discography_lyric', 'l')
    ->extend('PagerDefault')
    ->fields('a', array('aid'))
    ->fields('l')
    ->orderBy('l.moderate')
    ->orderBy('l.lid', 'DESC')
    ->limit(20);
  $query->innerJoin('discography_album', 'a', 'a.aid=l.aid');
  $query->innerJoin('discography_track', 't', 'l.aid=t.aid AND l.did=t.did AND l.tid=t.tid');
  $query->addField('t', 'title', 'ttitle');
  $query->addField('a', 'title', 'atitle');
  $result = $query->execute();

  foreach ($result as $lyric) {
    $options[$lyric->lid] = array(
      'ttitle' => check_plain($lyric->ttitle), 
      'atitle' => check_plain($lyric->atitle), 
      'language' => $languages[$lyric->language], 
      'lyric' => discography_truncate_lyric($lyric->lyric, 'xb2h', 100), 
      'operation' => theme('links', array('links' => array(
        'edit' => array(
          'title' => '编辑', 
          'href' => 'discography/lyric/edit/'.$lyric->lid,
          'attributes' => array('class' => array('popuplyric')),
          'query' => $destination,
        ),
        'delete' => array(
          'title' => '删除',
          'href' => 'discography/lyric/delete/'.$lyric->lid,
          'attributes' => array('class' => array('popuplyric')),
          'query' => $destination,
        ),
      ))),
      'moderate' => ($lyric->moderate ? l('取消','discography/lyric/moderate/'.$lyric->lid) : l('通过','discography/lyric/moderate/'.$lyric->lid)),
    );
  }
  //print_r($options);
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => '操作',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['options']['operation'] = array(
    '#type' => 'select', 
    '#title' => '操作', 
    '#title_display' => 'invisible', 
    '#options' => array(
      'delete' => '删除歌词',
      'moderate' => '审核歌词',
      'unmoderate' => '取消审核歌词',
    ), 
    '#default_value' => 'delete',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit', 
    '#value' => '更新', 
  );
  $form['lyrics'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No content available.'),
    //'#attributes' => array('class' => array('xxxxx')),
  );
  $form['#prefix'] = '<div class="help">&nbsp;&nbsp;&nbsp;&nbsp;在这里你可以删除、编辑、审核歌词。未审核通过的歌词将不会在专辑信息里显示。审核前请先使用“编辑”功能检查歌词内容、选项是否正确，必要时做一些修正。如果是广告等垃圾信息，请立刻删除。</div>';
  $form['pager'] = array('#theme' => 'pager');
  return $form;
}

function discography_admin_lyric_page_submit($form, &$form_state) {
  include_once(drupal_get_path('module', 'discography').'/discography.lyric.inc');
  foreach($form_state['values']['lyrics'] as $lid => $value) {
    if ($value) {
      switch ($form_state['values']['operation']) {
        case 'delete':
          discography_lyric_delete($lid);
          continue;
        case 'moderate':
          $lyric->moderate = false;
          $lyric->lid = $lid;
          discography_lyric_moderate($lyric, false);
          continue;
        case 'unmoderate':
          $lyric->moderate = true;
          $lyric->lid = $lid;
          discography_lyric_moderate($lyric, false);
          continue;
      }
      
    }
  }
}

function discography_admin_list_page() {
  discography_set_admin_breadcrumb();

  $listtypes = array('0' => '专辑', '1' => '曲目');
  $header = array('列表名称', '类型', '显示', '操作');

  $result = db_select('node', 'n')
    ->extend('PagerDefault')
    ->fields('n', array('nid'))
    ->condition('n.type', 'discography_list')
    ->orderBy('n.changed', 'DESC')
    ->limit(20)
    ->execute();
  foreach ($result as $node) {
    $list = node_load($node->nid);
    $listtype = reset($list->field_listtype);
    $rows[] = array(
      check_plain($list->title), 
      $listtypes[$listtype[0]['value']],  
      l(($list->sticky ? '是' :'否'), 
      "discography/admin/list/$list->nid/sticky", 
      array('query' => drupal_get_destination())), 
      l('编辑', 'node/'.$list->nid.'/edit').'&nbsp;'.l('删除', 'node/'.$list->nid.'/delete')
    );
  }

  return array(
    '#prefix' => '<div class="help">管理所有的自定义列表。“显示”指的是该列表是否显示在作品搜索首页上。</div>',
    'lists' => array('#theme' => 'table', '#header' => $header, '#rows' => $rows),
    'pager' => array('#theme' => 'pager'),
  );
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
function discography_admin_list_sticky($node) {
  $node->sticky = $node->sticky ? 0 : 1;
  node_save($node);
  drupal_goto();
}

function discography_admin_comment_page() {
  discography_set_admin_breadcrumb();

  include_once(drupal_get_path('module', 'comment').'/comment.admin.inc');
  return comment_admin();
}

function discography_admin_source() {
  discography_set_admin_breadcrumb();

  $sources = variable_get('discography_album_sources', array('其它'));

  $form['source']['#tree'] = TRUE;
  foreach ($sources as $key=>$source) {
    $form['source'][$key] = array(
      '#type' => 'textfield',
      '#field_prefix' => $key.' ==> ',
      '#default_value' => $source,
    );
  }
  $form['new'] = array(
    '#type' => 'textfield',
    '#default_value' => '',
    '#title' => '新增专辑出处',
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => '保存',
  );
  return $form;
}

function discography_admin_source_submit($form, &$form_state) {
  $sources = array();
  foreach ($form_state['values']['source'] as $key=>$source) {
    if (trim($source))
      $sources[$key] = trim($source);
  }
  drupal_set_message('已保存专辑出处设定。');

  if (trim($form_state['values']['new'])) {
    array_push($sources, trim($form_state['values']['new']));
    drupal_set_message('新增专辑出处：'. $form_state['values']['new']);
  }
  variable_set('discography_album_sources', $sources);
}


function discography_admin_list_reset() {
  discography_rebuild_list();
  drupal_set_message('重建所有列表成功。');
  drupal_goto('discography/admin');
}

function discography_admin_stat_reset() {
  discography_rebuild_stat();
  drupal_set_message('重设统计信息成功。');
  drupal_goto('discography/admin');
}

function discography_admin_site_page() {
  discography_set_admin_breadcrumb();
  include_once(drupal_get_path('module', 'system').'/system.admin.inc');
  return drupal_get_form('system_site_information_settings');
}

function discography_admin_maintenance_page() {
  discography_set_admin_breadcrumb();
  include_once(drupal_get_path('module', 'system').'/system.admin.inc');
  return drupal_get_form('system_site_maintenance_mode');
}

function discography_admin_slide_form() {
  discography_set_admin_breadcrumb();

	$max = 5;
	$news = unserialize(variable_get('top_news', ''));
	
	$form = array('#tree' => TRUE, '#theme' => 'discography_admin_slide_form');
	for ($i = 0; $i < $max; $i++) {
		$form['image'][$i] = array('#type' => 'textfield', '#size' => 30, '#default_value' => $news[$i-2]['image']);
		$form['title'][$i] = array('#type' => 'textfield', '#size' => 30, '#default_value' => $news[$i-2]['title']);
		$form['link'][$i] = array('#type' => 'textfield', '#size' => 30, '#default_value' => $news[$i-2]['link']);
		$form['weight'][$i] = array('#type' => 'weight', '#default_value' => $i-2, '#delta' => 2, );
	}
	$form['submit'] = array('#type' => 'submit', '#value' => '保存设置');
	return $form;
}

function theme_discography_admin_slide_form($variables) {
  $form = $variables['form'];

	drupal_add_tabledrag('slide-admin-table', 'order', 'sibling', 'news-weight', NULL, NULL, FALSE);
	
	$header = array('', '图片地址', '新闻标题', '链接地址', '顺序');
	foreach (element_children($form['image']) as $newsid) {
		if (is_array($form['image'][$newsid])) {
			$form['weight'][$newsid]['#attributes']['class'] = array('news-weight');
			$rows[] = array(
				'data' => array(
          '',
          drupal_render($form['image'][$newsid]), 
          drupal_render($form['title'][$newsid]), 
          drupal_render($form['link'][$newsid]), 
          drupal_render($form['weight'][$newsid]),
        ),
				'class' => array('draggable'),
      );
		}
	}
  drupal_add_tabledrag('slide-admin-table', 'order', 'sibling', 'news-weight');

	$output = '<div class="help">请分行填写，图片地址应不大于300×300，文字不要太长，删除新闻只要把留空即可。可以用行首的十字排序，最新的在最下面。</div>';
	$output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'slide-admin-table')));
	$output .= drupal_render_children($form);
	return $output;
}

function discography_admin_slide_form_submit($form, &$form_state) {
	$news = array();
	foreach ($form_state['values']['weight'] as $id => $weight) {
			$news[$weight] = array(
				'image' => $form_state['values']['image'][$id], 
				'title' => $form_state['values']['title'][$id],
				'link' => $form_state['values']['link'][$id],
			);
 }
	ksort($news);
	variable_set('top_news', serialize($news));
	cache_clear_all();
}