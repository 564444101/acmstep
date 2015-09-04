<?php
//路径常量
define("APP_PATH", dirname(__FILE__));
define("CONTROLLER_PATH", APP_PATH.'/controller');
define("MODEL_PATH", APP_PATH.'/model');
define("VIEW_PATH", APP_PATH.'/view');
define("LIB_PATH", APP_PATH.'/lib');
define("CONF_PATH", APP_PATH.'/conf');
define("LOG_PATH", APP_PATH.'/log');
define("DOMAIN_NAME", '192.168.1.3');
define("INDEX_NAME", 'index.php');
//定义全局变量保存句柄
$GLOBALS = array();
//全局配置
$CONFIGURE = require(CONF_PATH.'/conf.php');
//全局函数
require(LIB_PATH.'/lib.php');
//导入MVC核心代码文件
require(LIB_PATH.'/Controller.php');
require(LIB_PATH.'/Model.php');
//入口 默认调用首页接口

startRun();
?>
