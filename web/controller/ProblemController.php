<?php
    class ProblemController extends Controller{

		function showProblem(){
			$this->checkLogin();

			$type = $_GET['type'];
			$type_id = $_GET['type_id'];
			$problemInfo = array();
			
			$uid = $_SESSION['user_id'];
			$userModel = M('User');
			$userInfo = $userModel->getUserInfo($uid);

			if (0 == $type){
				$pid = $_GET['pid'];				
				if (!empty($_SESSION['topiclist'][$type_id]) && true == $_SESSION['topiclist'][$type_id]){
					if (!empty($_SESSION['problemlist'][$pid]) && true == $_SESSION['problemlist'][$pid]){
						$problemModel = M('Problem');					
						$problemInfo = $problemModel->getProblemInfo($pid);
						if (empty($problemInfo)){
							$this->error('get problem info failed ^^');
						}
					}	
					else{
						$this->error('you have no access to this problem ^^');
					}
				}
				else{
					$this->error('you have no access to this topic ^^');
				}
			} else { // ladder
				$atLadder = $userInfo['at_ladder'];			
				$lid = $type_id;
				if ($lid > $atLadder){
					$this->error('you have no access to this Problem ladder, come on ^_^');
				}

				$ladderModel = M('Ladder');	
				$ladderInfo = $ladderModel->getLadderInfo($lid);
				$pid = $ladderInfo['problem_id'];
				$problemModel = M('Problem');
				$problemInfo = $problemModel->getProblemInfo($pid);
				if (empty($problemInfo)){
					$this->error('the problem does not exist');
				}
			}
			
			//tip
			$tipModel = M('Tip');			
			$tipInfo = $tipModel->getTipInfo($uid, $pid);
			$openTip = false;
			$lastTime = array();
			if (!empty($tipInfo)){
				$openTime = strtotime($tipInfo['open_time']);
				$nowTime = getTime();
				$nowTime = strtotime($nowTime['date']);
				$last = $nowTime - $openTime;
				if ($last > 604800){
					$openTip = true;
				} else {
					$last = 604800 - $last;
					$lastTime['d'] = floor($last / 86400);
					$last = $last % 86400;
					$lastTime['h'] = floor($last / 3600);
					$last = $last % 3600;
					$lastTime['m'] = floor($last / 60);
					$lastTime['s'] = floor($last % 60);
				}
			} 

			$this->assign('lasttime', $lastTime);
			$this->assign('userinfo', $userInfo);
			$this->assign('opentip', $openTip);
			$this->assign('type', $type);
			$this->assign('type_id', $type_id);
			$this->assign('pid', $pid);
			$this->assign('probleminfo', $problemInfo);
			$this->display('showproblem');
		}

    } 
?>
