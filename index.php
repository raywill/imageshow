<?php
function searchDir($path,&$files){
 
  if(is_dir($path)){
 
    $opendir = opendir($path);
 
    while ($file = readdir($opendir)){
      if($file != '.' && $file != '..'){
        searchDir($path.'/'.$file, $files);
      }
    }
    closedir($opendir);
  }
  if(!is_dir($path)){
    $files[] = $path;
  }
}
//得到目录名
function getDir($dir){
  $files = array();
  searchDir($dir, $files);
  return $files;
}
$filenames = getDir('.');
 
foreach ($filenames as $value){
  echo "<a href='$value'>$value</a>".'<br/>';
} 
