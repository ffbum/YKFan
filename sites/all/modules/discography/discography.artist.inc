<?php

function discography_artist_page($artist_name) {
  if (!$artist_name)
    return array('#markup' => '没有指定艺术家！');

  $query = db_select('discography_artist', 's');
  $query->fields('s');
  $query->condition('s.name', $artist_name);
  $result = $query->execute();

  if ($result->rowCount() == 0)
    return array('#markup' => '该艺术家不存在！');

  drupal_set_title($artist_name);

  $artist_dutynames = array('artist' => '专辑艺术家', 'composer' => '作曲家', 'arranger' => '编曲家', 'lyrist' => '作词人', 'performer' => '歌手');
  $duties = array();
  foreach($result as $artist) {
    $duties[$artist->duty][] = array('aid' => $artist->aid, 'did' => $artist->did, 'tid' => $artist->tid);
  }

  $artist_duties = array();
  foreach($artist_dutynames as $dutyindex => $dutyname) {
    if (isset($duties[$dutyindex])) {
      $artist_duties[] = $dutyname.'('.count($duties[$dutyindex]).')';
    }
  }

  $build = array();
  $build['create_detail_link'] = array('#markup' => l('创建详细信息', 'node/add/discography-artist', array('query' => array('name' => $artist_name))));
  $build['duty'] = array('#markup' => '参与角色：'. implode(',', $artist_duties));
  return $build;
}