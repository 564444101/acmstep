<?php
/**
 * lib.php
 * 库函数
 */

//实例化controller的方法 运用单例模式
function C($className) {
    global $GLOBALS;
    $classPath = CONTROLLER_PATH.'/'.$className.'Controller.php';
    $className = $className.'Controller';
    if(isset($GLOBALS['controller'][$className])) return $GLOBALS['controller'][$className];
    else {
        if(file_exists($classPath)) {
            require($classPath);
            return $GLOBALS['controller'][$className] = new $className;
        }
        else {
            printErrorLog("$className method is wrong ");
            return NULL;
        }
    }
}

//实例化model的方法 运用单例模式
function M($className) {
    global $GLOBALS;
    $classPath = MODEL_PATH.'/'.$className.'Model.php';
    $className = $className.'Model';
    if(isset($GLOBALS['model'][$className])) {
        return $GLOBALS['model'][$className];
    }
    else {
        if(file_exists($classPath)) {
            require($classPath);
            return $GLOBALS['model'][$className] = new $className;
        }
        else {
            printErrorLog("$className method is wrong ");
            return NULL;
        }
    }
}

function U($controller, $action) {
    $path = substr(strchr(APP_PATH, "www"), 3);
    $url = 'http://'.DOMAIN_NAME."$path/".INDEX_NAME."?c=$controller&a=$action";
    return $url;
}


//格式化在网页上输出调试信息
function dump($args) {
    $content = "<div align=left><pre>\n" . htmlspecialchars(print_r($args, true)) . "\n</pre></div>\n";
    echo "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head><body>".$content."</body></html>";
}

/**
 *
 * 成功提示程序
 * 应用程序的控制器类可以覆盖该函数以使用自定义的成功提示
 *
 * @param $msg   成功提示需要的相关信息     
 * @param $url   跳转地址     
 *
 * */
function success($msg, $url = '') {		
	$url = empty($url) ? "window.history.back();" : "location.href=\"{$url}\";";		
	echo "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><script>function sptips(){alert(\"{$msg}\");{$url}}</script></head><body onload=\"sptips()\"></body></html>";		
	exit;    
}

/** 
 * 错误提示程序     
 * 应用程序的控制器类可以覆盖该函数以使用自定义的错误提示     
 * @param $msg   错误提示需要的相关信息     
 * @param $url   跳转地址     
 *
 * */
function error($msg, $url = ''){		
	$url = empty($url) ? "window.history.back();" : "location.href=\"{$url}\";";		
	echo "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><script>function sptips(){alert(\"{$msg}\");{$url}}</script></head><body onload=\"sptips()\"></body></html>";		
	exit;    
}

/**
 *
 * 跳转程序  
 * 应用程序的控制器类可以覆盖该函数以使用自定义的跳转程序     
 * @param $url  需要前往的地址     
 * @param $delay   延迟时间     
 *
 * */
function jump($url, $delay = 0){		
	echo "<html><head><meta http-equiv='refresh' content='{$delay};url={$url}'></head><body></body></html>";		
	exit;    
}

//获取当前时间函数
function getTime() {
    $date = date('Y-m-d H:i:s', time());
    $year = ((int)substr($date, 0, 4));//取得年份
    $month = ((int)substr($date, 5, 2));//取得月份
    $day = ((int)substr($date, 8, 2));//取得几号
    return compact("date", "year", "month", "day");
}

function printLog($message) {
    $date = getTime();
    $file = $date['year'].$date['month'].$date['day'].'.accesslog';
    $message = $date['date']."  $message\n";
    $filePath = LOG_PATH."/$file";
    $myFile = fopen($filePath, "a+");
    fwrite($myFile, $message);
}

function printErrorLog($message) {
    $date = getTime();
    $file = $date['year'].$date['month'].$date['day'].'.errorlog';
    $message = $date['date']."  $message\n";
    $filePath = LOG_PATH."/$file";
    $myFile = fopen($filePath, "a+");
    fwrite($myFile, $message);
}

function startRun() {
    session_start();
    $_controller; 
    $_action;
    if(isset($_GET['c'])) {
        $_controller = $_GET['c'];
    }
    else {
        $_controller = 'Index';
    }
    if(isset($_GET['a'])) {
        $_action = $_GET['a'];
    }
    else {
        $_action = 'index';
    }
    $handel = C($_controller);
    if(!is_object($handel)) {
        echo 'controller is null';
        exit;
    }
    if(!method_exists($handel, $_action)) {
        echo 'action is null';
        exit;
    }
    $handel->$_action();
}


?>
