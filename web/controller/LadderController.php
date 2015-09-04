<?php
    class LadderController extends Controller{

		function showLadderInfo(){
			$this->checkLogin();
			$lid = $_GET['lid'];
					
			$userModel = M('User');
			$uid = $_SESSION['user_id'];
			$userInfo = $userModel->getUserInfo($uid);
			$atLadder = $userInfo['at_ladder'];
			if ($lid > $atLadder){
				$this->error('you have no access to this ladder, comme on ^_^');
			}		
			
			$ladderModel = M('Ladder');
			$ladderInfo = $ladderModel->getLadderInfo($lid);
			$problemId = $ladderInfo['problem_id'];

			$problemModel = M('Problem');
				

		}

		function showLadderList(){
			$this->checkLogin();
				
			$userId = $_SESSION['user_id'];
			$userModel = M('User');
			$userInfo = $userModel->getUserInfo($userId);
			$atLadder = $userInfo['at_ladder'];
				
			$ladderModel = M('Ladder');
			$ladder_list = $ladderModel->getLadderListBeforeId($atLadder);
			if ($ladder_list == false || $userInfo == false){
				$this->error('get ladder list failed',U('Index','index'));
			}
		    $ladderNameRows = array();
			foreach ($ladder_list as $ladder){
				$ladder_id = $ladder['ladder_id'];
				$ladderNameRows[$ladder_id] = $ladder['ladder_name'];
			}	

			$this->assign('userinfo', $userInfo);
			$this->assign('ladderNames',$ladderNameRows);
			$this->display('ladderlist');
		}
    } 
?>
