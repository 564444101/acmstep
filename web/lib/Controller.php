<?php
	class Controller {
		public $_templateVals = array();
		function assign($name, $val) {
			$this->_templateVals[$name] = $val;
		}
		
        function getPage($pid = NULL, $pagesize = NULL, $rows = NULL){
            if ( NULL == $pid ){
                $pid = 1;
            } 
            if ( NULL == $pagesize ){
                $pagesize = 20;
            }

            $rownum = count($rows);
            $totpage = (int)($rownum / $pagesize);
            if($rownum % $pagesize != 0) $totpage += 1;
            if ( $pid > $totpage || $pid < 1 ){
                $pid = 1;
            }
            
            $newRows = array();
            $hasPre = true;
            $hasNext = true;

            if ($totpage == 0 || $pid == 1 ){
                $hasPre = false;
            }
            if ( $totpage == 0 || $pid == $totpage ){
                $hasNext = false;
            }
            $newRows ['pid']= $pid;
            $newRows ['hasPre']= $hasPre;
            $newRows ['hasNext']= $hasNext;
            $newRows ['rows']= array();
            
            $begin = ($pid-1) * $pagesize;
            $end = min($begin + $pagesize, $rownum);
            for ($i = $begin; $i < $end; ++$i){
                $newRows['rows'] []= $rows[$i];
            }     
            return $newRows;
        }
		function display($tpl) { 
			extract($this->_templateVals);
			$tplPath = VIEW_PATH.'/'.$tpl.'.html';
			if(file_exists($tplPath)) {
				require($tplPath);
			}
			else {
				echo 'tpl is null';
			}
		}
		function checkLogin() {
			if(!isset($_SESSION['username'])) {
				error('please first log in', U('Index','index'));
			}
		}
		function checkAdmin() {
			$this->checkLogin();
			if(isset($_SESSION['user_type']) && $_SESSION['user_type'] != 1) {
				error('user_type is wrong');
			}
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
	}
?>
