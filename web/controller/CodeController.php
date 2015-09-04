<?php
    class CodeController extends Controller {

		 function submittest() {
			
		//	$this->checkLogin();
			$pid = 1001;
			$type = 0;
			$type_id = 1;
			$is_inner=0;
				
			$codeInfo = array();
			$codeInfo['is_inner'] = $is_inner;
			$codeInfo['type'] = $type;
			$codeInfo['type_id'] = $type_id;
			$codeInfo['problem_id'] = $pid;
			$codeInfo['user_id'] = 3 ;
			$codeInfo['author'] = 'zz';

			$codeInfo['code'] = $_POST['code'];
			$codeInfo['lang'] = 1;

			$codeInfo['code_length'] = strlen($_POST['code']);

			$tTime = getTime();
			$codeInfo['submit_time'] = $tTime['date'];
			$codeModel = M('Code');
			if ($codeModel->addCode($codeInfo)){
				$this->success('submit success!',U('Code','showStatus').'&pid=1');
			}
			else{
				$this->error('submit failed!');
			}	
        }



        function submit() {
			
			$this->checkLogin();
			$pid = $_GET['pid'];
			$type = $_GET['type'];
			$type_id = $_GET['type_id'];
			
			if (0 == $type){ //topic
				if (!empty($_SESSION['topiclist'][$type_id]) && true == $_SESSION['topiclist'][$type_id]){
					if (!empty($_SESSION['problemlist'][$pid]) && true == $_SESSION['problemlist'][$pid]){
						$topicModel = M('Topic');
						$topicInfo = $topicModel->getTopicInfo($type_id);
						if (!empty($topicInfo) && in_array($pid, explode(',',$topicInfo['problem_ids']))){
							$problemModel = M('Problem');
							$problemInfo = $problemModel->getProblemInfo($pid);
							$is_inner = $problemInfo['is_inner'];
						} else {
							$this->error('problem not exist!');
						}
					} else {
						$this->error('you have no access to this problem, come on ^_^');
					}
				} else {
					$this->error('you have no access to this topic, come on ^_^');
				}
			} else { //ladder
				$uid = $_SESSION['user_id'];
				$userModel = M('User');
				$userInfo = $userModel->getUserInfo($uid);
				$atLadder = $userInfo['at_ladder'];
				$lid = $type_id;
				if ($lid > $atLadder){
					$this->error('you have no access to this ladder, come on ^_^');
				}
				$ladderModel = M('Ladder');
				$ladderInfo = $ladderModel->getLadderInfo($lid);
				$problem_id = $ladderInfo['problem_id'];
				if ($problem_id != $pid){
					$this->error('the ladder have no this problem');
				}
				$problemModel = M('Problem');
				$problemInfo = $problemModel->getProblemInfo($pid);
				$is_inner = $problemInfo['is_inner'];				
			}
	
			$codeInfo = array();
			$codeInfo['is_inner'] = $is_inner;
			$codeInfo['type'] = $type;
			$codeInfo['type_id'] = $type_id;
			$codeInfo['problem_id'] = $pid;
			$codeInfo['type'] = $_GET['type'];
			$codeInfo['user_id'] = $_SESSION['user_id'];
			$codeInfo['author'] = $_SESSION['username'];
			$codeInfo['code'] = $_POST['code'];
			$codeInfo['lang'] = $_POST['lang'];
			$codeInfo['code_length'] = strlen($_POST['code']);

			$tTime = getTime();
			$codeInfo['submit_time'] = $tTime['date'];
			$codeModel = M('Code');
			if ($codeModel->addCode($codeInfo)){
				$this->success('submit success!',U('Code','showStatus').'&pid=1');
			}
			else{
				$this->error('submit failed!');
			}	
        }

        function submitCode() {
			$this->checkLogin();
			$pid = $_GET['pid'];
			$type = $_GET['type'];
			$type_id = $_GET['type_id'];
			
			$userModel = M('User');
			$uid = $_SESSION['user_id'];
			$userInfo = $userModel->getUserInfo($uid);

			$this->assign('userinfo', $userInfo);
			$this->assign('type', $type);
			$this->assign('type_id', $type_id);
			$this->assign('pid', $pid);
			$this->display('submit');
        }

        function showStatus() {

			$this->checkLogin();

			$pid = 1;
			if (isset($_GET['pid'])){
				$pid = $_GET['pid'];
			} 
            $uid = $_SESSION['user_id'];
            $codeModel = M('Code');
			$judgeStatus = '';
			$lang = '';

			$conditons = array();
			if (isset($_POST['judgestatus']) && strlen($_POST['judgestatus'])>0){
				$conditions['judge_status'] = $_POST['judgestatus'];
				$judgeStatus = $_POST['judgestatus'];
			} 
			if (isset($_POST['lang']) && strlen($_POST['lang'])>0){
				$conditions['lang'] = $_POST['lang'];
				$lang = $_POST['lang'];
			}

			$conditions['user_id'] = $uid;
            $result = $codeModel->getCodes($conditions);
			$statusRows = $this->getPage($pid, 20, $result);
			$userModel = M('User');
			$uid = $_SESSION['user_id'];
			$userInfo = $userModel->getUserInfo($uid);	

			$this->assign('status', $judgeStatus);
			$this->assign('lang', $lang);
			$this->assign('userinfo', $userInfo);
			$this->assign('codeList',$statusRows['rows']);
			$this->assign('hasPre', $statusRows['hasPre']);
			$this->assign('hasNext', $statusRows['hasNext']);
			$this->assign('pid', $statusRows['pid']);
			$this->display('status');
		}

		function showCode(){
			$this->checkLogin();
			
			$uid = $_SESSION['user_id'];

			$cid = $_GET['cid'];
			$codeModel = M('Code');
			$codeInfo = $codeModel->getCodeInfoByCid($cid);
			if ($codeInfo['user_id'] != $uid){
				$this->error('you can not see this code');
			}	
			
			$userModel = M('User');
			$uid = $_SESSION['user_id'];
			$userInfo = $userModel->getUserInfo($uid);	

			$this->assign('codeinfo',$codeInfo);
			$this->assign('userinfo', $userInfo);
			$this->display('showcode');
		}

		function showCeInfo(){
				
			$this->checkLogin();

			$cid = $_GET['cid'];
			$codeModel = M('Code');
		    $codeInfo = $codeModel->getCodeInfoByCid($cid);	

			$userModel = M('User');
			$uid = $_SESSION['user_id'];
			$userInfo = $userModel->getUserInfo($uid);	

			$this->assign('userinfo', $userInfo);
			$this->assign('codeinfo', $codeInfo);
			$this->display('showceinfo');
		}

	}

	
?>
