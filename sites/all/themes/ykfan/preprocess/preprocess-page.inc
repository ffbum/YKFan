<?php

function ykfan_alpha_preprocess_page(&$vars) {
  // custom functionality here
  if ($vars['is_front']) $vars['title_hidden'] = true;
}