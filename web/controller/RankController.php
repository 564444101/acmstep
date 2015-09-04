<?php
    class RankController extends Controller{

		function showTopicRank(){
			$this->checkLogin();

			$pid = 1;
			if (isset($_GET['pid'])){
				$pid = $_GET['pid'];
			}

			$userModel = M('User');
			$userList = $userModel->getUserListByTopicRank();
			$userRows = $this->getPage($pid, 20, $userList);
			
			$ranklist = array();
			foreach ($userRows['rows'] as $user){
				$usertmp = array();
				$rank = $user['topic_rank'];
				$usertmp['rank'] = $rank;
				if ($user['nickname'] == NULL){
					$usertmp['username'] = $user['username'];	
				}
				else{
					$usertmp['username'] = $user['nickname'];
				}
				$usertmp['motto'] = $user['motto'];
				$passTopicNum = count(explode(',',$user['pass_topics']));				
				$usertmp['pass_topic_num'] = $passTopicNum;
				$solved = $user['topic_solved_times'];
				$submited = $user['topic_submited_times'];
				
				$usertmp['solved_times'] = $solved;
				$usertmp['submited_times'] = $submited;
				if ($submited == 0){
					$acratio = 1*100;	
				}
				else{
					$acratio = round($solved*100 / $submited);
				}
				$usertmp['ac_ratio'] = $acratio;
				$ranklist []= $usertmp;
			}
		
			$uid = $_SESSION['user_id'];
			$userInfo = $userModel->getUserInfo($uid);

			$this->assign('userinfo', $userInfo);
			$this->assign('ranklist', $ranklist);
			$this->assign('hasPre', $userRows['hasPre']);
			$this->assign('hasNext', $userRows['hasNext']);
			$this->assign('pid', $userRows['pid']);
			$this->display('topicrank');
		}

		function showLadderRank(){
			$this->checkLogin();

			$pid = 1;
			if (isset($_GET['pid'])){
				$pid = $_GET['pid'];
			}

			$userModel = M('User');
			$userlist = $userModel->getUserListByLadderRank();
			$userRows = $this->getPage($pid, 20, $userlist);
			
			$ranklist = array();
			foreach ($userRows['rows'] as $user){
				$usertmp = array();
				$rank = $user['ladder_rank'];
				$usertmp['rank'] = $rank;
				if ($user['nickname'] == NULL){
					$usertmp['username'] = $user['username'];	
				}
				else{
					$usertmp['username'] = $user['nickname'];
				}
				$usertmp['motto'] = $user['motto'];
				$passLadderNum = $user['at_ladder'] - 1;
				$usertmp['pass_ladder_num'] = $passLadderNum;
				$submited = $user['ladder_submited_times'];
				$usertmp['submited_times'] = $submited;
				if ($submited == 0){
					$acratio = 1*100;	
				}
				else{
					$acratio = round($passLadderNum*100 / $submited);
				}
				$usertmp['ac_ratio'] = $acratio;
				$ranklist []= $usertmp;
			}
		

			$uid = $_SESSION['user_id'];
			$userInfo = $userModel->getUserInfo($uid);
		
			$this->assign('ranklist', $ranklist);
			$this->assign('userinfo', $userInfo);
			$this->assign('hasPre', $userRows['hasPre']);
			$this->assign('hasNext', $userRows['hasNext']);
			$this->assign('pid', $userRows['pid']);
			$this->display('ladderrank');
		}
    } 
?>
